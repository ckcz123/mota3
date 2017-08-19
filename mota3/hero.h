#pragma once

#include "npc.h"
class c_hero
{
public:
	const static int MAX_DAMAGE = 99999999;

	c_hero();//���캯��
	void init();
	float nextX();//��һ���ĺ�����
	float nextY();//��һ����������
	bool canMove(int);//�Ƿ���Գ�ĳ�������ƶ�
	bool moveComplete();//�ƶ��Ƿ����
	bool openRedDoor();//�򿪺�ɫ����
	bool openBlueDoor();//����ɫ����
	bool openYellowDoor();//�򿪻�ɫ����
	bool openSpecialDoor();//��������
	void upstair();//��¥
	void downstair();//��¥
	void specialMove();// ����
	void fly(); //¥���Ծ
	void show();
	void printInfo();//�����ʿ��Ϣ
	bool canBeat(c_monster);//�ܷ��ܹ���
	int getDamage(c_monster);//��ܹ�����ͨ�˺�
	bool attack(c_monster&);//ÿ�غϴ����
	void beAttacked(c_monster);//ÿ�غϱ���
	void npc(int select=0);//ѡ��select
	void save(FILE*);//�浵
	void load(FILE*);//����
	bool nearStair();
	void specialDamage() {hp-=hp/3;}
	int getHP() {return hp;}
	int getAtk() {return atk;}
	int getDef() {return def+getSpeDef();}
	int getSpecialLv();
	int getSpeDef();
	int getSpeLife(int);
	int getSpeAtkTimes();
	void addHp(int delta) {hp+=delta;}
	int getNowFloor() {return now_floor;}
	void setMaxFloor() {if (max_floor<now_floor && now_floor<30) max_floor=now_floor;}
	void setFlyFloor(int delta=0);
	int getFlyFloor() {return fly_floor;}
	int dir[2][4];
private:
	int hp,lv,atk,def,money,experience,redkey,bluekey,yellowkey,special;
	int x,y,move,face,now_floor,max_floor,fly_floor;
};
