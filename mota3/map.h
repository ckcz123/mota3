#pragma once
#include "hero.h"

class c_map_door
{
public:
	void init(int);//��ʼ��
	void show(int,int);//��Ⱦ
	bool open();//��ĳ����ͨ��
	int getType() {return type;} //�ŵ�����
private:
	int type;
	bool opened;
	int state;
	hgeSprite *door[4];
};

class c_map_point
{
public:
	void init(int,int,int,int,int s_npc=0,int s_spe=0);//��ʼ��
	void show(GfxFont*,int,int);//��Ⱦ
	void animation() {monster.changeState();npc.changeState();}//npc״̬�ı�
	void printMonsterInfo() {monster.printInfo();} //���������Ϣ
	bool canMove(int);//�жϸõ��Ƿ���ƶ�
	bool openSpecial();
	int getItemID();//��ȡ�õ����Ʒ���
	int getSpecial() {return special;}
	void setSpecial(int _spe) {special=_spe;}
	void distroyItem() {item=NULL;} //��ȡ�õ���Ʒ
	c_map_door* getDoor() {return &door;}
	c_map_npc* getNpc() {return &npc;}
	c_monster* getMonster() {return &monster;}
	bool nearStair() {return door.getType()==7 || door.getType()==8;}
	bool hasMonster(int id=0) {return id==0?monster.getId()!=0:monster.getId()==id;}
	bool isGround();
	hgeSprite* getTypeSprite();
	hgeSprite* getItemSprite();
	void save(FILE*);//�浵
	void load(FILE*);//����
private:
	int type;//�������ͣ��� OR ǽ��
	int item;//��������
	c_monster monster;//��������
	c_map_door door;//������
	c_map_npc npc;// npc����
	int special; // ��������ⰴť
};

class c_map_floor
{
public:
	void init(int,int[30][30]);//��ʼ��
	void show();//��Ⱦ
	void animation();//״̬�ı�
	void printMonsterInfo(int,int);//���������Ϣ
	bool canMove(int,int,int);//�жϸõ�ͼ��ĳ���Ƿ���ƶ�
	int getItem(int,int);//��ȡ��ͼĳ�����Ʒ
	int getSpecial(int,int);
	void setSpecial(int,int,int);
	void getDownPosition(int&,int&);//�ҵ���ͼ����¥¥��
	void getUpPosition(int&,int&);//�ҵ���ͼ����¥¥��
	void save(FILE*);//�浵
	void load(FILE*);//����
	bool hasMonster(int id=0);
	c_map_point* getinfo(int x, int y) {return &info[x][y];}
private:
	int id;//¥����
	int dx,dy,ux,uy;//¥������
	c_map_point info[30][30];//ÿ����Ϣ
};