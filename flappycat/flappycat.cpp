#include <graphics.h>
#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include<string>
#include <Windows.h>
#include<Mmsystem.h>
#pragma comment(lib,"winmm.lib")
using namespace std;



//全局函数
void print_obstacle(); // 更新障碍物的位置
void hit_detection();	// 检测小鸟是否撞到了障碍物
void bird_jump();		//控制鸟的下降和上升
void score_print();		//更新游戏界面上的分数显示
void endorretry();		//游戏结束


//新添加的函数
void print();			//绘制游戏界面所有图形
bool login();			//登录
void init();			//载入最初界面的音乐和背景
int choose();			//选择
void play();			//进入游戏
void _rank();			//进入排行榜
void set();				//进入设置
void show();			//进入游戏玩法
void back();			//返回到主界面

DWORD WINAPI init_dynamic(LPVOID);


//全局变量
string nickname = "";   // 玩家昵称
int option = 0;			//鼠标选择哪个选项
int flag_init = 0;			//0表示主界面背景（子线程）不结束

int bird_x = 150, bird_y = 300, i = 0, k = 0     ;//鸟的左上角坐标,i是image bird1[]的元素位置，k是到5归0，实现小鸟翅膀的循环
int score = 0, t = 0;	//socre：分数  t：
int stone_x1, stone_y1;	//上截柱子左下角坐标
int stone_x2 = -800, stone_y2;	//下截柱子左上角坐标

IMAGE  title, start, ranking, setting,showing,background,showbackground, goback,  //这些是做主界面新加的
bird1[4], bird2[4], score1[10], score2[10], stone_up1, stone_up2, stone_down1, stone_down2
, stone_up3, stone_up4, stone_down3, stone_down4;//图片储存变量


ExMessage m;		// 定义鼠标消息


//主界面的雪花（可以参考easyx帮助文档里的流星示例，相差不大）

#define MAXSNOW 200	// 雪花总数
struct SNOW
{
	double	x;    //坐标
	int		y;
	double	step; //步长：每次刷新移动的横向距离
};

SNOW snow[MAXSNOW]; //设置全局变量snow

// 初始化雪花
void InitSnow(int i)
{
	snow[i].x = 0;               //一开始x坐标是0，因为后续更新，雪花都是从左往右开始走；一开始不是的
	snow[i].y = rand() % 600;
	snow[i].step = (rand() % 5000) / 4000.0 + 1;  //步长
}

// 移动雪花
void MoveSnow(int i)
{
	snow[i].x += snow[i].step;              //进入movesnow首先更新一下每个雪花的位置
	if (snow[i].x > 350)	InitSnow(i);    //当超出界面后，从最左边重新出现

	// 画新雪花
	fillcircle((int)snow[i].x, snow[i].y, 1); //就是画个圈
}

void init() //载入主界面背景
{
	loadimage(&background, "./source/initground.jpg");//载入背景图
	loadimage(&title, "./source/title.png", 270, 60);//载入标题图片
	loadimage(&start, "./source/start_1.png");//载入开始按钮
	loadimage(&ranking, "./source/rank_1.png");//载入排行榜按钮
	loadimage(&setting, "./source/setting_1.png");//载入设置按钮
	loadimage(&showing, "./source/show_1.png");//载入游戏玩法按钮
	loadimage(&goback, "./source/back_1.png");//载入后退按钮

	putimage(0, 0, &background);//放入背景图
	putimage((350 - title.getwidth()) / 2, 100, &title, SRCPAINT);  //放入标题图片,与背景取or
	putimage((350 - start.getwidth()) / 2, 230, &start);//放入开始按钮
	putimage((350 - ranking.getwidth()) / 2, 300, &ranking);//放入排行榜按钮
	putimage((350 - setting.getwidth()) / 2, 370, &setting);//放入设置按钮
	putimage((350 - showing.getwidth()) / 2, 440, &showing);//放入游戏玩法按钮*/


	/*if (t/100 < 255)      //这里理论上其实可以让雪花变化颜色，不知为何没有效果
	{
		setfillcolor(t/100);
		t++;
	}
	else
	{
		t = 0;
	}*/

}

int choose()    //这里是个死循环捕捉鼠标事件的函数。所以会考虑用线程
{
	int n = 0;
	while (1)
	{
		m = getmessage();
		if (m.message == WM_LBUTTONDOWN)   //如果鼠标左键按下了
		{
			if (m.x >= 123 && m.x <= 227 && m.y >= 230 && m.y <= 286) n = 1;	//开始
			if (m.x >= 123 && m.x <= 227 && m.y >= 300 && m.y <= 357) n = 2;	//排行榜
			if (m.x >= 123 && m.x <= 227 && m.y >= 370 && m.y <= 426) n = 3;	//设置
			if (m.x >= 123 && m.x <= 227 && m.y >= 440 && m.y <= 496) n = 4;	//游戏说明

			if (n) //只有n不等于0的时候，说明鼠标按了其中一个按钮，才能跳出循环
			{

				flag_init = 1;  //把flag置1，告诉子线程，需要停止了
				Sleep(10); //这个地方比较关键，因为子线程是每隔一段时间检查一下flag。如果我刚把flag置1就return，可能子线程还没来得及停止。就会死机
				return n;
			}
		}
	}

}


bool login() {
	bool ifBack = false;
	loadimage(&ranking, "./source/confirm2.png"); // 确定的图片
	putimage(60, 350, &ranking, SRCPAINT);
	loadimage(&ranking, "./source/skip2.png"); // 跳过的图片
	putimage(220, 350, &ranking, SRCPAINT);
	putimage(250, 550, &goback); // 放入后退按钮
	RECT r = { 30, 0, 320, 479 }; // 文字位置
	RECT rcin = { 100, 0, 320, 550 }; // 输入的昵称位置
	drawtext(_T("请输入游戏昵称:"), &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	while (1) {
		flushmessage();
		m = getmessage(EM_KEY | EM_CHAR | EM_MOUSE); // 键盘和鼠标事件检测
		if (m.message == WM_KEYDOWN || m.message == WM_CHAR) {
			if ((m.vkcode >= 48 && m.vkcode <= 57) || (m.vkcode >= 65 && m.vkcode <= 90)) { // 判断为字母数字就输入昵称中
				nickname += tolower(m.ch);
				drawtext(_T(nickname.data()), &rcin, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			}
			else if (m.vkcode == 13) { // 回车确定
				if (nickname == "") nickname = "admin";
				drawtext(_T(nickname.data()), &rcin, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				break; // 需改动
			}
			else if (m.vkcode == 8) { // 退回一个字符
				// 重新加载图片。。。
				putimage(0, 0, &background);
				loadimage(&ranking, "./source/confirm2.png"); // 确定的图片
				putimage(60, 350, &ranking, SRCPAINT);
				loadimage(&ranking, "./source/skip2.png"); // 跳过的图片
				putimage(220, 350, &ranking, SRCPAINT);
				putimage(250, 550, &goback); //放入后退按钮
				drawtext(_T("请输入游戏昵称:"), &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				nickname = nickname.substr(0, nickname.length() - 1);
				flushmessage();
				drawtext(_T(nickname.data()), &rcin, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			}
		}
		else if (m.message == WM_LBUTTONDOWN)   //如果鼠标左键按下了
		{
			if (m.x >= 250 && m.x <= 330 && m.y >= 550 && m.y <= 578) // 点击back
			{
				ifBack = true;
				loadimage(&goback, "./source/back_2.png");//载入后退按钮
				putimage(250, 550, &goback);				//放入后退按钮
				PlaySound(TEXT("./source/click.wav"), NULL, SND_FILENAME);
				break;
			}
			else if (m.x >= 60 && m.x <= 147 && m.y >= 350 && m.y <= 393) { // 点击确定
				loadimage(&ranking, "./source/confirm2.png"); // 确定的图片
				putimage(60, 350, &ranking, SRCINVERT);
				loadimage(&ranking, "./source/confirm.png"); // 确定的图片
				putimage(60, 350, &ranking, SRCPAINT);
				PlaySound(TEXT("./source/click.wav"), NULL, SND_FILENAME);
				break;
			}
			else if (m.x >= 220 && m.x <= 304 && m.y >= 350 && m.y <= 397) { // 点击跳过
				nickname = "admin";
				loadimage(&ranking, "./source/skip2.png"); // 跳过的图片
				putimage(220, 350, &ranking, SRCINVERT);
				loadimage(&ranking, "./source/skip.png"); // 跳过的图片
				putimage(220, 350, &ranking, SRCPAINT);
				PlaySound(TEXT("./source/click.wav"), NULL, SND_FILENAME);
				break;
			}
		}
	}
	if (ifBack) {
		ifBack = false;
		return false;
	}
	return true;
}


void play()
{
	//改变开始按钮
	loadimage(&start, "./source/start_2.png");			//载入开始按钮
	putimage((350 - start.getwidth()) / 2, 230, &start);//放入开始按钮
	PlaySound(TEXT("./source/click.wav"), NULL, SND_FILENAME); //实现音乐播放有两个函数：playsound函数效率更高，优先使用；另一个是mciSendString，在主函数中用
	putimage(0, 0, &background);
	bool state = login();
	// system("pause");

	m = getmessage();
	while (state)
	{
		print();
		hit_detection();
		bird_jump();
		if (m.message == WM_LBUTTONDOWN)   //如果鼠标左键按下了
		{ // 鼠标左键
			if (m.x >= 250 && m.x <= 330 && m.y >= 550 && m.y <= 578) //back图片存放的区域
			{
				loadimage(&goback, "./source/back_2.png");//载入后退按钮
				putimage(250, 550, &goback);				//放入后退按钮
				PlaySound(TEXT("./source/click.wav"), NULL, SND_FILENAME);
				break;
			}
		}
	}
}

void print()
{
	BeginBatchDraw();// 开启批量绘图模式
	putimage(0, 0, &background);//背景图像
	putimage(250, 550, &goback);//放入后退按钮
	print_obstacle();//画柱子


	putimage(bird_x, bird_y, &bird1[i % 3], NOTSRCERASE); // 鸟的左上角坐标 bird_x = 150, bird_y = 300, i = 0, k = 0 ；k是到5归0，实现小鸟翅膀的循环
	putimage(bird_x, bird_y, &bird2[i % 3], SRCINVERT);

	if (k % 5 == 0)
		i++;
	k++;        //到此为止，实现了小鸟位置的更新

	score_print();
	putimage(250, 550, &goback);//放入后退按钮
	//EndBatchDraw();
	FlushBatchDraw();		// 绘制	

}

void print_obstacle()//柱子移动规律
{
	/*
	stone_x1 = 0; stone_y1 = 0;
	cout << stone_up1.getwidth() << "  " << stone_up1.getheight() << endl;    //140 600
	putimage(stone_x1, stone_y1, &stone_up1);  //掩码图  上截柱子左下坐标一开始未初始化
	FlushBatchDraw();		// 绘制
	_getch();
	*/
	Sleep(30);
	if (stone_x1 > 210)//此时画面存在两根柱子
	{
		//三光栅操作
		putimage(stone_x1, stone_y1, &stone_up2, NOTSRCERASE);  //掩码图  上截柱子左下坐标一开始未初始化stone_x1, stone_y1:上截柱子左下坐标    stone_x2, stone_y2:上截柱子左下坐标
		putimage(stone_x1, stone_y1, &stone_up1, SRCINVERT);	//精灵图	
		putimage(stone_x1, stone_y1 + 750, &stone_down2, NOTSRCERASE);
		putimage(stone_x1, stone_y1 + 750, &stone_down1, SRCINVERT);
		putimage(stone_x2, stone_y2, &stone_up4, NOTSRCERASE);
		putimage(stone_x2, stone_y2, &stone_up3, SRCINVERT);
		putimage(stone_x2, stone_y2 + 750, &stone_down4, NOTSRCERASE);
		putimage(stone_x2, stone_y2 + 750, &stone_down3, SRCINVERT);
		stone_x1--; stone_x2--;
	}
	else if (stone_x1 == 210)//左柱子消失，将stone_x2,stone_y2值归位
	{
		stone_x2 = stone_x1;
		stone_y2 = stone_y1;
		putimage(stone_x1, stone_y1, &stone_up2, NOTSRCERASE);
		putimage(stone_x1, stone_y1, &stone_up1, SRCINVERT);
		putimage(stone_x1, stone_y1 + 750, &stone_down2, NOTSRCERASE);
		putimage(stone_x1, stone_y1 + 750, &stone_down1, SRCINVERT);
		stone_x1--;
		stone_x2--;
	}
	else if (stone_x1 < 210 && stone_x1>0)//画面只存在一根柱子的情况
	{
		putimage(stone_x1, stone_y1, &stone_up2, NOTSRCERASE);
		putimage(stone_x1, stone_y1, &stone_up1, SRCINVERT);
		putimage(stone_x1, stone_y1 + 750, &stone_down2, NOTSRCERASE);
		putimage(stone_x1, stone_y1 + 750, &stone_down1, SRCINVERT);
		stone_x1--;
		stone_x2--;
	}
	if (stone_x1 == 0)//柱子左端end，生成新柱子
	{
		stone_y1 = rand() % 310 - 600 + 50;
		stone_x1 = 350;
		putimage(stone_x1, stone_y1, &stone_up2, NOTSRCERASE);
		putimage(stone_x1, stone_y1, &stone_up1, SRCINVERT);
		putimage(stone_x1, stone_y1 + 600 + 150, &stone_down2, NOTSRCERASE);
		putimage(stone_x1, stone_y1 + 750, &stone_down1, SRCINVERT);
		stone_x1--;
		stone_x2--;
	}

}

void score_print()
{
	IMAGE *fen1[6], *fen2[6];
	int x0, weishu = 1, i = 0, n = score;
	if (n == 0)
	{
		putimage(250, 50, &score1[0], NOTSRCERASE);
		putimage(250, 50, &score2[0], SRCINVERT);

	}
	while (n > 0)
	{
		x0 = n % 10;
		fen1[i] = &score1[x0];
		fen2[i] = &score2[x0];
		putimage(300 - 50 * weishu, 50, fen1[i], NOTSRCERASE);
		putimage(300 - 50 * weishu, 50, fen2[i], SRCINVERT);
		n /= 10;
		i++;
		weishu++;
	}

}


void bird_jump()
{
	// 控制小鸟上升和下降
	static int control = 0; // 控制变量
	if (control > 0 && control < 5) { // 控制小鸟逐渐上升
		bird_y -= 10;
		++control;
		return;
	}
	else if (control == 5) {
		control = 0;
	}
	bird_y += 3; // 下落
	int key;
	if (_kbhit()) {
		key = _getch();
		if (key == 32) { // 空格跳跃
			bird_y -= 10;
			++control;
		}
		else if (key == 27) { // ESC暂停
			system("pause");
		}
	}
	while (peekmessage(&m, EM_MOUSE)) 
	{
		if (m.message == WM_LBUTTONDOWN) 
		{ // 鼠标左键
			bird_y -= 10;
			++control;
		}
		else if (m.message == WM_RBUTTONDOWN) { // 鼠标右键
			system("pause");
		}
	}
}


void hit_detection()
{
	if ((stone_x1 > 10 && stone_x1 < 20) || (stone_x1 > 174 && stone_x1 < 184))
	{
		if ((bird_y > (stone_y1 + 576) && bird_y < (stone_y1 + 600)) || ((bird_y > (stone_y1 + 726)) && bird_y < (stone_y1 + 750)))
		{
			endorretry();//结束游戏
		}
	}
	else if (stone_x1 >= 20 && stone_x1 <= 174)
	{
		if (!(bird_y > (stone_y1 + 600) && bird_y < (stone_y1 + 726)))
		{
			endorretry();	//结束游戏
		}
	}
	if (bird_y > 576)
	{
		endorretry();//结束游戏
	}
	if (stone_x1 == 150)
	{
		score++;
	}
}

void endorretry()
{
	if (bird_y < 550 && stone_x1>160)
	{
		while (bird_y < 550)
		{
			putimage(0, 0, &background);//背景图像
			putimage(stone_x1, stone_y1, &stone_up2, NOTSRCERASE);
			putimage(stone_x1, stone_y1, &stone_up1, SRCINVERT);
			putimage(stone_x1, stone_y1 + 750, &stone_down2, NOTSRCERASE);
			putimage(stone_x1, stone_y1 + 750, &stone_down1, SRCINVERT);
			putimage(stone_x2, stone_y2, &stone_up4, NOTSRCERASE);
			putimage(stone_x2, stone_y2, &stone_up3, SRCINVERT);
			putimage(stone_x2, stone_y2 + 750, &stone_down4, NOTSRCERASE);
			putimage(stone_x2, stone_y2 + 750, &stone_down3, SRCINVERT);
			putimage(bird_x, bird_y, &bird1[3], NOTSRCERASE);
			putimage(bird_x, bird_y, &bird2[3], SRCINVERT);
			FlushBatchDraw();
			bird_y++;
		}
	}
	IMAGE gameover1, gameover2, atlast;
	loadimage(&gameover1, "./source/gameover1.gif");
	loadimage(&gameover2, "./source/gameover2.gif");
	putimage(80, 200, &gameover1, NOTSRCERASE);
	putimage(80, 200, &gameover2, SRCINVERT);
	FlushBatchDraw();
	Sleep(1000);
	putimage(0, 0, &background);
	score_print();
	FlushBatchDraw();
	system("pause");
	//第一根柱子
	stone_y1 = rand() % 310 - 555;
	stone_x1 = 350;
	stone_x2 = stone_y2 = -9999;
	bird_y = 300;
	score = 0;
	play();

}


void _rank()
{
	//改变排行榜按钮
	loadimage(&ranking, "./source/rank_2.png");//载入排行榜按钮
	putimage((350 - ranking.getwidth()) / 2, 300, &ranking);//放入排行榜按钮
	PlaySound(TEXT("./source/click.wav"), NULL, SND_FILENAME);

	putimage(0, 0, &background);


	back();

}

void set()
{
	//改变设置按钮
	loadimage(&setting, "./source/setting_2.png");//载入设置按钮
	putimage((350 - setting.getwidth()) / 2, 370, &setting);//放入设置按钮
	PlaySound(TEXT("./source/click.wav"), NULL, SND_FILENAME);
	putimage(0, 0, &background);
	putimage(250, 550, &goback);//放入后退按钮
	//在设置界面添加字
	settextstyle(25, 0, "黑体");
	settextcolor(WHITE);

	outtextxy(75, 200, "游戏音效：");
	outtextxy(75, 300, "背景音乐：");


	back();

}

void show()
{
	//改变游戏玩法按钮
	loadimage(&showing, "./source/show_2.png");//载入游戏玩法按钮
	putimage((350 - showing.getwidth()) / 2, 440, &showing);//放入游戏玩法按钮
	PlaySound(TEXT("./source/click.wav"), NULL, SND_FILENAME);

	putimage(0, 0, &background);

	//设计游戏玩法界面
	loadimage(&showbackground, "./source/show_background.jpg", 280, 280, true);
	putimage(35, 20, &showbackground);
	loadimage(&showbackground, "./source/show_background.jpg", 280, 200, true);
	putimage(35, 340, &showbackground);

	loadimage(&showbackground, "./source/show_click_mask.jpg", 0, 0, true);
	putimage(50, 50, &showbackground, SRCAND);//and掩码
	loadimage(&showbackground, "./source/show_click.png", 0, 0, true);
	putimage(50, 50, &showbackground, SRCPAINT);//or原照片

	loadimage(&showbackground, "./source/show_space1.png", 0, 0, true);
	putimage(205, 80, &showbackground);

	loadimage(&showbackground, "./source/show_esc.png", 0, 0, true);
	putimage(140, 400, &showbackground);

	settextstyle(25, 0, "黑体");
	settextcolor(BLACK);
	Sleep(20);
	outtextxy(80, 200, "跳跃： 空格/左键");
	outtextxy(111, 470, "暂停： Esc");


	back();
}


void back()//返回到主界面
{
	putimage(250, 550, &goback);//放入后退按钮
	cout << "back" << endl;
	while (1)
	{
		m = getmessage();
		if (m.message == WM_LBUTTONDOWN)   //如果鼠标左键按下了
		{ // 鼠标左键
			if (m.x >= 250 && m.x <= 330 && m.y >= 550 && m.y <= 578) //back图片存放的区域
			{
				loadimage(&goback, "./source/back_2.png");//载入后退按钮
				putimage(250, 550, &goback);				//放入后退按钮
				PlaySound(TEXT("./source/click.wav"), NULL, SND_FILENAME);
				break;
			}
		}
	}
}


DWORD WINAPI init_dynamic(LPVOID)  //子线程（雪花飞舞）会进入这个函数
{
	while (1)
	{
		BeginBatchDraw(); //这个函数放置的位置比较重要，他和endbatchdraw是个组合。他们之间出现的绘图会一起出现。
		init();				//先是所有的背景图和按钮出现
		for (int i = 0; i < MAXSNOW; i++)	//所有的雪花位置更新完毕
		{
			MoveSnow(i);
		}
		EndBatchDraw();	//这里就达到了雪花和背景同时出现的效果。雪花是在最上层
		if (flag_init)		//子线程会重复循环，直到检测到用户按下按钮了->return 0。
			return 0;
	}
	return 1;
}



int main()
{
	srand((unsigned)time(NULL));	// 随机种子

	//加载背景音效，这里没有用playsound是因为，如果后续也用playsound实现单击音效。背景音乐将会自动停止。
	//mciSendString("play ./source/bkmusic.mp3 repeat", NULL, 0, NULL);

	initgraph(350, 600, EW_SHOWCONSOLE);
	HWND hwnd = GetHWnd();
	SetWindowText(hwnd, "Flappy Bird");


	// 初始化所有雪花
	for (int i = 0; i < MAXSNOW; i++)
	{
		InitSnow(i);
		snow[i].x = rand() % 350;//这里注意到，一开始的x是随机的，后续的才是从最左边出来的。所以刚进游戏界面是满屏的雪花。
	}




	//字体在主函数设置，后续就不需要再改了
	settextstyle(25, 0, "黑体");
	setbkmode(TRANSPARENT);//设置字体背景透明（输入汉字是有背景的）

	loadimage(&bird2[0], "./source/bird1-2.gif");
	loadimage(&bird1[0], "./source/bird1-1.gif");
	loadimage(&bird2[1], "./source/bird2-2.gif");
	loadimage(&bird1[1], "./source/bird2-1.gif");
	loadimage(&bird2[2], "./source/bird3-2.gif");
	loadimage(&bird1[2], "./source/bird3-1.gif");
	loadimage(&bird2[3], "./source/bird4-2.gif");
	loadimage(&bird1[3], "./source/bird4-1.gif");
	//柱子
	loadimage(&stone_up1, "./source/stone_up1.gif");
	loadimage(&stone_up2, "./source/stone_up2.gif");
	loadimage(&stone_down1, "./source/stone_down1.gif");
	loadimage(&stone_down2, "./source/stone_down2.gif");
	loadimage(&stone_up3, "./source/stone_up1.gif");    //代表第组柱子
	loadimage(&stone_up4, "./source/stone_up2.gif");
	loadimage(&stone_down3, "./source/stone_down1.gif");
	loadimage(&stone_down4, "./source/stone_down2.gif");

	//数字
	loadimage(&score1[0], "./source/0_1.jpg");
	loadimage(&score2[0], "./source/0_2.jpg");
	loadimage(&score1[1], "./source/1_1.jpg");
	loadimage(&score2[1], "./source/1_2.jpg");
	loadimage(&score1[2], "./source/2_1.jpg");
	loadimage(&score2[2], "./source/2_2.jpg");
	loadimage(&score1[3], "./source/3_1.jpg");
	loadimage(&score2[3], "./source/3_2.jpg");
	loadimage(&score1[4], "./source/4_1.jpg");
	loadimage(&score2[4], "./source/4_2.jpg");
	loadimage(&score1[5], "./source/5_1.jpg");
	loadimage(&score2[5], "./source/5_2.jpg");
	loadimage(&score1[6], "./source/6_1.jpg");
	loadimage(&score2[6], "./source/6_2.jpg");
	loadimage(&score1[7], "./source/7_1.jpg");
	loadimage(&score2[7], "./source/7_2.jpg");
	loadimage(&score1[8], "./source/8_1.jpg");
	loadimage(&score2[8], "./source/8_2.jpg");
	loadimage(&score1[9], "./source/9_1.jpg");
	loadimage(&score2[9], "./source/9_2.jpg");

	while (1)
	{
		//创建主界面线程
		DWORD  init_threadId;

		flag_init = 0; //循环回到起点，说明用户返回主界面了，需要把flag和option置0
		option = 0;

		CreateThread(NULL, 0, init_dynamic, 0, 0, &init_threadId); //每次循环都创建线程（之前的都已经return 0，自动死亡了)，
																			  //子线程进入到ThreadFunc_dynamic函数
																			  //父线程继续向下执行
		option = choose();//父线程阻塞在这里，等待用户单击按钮

		switch (option)
		{
		case 1:
			play(); break;
		case 2:
			_rank(); break;
		case 3:
			set(); break;
		case 4:
			show(); break;
		}
	}
	system("pause");
	return 0;
}
