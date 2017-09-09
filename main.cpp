#include<iostream>
#include<string>
#include<conio.h>
#include<windows.h>
#include"util.h"
#include"class.h"
int main(){
	ShowCur(false); //tắt nhấp nháy con trỏ văn bản
	srand(time(NULL));//khởi tạo thời gian để các hàm random chạy được
	snake a(10,10,3,1,RIGHT);//tạo một snake
	fruit b;//tạo một fruit
	b.init(5,219);
	int score=0;//điểm
	gotoxy(80,20);//hiển thị điểm
    SetColor(10);
    std::cout<<"Score: "<<score;
	while(true){
        showArea();//hiển thị khung
		a.controlAndMove();//điểu khiển snake
		if(handleEvent(a,b)){//xử lý event
            b.init(5,219);
            score++;
            gotoxy(80,20);
            SetColor(10);
            std::cout<<"Score: "<<score;
		}
		a.showToBuffer();//nạp snake vào buffer
		b.showToBuffer();//nạp fruit vào buffer
		showBuffer();//hiển thị buffer
	}
	return 1;
}
