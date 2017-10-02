#pragma once

#include "npc.h"
class c_hero
{
public:
	const static int MAX_DAMAGE = 99999999;

	c_hero();//���캯��
	void init();
	int nextX();//��һ���ĺ�����
	int nextY();//��һ����������
	bool canMove(int);//�Ƿ���Գ�ĳ�������ƶ�
	bool moveComplete();//�ƶ��Ƿ����
	bool openRedDoor();//�򿪺�ɫ����
	bool openBlueDoor();//����ɫ����
	bool openYellowDoor();//�򿪻�ɫ����
	void upstair();//��¥
	void downstair();//��¥
	void specialMove(int f=0);// ����
	void turn(); // ת��
	void fly(); //¥���Ծ
	void show();
	void printInfo();//�����ʿ��Ϣ
	bool canBeat(c_monster*);//�ܷ��ܹ���
	int getDamage(c_monster*);//��ܹ�����ͨ�˺�
	void beat(c_monster*);
	void npc(int select=0);//ѡ��select
	void save(FILE*);//�浵
	char* toString();
	void load(FILE*);//����
	bool nearStair(int t=0);
	bool nearDownStair() {return nearStair(8);}
	bool nearUpStair() {return nearStair(7);}
	bool canReborn();
	void useWand();
	bool canCenterFly();
	void centerFly();
	void getYellowKey() {yellowkey++;}
	void getBlueKey() {bluekey++;}
	void getRedKey() {redkey++;}
	int getX() {return x;}
	int getY() {return y;}
	int getHP() {return hp;}
	int getAtk() {return atk;}
	int getDef() {return def;}
	int getMoney() {return money;}
	int yellow() {return yellowkey;}
	int blue() {return bluekey;}
	int getNowFloor() {return now_floor;}
	int getDisplayFloor(int f=-1) {return f<0?getDisplayFloor(now_floor):(f>20?f-20:f);}
	void setFlyFloor(int delta=0);
	int getFlyFloor() {return fly_floor;}
	int dir[2][4];
private:
	int hp,atk,def,money,redkey,bluekey,yellowkey;
	int x,y,move,face,now_floor,fly_floor;
	hgeSprite* sprites[4][4];
};
