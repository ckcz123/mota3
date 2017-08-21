#pragma once

#include "npc.h"
class c_hero
{
public:
	const static int MAX_DAMAGE = 99999999;

	c_hero();//构造函数
	void init();
	float nextX();//下一步的横坐标
	float nextY();//下一步的纵坐标
	bool canMove(int);//是否可以朝某个方向移动
	bool moveComplete();//移动是否完成
	bool openRedDoor();//打开红色的门
	bool openBlueDoor();//打开蓝色的门
	bool openYellowDoor();//打开黄色的门
	void upstair();//上楼
	void downstair();//下楼
	void specialMove();// 传送
	void fly(); //楼层飞跃
	void show();
	void printInfo();//输出勇士信息
	bool canBeat(c_monster*);//能否打败怪物
	int getDamage(c_monster*);//打败怪物普通伤害
	void beat(c_monster*);
	void npc(int select=0);//选择select
	void save(FILE*);//存档
	void load(FILE*);//读档
	bool nearStair();
	bool canReborn();
	int getHP() {return hp;}
	int getAtk() {return atk;}
	int getDef() {return def;}
	int getNowFloor() {return now_floor;}
	void setMaxFloor() {if (max_floor<now_floor && now_floor<30) max_floor=now_floor;}
	void setFlyFloor(int delta=0);
	int getFlyFloor() {return fly_floor;}
	int dir[2][4];
private:
	int hp,atk,def,money,redkey,bluekey,yellowkey;
	int x,y,move,face,now_floor,max_floor,fly_floor;
	hgeSprite* sprites[4][4];
};
