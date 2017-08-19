#include "stdafx.h"

extern c_map_floor map_floor[50];
extern constants consts;

c_hero::c_hero()
{
	dir[0][0]=dir[0][3]=dir[1][1]=dir[1][2]=0;
	dir[0][1]=dir[1][3]=-1;
	dir[0][2]=dir[1][0]=1;
};
void c_hero::init()
{
	lv=1;
	hp=1000;
	atk=10;
	def=10;
	money=0;
	experience=0;
	redkey=0;
	bluekey=0;
	yellowkey=0;
	special=0;
	x=5;
	y=9;
	face=3;
	move=0;
	now_floor=0;
	max_floor=0;
	fly_floor=0;
}
float c_hero::nextX()
{
	return x+dir[0][face];
}
float c_hero::nextY()
{
	return y+dir[1][face];
}
bool c_hero::canMove(int f)
{
	face=f;
	int special=map_floor[now_floor].getSpecial(x,y);
	if (special==203 && f!=3) return false;
	if (special==204 && f!=1) return false;
	if (special==205 && f!=2) return false;
	if (special==206 && f!=0) return false;
	return map_floor[now_floor].canMove(nextX(),nextY(),f);
}
void c_hero::show()
{
	(new hgeSprite(consts.ht_hero,move*32,face*33,32,33))
		->Render(x*32+8*move*dir[0][face]+consts.ScreenLeft, y*32+8*move*dir[1][face]);
}
bool c_hero::nearStair()
{
	if (map_floor[now_floor].getinfo(y,x)->nearStair()) return true;
	if (x>0 && map_floor[now_floor].getinfo(y,x-1)->nearStair()) return true;
	if (y>0 && map_floor[now_floor].getinfo(y-1,x)->nearStair()) return true;
	if (x<consts.map_width-1 && map_floor[now_floor].getinfo(y,x+1)->nearStair()) return true;
	if (y<consts.map_width-1 && map_floor[now_floor].getinfo(y+1,x)->nearStair()) return true;
	return false;
}
bool c_hero::moveComplete()
{
	bool c=false;
	if(move==3)
	{
		c=true;
		x+=dir[0][face];
		y+=dir[1][face];
	}
	if (c)
	{
		char item=map_floor[now_floor].getItem(x,y);
		if(item>=11 && consts.music)
			consts.hge->Effect_PlayEx(consts.he_GetItem,consts.volume);
		switch(item)
		{
		case 10:
			hp-=100;
			if (hp<=0) hp=1;
			break;
		case 11:atk+=3;
			if (!consts.attack_flag) {
				consts.attack_flag=true;
				consts.setMsg(L"获取红宝石，攻击力+3");
			}
			break;
		case 12:def+=3;
			if (!consts.defence_flag) {
				consts.defence_flag=true;
				consts.setMsg(L"获取蓝宝石，防御力+3");
			}
			break;
		case 13:
			consts.book=true;
			consts.setMsg(L"获得怪物手册。\n你可以将鼠标放置在怪物上查\n看怪物的基本信息。从上到下\n依次为怪物的名称、血量、攻\n击、防御、打掉后获得的金币\n、经验，对你造成的伤害。");
			break;
		case 14:
			yellowkey++;bluekey++;redkey++;
			consts.setMsg(L"获取钥匙盒，全钥匙数量+1");
			break;
		case 15:yellowkey++;
			if (!consts.yellow_flag) {
				consts.yellow_flag=true;
				consts.setMsg(L"获得黄钥匙");
			}
			break;
		case 16:bluekey++;
			if (!consts.blue_flag) {
				consts.blue_flag=true;
				consts.setMsg(L"获得蓝钥匙");
			}
			break;
		case 17:redkey++;
			if (!consts.red_flag) {
				consts.red_flag=true;
				consts.setMsg(L"获得红钥匙");
			}
			break;
		case 18:hp+=200;
			if (!consts.small_flag) {
				consts.small_flag=true;
				consts.setMsg(L"获得小血瓶，生命+200");
			}
			break;
		case 19:hp+=500;
			if (!consts.big_flag) {
				consts.big_flag=true;
				consts.setMsg(L"获得大血瓶，生命+500");
			}
			break;
		case 20:atk+=10;consts.setMsg(L"获得铁剑，攻击力+10");break;
		case 21:def+=10;consts.setMsg(L"获得铁盾，防御力+10");break;
		case 22:money+=300;consts.setMsg(L"获得大金币，金币+300");break;
		case 23:lv++;hp+=700;atk+=7;def+=7;
			consts.setMsg(L"获得小飞羽，提升一级\n生命+700，攻击+7，防御+7");break;
		case 24:consts.cross=true;
			consts.setMsg(L"获得十字架，交给0楼的仙子\n可以大幅提升自身能力！");break;
		case 25:atk+=70;consts.setMsg(L"获得银剑，攻击力+70");break;
		case 26:def+=70;consts.setMsg(L"获得银盾，防御力+70");break;
		case 30:hp*=2;consts.setMsg(L"获得圣水，生命值翻倍。");break;
		case 27:consts.canfly=true;consts.setMsg(L"获得楼层飞行器，按F键使用");break;
		case 28:consts.stick=true;consts.setMsg(L"获得锄头，需要把它交给杰克。");break;
		case 29:lv+=3;hp+=2100;atk+=21;def+=21;consts.setMsg(L"获得大飞羽，提升三级\n生命+2100，攻击+21，防御+21");break;
		case 31:atk+=150;consts.setMsg(L"获得神圣剑，攻击力+150");break;
		case 32:def+=150;consts.setMsg(L"获得神圣盾，防御力+150");break;
		case 33:special=1;consts.setMsg(L"我：\n这是啥？要是能找人鉴定一下\n就好了。");break;
		case 34:special=2;consts.setMsg(L"我：\n这是啥？要是能找人鉴定一下\n就好了。");break;
		case 35:special=3;consts.setMsg(L"我：\n这是啥？要是能找人鉴定一下\n就好了。");break;
		}
		int special=map_floor[now_floor].getSpecial(x,y);
		if (special!=0) {
			if (special==201) {
				map_floor[now_floor].setSpecial(x,y,202);
			}
			if (special==209) {
				map_floor[now_floor].changeLight(x,y);
			}
			if (special==210 || special==211) {
				map_floor[now_floor].setSpecial(x,y,special==210?0:209);
				if (map_floor[now_floor].getSpecial(x+dir[0][face],y+dir[1][face])==0) {
					map_floor[now_floor].setSpecial(x+dir[0][face],y+dir[1][face],210);
				}
				else if (map_floor[now_floor].getSpecial(x+dir[0][face],y+dir[1][face])==209) {
					map_floor[now_floor].setSpecial(x+dir[0][face],y+dir[1][face],211);
				}
			}
		}
		if (map_floor[now_floor].checkChallenge())
			map_floor[now_floor].finishChallenge();
		consts.step++;
		if (now_floor==0 && x==5 && y==10) consts.msg=consts.MESSAGE_WIN;
	}
	move++;
	move%=4;
	return c;
}
bool c_hero::openRedDoor()
{
	if(redkey>0)
	{
		redkey--;
		return true;
	}
	return false;
}
bool c_hero::openBlueDoor()
{
	if(bluekey>0)
	{
		bluekey--;
		return true;
	}
	return false;
}
bool c_hero::openYellowDoor()
{
	if(yellowkey>0)
	{
		yellowkey--;
		return true;
	}
	return false;
}
bool c_hero::openSpecialDoor()
{
	if(map_floor[now_floor].getinfo(nextY(),nextX())->canOpen())
		return true;
	return false;
}
void c_hero::upstair()
{
	now_floor++;
	setFlyFloor();
	map_floor[now_floor].getDownPosition(x,y);
	consts.flooring=true;
	consts.step++;
}
void c_hero::downstair()
{
	now_floor--;
	setFlyFloor();
	map_floor[now_floor].getUpPosition(x,y);
	consts.flooring=true;
	consts.step++;
}
void c_hero::specialMove()
{
	consts.flooring=true;
	consts.step++;
	if (now_floor<30) {
		now_floor=31;
		x=10;y=0;
	}
	else {
		now_floor=10;
		x=10;y=0;
	}
}
void c_hero::fly()
{
	if(now_floor<=fly_floor && fly_floor!=0)map_floor[fly_floor].getDownPosition(x,y);
	else map_floor[fly_floor].getUpPosition(x,y);
	now_floor=fly_floor;
	consts.flooring=true;
}
int c_hero::getSpecialLv()
{
	if (lv<=10) return 1;
	if (lv<=20) return 2;
	if (lv<=45) return 3;
	if (lv<=80) return 4;
	return 5;
}
int c_hero::getSpeDef()
{
	int slv=getSpecialLv();
	if (special!=2 || slv==1) return 0;
	if (slv==2) return def*0.03;
	if (slv==3) return def*0.06;
	if (slv==4) return def*0.1;
	return def*0.15;
}
int c_hero::getSpeLife(int lf)
{
	int slv=getSpecialLv();
	if (special!=3 || slv==1) return 0;
	if (slv==2) return lf*0.2;
	else if (slv==3) return lf*0.33;
	else if (slv==4) return lf*0.5;
	return lf;
}
int c_hero::getSpeAtkTimes()
{
	if (special!=1) return 1;
	return getSpecialLv();
}
void c_hero::printInfo()
{
	int py=16;
	consts.s_storey->Render(16,py);
	if (now_floor<30)
		consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",now_floor);
	else consts.hgef->printf(60,py,HGETEXT_LEFT,"???");
	py+=32;
	consts.s_level->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",lv);
	if (special!=0) {
		if (special==1)
			consts.s_atk->Render(160,py);
		if (special==2)
			consts.s_def->Render(160,py);
		if (special==3)
			consts.s_life->Render(160,py);
		consts.hgef->printf(204,py,HGETEXT_LEFT,"%d",getSpecialLv());
	}
	py+=32;
	consts.s_heart->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",hp);
	py+=32;
	consts.s_sword1->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",atk);
	py+=32;
	consts.s_shield1->Render(16,py);
	if (special!=2)
		consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",def);
	else
		consts.hgef->printf(60,py,HGETEXT_LEFT,"%d (+%d)",def,getSpeDef());
	py+=32;
	consts.s_yellowkey->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",yellowkey);
	py+=32;
	consts.s_bluekey->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",bluekey);
	py+=32;
	consts.s_redkey->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",redkey);
	py+=32;
	consts.s_coin->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",money);
	py+=32;
	consts.s_expr->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",experience);

}
bool c_hero::canBeat(c_monster monster) // 判断能否打败
{
	return hp>getDamage(monster);
}
int c_hero::getDamage(c_monster monster) // 打败怪物，返回hp
{
	int monster_def=monster.getSpecial()==2?atk-1:monster.getDef();
	if (atk<=monster_def) return MAX_DAMAGE;
	int val=monster.getSpecial()==1?hp/3:0;
	if (monster.getSpecial()!=3 && getDef()>=monster.getAtk()) return val;
	return val+(monster.getHp()-1)/((atk-monster_def)*getSpeAtkTimes())*(monster.getAtk()-(monster.getSpecial()==3?0:getDef()));
}
bool c_hero::attack(c_monster &monster)
{
	if (consts.music)
		consts.hge->Effect_PlayEx(consts.he_Attack,consts.volume);
	int times=getSpeAtkTimes();
	for (int i=1;i<=times;i++) {
		if (monster.beAttacked(atk))
		{
			money+=monster.getMoney();
			experience+=monster.getExperience();
			return false;
		}
	}
	return true;
}
void c_hero::beAttacked(c_monster monster)
{
	if (monster.getSpecial()==3) hp-=monster.getAtk();
	else if (getDef()<monster.getAtk())
		hp-=monster.getAtk()-getDef();
}
void c_hero::npc(int select)
{
	int npcid=map_floor[now_floor].getinfo(nextY(),nextX())->getNpcID();
	int npctime=map_floor[now_floor].getinfo(nextY(),nextX())->getNpcVisit();
	switch (npcid)
	{
	case 40:
		if (npctime==0)
		{
			const wchar_t* npc_hint[50] = {
				L"老人：\n孩子，这座塔很危险，你怎么\n进来了？",
				L"我：\n我女朋友在里面，我要冲进去\n救她！",
				L"老人：\n勇气可嘉！我有本书遗失在了\n三楼，如果你能获得它，或许\n能对你有所帮助。",
				L"我：\n谢谢您！",
				L"老人：\n另外，多和塔里的老人们进行\n对话，会得到很多有用的情报\n的。",
				L"老人：\n还有，一定要注意随时存档！"
			};
			consts.setMsg(npc_hint);
			map_floor[now_floor].getinfo(nextY(),nextX())->visitNpc();
		}
		else
			consts.setMsg(L"老人：\n多和塔里的老人们进行对话，\n会得到很多有用的情报的。\n同时，一定要注意随时存档！");
		break;
	case 41:
		if (npctime==0)
		{
			atk+=30;
			map_floor[now_floor].getinfo(nextY(),nextX())->visitNpc();
			consts.setMsg(L"老人：\n孩子，感谢你救了我。\n这30点攻击就是我送你的！");
		}
		else
			consts.setMsg(L"老人：\n经过熔岩地面会扣除你100生命\n值，最低将你的生命值扣到1。");
		break;
	case 42:
		if (npctime==0)
		{
			def+=30;
			map_floor[now_floor].getinfo(nextY(),nextX())->visitNpc();
			consts.setMsg(L"老人：\n孩子，感谢你救了我。\n这30点防御就是我送你的！");
		}
		else
			consts.setMsg(L"老人：\n遇到圣水不要急着去取，它会\n将你的生命值翻倍。");
		break;
	case 43:
		if (npctime==0)
		{
			map_floor[2].getinfo(6,1)->setOpened();
			map_floor[now_floor].getinfo(nextY(),nextX())->visitNpc();
			const wchar_t* thief_hint[50] = {
				L"我：\n你是谁？怎么被关押在这里？",
				L"杰克：\n我叫杰克；是住在附近的一个\n居民。听说这个塔里有宝藏，\n我就进来了，结果刚进来就被\n抓了。",
				L"我：\n宝藏？什么宝藏？",
				L"杰克：\n我也只是听说... \n不过谢谢你救了我！",
				L"我：\n你会开门吗？",
				L"杰克：\n会啊，怎么了？",
				L"我：\n那你能把二楼的门打开吗？",
				L"杰克：\n好的，我这就去帮你打开；另\n外，如果你能找到我遗失的锄\n头，我也能帮你把18楼的门给\n打开。",
				L"我：\n谢谢，我这就去帮你找你的锄\n头。"
			};
			consts.setMsg(thief_hint);
		}
		else if (npctime==1)
		{
			if (consts.stick) {
				map_floor[18].getinfo(5,5)->setOpened();
				map_floor[now_floor].getinfo(nextY(),nextX())->visitNpc();
				consts.setMsg(L"杰克：\n你找到了我的锄头啊！\n18楼的门已经为你打开。");
			}
			else {
				consts.setMsg(L"杰克：\n锄头找到了吗？");
			}
		}
		else
			consts.setMsg(L"杰克：\n还有什么事吗？");
		break;

	case 44:
		if (npctime==0)
		{
			const wchar_t* fairy_hint[50]={
				L"123"
			};
			consts.setMsg(fairy_hint, L"仙子");
			map_floor[0].getinfo(7,5)->setOpened();
			map_floor[now_floor].getinfo(nextY(),nextX())->visitNpc();
		}
		else if(npctime==1)
		{
			if (consts.cross) {
				consts.setMsg(L"仙子：\n对，就是这个十字架！\n巴拉巴拉...轰！\n......\n全属性提高25%！\n去吧，我只能帮你到这里了！");
				hp=hp*1.25;
				atk=atk*1.25;
				def=def*1.25;
				map_floor[20].getinfo(6,5)->setOpened();
				map_floor[20].getinfo(7,4)->setOpened();
				map_floor[20].getinfo(7,6)->setOpened();
				map_floor[20].getinfo(8,5)->setOpened();
				map_floor[now_floor].getinfo(nextY(),nextX())->visitNpc();
			}
			else {
				consts.setMsg(L"仙子：\n找到十字架了吗？");
			}
		}
		else if (consts.lefttime<80) {
			if (consts.trueend) {
				consts.setMsg(L"仙子：\n快走吧孩子，再不走就来不及\n了！");
			}
			else {
				consts.trueend=true;
				const wchar_t* princess_hint[50]={
					L"123"
				};
				consts.setMsg(princess_hint);
			}
		}
		else consts.setMsg(L"仙子：\n我只能帮你到这里了，加油把\n你女朋友救出来吧！");
		break;

	case 45: // The first exp store
		if (select==1 && experience>=100)
		{
			lv++;
			hp+=700;
			atk+=7;
			def+=7;
			experience-=100;
		}
		if (select==2 && experience>=30)
		{
			atk+=5;
			experience-=30;
		}
		if (select==3 && experience>=30)
		{
			def+=5;
			experience-=30;
		}
		break;
	case 46: // The key store
		if (select==1 && money>=10)
		{
			yellowkey++;
			money-=10;
		}
		if (select==2 && money>=50)
		{
			bluekey++;
			money-=50;
		}
		if (select==3 && money>=100)
		{
			redkey++;
			money-=100;
		}
		break;
	case 47: // The first store
		if (select==1 && money>=25)
		{
			hp+=400;
			money-=25;
		}
		if (select==2 && money>=25)
		{
			atk+=4;
			money-=25;
		}
		if (select==3 && money>=25)
		{
			def+=4;
			money-=25;
		}
		break;
	case 48: // The second store
		if (select==1 && money>=100)
		{
			hp+=2000;
			money-=100;
		}
		if (select==2 && money>=100)
		{
			atk+=20;
			money-=100;
		}
		if (select==3 && money>=100)
		{
			def+=20;
			money-=100;
		}
		break;
	case 49: // The second exp store
		if (select==1 && experience>=270)
		{
			experience-=270;
			lv+=3;
			hp+=2100;
			atk+=21;
			def+=21;
		}
		if (select==2 && experience>=95)
		{
			experience-=95;
			atk+=17;
		}
		if (select==3 && experience>=95)
		{
			experience-=95;
			def+=17;
		}
		break;
	case 50: // sell keys store
		if (select==1 && yellowkey>=1)
		{
			yellowkey--;
			money+=7;
		}
		if (select==2 && bluekey>=1)
		{
			bluekey--;
			money+=35;
		}
		if (select==3 && redkey>=1)
		{
			redkey--;
			money+=70;
		}
		break;
	case 51:
		if(npctime==0)
		{
			const wchar_t *princess_hint[50]={
				L""
			};
			consts.setMsg(princess_hint);
			map_floor[now_floor].getinfo(nextY(),nextX())->visitNpc();
			map_floor[18].getinfo(10,9)->setOpened();
		}
		else {
			if (consts.lefttime<80)
				consts.setMsg(L"？？？：\n和我们一起陪葬吧，哈哈哈！");
			else 
				consts.setMsg(L"？？？：\n不敢来了？那你就别想见到她\n了！");
		}
		break;
	case 52:
		if(npctime==0)
		{
			const wchar_t *princess_hint[50]={
				L"你好"
			};
			consts.setMsg(princess_hint, L"仙子");
			map_floor[now_floor].getinfo(nextY(),nextX())->visitNpc();
			map_floor[21].getinfo(7,4)->setOpened();
			map_floor[21].getinfo(7,6)->setOpened();
			map_floor[0].getinfo(10,5)->setOpened();
		}
		else consts.setMsg(L"XX：\n塔要倒了，快走吧！我随后就\n跟来！");
		break;
	case 53:
		if(npctime==0)
		{
			if (select==1 && experience>=500)
			{
				experience-=500;
				atk+=120;
				map_floor[now_floor].getinfo(nextY(),nextX())->visitNpc();
			}
		}
		else
			consts.setMsg(L"老人：\n注意备足钥匙，小心不够。");
		break;
	case 54:
		if(npctime==0)
		{
			if (select==1 && money>=500)
			{
				money-=500;
				def+=120;
				map_floor[now_floor].getinfo(nextY(),nextX())->visitNpc();
			}
		}
		else
			consts.setMsg(L"老人：\n十字架最好冲到17楼时再给仙\n子，太早的话是没办法过的。\n当然，如果能在最终决战前再\n把十字架给仙子，那就更没有\n压力了。");
		break;
	default:
		consts.setMsg(L"我：\n这是啥？");
		break;
	}
}
void c_hero::save(FILE* f)
{
	fprintf_s(f,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
		lv,hp,atk,def,money,experience,redkey,bluekey,yellowkey,special,x,y,face,now_floor,max_floor,fly_floor);
}
void c_hero::load(FILE* f)
{
	fscanf_s(f,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
		&lv,&hp,&atk,&def,&money,&experience,&redkey,&bluekey,&yellowkey,&special,&x,&y,&face,&now_floor,&max_floor,&fly_floor);
}
void c_hero::setFlyFloor(int delta)
{
	if (delta==0 && now_floor<30) 
		fly_floor=now_floor;
	else
	{
		int tmpfloor=fly_floor+delta;
		if (tmpfloor<=max_floor && tmpfloor>=0 && tmpfloor<=20) fly_floor=tmpfloor;
	}
}