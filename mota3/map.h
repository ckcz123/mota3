#pragma once
#include "hero.h"

class c_map_door
{
public:
	void init(int);//��ʼ��
	void show(int,int);//��Ⱦ
	bool open();//��ĳ����ͨ��
	void setOpened(bool on=true) {opened=on;} //��һ��������
	bool isOpened() {return opened;}//�ж�һ�����Ƿ񱻴�
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
	void init(hgeSprite*,hgeSprite*,int,int,int s=0);//��ʼ��
	void show(GfxFont*,int,int);//��Ⱦ
	void animation() {monster.changeState();npc.changeState();}//npc״̬�ı�
	void printMonsterInfo() {monster.printInfo();} //���������Ϣ
	bool canMove(int);//�жϸõ��Ƿ���ƶ�
	bool canOpen() {return door.isOpened();}//�ж�һ�����ܷ��
	void setOpened() {door.setOpened();}//��һ����
	bool openSpecial();
	int getItemID();//��ȡ�õ����Ʒ���
	int getSpecial() {return special;}
	void setSpecial(int _spe) {special=_spe;}
	void distroyItem() {item=NULL;} //��ȡ�õ���Ʒ
	int getNpcID() {return npc.getId();}//����npc��id
	int getNpcVisit() {return npc.getVisit();} //����npc�ķ��ʴ���
	void visitNpc() {npc.setVisit(npc.getVisit()+1);} //����npc�ķ��ʴ���
	bool nearStair() {return door.getType()==4 || door.getType()==5;}
	void changeLight();
	bool isGround();
	void save(FILE*);//�浵
	void load(FILE*);//����
private:
	hgeSprite *type;//�������ͣ��� OR ǽ��
	hgeSprite *item;//��������
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
	bool checkChallenge();//�������ز��Ƿ���� 
	void finishChallenge();//���ظò�
	void changeLight(int,int);//33-34������Ϸ
	void save(FILE*);//�浵
	void load(FILE*);//����
	c_map_point* getinfo(int x, int y) {return &info[x][y];}
private:
	int id;//¥����
	int dx,dy,ux,uy;//¥������
	bool finished; // ���ز��Ƿ����
	c_map_point info[30][30];//ÿ����Ϣ
};