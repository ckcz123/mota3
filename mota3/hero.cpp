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
	hp=1000;
	atk=10;
	def=10;
	money=0;
	redkey=0;
	bluekey=0;
	yellowkey=0;
	x=6;
	y=12;
	face=3;
	move=0;
	now_floor=0;
	max_floor=0;
	fly_floor=0;
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			sprites[i][j]=new hgeSprite(consts.ht_hero, 32*i, 33*j, 32, 33);
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
	int nx=nextX(), ny=nextY();
	if (nx<0 || nx>=consts.map_width || ny<0 || ny>=consts.map_height)
		return false;
	return map_floor[now_floor].canMove(nx,ny,f);
}
void c_hero::show()
{
	sprites[move][face]->Render(x*32+8*move*dir[0][face]+consts.ScreenLeft, y*32+8*move*dir[1][face]);
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
		int level=(now_floor+4)/5;
		switch(item)
		{
		case 10:
			hp-=100;
			if (hp<=0) hp=1;
			break;
		case 11:atk+=2*level;
			break;
		case 12:def+=2*level;
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
			break;
		case 16:bluekey++;
			break;
		case 17:redkey++;
			break;
		case 18:hp+=200;
			break;
		case 19:hp+=500;
			break;
		case 20:
			atk+=10;
			consts.setMsg(L"获得铁剑，攻击力+10。");
			break;
		case 21:
			def+=10;
			consts.setMsg(L"获得铁盾，防御力+10。");
			break;
		}
		int special=map_floor[now_floor].getSpecial(x,y);
		if (special!=0) {
			if (special==201) {
				map_floor[now_floor].setSpecial(x,y,202);
			}
		}
		consts.step++;
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
}
void c_hero::fly()
{
	if(now_floor<=fly_floor && fly_floor!=0)map_floor[fly_floor].getDownPosition(x,y);
	else map_floor[fly_floor].getUpPosition(x,y);
	now_floor=fly_floor;
	consts.flooring=true;
}
void c_hero::printInfo()
{
	int py=16;
	consts.s_storey->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",now_floor);
	py+=32;
	consts.s_heart->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",hp);
	py+=32;
	consts.s_sword1->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",atk);
	py+=32;
	consts.s_shield1->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",def);
	py+=32;
	consts.s_coin->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",money);
	py+=32;
	consts.s_yellowkey->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",yellowkey);
	py+=32;
	consts.s_bluekey->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",bluekey);
	py+=32;
	consts.s_redkey->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",redkey);

}
bool c_hero::canBeat(c_monster* monster) // 判断能否打败
{
	return hp>getDamage(monster);
}
int c_hero::getDamage(c_monster* monster) // 打败怪物，返回hp
{
	int mon_def=monster->getDef(), mon_atk=monster->getAtk();
	if (atk<=mon_def) return MAX_DAMAGE;
	// 魔攻
	int hero_def=monster->getSpecial()==2?0:def;
	if (hero_def>=mon_atk) return 0;
	// 先攻
	int damage=monster->getSpecial()==1?mon_atk-hero_def:0;
	return damage+(monster->getHp()-1)/(atk-mon_def)*(mon_atk-hero_def);
}
void c_hero::beat(c_monster* monster)
{
	int d=getDamage(monster), id=monster->getId();
	hp-=d;
	if (consts.music)
		consts.hge->Effect_PlayEx(consts.he_Attack, consts.volume);
	if (hp<=0) {
		hp=0;
		consts.msg=consts.MESSAGE_LOSE;
		return;
	}

	money+=monster->getMoney();

	// 重生
	if (d!=0)
	{
		int nx=x-dir[0][face], ny=y-dir[1][face];
		if (nx>=0 && nx<consts.map_width && ny>=0 && ny<consts.map_height) {
			c_map_point* point=map_floor[now_floor].getinfo(ny,nx);
			if (point->isGround()) {
				point->init(0,0,monster->getId(),0);
				point->getMonster()->setTimes(monster->getTimes()+1);
			}
		}
	}

	monster->init(0);

	// 打完，触发一些事件

	if (now_floor==2 && id==22) {
		// 2楼卫兵彻底打死且不能重生
		if (!map_floor[now_floor].hasMonster(22)) {
			map_floor[now_floor].getinfo(3,6)->openSpecial();
		}
	}
	if (now_floor==4 && x<=2 && y>=3 && y<=5)
	{
		// 4楼开门
		bool canopen=true;
		for (int i=0;i<=2;i++) {
			for (int j=3;j<=5;j++) {
				if (map_floor[now_floor].getinfo(j,i)->hasMonster()) {
					canopen=false;
				}
			}
		}
		if (canopen) {
			map_floor[now_floor].getinfo(2,1)->openSpecial();
		}
	}

	consts.lasttime=clock();
}
bool c_hero::canReborn()
{
	int nx=x-dir[0][face], ny=y-dir[1][face];
	if (nx>=0 && nx<consts.map_width && ny>=0 && ny<consts.map_height) {
		c_map_point* point=map_floor[now_floor].getinfo(ny,nx);
		if (point->isGround())
			return true;
	}
	return false;
}
void c_hero::npc(int select)
{
	c_map_npc* map_npc=map_floor[now_floor].getinfo(nextY(),nextX())->getNpc();
	if (map_npc->getId()==0) return;
	int npcid=map_npc->getId();
	int npctime=map_npc->getVisit();
	switch (npcid)
	{
	case 41:
		{
			if (!consts.book) {
				const wchar_t* msg[50]={
					L"？？？\t又来一个吗？快回去，别来这里！这\n里很危险的！",
					L"勇士\t您好，请问您这是。。？",
					L"？？？\t哎，说来话长，我们都是之前尝试闯\n进这座塔的勇士，但都失败了。\n我心有不甘，不愿意就此逝去，执念\n在这座塔的作用下竟然留存了下来，\n就成了现在这个样子。\n你可以叫我徘徊之影吧。",
					L"勇士\t前辈你好...",
					L"徘徊之影\t孩子，快离开这吧。这座塔太危险了\n！塔顶一直在散发着波动，怪物们都\n是打不死的啊！",
					L"勇士\t塔顶？上面有什么？",
					L"徘徊之影\t谁知道呢，也许有个什么水晶吧。\n别关心这个了孩子，你还年轻，快离\n开，不要葬送了自己的性命啊！",
					L"勇士\t不，我们的国家现在已经处于危难之\n中了，我必须要把这个塔的秘密弄清\n楚，解救天下苍生！",
					L"徘徊之影\t哎，自从我呆在这里后，已经有很多\n人来过，都这么说，但是却没有一个\n成功的。也罢，我也知道，帝国的勇\n士是不会退却的。没办法，那我把这\n个给你吧，希望能帮助你一些。"
				};
				consts.setMsg(msg);
				consts.map_npc=map_npc;
			}
			else {
				consts.setMsg(L"徘徊之影\t加油吧小伙子，我看好你！");
			}
			break;
		}
	case 42:
		{
			// 已经拯救
			if (map_floor[now_floor].getinfo(1,0)->getNpc()->getId()==43) {
				consts.setMsg(L"徘徊之影\t谢谢你救了我的儿子！\n\n按照之前所说，我把我在这座塔里的\n研究成果送给你了，请收好。");
				consts.map_npc=map_npc;
			}
			else {
				if (npctime==0) {
					const wchar_t* msg[50]={
						L"徘徊之影\t儿啊，儿啊，我的儿啊！",
						L"勇士\t怎么了，发生什么了？",
						L"徘徊之影\t我几年前为了养活家里人，而进入这\n座塔冒险，希望能得到国王的赏金。\n但我却不幸陷落在了这里。没想到，\n前几天，我的儿子也进来了！他竟然\n来了！",
						L"徘徊之影\t我知道他是来找妈妈的，可以妈妈只\n希望他平平安安啊！可我还没和他说\n上话，他就被抓起来关在这里了！",
						L"徘徊之影\t可惜我已是一股执念而已，没有办法\n救他，只能眼睁睁地看着他就这么被\n关着。",
						L"徘徊之影\t勇士，勇士，快去救救他吧！他还活\n着！他还活着啊！再不救他就要死了\n！我不想这样，我要救他出来，快去\n把他救出来！只能靠你了啊！",
						L"徘徊之影\t以青叶帝国的名义，我发誓，如果你\n安然无恙地把他救了出来，我就送你\n一个好东西！我这几年我的心血！",
						L"勇士\t好的，别急别急，我一定会把他救出\n来的！相信我",
						L"徘徊之影\t嗯.. 我现在只能靠你了..."
					};
					consts.setMsg(msg);
					map_npc->setVisit(1);
				}
				else {
					consts.setMsg(L"徘徊之影\t求求你把我儿子救出来吧！他还活着\n呢！以青叶帝国的名义我发誓，你把\n他安全救出来后我就送你个好东西！");
				}


			}
			break;
		}
	case 43:
		{
			// 已经拯救
			if (map_floor[now_floor].getinfo(1,0)->getNpc()->getId()==43) {
				consts.setMsg(L"杰克\t妈妈... 你怎么成了这个样子...");
			}
			else {
				const wchar_t* msg[50]={
					L"杰克\t终于有人来了，你是不是来杀我的，\n快杀了我啊我已经受够这地方了！",
					L"勇士\t不不你误会了，我由你妈妈所托，想\n办法杀了守卫来救你了",
					L"杰克\t妈妈？她还活着？她在哪？\n\n妈妈，妈妈！",
					L"勇士\t她就在外面呢，快去找她吧。",
					L"杰克\t嗯，我这就去找她。谢谢你！\n\n妈妈，妈妈！"
				};
				consts.setMsg(msg);
				consts.map_npc=map_npc;
			}
			break;
		}
	case 44:
		consts.setMsg(L"徘徊之影\t这塔里有一些比较容易垮塌的墙壁，\n找到它们也许能让你的过关之路更为\n轻松。");
		break;
	case 45:
		consts.setMsg(L"徘徊之影\t像这种机关门，往往都是需要将其守\n卫怪物全部打死后才会开启。");
		break;
	case 46:
		consts.setMsg(L"徘徊之影\t这座塔每五层都有一个小头目一样的\n存在，而且小头目也是能重生的。");
		break;
	default:
		consts.setMsg(L"勇士\t这是啥？");
		break;
	}
}
void c_hero::save(FILE* f)
{
	fprintf_s(f,"%d %d %d %d %d %d %d %d %d %d %d %d %d\n",
		hp,atk,def,money,redkey,bluekey,yellowkey,x,y,face,now_floor,max_floor,fly_floor);
}
void c_hero::load(FILE* f)
{
	fscanf_s(f,"%d %d %d %d %d %d %d %d %d %d %d %d %d\n",
		&hp,&atk,&def,&money,&redkey,&bluekey,&yellowkey,&x,&y,&face,&now_floor,&max_floor,&fly_floor);
}

void c_hero::setFlyFloor(int delta)
{
	if (delta==0 && now_floor<30) 
		fly_floor=now_floor;
	else
	{
		int tmpfloor=fly_floor+delta;
		if (tmpfloor<=max_floor && tmpfloor>=0 && tmpfloor<=consts.map_floornum) fly_floor=tmpfloor;
	}
}