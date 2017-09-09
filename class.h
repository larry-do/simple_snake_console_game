#include<vector>
#include<string>
#include<conio.h>
#include<cctype>
#include<stdio.h>
#include<ctime>
#include<windows.h>
#define CONSOLE_HEIGHT 30
#define CONSOLE_WIDTH 120
#define RIGHT_EDGE 39
#define LEFT_EDGE 0
#define TOP_EDGE 0
#define BOTTOM_EDGE 27
enum statusEnum {UP,DOWN,LEFT,RIGHT};//khai báo biến enum trạng thái
////////////////////////////////////////////////////////////////////
class bufferClass{//khai báo class buffer
	private:
		int color;
		char character;
	public:
		void setColor(int _color);
		void setChar(char _character);
		int getColor();
		char getChar();
};
void bufferClass::setChar(char _character){
	character=_character;
}
void bufferClass::setColor(int _color){
	color=_color;
}
int bufferClass::getColor(){
	return color;
}
char bufferClass::getChar(){
	return character;
}
bufferClass buffer[CONSOLE_HEIGHT][CONSOLE_WIDTH];//khai báo buffer
void assignBuffer(int x,int y,int color,char character){//hàm nạp vào buffer
	buffer[y][x].setColor(color);
	buffer[y][x].setChar(character);
}
void showBuffer(){//hiển thị buffer
	gotoxy(0,0);
	for(int i=0;i<CONSOLE_HEIGHT-1;i++){
		for(int j=0;j<CONSOLE_WIDTH/2;j++){
			SetColor(buffer[i][j].getColor());
			putchar(buffer[i][j].getChar());
			buffer[i][j].setChar(' ');//refresh buffer
		}
		putchar('\n');
	}
}
////////////////////////////////////////////////////////////////////
class snakeDot{//khai báo một đốt của con rắn
	private:
		int x,y;
		char character;
		int color;
	public:
		snakeDot(int _x,int _y,int _color,char _char);
		~snakeDot();
		void showToBuffer();
		void setPos(int _x,int _y);
		int X();
		int Y();
};
snakeDot::snakeDot(int _x,int _y,int _color,char _char){
	if(_x>=LEFT_EDGE&&_x<=RIGHT_EDGE) x=_x;
	if(_y>=TOP_EDGE&&_y<=BOTTOM_EDGE) y=_y;
	character=_char;
	color=_color;
}
snakeDot::~snakeDot(){
}
int snakeDot::X(){
	return x;
}
int snakeDot::Y(){
	return y;
}
void snakeDot::setPos(int _x,int _y){
	if(_x>=LEFT_EDGE&&_x<=RIGHT_EDGE) x=_x;
	if(_y>=TOP_EDGE&&_y<=BOTTOM_EDGE) y=_y;
}
void snakeDot::showToBuffer(){
	assignBuffer(x,y,color,character);
}
/////////////////////////////////////////////////////////////////////
class fruit;//tự dưng có khai báo class fruit ở đây vì cần cho hàm friend ở dưới. Chỗ này gọi là khai báo nguyên mẫu hàm
class snake {
	private:
		int x,y;
		std::vector<snakeDot> body;//mảng vector các snakeDot, có thể thay bằng mảng thường
		statusEnum status;
		int speed;
	public:
		snake(int _x,int _y,int _length,int _speed,statusEnum _status);
		~snake();
		void showToBuffer();
		void controlAndMove();
		friend bool handleEvent(snake &a,fruit &b);//hàm friend này kết nối snake với fruit, dùng để biết khi nào ăn được fruit
};
snake::snake(int _x,int _y,int _length,int _speed,statusEnum _status){
	if(_x>=LEFT_EDGE&&_x<=RIGHT_EDGE) x=_x;
	if(_y>=TOP_EDGE&&_y<=BOTTOM_EDGE) y=_y;
	if(_speed>0) speed=_speed;
	status=_status;
	if(_length>2){//khởi tạo snake với độ dài length
		snakeDot a(x,y,9,219);
		body.push_back(a);
		for(int i=1;i<=_length;i++){
			snakeDot a(x-i,y,2,219);
			body.push_back(a);
		}
	}
}
snake::~snake(){
}
void snake::controlAndMove(){
    for(int i=body.size()-1;i>0;i--){
		body[i].setPos(body[i-1].X(),body[i-1].Y());
	}
	if(status==UP){
		body[0].setPos(x,--y);
	}
	if(status==DOWN){
		body[0].setPos(x,++y);
	}
	if(status==LEFT){
		body[0].setPos(--x,y);
	}
	if(status==RIGHT){
		body[0].setPos(++x,y);
	}
	if(GetAsyncKeyState(VK_UP)&&status!=DOWN) status=UP;
	if(GetAsyncKeyState(VK_DOWN)&&status!=UP) status=DOWN;
	if(GetAsyncKeyState(VK_LEFT)&&status!=RIGHT) status=LEFT;
	if(GetAsyncKeyState(VK_RIGHT)&&status!=LEFT) status=RIGHT;
}
void snake::showToBuffer(){
	for(int i=0;i<body.size();i++){
		body[i].showToBuffer();
	}
}
/////////////////////////////////////////////////////////////////
class fruit {
private:
    int x,y;
    int color;
    char character;
public:
    void init(int _color,char _character);
    void showToBuffer();
    int X();
    int Y();
    friend bool handleEvent(snake &a, fruit &b);
};
void fruit::init(int _color,char _character){
    x=1+rand()%(RIGHT_EDGE-1);//hàm random
    y=1+rand()%(BOTTOM_EDGE-1);
	color=_color;
	character=_character;
}
int fruit::X(){
    return x;
}
int fruit::Y(){
    return y;
}
void fruit::showToBuffer(){
    assignBuffer(x,y,color,character);
}
bool handleEvent(snake &a, fruit &b){
    if(a.x<=LEFT_EDGE||a.x>=RIGHT_EDGE||a.y<=TOP_EDGE||a.y>=BOTTOM_EDGE){
        SetColor(9);
        std::cout<<"You Lose";
        Sleep(2000);
        exit (EXIT_FAILURE);//thoat chương trình
    }
    for(int i=1;i<a.body.size();i++){
        if(a.body[0].X()==a.body[i].X()&&a.body[0].Y()==a.body[i].Y()){
            SetColor(9);
            std::cout<<"You Lose";
            Sleep(2000);
            exit (EXIT_FAILURE);//thoat chương trình
        }
    }
    if(a.x==b.x&&a.y==b.y){
        snakeDot c(0,0,2,219);
        a.body.push_back(c);
        return true;
    }
    return false;
}
/////////////////////////////////////////////////////////////////
void showArea(){//hàm hiển thị khung chơi
    for(int i=LEFT_EDGE;i<=RIGHT_EDGE;i++){
        assignBuffer(i,TOP_EDGE,8,219);
        assignBuffer(i,BOTTOM_EDGE,8,219);
    }
    for(int i=TOP_EDGE;i<=BOTTOM_EDGE;i++){
        assignBuffer(LEFT_EDGE,i,8,219);
        assignBuffer(RIGHT_EDGE,i,8,219);
    }
}
