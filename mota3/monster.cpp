#include "stdafx.h"

extern c_hero hero;
extern constants consts;
void c_monster::init(int t)
{
	id=t;
	state=0;
	times=0;
	special=0;
	
	// 1 - 先攻
	// 2 - 魔攻
	// 3 - 坚固

	switch(t)
	{
	case 1:wcscpy_s(name,L"绿色史莱姆");position=0;hp=50;atk=15;def=5;money=1;break;
	case 2:wcscpy_s(name,L"红色史莱姆");position=1;hp=70;atk=20;def=3;money=2;break;
	case 3:wcscpy_s(name,L"青头怪");position=2;hp=150;atk=35;def=9;money=5;break;
	case 4:wcscpy_s(name,L"史莱姆王");position=3;hp=700;atk=250;def=125;money=32;break;
	case 5:wcscpy_s(name,L"小蝙蝠");position=4;hp=110;atk=30;def=0;money=3;special=1;break;
	case 6:wcscpy_s(name,L"大蝙蝠");position=5;hp=150;atk=65;def=30;money=10;break;
	case 7:wcscpy_s(name,L"红蝙蝠");position=6;hp=550;atk=160;def=95;money=25;break;
	case 8:wcscpy_s(name,L"冥灵魔王");position=7;hp=30000;atk=2100;def=1700;money=250;break;
	case 9:wcscpy_s(name,L"初级法师");position=8;hp=125;atk=50;def=0;money=10;special=2;break;
	case 10:wcscpy_s(name,L"高级法师");position=9;hp=1000;atk=200;def=110;money=30;break;
	case 11:wcscpy_s(name,L"初级巫师");position=10;hp=500;atk=120;def=70;money=20;break;
	case 12:wcscpy_s(name,L"高级巫师");position=11;hp=1234;atk=400;def=260;money=47;break;
	case 13:wcscpy_s(name,L"骷髅人");position=12;hp=130;atk=30;def=5;money=5;break;
	case 14:wcscpy_s(name,L"骷髅士兵");position=13;hp=300;atk=40;def=20;money=8;break;
	case 15:wcscpy_s(name,L"骷髅队长");position=14;hp=400;atk=90;def=50;money=15;break;
	case 16:wcscpy_s(name,L"冥队长");position=15;hp=3000;atk=880;def=790;money=80;break;
	case 17:wcscpy_s(name,L"兽人");position=16;hp=300;atk=75;def=45;money=13;break;
	case 18:wcscpy_s(name,L"兽面武士");position=17;hp=900;atk=450;def=330;money=50;break;
	case 19:wcscpy_s(name,L"石头人");position=18;hp=100;atk=25;def=0;money=15;special=3;break;
	case 20:wcscpy_s(name,L"影子战士");position=19;hp=3100;atk=1250;def=1050;money=105;break;
	case 21:wcscpy_s(name,L"初级卫兵");position=20;hp=450;atk=150;def=90;money=22;break;
	case 22:wcscpy_s(name,L"中级卫兵");position=21;hp=1250;atk=500;def=400;money=55;break;
	case 23:wcscpy_s(name,L"高级卫兵");position=22;hp=1500;atk=560;def=460;money=60;break;
	case 24:wcscpy_s(name,L"双手剑士");position=23;hp=1200;atk=620;def=520;money=65;break;
	case 25:wcscpy_s(name,L"冥战士");position=24;hp=2000;atk=680;def=590;money=70;break;
	case 26:wcscpy_s(name,L"初级骑士");position=25;hp=850;atk=350;def=200;money=45;break;
	case 27:wcscpy_s(name,L"高级骑士");position=26;hp=900;atk=750;def=650;money=77;break;
	case 28:wcscpy_s(name,L"灵武士");position=27;hp=3000;atk=980;def=900;money=88;break;
	case 29:wcscpy_s(name,L"红衣魔王");position=28;hp=15000;atk=1000;def=1000;money=100;break;
	case 30:wcscpy_s(name,L"魔法警卫");position=29;hp=1300;atk=300;def=150;money=40;break;
	case 31:wcscpy_s(name,L"灵法师");position=30;hp=1500;atk=830;def=730;money=80;break;
	case 36:wcscpy_s(name,L"冥队长");position=15;hp=3333;atk=1200;def=1133;money=112;break;
	case 38:wcscpy_s(name,L"黑衣魔王");position=36;hp=50000;atk=3300;def=2600;money=0;break;
	case 39:wcscpy_s(name,L"铁面人");position=34;hp=50;atk=1221;def=0;money=127;break;
	case 46:wcscpy_s(name,L"高级巫师");position=11;hp=5000;atk=2500;def=1500;money=0;break;
	case 47:wcscpy_s(name,L"铁面人");position=34;hp=100;atk=2333;def=0;money=0;break;
	case 48:wcscpy_s(name,L"灵武士");position=27;hp=1600;atk=1306;def=1200;money=117;break;
	case 49:wcscpy_s(name,L"红衣魔王");position=28;hp=20000;atk=1777;def=1444;money=133;break;
	default:position=-1;
	}
	if (position!=-1)
		for(int i=0;i<4;i++)
			monster[i]=new hgeSprite(consts.ht_monster,32*i,32*position,32,32);
}
int c_monster::getDef() {
	return special==3?hero.getAtk()-1:def;
}
void c_monster::show(GfxFont* f,int i,int j)
{
	if(id!=0)
	{
		monster[state]->Render(j*32+consts.ScreenLeft,i*32);
		if (consts.book && f!=NULL) {
			int damage=hero.getDamage(this);
			int hp=hero.getHP();
			if (damage>=hp)
				f->SetColor(0xFFFF0000);
			else if (damage<=hp/10)
				f->SetColor(0xFF00FF00);
			else if (damage<=hp/5)
				f->SetColor(0xFF66CD00);
			else if (damage<=hp/2)
				f->SetColor(0xFF96CDCD);
			else if (damage<=hp*3/4)
				f->SetColor(0xFFEE7942);
			else f->SetColor(0xFFEE3B3B);
			if (damage>=c_hero::MAX_DAMAGE)
				f->Print(j*32+consts.ScreenLeft, i*32+20, L"???");
			else if (damage<100000)
				f->Print(j*32+consts.ScreenLeft, i*32+20, L"%d", damage);
			else
				f->Print(j*32+consts.ScreenLeft, i*32+20, L"%.1fW", damage/10000.0);
		}
	}
}
void c_monster::printInfo()
{
	if(id!=0)
	{
		int py=16;
		GfxFont *f=new GfxFont(L"楷体",24);
		wchar_t s0[100]=L"";
		if (special==1) wcscpy_s(s0, L"（先攻）");
		else if (special==2) wcscpy_s(s0, L"（魔攻）");
		else if (special==3) wcscpy_s(s0, L"（坚固）");
		f->Print(consts.ScreenLeft+consts.map_width*32+16,py,L"%s%s",name,s0);
		delete f;
		py+=32;
		consts.s_heart->Render(consts.ScreenLeft+consts.map_width*32+16,py);
		consts.hgef->printf(consts.ScreenLeft+consts.map_width*32+60,py,HGETEXT_LEFT,"%d",getHp());
		py+=32;
		consts.s_sword1->Render(consts.ScreenLeft+consts.map_width*32+16,py);
		consts.hgef->printf(consts.ScreenLeft+consts.map_width*32+60,py,HGETEXT_LEFT,"%d",getAtk());
		py+=32;
		consts.s_shield1->Render(consts.ScreenLeft+consts.map_width*32+16,py);
		consts.hgef->printf(consts.ScreenLeft+consts.map_width*32+60,py,HGETEXT_LEFT,"%d",getDef());
		py+=32;
		consts.s_coin->Render(consts.ScreenLeft+consts.map_width*32+16,py);
		consts.hgef->printf(consts.ScreenLeft+consts.map_width*32+60,py,HGETEXT_LEFT,"%d",getMoney());
		py+=32;
		consts.s_damage->Render(consts.ScreenLeft+consts.map_width*32+16,py);
		int damage=hero.getDamage(this);
		if (damage==c_hero::MAX_DAMAGE)
			consts.hgef->printf(consts.ScreenLeft+consts.map_width*32+60,py,HGETEXT_LEFT,"???");
		else
			consts.hgef->printf(consts.ScreenLeft+consts.map_width*32+60,py,HGETEXT_LEFT,"%d",damage);
	}
}
void c_monster::changeState()
{
	state++;
	state%=4;
}