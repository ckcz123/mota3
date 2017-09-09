#include "stdafx.h"

extern constants consts;

void c_map_npc::init(int t)
{
	if (t<=9) {id=0;return;}
	id=t;
	state=0;
	visit=0;
	if (id==46 || id==51 || id==52 || id==53) {
		for(int i=0;i<4;i++) npc[i]=new hgeSprite(consts.ht_npc,32*i,32,32,32);
	}
	else if (id==45 || id==50) {
		for(int i=0;i<4;i++) npc[i]=new hgeSprite(consts.ht_npc,32*i,192,32,32);
	}
	else if (id==69) {
		for(int i=0;i<4;i++) npc[i]=new hgeSprite(consts.ht_special2,160,64,32,32);
	}
	else if (id==70) {
		for(int i=0;i<4;i++) npc[i]=new hgeSprite(consts.ht_special2,160,96,32,32);
	}
	else {
		for(int i=0;i<4;i++) npc[i]=new hgeSprite(consts.ht_npc,32*i,0,32,32);
	}
}
void c_map_npc::show(int i,int j)
{
	if(id!=0) npc[state]->Render(j*32+consts.ScreenLeft,i*32);
}
void c_map_npc::changeState()
{
	state++;
	state%=4;
}
int c_map_npc::getId()
{
	return id;
}