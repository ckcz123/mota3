#include "stdafx.h"

extern c_map_floor map_floor[50];
extern c_hero hero;
extern constants consts;

void c_map_door::init(int t)
{
	type=t;
	if(t<=4)for(int i=0;i<4;i++) door[i]=new hgeSprite(consts.ht_map,32*(t-1),32*(i+2),32,32);
	if(t==5)for(int i=0;i<4;i++) door[i]=consts.s_shopleft;
	if(t==6)for(int i=0;i<4;i++) door[i]=consts.s_shopright;
	if(t==7)for(int i=0;i<4;i++) door[i]=consts.s_upstair;
	if(t==8)for(int i=0;i<4;i++) door[i]=consts.s_downstair;
	state=0;
}
void c_map_door::show(int i,int j)
{
	if(type!=0 && type<=10)door[state]->Render(j*32+consts.ScreenLeft,i*32);
}
bool c_map_door::open()
{
	state++;
	if(state==3)
	{
		this->init(0);
		return true;
	}
	return false;
}
void c_map_point::init(int _type,int _item,int s_monster,int s_door,int s_npc,int s_spe)
{
	type=_type;
	item=_item;
	monster.init(s_monster);
	door.init(s_door);
	npc.init(s_npc);
	special=s_spe;
}
void c_map_point::show(GfxFont* f,int i,int j)
{
	// type->Render(j*32+consts.ScreenLeft,i*32);
	if (type==5) consts.s_ground->Render(j*32+consts.ScreenLeft,i*32);
	getTypeSprite()->Render(j*32+consts.ScreenLeft,i*32);
	if (getItemSprite()!=NULL)
		getItemSprite()->Render(j*32+consts.ScreenLeft,i*32);
	monster.show(f,i,j);
	door.show(i,j);
	npc.show(i,j);
	if(special!=0) {
		switch (special)
		{
		case 201:
			consts.s_sigh->Render(j*32+consts.ScreenLeft,i*32);
			break;
		case 202:
			consts.s_sighed->Render(j*32+consts.ScreenLeft,i*32);
			break;
		case 203:
			consts.s_up->Render(j*32+consts.ScreenLeft,i*32);
			break;
		case 204:
			consts.s_left->Render(j*32+consts.ScreenLeft,i*32);
			break;
		case 205:
			consts.s_right->Render(j*32+consts.ScreenLeft,i*32);
			break;
		case 206:
			consts.s_down->Render(j*32+consts.ScreenLeft,i*32);
			break;
		case 207:
			consts.s_wall_hidden->Render(j*32+consts.ScreenLeft,i*32);
			break;
		case 208:
			consts.s_portal->Render(j*32+consts.ScreenLeft,i*32);
			break;
		case 209:
			consts.s_flower->Render(j*32+consts.ScreenLeft,i*32);
			break;
		case 210:
			consts.s_box->Render(j*32+consts.ScreenLeft,i*32);
			break;
		default:
			break;
		}

	}
}
hgeSprite* c_map_point::getTypeSprite() {
	if (type==1) return consts.s_wall;
	if (type==2) return consts.s_wall2;
	if (type==3) return consts.s_water;
	if (type==4) return consts.s_sky;
	if (type==5) return consts.s_barrier;
	return consts.s_ground;
}
hgeSprite* c_map_point::getItemSprite() {
	switch (item)
	{
	case 10:return consts.s_lightning;
	case 11:return consts.s_redjewel;break;
	case 12:return consts.s_bluejewel;break;
	case 13:return consts.s_enemyinfo;break;
	case 14:return consts.s_allkey;break;
	case 15:return consts.s_yellowkey;break;
	case 16:return consts.s_bluekey;break;
	case 17:return consts.s_redkey;break;
	case 18:return consts.s_redpotion;break;
	case 19:return consts.s_bluepotion;break;
	case 20:return consts.s_sword1;break;
	case 21:return consts.s_shield1;break;
	case 22:return consts.s_coin;break;
	case 23:return consts.s_fly;break;
	case 24:return consts.s_cross;break;
	case 25:return consts.s_sword2;break;
	case 26:return consts.s_shield2;break;
	case 27:return consts.s_floor;break;
	case 28:return consts.s_stick;break;
	case 29:return consts.s_fly2;break;
	case 30:return consts.s_drink;break;
	case 31:return consts.s_sword3;break;
	case 32:return consts.s_shield3;break;
	case 33:return consts.s_atk;break;
	case 34:return consts.s_def;break;
	case 35:return consts.s_life;break;
	}
	return NULL;
}
bool c_map_point::isGround()
{
	return type==0 && item==0 && monster.getId()==0 && door.getType()==0 && npc.getId()==0 && special==0;
}
bool c_map_point::canMove(int f)
{
	if(type==0)
	{
		if(door.getType()!=0)
		{
			bool c=false;
			switch(door.getType())
			{
			case 1:c=hero.openYellowDoor();break;
			case 2:c=hero.openBlueDoor();break;
			case 3:c=hero.openRedDoor();break;
			case 7:hero.upstair();break;
			case 8:hero.downstair();break;
			}
			if(c)
			{
				consts.opening=true;
				consts.map_openingdoor=&door;
				if (consts.music)
					consts.hge->Effect_PlayEx(consts.he_OpenDoor,consts.volume);
			}
			return c;
		}
		if(npc.getId()!=0) {
			consts.msg=consts.MESSAGE_NPC;
			return false;
		}
		if(monster.getId()!=0)
		{
			if(hero.canBeat(&monster))
				hero.beat(&monster);
			return false;
		}
		if(special!=0)
		{
			if(special==207) {
				special=0;
				consts.lasttime=clock();
				consts.hge->Effect_PlayEx(consts.he_Attack, consts.volume);
				return false;
			}
			if(special==208) {
				hero.specialMove();
				return false;
			}
			if(special==202) return false;
			if(special==203 && f==0) return false;
			if(special==204 && f==2) return false;
			if(special==205 && f==1) return false;
			if(special==206 && f==3) return false;
		}
		return true;
	}
	else return false;
}
bool c_map_point::openSpecial()
{
	if (door.getType()!=4) return false;
	consts.opening=true;
	consts.map_openingdoor=&door;
	if (consts.music)
		consts.hge->Effect_PlayEx(consts.he_OpenDoor,consts.volume);
	return true;
}
int c_map_point::getItemID()
{
	return item;
}
void c_map_point::save(FILE* f)
{
	fprintf_s(f,"%d %d %d %d %d %d %d %d\n",type,item,door.getType(),monster.getId(),
		monster.getTimes(),npc.getId(),npc.getVisit(),special);
}
void c_map_point::load(FILE* f)
{
	int d,m,t1,n,t2;
	fscanf_s(f,"%d %d %d %d %d %d %d %d\n",&type,&item,&d,&m,&t1,&n,&t2,&special);
	door.init(d);
	monster.init(m);
	monster.setTimes(t1);
	npc.init(n);
	npc.setVisit(t2);
}
void c_map_floor::init(int d,int ch[30][30])
{
	id=d;
	ux=-100;uy=-100;dx=-100;dy=-100;
	for(int i=0;i<consts.map_height;i++)
	{
		for(int j=0;j<consts.map_width;j++)
		{
			if (ch[i][j]<=9)info[i][j].init(ch[i][j],0,0,0);
			else if (ch[i][j]<=40)info[i][j].init(0,ch[i][j],0,0);
			else if (ch[i][j]<=80)info[i][j].init(0,0,0,0,ch[i][j]);
			else if (ch[i][j]<=100)info[i][j].init(0,0,0,ch[i][j]-80);
			else if (ch[i][j]<=200)info[i][j].init(0,0,ch[i][j]-100,0);
			else info[i][j].init(0,0,0,0,0,ch[i][j]);
			if(ch[i][j]==87)
			{
				ux=j;
				uy=i;
			}
			if(ch[i][j]==88)
			{
				dx=j;
				dy=i;
			}
		}
	}
}
void c_map_floor::show()
{
	GfxFont *f=new GfxFont(L"����",12,true);
	for(int i=0;i<consts.map_height;i++)
	{
		for(int j=0;j<consts.map_width;j++)
		{
			info[i][j].show(f,i,j);
		}
	}
	delete f;
}
void c_map_floor::animation()
{	
	for(int i=0;i<consts.map_height;i++)
		for(int j=0;j<consts.map_width;j++)
			info[i][j].animation();
}
void c_map_floor::printMonsterInfo(int i,int j)
{
	info[j][i].printMonsterInfo();
}
bool c_map_floor::canMove(int x,int y,int f)
{
	if (info[y][x].canMove(f)) {
		int spe=info[y][x].getSpecial();
		if (spe!=210 && spe!=211) return true;
		int sx=x+hero.dir[0][f],sy=y+hero.dir[1][f];
		if (!info[sy][sx].isGround()) return false;
		if (info[sy][sx].getSpecial()==0 || info[sy][sx].getSpecial()==209) return true;
		return false;
	}
	return false;
}
int c_map_floor::getItem(int x,int y)
{
	int c=info[y][x].getItemID();
	if (c!=10)
		info[y][x].distroyItem();
	return c;
}
int c_map_floor::getSpecial(int x, int y)
{
	return info[y][x].getSpecial();
}
void c_map_floor::setSpecial(int x, int y, int _spe)
{
	info[y][x].setSpecial(_spe);
}
void c_map_floor::getDownPosition(int &x,int &y)
{
	x=dx;
	y=dy;
	if (hero.getNowFloor()==21)
		y-=2;
}
void c_map_floor::getUpPosition(int &x,int &y)
{
	x=ux;
	y=uy;
}
bool c_map_floor::hasMonster(int id) {
	for(int i=0;i<consts.map_height;i++)
		for(int j=0;j<consts.map_width;j++)
			if (info[i][j].hasMonster(id))
				return true;
	return false;
}
void c_map_floor::save(FILE* f)
{
	fprintf_s(f,"%d %d %d %d\n",dx,dy,ux,uy);
	for(int i=0;i<consts.map_height;i++)
	{
		for(int j=0;j<consts.map_width;j++)
		{
			info[i][j].save(f);
		}
	}
}
void c_map_floor::load(FILE* f)
{
	fscanf_s(f,"%d %d %d %d",&dx,&dy,&ux,&uy);
	for(int i=0;i<consts.map_height;i++)
	{
		for(int j=0;j<consts.map_width;j++)
		{
			info[i][j].load(f);
		}
	}
}