/********************************��Ϸ˵��**********************************
	
	1ؼ�ؿ�ֻ�����ð��ģʽ�ĵ�һ�أ������˳��ͨ���ˣ���ϲ�㣬��Ϸ�����ˡ�
	2ؼ������ֻ��ǰ�������ã����������ô�ֻ���Ǳ���ʬ�ԣ�100Ѫ��
	3ؼż���������������������һ�£����þ��ţ�����bug��ֻ����������ˡ�


*/

/*���������
1ؼ���ó��ֵ�������˸���¾�û��
2ؼͦ��ģ�������


***************************************************************************/
#include<stdio.h>
#include<Windows.h>
#include<graphics.h>
#include<conio.h>
#include<time.h>
#include<mmsystem.h>
#include<math.h>
#include "tools.h"
#include"vector2.h"

#pragma comment(lib,"winmm.lib")

#define WIN_WIDTH 900
#define WIN_HEIGHT 550


struct plant
{
	int type;
	int frameindex;
	int row,col;
	int X, Y;
	bool catching;
	int blood;
	bool changed;
	int changeframeindex;
	int timer;//�����տ�ÿ��������һ������
};
struct sunshine
{
	int X, Y;//�����λ��
	int frameindex;//ÿ֡����ͼƬ�����
	bool used;//�����Ƿ��Ѿ���ʾ
	int timer;
	float t;//0��1���������߽���
	vector2 p1, p2, p3, p4;//�ĸ�������ȷ�����߹켣
	vector2 pCur;
	float speed;
	int status;

};

struct zb    //zombie��ʬ
{
	int X, Y;
	int frameindex;
	int diedframeindex;
	bool used;
	int row, col;
	int blood;
	bool died;
	bool eating;
	int eatingframeindex;


};

struct bl  //bullet�ӵ�
{
	int X, Y;
	bool used;
	int row;
	int speed;
	bool boomed;
	int frameindex;

};

struct card//��ɫ����
{
	int overcooldown;//�Ƿ���ȴ���
	int cooldowntime;//��ȴʱ��
	int X, Y;//��ɫ���ƿ�ȣ��߶�
	int speed;//��ȴˢ���ٶ�
	bool enoughsunball;
};

struct tp//����
{
	vector2 p1, p2, p3, p4;
	int frame;
	int biggerframe;	
};

struct pbar//������
{
	int X, Y;
	int imagewidth, imageheight;
};

enum {OPEN_WIN,UI_WIN,ADVENTURE_WIN,LITTLEGAME_WIN,PUZZLE_WIN,SURVIVAL_WIN,
	}gamewindow;
enum { WAN_DOU, XIANG_RI_KUI,XIAO_JIAN_GUO,YING_TAO_ZHA_DAN,HUO_BAO_LA_JIAO,SHI_REN_HUA, 
	BING_ZHEN_GU,HUI_MIE_GU,HUO_JU_SHU_ZHUANG,XIANG_JIAO,SHUI_JING_KE,JI_QIANG_SHE_SHOU,
	COUNT };
enum { SUNSHINE_DOWN, SUNSHINE_STAY, SUNSHINE_COLLECT, SUNSHINE_CREAT };


IMAGE backgroundpic;//����ͼ���������͵ı���
IMAGE boardpic;
IMAGE cardpic[COUNT];
IMAGE cardgraypic[COUNT];
IMAGE* plantpic[COUNT][20];
IMAGE* plantchangepic[COUNT][20];
IMAGE sunballpic[30];
IMAGE zombiepic[22];
IMAGE zombiediedpic[20];
IMAGE zombieeating[21];
IMAGE bulletoriginal;
IMAGE bulletboom[4];
IMAGE elsepic[20];
IMAGE zombiestandpic[11];
IMAGE shovelpic;
IMAGE shovelbackpic;//����ͼƬ
IMAGE shovelmovepic;
IMAGE trophypic[20];//����ͼƬ
IMAGE trophybigpic[20];//��󽱱�ͼƬ
IMAGE trophybiggerpic[100];//���ڱ�󽱱�ͼƬ
IMAGE pointerdownpic;
IMAGE progressbar[3];
IMAGE zombiehead;
IMAGE flagvertical;
IMAGE manyzombiescomingpic;
IMAGE lastmanyzombiespic;

int game_window;
int zombieheadx = 0, zombieheady = 0;//��������ʬͷ��λ��
int zhunbeiframe = 0;
int curX, curY;
int curindex=0;
const int ballMax = 20;
int sunnumber;//�����ʼֵ
const int pool_zombieMax = 100;
int firstpass_zombieMax = 50;//�޸ĵ�һ�ؽ�ʬ��������
int appeared_zombie = 0;//Ŀǰ�Ѿ������˵Ľ�ʬ��
int died_zombie = 0;//Ŀǰ�Ѿ������˵Ľ�ʬ��
const int bulletMax = 500;
int plantmoney[20] = {100,50,50,150,125,150,75,200,100,50,50,250};
char passionstartmusic[1024];
bool hugewavewords = 0;
bool lastwavewords = 0;
int hugewordstime=0;
int lastwordstime = 0;
int zombiescomingtime = 0;//һ�󲨽�ʬ�볡ʱ��


struct plant map[5][9];//5��9�е�ֲ��
struct sunshine sunball[20];//��ʮ������sunball[i]
struct zb zombie[100];//һ�ٸ���ʬzombie[i]
struct bl bullet[500];//һ�ٿ��ӵ�bullet[i]
struct card cards[20];//20�Ż�ɫ����
struct tp trophy;
struct pbar progressbarr;

void game_init(void);//��ʼ������
void updateWindow(void);//��Ⱦͼ����
void userclick(void);//�û������������
bool fileExist(char*);//�ļ��Ƿ���ں���
void updategame(void);//������Ϸ���ݺ���
void startUI(void);
void creat_sunball(void);
void update_sunball(void);
void creat_zombie(void);
void update_zombie(void);
void creat_bullet(void);
void update_bullet(void);
void transition(void);
void bardown(void);
void openUI(void);
void game_win(void);

int main(void)
{
	gamewindow = UI_WIN;
	DWORD timer = 0;
	bool flag = true;
	game_init();
	openUI();
	while (1)
	{
		if (gamewindow==UI_WIN)
		{
			gamewindow = ADVENTURE_WIN;
			game_init();

			startUI();

			transition();

			bardown();
		}

		userclick();
		timer += getDelay();//ͷ�ļ�tools.c������getDelay()����
		if (timer > 60)
		{
			flag = true;
			timer = 0;
		}
		if (flag)
		{
			flag = false;
			updateWindow();
			
			updategame();
		
		}

	}
	return 0;
}

/******************��ʼ������*******************/
void game_init(void)
{
	char loction[64];
	loadimage(&backgroundpic, "res/backgroundpic.jpg");   //���ر���ͼ
	loadimage(&boardpic, "res/bar.png");//���ض�������
	loadimage(&shovelpic, "res/shovel.png");//���ز���
	loadimage(&shovelbackpic, "res/shovelback.png");//���ز������
	loadimage(&shovelmovepic, "res/shovelmove.png");//�����ƶ��Ĳ���
	for (int i = 0; i < 3; i++)
	{
		sprintf_s(loction, sizeof(loction), "res/else/progressbar%d.png", i + 1);
		loadimage(&progressbar[i], loction);//���ؽ�����
	}
	loadimage(&zombiehead, "res/else/ZombieHead.png");
	loadimage(&flagvertical, "res/else/flagvertical.png");
	loadimage(&manyzombiescomingpic, "res/else/LargeWave.png");
	loadimage(&lastmanyzombiespic, "res/else/FinalWave.png");


	memset(plantpic, 0, sizeof(plantpic));
	memset(map, 0, sizeof(map));
	memset(sunball, 0, sizeof(sunball));
	memset(zombie, 0, sizeof(zombie));
	memset(bullet, 0, sizeof(bullet));
	memset(cards, 0, sizeof(cards));
	memset(&trophy, 0, sizeof(struct tp));
	memset(&progressbarr, 0, sizeof(progressbarr));
	for (int i = 0; i < COUNT; i++)
	{
		sprintf_s(loction, sizeof(loction), "res/Cards/card_%d.png", i + 1);
		loadimage(&cardpic[i], loction);//���ذ����Ͽ�Ƭ

		sprintf_s(loction, sizeof(loction), "res/Cards/cardgray_%d.png", i + 1);
		loadimage(&cardgraypic[i], loction);//���ػ�ɫ��Ƭ

		for (int j = 0; j < 20; j++)
		{
			cards[j].Y = cardpic[0].getheight();//��ɫ���Ƴ�ʼ�߶�
			cards[j].enoughsunball = true;//�����㹻��ÿһ��ֲ��
		}
		/***********ÿ������ˢ��ʱ��***********/
		for (int j = 0; j < 20; j++)
		{
			cards[j].speed = 1;
		}
		cards[0].speed = 40;//��ֵԽ��ˢ��ʱ��Խ��
		cards[1].speed = 20; cards[2].speed = 60; cards[3].speed = 40; cards[4].speed = 40;
		cards[5].speed = 40; cards[6].speed = 100; cards[7].speed = 200; cards[8].speed = 40;
		cards[9].speed = 40; cards[10].speed = 60; cards[11].speed = 100;
		for (int j = 0; j < 20; j++)
		{
			sprintf_s(loction, sizeof(loction), "res/plants/%d/%d.png", i, j + 1);
			if (fileExist(loction))
			{
				plantpic[i][j] = new IMAGE;
				loadimage(plantpic[i][j], loction);//����ֲ��ͼƬ
			}
			else
				break;
		}
	}
	for (int i = 0; i < COUNT; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			sprintf_s(loction, sizeof(loction), "res/plants/%d/change/%d.png", i, j + 1);
			if (fileExist(loction))
			{
				plantchangepic[i][j] = new IMAGE;
				loadimage(plantchangepic[i][j], loction);//����ֲ��ͼƬ
			}
		}
	}
	for (int i = 0; i <= 29; i++)
	{
		sprintf_s(loction, sizeof(loction), "res/sunshine/%d.png", i);
		loadimage(&sunballpic[i], loction);//��������
	}

	sunnumber = 500;//��������ʼֵ
	for (int i = 0; i < 22; i++)
	{
		sprintf_s(loction, sizeof(loction), "res/zm/0/%d.png", i + 1);
		loadimage(&zombiepic[i], loction);//���ؽ�ʬͼƬ
	}
	for (int i = 0; i < 20; i++)
	{
		sprintf_s(loction, sizeof(loction), "res/zm_dead/%d.png", i + 1);
		loadimage(&zombiediedpic[i], loction);//���ؽ�ʬ����ͼƬ
	}
	for (int i = 0; i < 21; i++)
	{
		sprintf_s(loction, sizeof(loction), "res/zm_eat/%d.png", i + 1);
		loadimage(&zombieeating[i], loction);
	}

	loadimage(&bulletoriginal, "res/bullet.png");//�����ӵ�ͼƬ
	for (int i = 0; i < 4; i++)
	{
		float multiple = 0.2 * (i + 1);
		float X = (bulletboom[i].getwidth()) * multiple;
		float Y = (bulletboom[i].getheight()) * multiple;
		loadimage(&bulletboom[i], "res/bulletboom.png",X,Y,true);//true����ȱ�������
	}

	//���ؿ���վ����ʬͼƬ
	for (int i = 0; i < 11; i++)
	{
		sprintf_s(loction, sizeof(loction), "res/zm_stand/%d.png", i + 1);
		loadimage(&zombiestandpic[i], loction);
	}

	/******************��������ͼƬ******************/
	loadimage(&elsepic[0], "res/ZombiesWon.png");//��ʬ�Ե����������ͼƬ
	for (int i = 0; i < 3; i++)
	{
		sprintf_s(loction, sizeof(loction), "res/%d.png", i + 1);
		loadimage(&elsepic[i+1], loction);//׼�������ţ�ֲ��ͼƬ
	}


	//�����������
	srand(time(NULL));

	/****************��ʼ������************************/
	initgraph(WIN_WIDTH, WIN_HEIGHT);//���� ,1 ����ʾprintf����


	//��������
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 25;
	f.lfWeight = 20;
	//strcpy(f.lfFaceName, "Microsoft YaHei");//����ΪMicrosoft YaHei����
	strcpy(f.lfFaceName, "Segoe UI Black" );//����ΪSegoe UI Black����
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	setcolor(BLACK);





}

/******************ͼ����Ⱦ����*******************/
void updateWindow(void)//��Ⱦͼ��
{
	BeginBatchDraw();//��ʼ˫����
	char loction[64];
	static int zhunbeitimer=45;
	putimage(0, 0, &backgroundpic);
	putimage(140, 0, &boardpic);
	putimage(837, 0, &shovelbackpic);
	putimagePNG(843, 5, &shovelpic);//��Ⱦ����
	
	/*****************��ǰ���Ŷ���***************/
	if (zhunbeitimer > 0)
	{
		mciSendString("stop res/music/start.wav", 0, 0, 0);
		mciSendString("play res/music/readysetplant.wav", 0, 0, 0);	
		zhunbeitimer--;
		if (zhunbeitimer > 40 && zhunbeitimer <= 45)
		{
			putimagePNG(420, 220, &elsepic[1]);//׼�� ͼƬ
		}
		else if (zhunbeitimer > 30 && zhunbeitimer <= 40)
		{
			putimagePNG(420, 220, &elsepic[2]);//���� ͼƬ
		}
		else if (zhunbeitimer > 0 && zhunbeitimer < 30)
		{
			putimagePNG(420, 220, &elsepic[3]);//ֲ�� ͼƬ
			mciSendString("play res/music/day.mp3", 0, 0, 0);
		}
	}


	for (int i = 0; i < COUNT; i++)
	{
		sprintf_s(loction, sizeof(loction), "res/Cards/card_%d.png", i + 1);
		putimage(230 + 50 * (i), 5, &cardpic[i]);//��Ⱦ�ϲ����ӿ���ͼƬ
		if (cards[i].enoughsunball)
		{
			putimage(230 + 50 * (i), 5, cards[i].X, cards[i].Y,
				&cardgraypic[i], 0, 0);//����δˢ�º�
		}
		else if (cards[i].enoughsunball == false)
		{
			putimage(230 + 50 * (i), 5, cards[i].X, cardpic[i].getheight(),
				&cardgraypic[i], 0, 0);//������ȫδˢ��
		}
	}

	for (int j = 0; j < 5; j++)
	{
		for (int k = 0; k < 9; k++)
		{
			//printf("%d %d", j, k);
			//��Ⱦ��ͼ�ϵ�ֲ��
			if (map[j][k].type > 0)
			{
				if (map[j][k].changed)
				{
					if (map[j][k].type == 1)
					{
						map[j][k].row = j + 1;
						map[j][k].col = k + 1;
						map[j][k].X = 220 + 70 * k;
						map[j][k].Y = 70 + 90 * j;
						int X = map[j][k].X;
						int Y = map[j][k].Y;
						int planttype = map[j][k].type;
						int frametype = map[j][k].frameindex;
						if (frametype <= 5)
						{
							putimagePNG(X - 5 * frametype, Y - 10 * frametype, plantchangepic[planttype - 1][frametype]);
						}

						else if (frametype > 5)
						{
							putimagePNG(X - 5 * (11-frametype), Y - 10 * (11-frametype), plantchangepic[planttype - 1][frametype]);
						}
					}

				}
				else
				{
					map[j][k].row = j + 1;
					map[j][k].col = k + 1;
					map[j][k].X = 220 + 70 * k;
					map[j][k].Y = 70 + 90 * j;
					int X = map[j][k].X;
					int Y = map[j][k].Y;
					int planttype = map[j][k].type;
					int frametype = map[j][k].frameindex;

					putimagePNG(X, Y, plantpic[planttype - 1][frametype]);
				}
					
			}
		}

	}

	if (curindex > 0)
	{
		IMAGE* img = plantpic[curindex - 1][0];
		putimagePNG(curX - img->getwidth() / 2, curY - img->getheight() * 3 / 4, img);
		//����ͼ����������λ�ù�ϵ
	}
	if (curindex == -1)
	{
		putimagePNG(curX - shovelmovepic.getwidth() / 2, curY - shovelmovepic.getheight() / 2,
			&shovelmovepic);
	}

	//��Ⱦ����
	for (int j = 0; j < ballMax; j++)
	{
		if (sunball[j].used || (sunball[j].status == SUNSHINE_COLLECT))
		{	
			IMAGE* img = &sunballpic[sunball[j].frameindex];
			if (sunball[j].status == SUNSHINE_STAY)
			{
				putimagePNG(sunball[j].p4.x, sunball[j].p4.y, img);
			}
			else
			{
				if (sunball[j].t == 0)
				{
					sunball[j].pCur = sunball[j].p4;
				}
				else
				{
					sunball[j].pCur = (calcBezierPoint(sunball[j].t, sunball[j].p1, sunball[j].p2,
						sunball[j].p3, sunball[j].p4));
					putimagePNG(sunball[j].pCur.x, sunball[j].pCur.y, img);
				}
			}
				
		}
	}

	//��Ⱦ����ֵ�ı�
	char sunnumbertext[8];
	sprintf_s(sunnumbertext, sizeof(sunnumbertext), "%d", sunnumber);
	outtextxy(177, 53, sunnumbertext);

	
	
	//��Ⱦ��ʬ
	for (int i = 0; i < pool_zombieMax; i++)
	{
		if (zombie[i].used)
		{
			if (zombie[i].died)
			{
				IMAGE* imgdied = &zombiediedpic[zombie[i].diedframeindex];
				putimagePNG(zombie[i].X, zombie[i].Y, imgdied);
				//if (zombie[i].diedframeindex >= 18)
				//{
				//	zombie[i].used = false;
				//	memset(&zombie[i], 0, sizeof(zombie[i]));
				//}
			}
			else if (zombie[i].eating)
			{
				IMAGE* imgeating = &zombieeating[zombie[i].eatingframeindex];
				putimagePNG(zombie[i].X, zombie[i].Y, imgeating);
			}
			else
			{
				IMAGE* img = &zombiepic[zombie[i].frameindex];
				putimagePNG(zombie[i].X, zombie[i].Y, img);
			}
		}
	}
	if (died_zombie >= firstpass_zombieMax)
	{
		game_win();
		gamewindow = UI_WIN;
		return;
	}

	//��Ⱦ�ӵ�
	for (int i = 0; i < bulletMax; i++)
	{
		if (bullet[i].used)
		{
			if (bullet[i].boomed)
			{
				IMAGE* img = &bulletboom[bullet[i].frameindex-1];
				putimagePNG(bullet[i].X-20, bullet[i].Y, img);
				if (bullet[i].frameindex >= 3)
				{
					bullet[i].used = false;
					memset(bullet, 0, sizeof(bullet));
				}
			}
			else
			{
				putimagePNG(bullet[i].X, bullet[i].Y, &bulletoriginal);
			}
		}
	}
			
	
	putimagePNG(680, 525, &progressbar[0]);//��Ⱦ�������ϵ���
	putimage(progressbarr.X, progressbarr.Y, progressbarr.imagewidth,
		progressbarr.imageheight,&progressbar[1],0,0);//��Ⱦ��������������
	putimagePNG(zombieheadx, zombieheady, &zombiehead);//��Ⱦ��ʬͷ
	

	putimagePNG(715, 539, &progressbar[2]);//���ؽ�����������	
	putimagePNG(680, 490, &flagvertical);
	putimagePNG(740, 490, &flagvertical);

	//��Ⱦ��Ϸ��ʾ�ı�
	if (hugewavewords == true)
	{
		hugewordstime++;

		if (hugewordstime >= 50)
		{
			zombiescomingtime = 1;
			hugewavewords = false;
			hugewordstime = 0;
		}
		putimagePNG(350, 250, &manyzombiescomingpic);
		mciSendString("play res/music/hugewave.wav", 0, 0, 0);
	}
	if (lastwavewords == true && hugewordstime == 0)
	{
		lastwordstime++;
		if (lastwordstime >= 50)
		{
			lastwavewords = false;
			lastwordstime = 0;
		}
		putimagePNG(330, 220, &lastmanyzombiespic);
		if (lastwordstime == 5)
		{
			mciSendString("play res/music/alarm.mp3", 0, 0, 0);
		}
	}

	EndBatchDraw();//����˫����
}


/******************�û������������*******************/
void userclick(void)
{
	ExMessage msg;//����ExMessage�ṹ���������͵� ����msg
	static int status = 0;
	int judgemoneyindex = 0;
	if (peekmessage(&msg))//peekmessage������ȡ����
	{
		if (msg.message == WM_LBUTTONDOWN)
		{
			//���ѡ��ֲ�￨Ƭ
			if (msg.x > 230 && msg.x < 230 + 50 * COUNT && msg.y>5 && msg.y < 75)
			{
				judgemoneyindex= ((msg.x - 230) / 50) + 1;
				if (sunnumber >= plantmoney[judgemoneyindex - 1] 
					&& cards[judgemoneyindex-1].overcooldown)
				{
					curindex = ((msg.x - 230) / 50) + 1;
					status = 1;
				}
			}
			else if (msg.x > 843 && msg.x < (843 + shovelpic.getwidth())
				&& msg.y>5 && msg.y < 5 + shovelpic.getheight())
			{
				curindex = -1;
			}
			else
			{
				   /*********************����ռ�����***********************/

				int W = sunballpic[0].getwidth();
				int H = sunballpic[0].getheight();
				for (int i = 0; i < ballMax; i++) 
				{
					if (sunball[i].used)
					{
						int X = sunball[i].pCur.x;
						int Y = sunball[i].pCur.y;
						
						if (msg.x > X && msg.x<X + W && msg.y>Y && msg.y < Y + H)
						{

							sunball[i].used = false;
							sunball[i].status = SUNSHINE_COLLECT;
							sunnumber += 25;
							/*sunball[i].offsetX = 1;*/
							//mciSendString("play res/sunshine.mp3", 0, 0, 0);//��������
							PlaySound("res/music/sunshine.wav",NULL,  SND_ASYNC);//�첽��������
							return;
							
						}
					}
				}
			}

		}
		else if (msg.message == WM_MOUSEMOVE )
		{
			curX = msg.x;
			curY = msg.y;

		}
		else if (msg.message == WM_LBUTTONUP)
		{
			if (msg.x > 220 && msg.x < 850 && msg.y>80 && msg.y < 530)
			{
				int row = (msg.y - 60) / 90 ;
				int col = (msg.x - 220) / 70;
				if (curindex > 0)
				{
					if (map[row][col].type == 0)//�ڵ�ͼ�ϴ���һ��ֲ��
					{
						memset(&map[row][col], 0, sizeof(map[row][col]));
						map[row][col].type = curindex;
						map[row][col].frameindex = 0;
						if(map[row][col].type==XIAO_JIAN_GUO+1)
							map[row][col].blood = 500;//С���Ѫ��Ϊ500
						else
							map[row][col].blood = 100;//����ֲ���ʼѪ��Ϊ100
						map[row][col].catching = false;
						map[row][col].row = row + 1;
						sunnumber -= plantmoney[curindex - 1];
						cards[curindex - 1].overcooldown = 0;
						cards[curindex - 1].Y = cardpic[0].getheight();
					}
					else if (map[row][col].type == 1 && curindex == 10)
					{
						map[row][col].changed = true;
						sunnumber -= plantmoney[curindex - 1];
						cards[curindex - 1].overcooldown = 0;
					}
				}
				else if (curindex == -1)
				{
					if (map[row][col].type > 0)
					{
						memset(&map[row][col], 0, sizeof(map[row][col]));
					}
				}
			}
			

			curindex = 0;
			status = 0;
		}

	}
}
/******************�ļ��Ƿ���ں���*******************/
bool fileExist(char* loction)
{
	FILE* fp = fopen(loction, "r");
	if (fp == NULL)
	{
		return false;
	}
	else
		return true;

}

void updategame(void)
{
	/************���¿���*************/
	for (int i = 0; i < 12; i++)
	{
		if (cards[i].overcooldown == 0)
		{
			static int tt=0;//��������ֲ��ˢ���ٶ�
			tt++;
			if (tt == 6) { tt = 1; }
			cards[i].X = cardpic[0].getwidth();
			if (tt % 5 == 0)
			{
				
				cards[i].Y -= cardpic[0].getheight() * 3 / cards[i].speed;
				if (cards[i].Y <= 0)
				{
					cards[i].overcooldown = 1;//��ȴ���
					cards[i].X = cardpic[0].getwidth();
					cards[i].Y = 0;
				}
			}
		}

		if (sunnumber < plantmoney[i])
			cards[i].enoughsunball = false;
		else
			cards[i].enoughsunball = true;

	}

	/************����ֲ��************/
	for(int i=0;i<5;i++)
		for (int j = 0; j < 9; j++)
		{
			if (map[i][j].type>0)
			{
				if (map[i][j].catching)
				{
					map[i][j].blood -= 1;//ÿ���Լ��1*20Ѫ��
					mciSendString("play res/music/chomp.wav", 0, 0, 0);
					if (map[i][j].blood <= 0)
					{
						map[i][j].type = 0;
						memset(&map[i][j], 0, sizeof(map[i][j]));
					}
				}
				else
				{
					map[i][j].frameindex++;
					int frametype = map[i][j].frameindex;
					int type = map[i][j].type;

					if (plantpic[type - 1][frametype] == NULL)
					{
						map[i][j].frameindex = 0;
					}
				}
			}

		}

	/****************���½�����***************/
	float percentage = 0;
	float progressbarchangex = 0;

	percentage = float(appeared_zombie) / firstpass_zombieMax;
	progressbarchangex = (progressbar[1].getwidth()-20) * percentage;
	//progressbarchangex��������ɫ���������ӵĳ���
	progressbarr.X = 680 + (137 - progressbarchangex);
	progressbarr.Y = 525;
	progressbarr.imagewidth = 15+progressbarchangex;
	//��ɫ��������ʼ��15�ĳ���
	progressbarr.imageheight = 21;

	zombieheadx = progressbarr.X-10;
	zombieheady = progressbarr.Y-10;

	creat_sunball();//��������
	update_sunball();//��������״̬

	creat_zombie();//������ʬ
	update_zombie();//���½�ʬ״̬

	creat_bullet();//�����ӵ�
	update_bullet();//�����ӵ�

	
}

           /**********************��Ϸ�˵�����**********************/
void startUI(void)
{
	mciSendString("play res/music/passionstart.mp3", 0, 0, 0);
	
	IMAGE backstone;
	IMAGE startgostone0;//��
	IMAGE startgostone1;//��
	IMAGE littlegamestone0;//��
	IMAGE littlegamestone1;//��
	IMAGE puzzlestone0;//��
	IMAGE puzzlestone1;//��
	IMAGE survivalstone0;//��
	IMAGE survivalstone1;//��
	IMAGE darkselect[10];
	IMAGE lightselect[10];

	enum {NO_CLICK,STARTGOSTONE,LITTLEGAMESTONE,PUZZLESTONE,SURVIVALSTONE,
		WOODBOARDUP,WOODBOARDDOWN,GOADFLOWER,WATERINGPOT,BOOK,KEY,
		SELECTWORD,HELPWORD,QUITWORD };
	int mouseposition = 0;
	int In[20];
	int justone[20];
	char loction[64];
	//float lasttime = float(GetTickCount());
	//float currenttime = 0;
	//float intervaltime = 0;

	memset(justone, 0, sizeof(justone));
	memset(In, 0, sizeof(In));
	memset(loction, 0, sizeof(loction));

	loadimage(&backstone, "res/menu.png");//����UI���汳��
	loadimage(&startgostone0, "res/else/SelectorScreen_Adventure_button.png",330,100,false);
	//���ؿ�ʼð��ʯ���
	loadimage(&startgostone1, "res/else/SelectorScreen_Adventure_highlight.png", 330, 100, false);
	//���ؿ�ʼð��ʯ����
	loadimage(&littlegamestone0, "res/else/SelectorScreen_Survival_button.png",313,110,false);
	//��������С��Ϸʯ���
	loadimage(&littlegamestone1, "res/else/SelectorScreen_Survival_highlight.png",313, 110, false);
	//��������С��Ϸʯ����
	loadimage(&puzzlestone0, "res/else/SelectorScreen_Challenges_button.png",286,102,false);
	//���ؽ���ģʽʯ���
	loadimage(&puzzlestone1, "res/else/SelectorScreen_Challenges_highlight.png", 286, 102, false);
	//���ؽ���ģʽʯ����
	loadimage(&survivalstone0, "res/else/SelectorScreen_Vasebreaker_button.png",266,103,false);
	//��������ģʽʯ���
	loadimage(&survivalstone1, "res/else/SelectorScreen_vasebreaker_highlight.png",266,103,false);
	//��������ģʽʯ����
	for (int i = 0; i < 9; i++)
	{
		sprintf_s(loction, sizeof(loction), "res/UI/dark_%d.png", i + 1);
		loadimage(&darkselect[i], loction);
		sprintf_s(loction, sizeof(loction), "res/UI/light_%d.png", i + 1);
		loadimage(&lightselect[i], loction);
	}//����UI������ֵ��ͼƬ
	//mciSendString("play res/music/passionstart.mp3", 0, 0, 0);
	while (1)
	{
		//currenttime = GetTickCount();
		//intervaltime = currenttime - lasttime;
		//if (intervaltime >= 0.01)
		////{
		//	lasttime = currenttime;
			BeginBatchDraw();
			putimage(0, 0, &backstone);//��ȾUI���汳��
			putimagePNG(470, 71, mouseposition == STARTGOSTONE ? &startgostone1 : &startgostone0);
			//��Ⱦ��ʼð��ʯ��
			putimagePNG(470, 151, mouseposition == LITTLEGAMESTONE ? &littlegamestone1 : &littlegamestone0);
			//��Ⱦ����С��Ϸʯ��
			putimagePNG(470, 225, mouseposition == PUZZLESTONE ? &puzzlestone1 : &puzzlestone0);
			//��Ⱦ����ģʽʯ��
			putimagePNG(470, 289, mouseposition == SURVIVALSTONE ? &survivalstone1 : &survivalstone0);
			//��Ⱦ����ģʽʯ��
			putimagePNG(25, 0, mouseposition == WOODBOARDUP ? &lightselect[0] : &darkselect[0]);
			putimagePNG(30, 140, mouseposition == WOODBOARDDOWN ? &lightselect[1] : &darkselect[1]);
			putimagePNG(23, 296, mouseposition == GOADFLOWER ? &lightselect[2] : &darkselect[2]);
			putimagePNG(191,363, mouseposition == WATERINGPOT ? &lightselect[3] : &darkselect[3]);
			putimagePNG(374,388, mouseposition == BOOK ? &lightselect[4] : &darkselect[4]);
			putimagePNG(460,450, mouseposition == KEY ? &lightselect[5] : &darkselect[5]);
			putimagePNG(640,442, mouseposition == SELECTWORD ? &lightselect[6] : &darkselect[6]);
			putimagePNG(733,475, mouseposition == HELPWORD ? &lightselect[7] : &darkselect[7]);
			putimagePNG(810,462, mouseposition == QUITWORD ? &lightselect[8] : &darkselect[8]);
			ExMessage msg;
			if (peekmessage(&msg))
			{
				if (msg.message == WM_MOUSEMOVE)
				{
					if (msg.x > 470 && msg.x < 800 && msg.y>71 && msg.y < 171)
					{
						mouseposition = STARTGOSTONE;//��ʼð��
						if (justone[STARTGOSTONE] == 0 && In[STARTGOSTONE] == 0)
						{
							PlaySound("res/music/button.wav", NULL, SND_ASYNC);
							justone[STARTGOSTONE]++;
							In[STARTGOSTONE] = 1;
						}


					}
					else if (msg.x > 470 && msg.x < 783 && msg.y>151 && msg.y < 261)
					{
						mouseposition = LITTLEGAMESTONE;//����С��Ϸ
						if (justone[LITTLEGAMESTONE] == 0 && In[LITTLEGAMESTONE] == 0)
						{
							PlaySound("res/music/button.wav", NULL, SND_ASYNC);
							justone[LITTLEGAMESTONE]++;
							In[LITTLEGAMESTONE] = 1;
						}

					}
					else if (msg.x > 470 && msg.x < 756 && msg.y>225 && msg.y < 327)
					{
						mouseposition = PUZZLESTONE;//����ģʽ
						if (justone[PUZZLESTONE] == 0 && In[PUZZLESTONE] == 0)
						{
							PlaySound("res/music/button.wav", NULL, SND_ASYNC);
							justone[PUZZLESTONE]++;
							In[PUZZLESTONE] = 1;
						}

					}
					else if (msg.x > 470 && msg.x < 736 && msg.y>289 && msg.y < 392)
					{
						mouseposition = SURVIVALSTONE;//����ģʽ
						if (justone[SURVIVALSTONE] == 0 && In[SURVIVALSTONE] == 0)
						{
							PlaySound("res/music/button.wav", NULL, SND_ASYNC);
							justone[SURVIVALSTONE]++;
							In[SURVIVALSTONE] = 1;
						}
					}
					else if (msg.x > 25 && msg.x < 318 && msg.y>0 && msg.y < 150)
					{
						mouseposition = WOODBOARDUP;//�������ľ�崦
						if (justone[WOODBOARDUP] == 0 && In[WOODBOARDUP] == 0)
						{
							PlaySound("res/music/button.wav", NULL, SND_ASYNC);
							justone[WOODBOARDUP]++;
							In[WOODBOARDUP] = 1;
						}
					}
					else if (msg.x > 30 && msg.x < 321 && msg.y>140 && msg.y < 211)
					{
						mouseposition = WOODBOARDDOWN;//�������ľ�崦
						if (justone[WOODBOARDDOWN] == 0 && In[WOODBOARDDOWN] == 0)
						{
							PlaySound("res/music/button.wav", NULL, SND_ASYNC);
							justone[WOODBOARDDOWN]++;
							In[WOODBOARDDOWN] = 1;
						}
					}
					else if (msg.x > 23 && msg.x < 163 && msg.y>296 && msg.y < 533)
					{
						mouseposition = GOADFLOWER;//����ڽ𻨴�
						if (justone[GOADFLOWER] == 0 && In[GOADFLOWER] == 0)
						{
							PlaySound("res/music/button.wav", NULL, SND_ASYNC);
							justone[GOADFLOWER]++;
							In[GOADFLOWER] = 1;
						}
					}
					else if (msg.x > 191 && msg.x < 382 && msg.y>363 && msg.y < 526)
					{
						mouseposition = WATERINGPOT;//�������ˮ����
						if (justone[WATERINGPOT] == 0 && In[WATERINGPOT] == 0)
						{
							PlaySound("res/music/button.wav", NULL, SND_ASYNC);
							justone[WATERINGPOT]++;
							In[WATERINGPOT] = 1;
						}
					}
					else if (msg.x > 374 && msg.x < 473 && msg.y>388 && msg.y < 487)
					{
						mouseposition = BOOK;//������鱾��
						if (justone[BOOK] == 0 && In[BOOK] == 0)
						{
							PlaySound("res/music/button.wav", NULL, SND_ASYNC);
							justone[BOOK]++;
							In[BOOK] = 1;
						}
					}
					else if (msg.x > 460 && msg.x < 590 && msg.y>450 && msg.y < 539)
					{
						mouseposition = KEY;//�����Կ�״�
						if (justone[KEY] == 0 && In[KEY] == 0)
						{
							PlaySound("res/music/button.wav", NULL, SND_ASYNC);
							justone[KEY]++;
							In[KEY] = 1;
						}
					}
					else if (msg.x > 640 && msg.x < 721 && msg.y>442 && msg.y < 473)
					{
						mouseposition = SELECTWORD;//����ڡ�ѡ���
						if (justone[SELECTWORD] == 0 && In[SELECTWORD] == 0)
						{
							PlaySound("res/music/button.wav", NULL, SND_ASYNC);
							justone[SELECTWORD]++;
							In[SELECTWORD] = 1;
						}
					}
					else if (msg.x > 733 && msg.x < 781 && msg.y>475 && msg.y < 497)
					{
						mouseposition = HELPWORD;//����ڡ���������
						if (justone[HELPWORD] == 0 && In[HELPWORD] == 0)
						{
							PlaySound("res/music/button.wav", NULL, SND_ASYNC);
							justone[HELPWORD]++;
							In[HELPWORD] = 1;
						}
					}
					else if (msg.x > 810 && msg.x < 857 && msg.y>462 && msg.y < 489)
					{
						mouseposition = QUITWORD;//����ڡ��˳�����
						if (justone[QUITWORD] == 0 && In[QUITWORD] == 0)
						{
							PlaySound("res/music/button.wav", NULL, SND_ASYNC);
							justone[QUITWORD]++;
							In[QUITWORD] = 1;
						}
					}

					else
					{
						mouseposition = 0;
					}

					if (msg.message == WM_MOUSEMOVE && (msg.x < 470
						|| msg.x > 800 || msg.y < 71 || msg.y > 171))
					{
						justone[STARTGOSTONE] = 0;
						In[STARTGOSTONE] = 0;
					}
					if (msg.message == WM_MOUSEMOVE && msg.x < 470
						|| msg.x > 783 || msg.y < 151 || msg.y > 261)
					{
						justone[LITTLEGAMESTONE] = 0;
						In[LITTLEGAMESTONE] = 0;
					}
					if (msg.message == WM_MOUSEMOVE && msg.x < 470
						|| msg.x > 756 || msg.y < 225 || msg.y > 327)
					{
						justone[PUZZLESTONE] = 0;
						In[PUZZLESTONE] = 0;
					}
					if (msg.message == WM_MOUSEMOVE && msg.x < 470
						|| msg.x > 736 || msg.y < 289 || msg.y > 392)
					{
						justone[SURVIVALSTONE] = 0;
						In[SURVIVALSTONE] = 0;
					}
					if (msg.message == WM_MOUSEMOVE && msg.x < 25 
						|| msg.x > 318 || msg.y < 0 || msg.y > 150)
					{
						justone[WOODBOARDUP] = 0;
						In[WOODBOARDUP] = 0;
					}
					if (msg.message == WM_MOUSEMOVE && msg.x < 30
						|| msg.x > 321 || msg.y < 140 || msg.y > 211)
					{
						justone[WOODBOARDDOWN] = 0;
						In[WOODBOARDDOWN] = 0;
					}
					if (msg.message == WM_MOUSEMOVE && msg.x < 23
						|| msg.x > 163 || msg.y <296 || msg.y > 533)
					{
						justone[GOADFLOWER] = 0;
						In[GOADFLOWER] = 0;
					}
					if (msg.message == WM_MOUSEMOVE && msg.x < 191
						|| msg.x > 382 || msg.y <363 || msg.y > 526)
					{
						justone[WATERINGPOT] = 0;
						In[WATERINGPOT] = 0;
					}
					if (msg.message == WM_MOUSEMOVE && msg.x < 374 
						|| msg.x > 473 || msg.y <388 || msg.y > 487)
					{
						justone[BOOK] = 0;
						In[BOOK] = 0;
					}
					if (msg.message == WM_MOUSEMOVE && msg.x < 460 
						|| msg.x > 590 || msg.y <450 || msg.y > 539)
					{
						justone[KEY] = 0;
						In[KEY] = 0;
					}
					if (msg.message == WM_MOUSEMOVE && msg.x < 640
						|| msg.x > 721 || msg.y <442 || msg.y > 473)
					{
						justone[SELECTWORD] = 0;
						In[SELECTWORD] = 0;
					}
					if (msg.message == WM_MOUSEMOVE && msg.x < 733 
						|| msg.x > 781 || msg.y <475 || msg.y > 497)
					{
						justone[HELPWORD] = 0;
						In[HELPWORD] = 0;
					}
					if (msg.message == WM_MOUSEMOVE && msg.x < 810
						|| msg.x > 857 || msg.y <462 || msg.y > 489)
					{
						justone[QUITWORD] = 0;
						In[QUITWORD] = 0;
					}

				}

				if (msg.message == WM_LBUTTONDOWN && mouseposition == STARTGOSTONE)
				{
					mciSendString("stop res/music/passionstart.mp3", 0, 0, 0);
					mciSendString("play res/music/evillaugh.mp3", 0, 0, 0);
					Sleep(2000);
					
					return;
				}
			}
			EndBatchDraw();
		//}
	}
}

/**********************��������*******************************/
void creat_sunball(void)
{
	int i;
	static int count;
	static int refresh = 200;//���ֵ�10�����һ������
	count++;
	if (count > refresh)
	{
		refresh = 150 + rand() % 50;//ÿ7.5�뵽10�����һ������
		//refresh = 225+rand() % 225;//ÿ10�뵽20�����һ������
		count = 0;
		//���������ȡһ������ʹ�õ�
		for (i = 0; i < ballMax && (sunball[i].used || (sunball[i].status == SUNSHINE_COLLECT)); i++);
		if (i >= ballMax)
			return;
		else
		{
			sunball[i].used = true;
			sunball[i].status = SUNSHINE_DOWN;
			sunball[i].frameindex = 0;
			sunball[i].timer = 0;
			sunball[i].t = 0;
			sunball[i].p1 = vector2(250 + rand() % (800 - 250), 30);
			sunball[i].p2 = vector2(sunball[i].p1.x, 30);
			sunball[i].p3 = vector2(sunball[i].p1.x, rand() % 4 * 90 + 200);
			sunball[i].p4 = vector2(sunball[i].p1.x, sunball[i].p3.y);
			sunball[i].speed = float(3) / float((dis(sunball[i].p1 - sunball[i].p4)));
		}	
	}
	for (i = 0; i < ballMax && (sunball[i].used || (sunball[i].status == SUNSHINE_COLLECT)); i++);
	if (i >= ballMax)
		return;
	else
	{
		for (int j = 0; j < 5; j++)
			for (int k = 0; k < 9; k++)
			{
				if (map[j][k].type == XIANG_RI_KUI + 1)
				{
					map[j][k].timer++;
					if (map[j][k].timer > 200)//���տ�10�����һ������
					{
						map[j][k].timer = 0;
						sunball[i].used = true;
						sunball[i].status = SUNSHINE_CREAT;
						sunball[i].frameindex = 0;
						sunball[i].p1 = vector2(map[j][k].X, map[j][k].Y);
						sunball[i].p4 = vector2(map[j][k].X + 50, map[j][k].Y);
						int w = dis(sunball[i].p1 - sunball[i].p4);//p1��p4�ĺ������x
						sunball[i].p2 = vector2(map[j][k].X + 0.3 * w, map[j][k].Y - 100);
						sunball[i].p3 = vector2(map[j][k].X + 0.6 * w, map[j][k].Y - 100);
						sunball[i].t = float(0);
						sunball[i].speed = float(2) / float(20);
						sunball[i].pCur = sunball[i].p1;
					}
				}

			}
	}
}

/**********************��������*******************************/
void update_sunball(void)
{
	for (int i = 0; i < ballMax; i++)
	{
		if (sunball[i].used||(sunball[i].status == SUNSHINE_COLLECT))
		{
			if (sunball[i].status == SUNSHINE_DOWN)
			{
				sunball[i].t += sunball[i].speed;
				sunball[i].pCur = sunball[i].p1 - sunball[i].t * (sunball[i].p1 - sunball[i].p4);
				sunball[i].frameindex++;
				if (sunball[i].frameindex >= 29)
				{
					sunball[i].frameindex = 0;
				}
				if (sunball[i].t >= 1)
				{
					sunball[i].status = SUNSHINE_STAY;
					sunball[i].t = 0;
				}
			}
			if (sunball[i].status == SUNSHINE_STAY)
			{
				sunball[i].timer++;
				sunball[i].frameindex++;
				sunball[i].pCur = sunball[i].p4;
				if (sunball[i].frameindex >= 29)
				{
					sunball[i].frameindex = 0;
				}
				if (sunball[i].timer >= 100)
				{
					sunball[i].timer = 0;
					sunball[i].used = false;
					memset(&sunball[i], 0, sizeof(sunball[i]));
				}
			}
			if (sunball[i].status == SUNSHINE_COLLECT)
			{
				sunball[i].p1 = vector2(sunball[i].pCur.x, sunball[i].pCur.y);
				sunball[i].p2 = vector2(sunball[i].p1.x, sunball[i].p1.y);
				sunball[i].p3 = vector2(140, 0);
				sunball[i].p4 = vector2(140, 0);				
				sunball[i].speed = float(10)/ float((dis(sunball[i].p1 - sunball[i].p4)));
				sunball[i].t += sunball[i].speed;
				sunball[i].pCur = calcBezierPoint(sunball[i].t, sunball[i].p1,
					sunball[i].p2, sunball[i].p3, sunball[i].p4);
				sunball[i].frameindex++;
				if (sunball[i].t >= 1)
				{
					memset(&sunball[i], 0, sizeof(sunball[i]));
				}
				if (sunball[i].frameindex >= 29)
				{
					sunball[i].frameindex = 0;
				}
			}
			if (sunball[i].status == SUNSHINE_CREAT)
			{
				sunball[i].t += sunball[i].speed;
				sunball[i].pCur = calcBezierPoint(sunball[i].t, sunball[i].p1,
					sunball[i].p2, sunball[i].p3, sunball[i].p4);
				if (sunball[i].t >= 1)
				{
					sunball[i].status = SUNSHINE_STAY;
					sunball[i].t = 0;
				}
				sunball[i].frameindex++;
				if (sunball[i].frameindex >= 29)
				{
					sunball[i].frameindex = 0;
				}
			}

		}


	}
}

/**********************������ʬ*******************************/
void creat_zombie(void)
{
	if (appeared_zombie >= firstpass_zombieMax)
	{
		return;
	}
	
	static int fre = 100;
	static int count = 0;
	static int one = 1;//one����������ִֻ��һ��
	count++;
	if (count > 95 && one == 1)
	{
		one = 0;
		PlaySound("res/music/herecoming.wav", NULL, SND_SYSTEM | SND_ASYNC);

	}
	if (zombiescomingtime >=1)
	{
		fre = 5;
		zombiescomingtime++;
		if (zombiescomingtime == 75)
		{
			zombiescomingtime = 0;
		}
	}
	else
		fre = 100;
	if (count > fre)
	{
		int i;
		count = 0;
		fre = rand() % 100 + 100;//5��10�����һ����ʬ
		//fre = rand() % 10 + 10;//���԰棬�ӿ콩ʬ�����ٶ�
		//����һ����ʬ
		for (i = 0; (i < pool_zombieMax) && zombie[i].used; i++);	
			if(i<pool_zombieMax)
			{
				appeared_zombie++;
				memset(&zombie[i], 0, sizeof(zombie[i]));
				int randnumber = rand() % 5;
				zombie[i].X = 900;
				//zombie[i].Y = 80 + (randnumber) * 90;
				zombie[i].Y = 10 + (randnumber) * 90;
				zombie[i].used = true;
				zombie[i].row = randnumber+1;//��ʬ��������Ϊ1��5��
				zombie[i].blood = 150;
				zombie[i].died = false;
				zombie[i].diedframeindex = 0;			
			}

			for (int i = 0; i < 10; i++)
			{
				int num = rand() % 5 + 1;
				if (appeared_zombie == firstpass_zombieMax * i / 10)
				{
					switch (num)
					{
					case 1:
						mciSendString("play res/music/groan1.wav", 0, 0, 0);
						break;

					case 2:
						mciSendString("play res/music/groan2.wav", 0, 0, 0);
						break;

					case 3:
						mciSendString("play res/music/groan3.wav", 0, 0, 0);
						break;

					case 4:
						mciSendString("play res/music/groan4.wav", 0, 0, 0);
						break;

					case 5:
						mciSendString("play res/music/groan5.wav", 0, 0, 0);
						break;
					}
				}
			}
	}

}

/**********************���½�ʬ*******************************/
void update_zombie(void)
{
	static int count=0;
	count++;
	if (count >= 3)
	{
		count = 0;
		for (int i = 0; i < pool_zombieMax; i++)
		{
			if (zombie[i].used)
			{
				if (zombie[i].X <= 90)
				{
					PlaySound("res/music/lose.wav", NULL, SND_ASYNC);
					zombie[i].used = false;
					putimagePNG(230, 20, &elsepic[0]);
					MessageBox(NULL, "FAIL", "GAME FIAL!!!", 0);
					exit(-1);
				}
				if (zombie[i].died)
				{
					int row = zombie[i].row - 1;

					for (int j = 8; j >= 0; j--)
					{
						if (map[row][j].catching == true)
						{
							map[row][j].catching = false;
							break;
						}
					}
					zombie[i].diedframeindex+=2;
					if (zombie[i].diedframeindex >= 22)
					{
						died_zombie++;
						zombie[i].used = false;
						memset(&zombie[i], 0, sizeof(zombie[i]));
					}

				}
				else if (zombie[i].eating)
				{
					zombie[i].eatingframeindex += 1;
					if (zombie[i].eatingframeindex >= 21)
					{
						zombie[i].eatingframeindex = 0;
					}
				}
				else
				{
					int X;
					zombie[i].frameindex = (zombie[i].frameindex + 1) % 22;
					zombie[i].X -= 3;
					X = zombie[i].X;
					//zombie[i].col = (X  - 220) / 70 + 1;
					zombie[i].col = (X + zombiepic[0].getwidth() * 3 / 4 - 220) / 70 + 1;//��ʬ����Ϊ1��9

				//	zombie[i].X -= 50;//���԰棬�ӿ콩ʬǰ���ٶ�
				
				}
				if (zombie[i].blood <= 0)
				{
					zombie[i].died = true;	
				}


				
				/***************��ʬ��ֲ����ײ���*****************/
				int row = zombie[i].row-1;
				
				for (int j = 0; j < 9; j++)
				{
					if (map[row][j].type == 0)
					{
						map[row][j].catching = false;
						continue;
					}		
					int x1 = map[row][j].X + 15;
					int x2 = map[row][j].X + 50;
					int x3 = zombie[i].X + 70;
					if (x3>x1 && x3<x2)
					{
						zombie[i].eating = true; 
						map[row][j].catching = true;
						if (map[row][j].blood <= 10)
						{
							zombie[i].eating = false;
						}
						
					}

				}
			}
			
		}
	}
	if (appeared_zombie == 0.3 * firstpass_zombieMax)
	{
		hugewavewords = true;//����15����ʬ����ʾ��һ�󲨽�ʬ�������١�

	}
	if (appeared_zombie == 0.7 * firstpass_zombieMax)
	{
		hugewavewords = true;//����35����ʬ����ʾ��һ�󲨽�ʬ�������١�
		lastwavewords = true;//��ʾ�����һ����

	}
}

/**********************�����ӵ�*******************************/
void creat_bullet()
{
	static int count0 = 0;//ֲ������״̬
	static int count1 = 0;//ֲ������״̬
	static int freWAN_DOU0 = 10;//ֲ������״̬
	static int freWAN_DOU1 = 40;//ֲ������״̬
	count0++;
	count1++;
	for (int i = 0; i < 5; i++)
	{ 
		for (int j = 0; j < 9; j++)//�ұ��ͼ���и���
		{
			if (map[i][j].type == 1)//��ʾ�ø��Ӵ����㶹����
			{
				int row = map[i][j].row;
				if (map[i][j].changed)
				{
					if (count0 > freWAN_DOU0)
					{
					/*	if (i == 4 && j == 8)
						{
							count0 = 0;
						}*/

						for (int k = 0; k < pool_zombieMax; k++)//�ұ���������н�ʬ
						{
							if (zombie[k].used)
							{
								if (zombie[k].used && (row == zombie[k].row) && zombie[k].X <= 800)
								{
									int m;
									for (m = 0; m < bulletMax && bullet[m].used; m++);
									if (m < bulletMax)
									{
										memset(&bullet[m], 0, sizeof(bullet[m]));
										bullet[m].X = map[i][j].X + (*plantpic[0][1]).getwidth() - 20;
										bullet[m].Y = 78 + (i) * 90;
										bullet[m].used = true;
										bullet[m].row = i + 1;
										bullet[m].boomed = false;
										bullet[m].frameindex = 0;
										bullet[m].speed = 40;

									}
								}
							}
						}
					}
				}

				else if (map[i][j].changed == false)
				{
					if (count1 > freWAN_DOU1)
					{
						/*if (i == 4 && j == 8)
						{
							count1 = 0;
						}*/
						for (int k = 0; k < pool_zombieMax; k++)//�ұ���������н�ʬ
						{
							if (zombie[k].used)
							{
								if (zombie[k].used && (row == zombie[k].row) && zombie[k].X <= 800)
								{
									int m;
									for (m = 0; m < bulletMax && bullet[m].used; m++);
									if (m < bulletMax)
									{
										memset(&bullet[m], 0, sizeof(bullet[m]));
										bullet[m].X = map[i][j].X + (*plantpic[0][1]).getwidth() - 20;
										bullet[m].Y = 78 + (i) * 90;
										bullet[m].used = true;
										bullet[m].row = i + 1;
										bullet[m].boomed = false;
										bullet[m].frameindex = 0;
										bullet[m].speed = 10;
			
									}
								}
							}

						}

					}
				}

			}
			
		}
	}
	if (count0 > 10) { count0 = 0; }
	if (count1 > 40) { count1 = 0; }

	
}

/**********************�����ӵ�*******************************/
void update_bullet(void)
{
	for (int i = 0; i < bulletMax; i++)
	{
		if (bullet[i].used)
		{
			bullet[i].X += bullet[i].speed;
			//printf("��%d���ӵ���x=%d ", i, bullet[i].X);
			if (bullet[i].X >= 1000)
				bullet[i].used = false;

			/*************�ӵ��ͽ�ʬ��ײ���***************/
			for (int j = 0; j < pool_zombieMax; j++)
			{
				if (zombie[j].used && !zombie[j].died &&(bullet[i].row == zombie[j].row)
					&& (bullet[i].X > zombie[j].X + 85) && (bullet[i].X < zombie[j].X + 120))				
				{
					mciSendString("play res/music/peahit.wav", 0, 0, 0);
					bullet[i].boomed = true;
					bullet[i].frameindex++;
					if (bullet[i].frameindex >=3)
					{
						bullet[i].used = false;
						memset(&bullet[i], 0, sizeof(bullet[i]));

					}
					zombie[j].blood -= 5;//����3(frameindex)=15��Ϊһ���ӵ�����Ѫ��
					break;
				}
			}
		}
	}
}

void transition(void)
{
	int zmstandindex[11];
	memset(zmstandindex, 0, sizeof(zmstandindex));
	vector2 stand[8] = { {900,50} ,{850,100} ,{950,200} ,{830,250} ,
		{910,300} ,{1000,320}, {850,330}, {860,160} };
	//mciSendString("play res/start.mp3", 0, 0, 0);//���ſ�ʼ����
	mciSendString("play res/music/start.wav", 0, 0, 0);
	for (int i = 0; i < 11; i++)
	{
		zmstandindex[i] = rand() % 11;
	}
	while (1)
	{
		BeginBatchDraw();
		static int x = 0;
		static int returnx = -300;
		if (x > -300)
		{
			putimage(x, 0, &backgroundpic);
			for (int i = 0; i < 11; i++)
			{
				if (x % 5 == 0)
				{
					zmstandindex[i] = (zmstandindex[i] + 1) % 11;
				}
			}
			for (int k = 0; k < 8; k++)
			{
				putimagePNG(stand[k].x + x, stand[k].y, &zombiestandpic[zmstandindex[k]]);
			}
			x -= 1;
		}
		else if (x <= -300)
		{
			
			static int reciprocal = 0;
			reciprocal++;
			if (reciprocal < 200)
			{
				putimage(returnx, 0, &backgroundpic);
				for (int i = 0; i < 11; i++)
				{
					if (reciprocal % 5 == 0)
					{
						zmstandindex[i] = (zmstandindex[i] + 1) % 11;
					}
				}
				for (int k = 0; k < 8; k++)
				{
					putimagePNG(stand[k].x-300, stand[k].y, &zombiestandpic[zmstandindex[k]]);
				}
			}
			if (reciprocal >= 200)
			{
				
				returnx++;
				putimage(returnx, 0, &backgroundpic);				
				for (int i = 0; i < 11; i++)
				{
					if (returnx % 5 == 0)
					{
						zmstandindex[i] = (zmstandindex[i] + 1) % 11;
					}
				}
				for (int k = 0; k < 8; k++)
				{
					putimagePNG(stand[k].x + returnx, stand[k].y, &zombiestandpic[zmstandindex[k]]);
				}
				if (returnx > 0)
				{
					reciprocal = 0;
					x = 0;
					returnx = -300;
					return;
				}
			}
		}
		EndBatchDraw();
		Sleep(5);

		
	}
}

void bardown(void)
{
	char loction[64];
	for (int y = -boardpic.getheight(); y <= 0; y++)
	{
		BeginBatchDraw();
		putimage(0, 0, &backgroundpic);
		putimage(140,y,&boardpic);
		for (int i = 0; i < COUNT; i++)
		{
			sprintf_s(loction, sizeof(loction), "res/Cards/card_%d.png", i + 1);
			putimage(230 + 50 * (i), 5+y, &cardpic[i]);//��Ⱦ�ϲ����ӿ���ͼƬ
		}


		EndBatchDraw();
		Sleep(5);
	}
}

void openUI(void)
{
	IMAGE openpic;
	IMAGE openstartpic;

	int inopenstart = 0;
	int justone = 0;

	//PlaySound("res/music/Cerebrawl.wav", NULL, SND_ASYNC | SND_LOOP);
	//mciSendString("play res/music/Cerebrawl.wav",0,0,0);

	loadimage(&openpic, "res/else/open.png");
	loadimage(&openstartpic, "res/else/openstart.png");

	while (1)
	{

		BeginBatchDraw();
		putimage((inopenstart ? -1 : 0),0 , (inopenstart ? &openstartpic : &openpic));
		EndBatchDraw();

		mciSendString("play res/music/Cerebrawl.wav", 0, 0, 0);
		//mciSendString("play res/music/groan5.wav", 0, 0, 0);
		//mciSendString("play res/music/groan3.wav", 0, 0, 0);
		//mciSendString�첽����(��ֱ�ӷ��أ�������һ���ڵ������������Ҫ����һ�εĲ�����),
		// ���Ҳ�ͬ������Ҳ��ͬʱ����
		//Sleep(5000);
		//mciSendString("pause res/music/Cerebrawl.wav", 0, 0, 0);
		//Sleep(2000);
		//mciSendString("resume res/music/Cerebrawl.wav", 0, 0, 0);


		ExMessage msg;
		if (peekmessage(&msg))
		{
			if (msg.message == WM_MOUSEMOVE ||msg.message == WM_LBUTTONDOWN)
			{
				if (msg.x > 280 && msg.x < 618 && msg.y>496 && msg.y < 528)
				{
					inopenstart = 1;
					if (justone == 0)
					{
						PlaySound("res/music/button.wav", NULL, SND_ASYNC);
						justone = 1;
					}
					if (msg.message == WM_LBUTTONDOWN)
					{
						mciSendString("stop res/music/Cerebrawl.wav", 0, 0, 0);
						//mciSendString("play res/music/passionstart.mp3", 0, 0, 0);
						//mciSendString("play res/music/passionstart.mp3", 0, 0, 0);
						return;
					}
				}
				else if (msg.x < 280 || msg.x > 618 || msg.y < 496 || msg.y > 528)
				{
					inopenstart = 0;
					justone = 0;
				}
			}
		}
		
		
	}
}

void game_win(void)
{
	char loction[64];
	loadimage(&pointerdownpic, "res/else/pointerdown.png");//����ָ�¼�ͷ
	for (int i = 0; i < 20; i++)
	{
		sprintf_s(loction, sizeof(loction), "res/else/trophy/%d.png", i + 1);
		loadimage(&trophypic[i], loction);//���ؽ���
		sprintf_s(loction, sizeof(loction), "res/else/trophybigger/%d.png", i + 1);
		loadimage(&trophybigpic[i], loction);//���ر�󽱱�
	}
	for (int i = 0; i < 100; i++)
	{
		int X = trophybigpic[0].getwidth() * (0.5 + float(i) / float(200));
		int Y = trophybigpic[0].getheight() * (0.5 + float(i) / float(200));
		sprintf_s(loction, sizeof(loction), "res/else/trophybigger/%d.png", (i)%20+1);
		loadimage(&trophybiggerpic[i], loction, X, Y, true);
	}

	float t = 0;//���������ߵ�ʱ��t��0-1
	vector2 vectorcur;
	int pointerdownx = 805, pointerdowny = 385;
	int time = 0;//�������ݵ�ʱ�䣬ÿˢ���θ���һ������
	int firstnum = 50, secondnum = 51;
	bool pointerfade = 0;
	enum { APPEAR, COLLECT,FADE }trophystatus;
	trophystatus = APPEAR;
	trophy.p1 = vector2(714, 363);
	trophy.p2 = vector2(726, 313);
	trophy.p3 = vector2(730, 313);
	trophy.p4 = vector2(739, 415);
	while (1)
	{
		time += 1;
		if (time == 100)
			time = 0;
		ExMessage msg;
		if (peekmessage(&msg))
		{
			if (msg.message == WM_LBUTTONDOWN && msg.x > 739 && msg.x < 739 + trophypic[0].getwidth()
				&& msg.y>415 && msg.y < 415 + trophypic[0].getheight())
			{
				mciSendString("stop res/music/day.mp3", 0, 0, 0);
				mciSendString("play res/music/win.wav", 0, 0, 0);
				trophystatus = COLLECT;
				pointerfade = true;
				t = float(0);
			}
			if (trophystatus == FADE)
			{
				if (msg.message == WM_LBUTTONDOWN && msg.x > 350 && msg.x < 350 + trophybigpic[0].getwidth()
					&& msg.y>150 && msg.y < 150 + trophybigpic[0].getheight())
				{
					return;
				}
			}
		}
		if (trophystatus == APPEAR)
		{
			if (time % 4 == 0)
			{
				if (t < 1)
					t += (float(1) / float(50));
				else
					t = 1;

				vectorcur = calcBezierPoint(t, trophy.p1, trophy.p2, trophy.p3, trophy.p4);
				if (time % 5 == 0)
				{
					trophy.frame++;
					if (trophy.frame == 20)
					{
						trophy.frame = 0;
					}//���½���
				}

				if (time % 100 < 48)
					pointerdowny--;//�����¼�ͷ
				else if (time % 100 >= 52)
					pointerdowny++;
			}
		}
		else if (trophystatus == COLLECT||trophystatus == FADE)
		{
			trophy.p1 = vector2(739, 415);
			trophy.p2 = vector2(739, 415);
			trophy.p3 = vector2(350, 150);
			trophy.p4 = vector2(350, 150);
			vectorcur = calcBezierPoint(t, trophy.p1, trophy.p2, trophy.p3, trophy.p4);
			
			if (time % 4 == 0)
			{
				if (t < 1)
					t += (float(1) / float(250));
				else
					t = 1;
				if (t == 1)
				{
					trophystatus = FADE;
				}
				if ((t >= float(firstnum) / float(250)) && (t <= float(secondnum) / float(250)) )
				{
					if (firstnum < 250 && secondnum < 251)
					{
						firstnum += 2;
						secondnum += 2;
					}
					if (trophy.biggerframe<=98)
						trophy.biggerframe++;					
				}
				if (time % 5 == 0)
				{
					trophy.frame++;	
					if (trophy.frame == 19)
					{
						trophy.frame = 0;
					}//���½���

				}
			}
		}

		BeginBatchDraw();
		putimage(0, 0, &backgroundpic);
		putimage(140, 0, &boardpic);
		putimage(837, 0, &shovelbackpic);
		putimagePNG(843, 5, &shovelpic);//��Ⱦ����
		for (int i = 0; i < COUNT; i++)
		{
			sprintf_s(loction, sizeof(loction), "res/Cards/card_%d.png", i + 1);
			putimage(230 + 50 * (i), 5, &cardpic[i]);//��Ⱦ�ϲ����ӿ���ͼƬ
		}
		if (trophystatus == APPEAR || t <= float(0.2))
		{
			putimagePNG(vectorcur.x, vectorcur.y, &trophypic[trophy.frame]);
		}
		else if (trophystatus == COLLECT && t > float(0.2) && t < float(1))
		{
			putimagePNG(vectorcur.x, vectorcur.y, &trophybiggerpic[trophy.biggerframe]);
		}
		else if (trophystatus == FADE && t >= float(1))
		{
			putimagePNG(vectorcur.x, vectorcur.y, &trophybigpic[trophy.frame]);			
		}
		if (t >= 1 && pointerfade == false)
		{
			putimagePNG(pointerdownx, pointerdowny, &pointerdownpic);
		}
		EndBatchDraw();

		
	}
}

	
