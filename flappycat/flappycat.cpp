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



//ȫ�ֺ���
void print_obstacle(); // �����ϰ����λ��
void hit_detection();	// ���С���Ƿ�ײ�����ϰ���
void bird_jump();		//��������½�������
void score_print();		//������Ϸ�����ϵķ�����ʾ
void endorretry();		//��Ϸ����


//����ӵĺ���
void print();			//������Ϸ��������ͼ��
void login();			//��¼
void init();			//���������������ֺͱ���
int choose();			//ѡ��
void play();			//������Ϸ
void _rank();			//�������а�
void set();				//��������
void show();			//������Ϸ�淨
void back();			//���ص�������

DWORD WINAPI init_dynamic(LPVOID);


//ȫ�ֱ���
string nickname = "";   // ����ǳ�
int option = 0;			//���ѡ���ĸ�ѡ��
int flag_init = 0;			//0��ʾ�����汳�������̣߳�������

int bird_x = 150, bird_y = 300, i = 0, k = 0     ;//������Ͻ�����,i��image bird1[]��Ԫ��λ�ã�k�ǵ�5��0��ʵ��С�����ѭ��
int score = 0, t = 0;	//socre������  t��
int stone_x1, stone_y1;	//�Ͻ��������½�����
int stone_x2 = -800, stone_y2;	//�½��������Ͻ�����

IMAGE  title, start, ranking, setting,showing,background,showbackground, goback,  //��Щ�����������¼ӵ�
bird1[4], bird2[4], score1[10], score2[10], stone_up1, stone_up2, stone_down1, stone_down2
, stone_up3, stone_up4, stone_down3, stone_down4;//ͼƬ�������


ExMessage m;		// ���������Ϣ


//�������ѩ�������Բο�easyx�����ĵ��������ʾ��������

#define MAXSNOW 200	// ѩ������
struct SNOW
{
	double	x;    //����
	int		y;
	double	step; //������ÿ��ˢ���ƶ��ĺ������
};

SNOW snow[MAXSNOW]; //����ȫ�ֱ���snow

// ��ʼ��ѩ��
void InitSnow(int i)
{
	snow[i].x = 0;               //һ��ʼx������0����Ϊ�������£�ѩ�����Ǵ������ҿ�ʼ�ߣ�һ��ʼ���ǵ�
	snow[i].y = rand() % 600;
	snow[i].step = (rand() % 5000) / 4000.0 + 1;  //����
}

// �ƶ�ѩ��
void MoveSnow(int i)
{
	snow[i].x += snow[i].step;              //����movesnow���ȸ���һ��ÿ��ѩ����λ��
	if (snow[i].x > 350)	InitSnow(i);    //����������󣬴���������³���

	// ����ѩ��
	fillcircle((int)snow[i].x, snow[i].y, 1); //���ǻ���Ȧ
}

void init() //���������汳��
{
	loadimage(&background, "./source/initground.jpg");//���뱳��ͼ
	loadimage(&title, "./source/title.png", 270, 60);//�������ͼƬ
	loadimage(&start, "./source/start_1.png");//���뿪ʼ��ť
	loadimage(&ranking, "./source/rank_1.png");//�������а�ť
	loadimage(&setting, "./source/setting_1.png");//�������ð�ť
	loadimage(&showing, "./source/show_1.png");//������Ϸ�淨��ť
	loadimage(&goback, "./source/back_1.png");//������˰�ť

	putimage(0, 0, &background);//���뱳��ͼ
	putimage((350 - title.getwidth()) / 2, 100, &title, SRCPAINT);  //�������ͼƬ,�뱳��ȡor
	putimage((350 - start.getwidth()) / 2, 230, &start);//���뿪ʼ��ť
	putimage((350 - ranking.getwidth()) / 2, 300, &ranking);//�������а�ť
	putimage((350 - setting.getwidth()) / 2, 370, &setting);//�������ð�ť
	putimage((350 - showing.getwidth()) / 2, 440, &showing);//������Ϸ�淨��ť*/


	/*if (t/100 < 255)      //������������ʵ������ѩ���仯��ɫ����֪Ϊ��û��Ч��
	{
		setfillcolor(t/100);
		t++;
	}
	else
	{
		t = 0;
	}*/

}

int choose()    //�����Ǹ���ѭ����׽����¼��ĺ��������Իῼ�����߳�
{
	int n = 0;
	while (1)
	{
		m = getmessage();
		if (m.message == WM_LBUTTONDOWN)   //���������������
		{
			if (m.x >= 123 && m.x <= 227 && m.y >= 230 && m.y <= 286) n = 1;	//��ʼ
			if (m.x >= 123 && m.x <= 227 && m.y >= 300 && m.y <= 357) n = 2;	//���а�
			if (m.x >= 123 && m.x <= 227 && m.y >= 370 && m.y <= 426) n = 3;	//����
			if (m.x >= 123 && m.x <= 227 && m.y >= 440 && m.y <= 496) n = 4;	//��Ϸ˵��

			if (n) //ֻ��n������0��ʱ��˵����갴������һ����ť����������ѭ��
			{

				flag_init = 1;  //��flag��1���������̣߳���Ҫֹͣ��
				Sleep(10); //����ط��ȽϹؼ�����Ϊ���߳���ÿ��һ��ʱ����һ��flag������Ҹհ�flag��1��return���������̻߳�û���ü�ֹͣ���ͻ�����
				return n;
			}
		}
	}

}

/*
void login() {
	loadimage(&ranking, "./source/confirm2.png"); // ȷ����ͼƬ
	putimage(60, 350, &ranking, SRCPAINT);
	loadimage(&ranking, "./source/skip2.png"); // ������ͼƬ
	putimage(220, 350, &ranking, SRCPAINT);
	putimage(250, 550, &goback); // ������˰�ť
	RECT r = { 30, 0, 320, 479 }; // ����λ��
	RECT rcin = { 100, 0, 320, 550 }; // ������ǳ�λ��
	drawtext(_T("��������Ϸ�ǳ�:"), &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	while (1) {
		flushmessage();
		m = getmessage(EM_KEY | EM_CHAR | EM_MOUSE); // ���̺�����¼����
		if (m.message == WM_KEYDOWN || m.message == WM_CHAR) {
			if ((m.vkcode >= 48 && m.vkcode <= 57) || (m.vkcode >= 65 && m.vkcode <= 90)) { // �ж�Ϊ��ĸ���־������ǳ���
				nickname += tolower(m.ch);
				drawtext(_T(nickname.data()), &rcin, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			}
			else if (m.vkcode == 13) { // �س�ȷ��
				if (nickname == "") nickname = "admin";
				drawtext(_T(nickname.data()), &rcin, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				break; // ��Ķ�
			}
			else if (m.vkcode == 8) { // �˻�һ���ַ�
				// ���¼���ͼƬ������
				putimage(0, 0, &background);
				loadimage(&ranking, "./source/confirm2.png"); // ȷ����ͼƬ
				putimage(60, 350, &ranking, SRCPAINT);
				loadimage(&ranking, "./source/skip2.png"); // ������ͼƬ
				putimage(220, 350, &ranking, SRCPAINT);
				putimage(250, 550, &goback); //������˰�ť
				drawtext(_T("��������Ϸ�ǳ�:"), &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				nickname = nickname.substr(0, nickname.length() - 1);
				flushmessage();
				drawtext(_T(nickname.data()), &rcin, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			}
		}
		else if (m.message == WM_LBUTTONDOWN)   //���������������
		{
			if (m.x >= 250 && m.x <= 330 && m.y >= 550 && m.y <= 578) //backͼƬ��ŵ�����
			{
				loadimage(&goback, "./source/back_2.png");//������˰�ť
				putimage(250, 550, &goback);				//������˰�ť
				PlaySound(TEXT("./source/click.wav"), NULL, SND_FILENAME);
				break;// ��Ķ�
			}
			else if (m.x >= 60 && m.x <= 147 && m.y >= 350 && m.y <= 393) { // ���ȷ��
				loadimage(&ranking, "./source/confirm2.png"); // ȷ����ͼƬ
				putimage(60, 350, &ranking, SRCINVERT);
				loadimage(&ranking, "./source/confirm.png"); // ȷ����ͼƬ
				putimage(60, 350, &ranking, SRCPAINT);
				PlaySound(TEXT("./source/click.wav"), NULL, SND_FILENAME);
				break;// ��Ķ�
			}
			else if (m.x >= 220 && m.x <= 304 && m.y >= 350 && m.y <= 397) { // �������
				nickname = "admin";
				loadimage(&ranking, "./source/skip2.png"); // ������ͼƬ
				putimage(220, 350, &ranking, SRCINVERT);
				loadimage(&ranking, "./source/skip.png"); // ������ͼƬ
				putimage(220, 350, &ranking, SRCPAINT);
				PlaySound(TEXT("./source/click.wav"), NULL, SND_FILENAME);
				break;// ��Ķ�
			}
		}
	}
	// ����жϡ�����
}
*/

void play()
{
	//�ı俪ʼ��ť
	loadimage(&start, "./source/start_2.png");			//���뿪ʼ��ť
	putimage((350 - start.getwidth()) / 2, 230, &start);//���뿪ʼ��ť
	PlaySound(TEXT("./source/click.wav"), NULL, SND_FILENAME); //ʵ�����ֲ���������������playsound����Ч�ʸ��ߣ�����ʹ�ã���һ����mciSendString��������������
	/*putimage(0, 0, &background);
	login();
	_getch();
	*/
	m = getmessage();
	while (1)
	{
		print();
		hit_detection();
		bird_jump();
		if (m.message == WM_LBUTTONDOWN)   //���������������
		{ // ������
			if (m.x >= 250 && m.x <= 330 && m.y >= 550 && m.y <= 578) //backͼƬ��ŵ�����
			{
				loadimage(&goback, "./source/back_2.png");//������˰�ť
				putimage(250, 550, &goback);				//������˰�ť
				PlaySound(TEXT("./source/click.wav"), NULL, SND_FILENAME);
				break;
			}
		}
	}
}

void print()
{
	BeginBatchDraw();// ����������ͼģʽ
	putimage(0, 0, &background);//����ͼ��
	putimage(250, 550, &goback);//������˰�ť
	print_obstacle();//������


	putimage(bird_x, bird_y, &bird1[i % 3], NOTSRCERASE); // ������Ͻ����� bird_x = 150, bird_y = 300, i = 0, k = 0 ��k�ǵ�5��0��ʵ��С�����ѭ��
	putimage(bird_x, bird_y, &bird2[i % 3], SRCINVERT);

	if (k % 5 == 0)
		i++;
	k++;        //����Ϊֹ��ʵ����С��λ�õĸ���

	score_print();
	putimage(250, 550, &goback);//������˰�ť
	//EndBatchDraw();
	FlushBatchDraw();		// ����	

}

void print_obstacle()//�����ƶ�����
{
	/*
	stone_x1 = 0; stone_y1 = 0;
	cout << stone_up1.getwidth() << "  " << stone_up1.getheight() << endl;    //140 600
	putimage(stone_x1, stone_y1, &stone_up1);  //����ͼ  �Ͻ�������������һ��ʼδ��ʼ��
	FlushBatchDraw();		// ����
	_getch();
	*/
	Sleep(30);
	if (stone_x1 > 210)//��ʱ���������������
	{
		//����դ����
		putimage(stone_x1, stone_y1, &stone_up2, NOTSRCERASE);  //����ͼ  �Ͻ�������������һ��ʼδ��ʼ��stone_x1, stone_y1:�Ͻ�������������    stone_x2, stone_y2:�Ͻ�������������
		putimage(stone_x1, stone_y1, &stone_up1, SRCINVERT);	//����ͼ	
		putimage(stone_x1, stone_y1 + 750, &stone_down2, NOTSRCERASE);
		putimage(stone_x1, stone_y1 + 750, &stone_down1, SRCINVERT);
		putimage(stone_x2, stone_y2, &stone_up4, NOTSRCERASE);
		putimage(stone_x2, stone_y2, &stone_up3, SRCINVERT);
		putimage(stone_x2, stone_y2 + 750, &stone_down4, NOTSRCERASE);
		putimage(stone_x2, stone_y2 + 750, &stone_down3, SRCINVERT);
		stone_x1--; stone_x2--;
	}
	else if (stone_x1 == 210)//��������ʧ����stone_x2,stone_y2ֵ��λ
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
	else if (stone_x1 < 210 && stone_x1>0)//����ֻ����һ�����ӵ����
	{
		putimage(stone_x1, stone_y1, &stone_up2, NOTSRCERASE);
		putimage(stone_x1, stone_y1, &stone_up1, SRCINVERT);
		putimage(stone_x1, stone_y1 + 750, &stone_down2, NOTSRCERASE);
		putimage(stone_x1, stone_y1 + 750, &stone_down1, SRCINVERT);
		stone_x1--;
		stone_x2--;
	}
	if (stone_x1 == 0)//�������end������������
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

	// ����С���������½�
	static int control = 0; // ���Ʊ���
	if (control > 0 && control < 5) { // ����С��������
		bird_y -= 10;
		++control;
		return;
	}
	else if (control == 5) {
		control = 0;
	}
	bird_y += 3; // ����
	int key;
	if (_kbhit()) {
		key = _getch();
		if (key == 32) { // �ո���Ծ
			bird_y -= 10;
			++control;
		}
		else if (key == 27) { // ESC��ͣ
			system("pause");
		}
	}
	while (peekmessage(&m, EM_MOUSE)) 
	{
		if (m.message == WM_LBUTTONDOWN) 
		{ // ������
			bird_y -= 10;
			++control;
		}
		else if (m.message == WM_RBUTTONDOWN) { // ����Ҽ�
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
			endorretry();//������Ϸ
		}
	}
	else if (stone_x1 >= 20 && stone_x1 <= 174)
	{
		if (!(bird_y > (stone_y1 + 600) && bird_y < (stone_y1 + 726)))
		{
			endorretry();	//������Ϸ
		}
	}
	if (bird_y > 576)
	{
		endorretry();//������Ϸ
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
			putimage(0, 0, &background);//����ͼ��
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
	_getch();
	//��һ������
	stone_y1 = rand() % 310 - 555;
	stone_x1 = 350;
	stone_x2 = stone_y2 = -9999;
	bird_y = 300;
	score = 0;
	play();

}


void _rank()
{
	//�ı����а�ť
	loadimage(&ranking, "./source/rank_2.png");//�������а�ť
	putimage((350 - ranking.getwidth()) / 2, 300, &ranking);//�������а�ť
	PlaySound(TEXT("./source/click.wav"), NULL, SND_FILENAME);

	putimage(0, 0, &background);


	back();

}

void set()
{
	//�ı����ð�ť
	loadimage(&setting, "./source/setting_2.png");//�������ð�ť
	putimage((350 - setting.getwidth()) / 2, 370, &setting);//�������ð�ť
	PlaySound(TEXT("./source/click.wav"), NULL, SND_FILENAME);
	putimage(0, 0, &background);
	putimage(250, 550, &goback);//������˰�ť
	//�����ý��������
	settextstyle(25, 0, "����");
	settextcolor(WHITE);

	outtextxy(75, 200, "��Ϸ��Ч��");
	outtextxy(75, 300, "�������֣�");


	back();

}

void show()
{
	//�ı���Ϸ�淨��ť
	loadimage(&showing, "./source/show_2.png");//������Ϸ�淨��ť
	putimage((350 - showing.getwidth()) / 2, 440, &showing);//������Ϸ�淨��ť
	PlaySound(TEXT("./source/click.wav"), NULL, SND_FILENAME);

	putimage(0, 0, &background);

	//�����Ϸ�淨����
	loadimage(&showbackground, "./source/show_background.jpg", 280, 280, true);
	putimage(35, 20, &showbackground);
	loadimage(&showbackground, "./source/show_background.jpg", 280, 200, true);
	putimage(35, 340, &showbackground);

	loadimage(&showbackground, "./source/show_click_mask.jpg", 0, 0, true);
	putimage(50, 50, &showbackground, SRCAND);//and����
	loadimage(&showbackground, "./source/show_click.png", 0, 0, true);
	putimage(50, 50, &showbackground, SRCPAINT);//orԭ��Ƭ

	loadimage(&showbackground, "./source/show_space1.png", 0, 0, true);
	putimage(205, 80, &showbackground);

	loadimage(&showbackground, "./source/show_esc.png", 0, 0, true);
	putimage(140, 400, &showbackground);

	settextstyle(25, 0, "����");
	settextcolor(BLACK);
	Sleep(20);
	outtextxy(80, 200, "��Ծ�� �ո�/���");
	outtextxy(111, 470, "��ͣ�� Esc");


	back();
}


void back()//���ص�������
{
	putimage(250, 550, &goback);//������˰�ť
	cout << "back" << endl;
	while (1)
	{
		m = getmessage();
		if (m.message == WM_LBUTTONDOWN)   //���������������
		{ // ������
			if (m.x >= 250 && m.x <= 330 && m.y >= 550 && m.y <= 578) //backͼƬ��ŵ�����
			{
				loadimage(&goback, "./source/back_2.png");//������˰�ť
				putimage(250, 550, &goback);				//������˰�ť
				PlaySound(TEXT("./source/click.wav"), NULL, SND_FILENAME);
				break;
			}
		}
	}
}


DWORD WINAPI init_dynamic(LPVOID)  //���̣߳�ѩ�����裩������������
{
	while (1)
	{
		BeginBatchDraw(); //����������õ�λ�ñȽ���Ҫ������endbatchdraw�Ǹ���ϡ�����֮����ֵĻ�ͼ��һ����֡�
		init();				//�������еı���ͼ�Ͱ�ť����
		for (int i = 0; i < MAXSNOW; i++)	//���е�ѩ��λ�ø������
		{
			MoveSnow(i);
		}
		EndBatchDraw();	//����ʹﵽ��ѩ���ͱ���ͬʱ���ֵ�Ч����ѩ���������ϲ�
		if (flag_init)		//���̻߳��ظ�ѭ����ֱ����⵽�û����°�ť��->return 0��
			return 0;
	}
	return 1;
}



int main()
{
	srand((unsigned)time(NULL));	// �������

	//���ر�����Ч������û����playsound����Ϊ���������Ҳ��playsoundʵ�ֵ�����Ч���������ֽ����Զ�ֹͣ��
	//mciSendString("play ./source/bkmusic.mp3 repeat", NULL, 0, NULL);

	initgraph(350, 600, EW_SHOWCONSOLE);
	HWND hwnd = GetHWnd();
	SetWindowText(hwnd, "Flappy Bird");


	// ��ʼ������ѩ��
	for (int i = 0; i < MAXSNOW; i++)
	{
		InitSnow(i);
		snow[i].x = rand() % 350;//����ע�⵽��һ��ʼ��x������ģ������Ĳ��Ǵ�����߳����ġ����Ըս���Ϸ������������ѩ����
	}




	//���������������ã������Ͳ���Ҫ�ٸ���
	settextstyle(25, 0, "����");
	setbkmode(TRANSPARENT);//�������屳��͸�������뺺�����б����ģ�

	loadimage(&bird2[0], "./source/bird1-2.gif");
	loadimage(&bird1[0], "./source/bird1-1.gif");
	loadimage(&bird2[1], "./source/bird2-2.gif");
	loadimage(&bird1[1], "./source/bird2-1.gif");
	loadimage(&bird2[2], "./source/bird3-2.gif");
	loadimage(&bird1[2], "./source/bird3-1.gif");
	loadimage(&bird2[3], "./source/bird4-2.gif");
	loadimage(&bird1[3], "./source/bird4-1.gif");
	//����
	loadimage(&stone_up1, "./source/stone_up1.gif");
	loadimage(&stone_up2, "./source/stone_up2.gif");
	loadimage(&stone_down1, "./source/stone_down1.gif");
	loadimage(&stone_down2, "./source/stone_down2.gif");
	loadimage(&stone_up3, "./source/stone_up1.gif");    //�����������
	loadimage(&stone_up4, "./source/stone_up2.gif");
	loadimage(&stone_down3, "./source/stone_down1.gif");
	loadimage(&stone_down4, "./source/stone_down2.gif");

	//����
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
		//�����������߳�
		DWORD  init_threadId;

		flag_init = 0; //ѭ���ص���㣬˵���û������������ˣ���Ҫ��flag��option��0
		option = 0;

		CreateThread(NULL, 0, init_dynamic, 0, 0, &init_threadId); //ÿ��ѭ���������̣߳�֮ǰ�Ķ��Ѿ�return 0���Զ�������)��
																			  //���߳̽��뵽ThreadFunc_dynamic����
																			  //���̼߳�������ִ��
		option = choose();//���߳�����������ȴ��û�������ť

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
