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
	yellowkey=0;
	bluekey=0;
	redkey=0;
	x=6;
	y=12;
	face=3;
	move=0;
	now_floor=0;
	fly_floor=0;
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			sprites[i][j]=new hgeSprite(consts.ht_hero, 32*i, 33*j, 32, 33);
}
int c_hero::nextX()
{
	return x+dir[0][face];
}
int c_hero::nextY()
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
bool c_hero::nearStair(int t)
{
	if (t==0) return nearDownStair() || nearUpStair();
	if (map_floor[now_floor].getinfo(y,x)->nearStair(t)) return true;
	if (x>0 && map_floor[now_floor].getinfo(y,x-1)->nearStair(t)) return true;
	if (y>0 && map_floor[now_floor].getinfo(y-1,x)->nearStair(t)) return true;
	if (x<consts.map_width-1 && map_floor[now_floor].getinfo(y,x+1)->nearStair(t)) return true;
	if (y<consts.map_height-1 && map_floor[now_floor].getinfo(y+1,x)->nearStair(t)) return true;
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
		case 11:atk+=1+level;
			break;
		case 12:def+=1+level;
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
		case 23:
			if (consts.fly<0) consts.fly=0;
			consts.fly++;
			consts.setMsg(L"获得中心对称飞行器。");
			break;
		case 25:
			atk+=20;
			consts.setMsg(L"获得银剑，攻击力+20。");
			break;
		case 26:
			def+=20;
			consts.setMsg(L"获得银盾，防御力+20。");
			break;
		case 27:
			consts.canfly=true;
			consts.setMsg(L"获得楼层飞行器。\n你可以飞往任意比当前层数低的楼层\n，或比当前层数高且存在通路可达的\n楼层。\n[G]键使用。");
			break;
		case 31:
			atk+=40;
			consts.setMsg(L"获得神圣剑，攻击+40。");
			break;
		case 32:
			def+=40;
			consts.setMsg(L"获得神圣盾，防御+40。");
			break;
		}
		int special=map_floor[now_floor].getSpecial(x,y);
		if (special!=0) {
			if (special==201) {
				map_floor[now_floor].setSpecial(x,y,202);
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
		consts.step++;
		if (now_floor==20 && x==6 && y==8 && map_floor[now_floor].hasMonster() && map_floor[now_floor].getinfo(9,6)->isGround()) {
			if (consts.music)
				consts.hge->Effect_PlayEx(consts.he_OpenDoor,consts.volume);
			consts.upload();
			map_floor[now_floor].getinfo(9,6)->init(84);
		}
		if (!consts.ending && now_floor==0 && x==6 && y==12 && consts.lefttime<80)
			consts.goodEnd();
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
	if (now_floor>=consts.map_floornum)
		consts.map_floornum=now_floor+1;
}
void c_hero::downstair()
{
	now_floor--;
	setFlyFloor();
	map_floor[now_floor].getUpPosition(x,y);
	consts.flooring=true;
	consts.step++;
}
void c_hero::specialMove(int f)
{
	if (f==0) {
		now_floor=17;
		x=10;
		y=2;
		face=3;
	}
	else {
		now_floor=20+f;
		x=1;
		y=11;
		face=3;
	}
}
void c_hero::turn()
{
	if (face==0) face=2;
	else if (face==2) face=3;
	else if (face==3) face=1;
	else face=0;
}
void c_hero::useWand()
{
	if (consts.wand<=0) return;
	int nx=nextX(), ny=nextY();
	if (nx>=0 && nx<consts.map_width && ny>=0 && ny<consts.map_height) {
		c_map_point* point=map_floor[now_floor].getinfo(ny,nx);
		if (point->isGround()) {
			point->setSpecial(210);
			consts.wand--;
		}
		else if (point->getSpecial()==209) {
			point->setSpecial(211);
			consts.wand--;
		}
	}
}
void c_hero::fly()
{
	if(now_floor<=fly_floor && fly_floor!=0)map_floor[fly_floor].getDownPosition(x,y);
	else map_floor[fly_floor].getUpPosition(x,y);
	now_floor=fly_floor;
	consts.flooring=true;
}
bool c_hero::canCenterFly()
{
	int nx=consts.map_width-1-x, ny=consts.map_height-1-y;
	if (!map_floor[now_floor].getinfo(ny,nx)->isGround())
		return false;
	return true;
}
void c_hero::centerFly()
{
	int nx=consts.map_width-1-x, ny=consts.map_height-1-y;
	x=nx; y=ny;
}
void c_hero::printInfo()
{
	int py=16;
	consts.s_storey->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",getDisplayFloor());
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
	int val=damage+(monster->getHp()-1)/(atk-mon_def)*(mon_atk-hero_def);

	if (consts.hard==1) return val*.85;
	if (consts.hard==2) return val*.95;
	return val;

}
void c_hero::beat(c_monster* monster)
{
	int d=getDamage(monster), id=monster->getId();
	hp-=d;
	if (consts.music)
		consts.hge->Effect_PlayEx(consts.he_Attack, consts.volume);

	money+=monster->getMoney();

	// 重生
	if (d!=0 && consts.lefttime>=80)
	{
		int nx=x-dir[0][face], ny=y-dir[1][face];
		if (nx>=0 && nx<consts.map_width && ny>=0 && ny<consts.map_height) {
			c_map_point* point=map_floor[now_floor].getinfo(ny,nx);
			if (point->isGround()) {
				point->init(100+monster->getId(), monster->getTimes()+1);
			}
		}
	}

	// 分裂
	if (monster->getSpecial()==4) {
		int nx=nextX(), ny=nextY();
		for (int i=0;i<4;i++) {
			int nnx=nx+2*dir[0][i], nny=ny+2*dir[1][i];
			if (nnx>=0 && nnx<consts.map_width && nny>=0 && nny<consts.map_height) {
				c_map_point* point=map_floor[now_floor].getinfo(nny,nnx);
				if (point->isGround()) {
					point->init(100+monster->getId());
				}
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
	if (now_floor==5 && id==15 && !map_floor[now_floor].hasMonster(15)) {
		// 5楼开门
		map_floor[now_floor].getinfo(3,6)->openSpecial();
		consts.upload();
	}
	if (now_floor==6 && x>=4 && x<=6 && y>=6 && y<=8) {
		// 6楼中间开门
		bool canopen=true;
		for (int i=4;i<=6;i++) {
			for (int j=6;j<=8;j++) {
				if (map_floor[now_floor].getinfo(j,i)->hasMonster()) {
					canopen=false;
				}
			}
		}
		if (canopen) {
			map_floor[now_floor].getinfo(7,7)->openSpecial();
		}
	}
	if (now_floor==6 && id==17 && !map_floor[now_floor].hasMonster(17)) {
		// 6楼商店开门
		map_floor[now_floor].getinfo(2,9)->openSpecial();
	}
	if (now_floor==8 && id==18 && !map_floor[now_floor].hasMonster(18)) {
		// 8楼花门
		map_floor[now_floor].getinfo(7,1)->openSpecial();
	}
	if (now_floor==10 && !map_floor[now_floor].hasMonster()) {
		// 10楼机关门
		map_floor[now_floor].getinfo(3,6)->openSpecial();
		consts.upload();
	}
	if (now_floor==12 && id==23 && !map_floor[now_floor].hasMonster(23)) {
		// 12楼机关门
		map_floor[now_floor].getinfo(2, 10)->openSpecial();
	}
	if (now_floor==15 && id==26 && !map_floor[now_floor].hasMonster(26)) {
		map_floor[now_floor].getinfo(3, 6)->openSpecial();
		consts.upload();
	}
	if (now_floor==17 && id==30 && !map_floor[now_floor].hasMonster(30)) {
		map_floor[now_floor].getinfo(3, 10)->openSpecial();
	}
	if (now_floor==17 && id==27 && !map_floor[now_floor].getinfo(10,9)->hasMonster() && !map_floor[now_floor].getinfo(10,11)->hasMonster()) {
		map_floor[now_floor].getinfo(9,10)->openSpecial();
	}
	if (now_floor==19 && id==33) {
		if (!map_floor[now_floor].getinfo(1,5)->hasMonster() && !map_floor[now_floor].getinfo(3,5)->hasMonster()) {
			map_floor[now_floor].getinfo(2,4)->openSpecial();
		}
		if (!map_floor[now_floor].getinfo(1,7)->hasMonster() && !map_floor[now_floor].getinfo(3,7)->hasMonster()) {
			map_floor[now_floor].getinfo(2,8)->openSpecial();
		}
	}
	if (now_floor==20 && !map_floor[now_floor].hasMonster()) {
		map_floor[now_floor].getinfo(9,6)->init(0);
		map_floor[now_floor].getinfo(3,6)->openSpecial();
		consts.upload();
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
					L"徘徊之影\t谁知道呢......\n别关心这个了孩子，你还年轻，快离\n开，不要葬送了自己的性命啊！",
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
			const wchar_t* msg[50]={
				L"徘徊之影\t这塔里有一些比较容易垮塌的墙壁，\n找到它们也许能让你的过关之路更为\n轻松。\n（PS: 暗墙的存在是为了让本塔的地\n图更好看，它们都很容易找到的。）",
				L"徘徊之影\t另外，如何正确处理上面的坚固怪是\n能否顺利通过一区的关键所在。"
			};
			consts.setMsg(msg);
			break;
		}
	case 43:
		consts.setMsg(L"徘徊之影\t像这种机关门，往往都是需要将其守\n卫怪物全部打死后才会开启。");
		break;
	case 44:
		{
			const wchar_t* msg[50]={
				L"徘徊之影\t本塔共有两个商店，6楼和12楼。两\n个商店类似于新新魔塔的那种关系。",
				L"徘徊之影\t另外给个友情提示：\n爆防一时爽，三区火葬场！\n请适当平衡加点，以免三区骂作者。\n（大佬们请无视这段话...）",
				L"徘徊之影\t攻低了的话三区一开始会很难受的！\n攻低了的话三区一开始会很难受的！\n攻低了的话三区一开始会很难受的！\n（重要的事情说三遍）"
			};
			consts.setMsg(msg);
			break;
		}
	case 45:
		{
			int need=25+2*npctime;
			if (money<need) break;
			money-=need;
			if (select==1) atk+=2;
			if (select==2) def+=3;
			map_npc->visitNpc();
			break;
		}
	case 46:
		{
			if (npctime==0) {
				consts.setMsg(L"徘徊之影\t你竟然找到这来了！那我就送你一个\n好东西吧~");
				map_npc->visitNpc();
				consts.map_npc=map_npc;
				break;
			}
			else {
				int need=25*(1+npctime);
				if (money<need) {
					consts.setMsg(L"金币不足。");
					break;
				}
				money-=need;
				consts.wand++;
				map_npc->visitNpc();
				break;
			}
		}
	case 47:
		{
			const wchar_t* msg[50]={
				L"作者提示\t作者测试时困难难度下的属性：\n生命2103，攻击179，防御159，金币\n97，黄钥匙4，蓝钥匙1，魔杖4次。\n（以上数据仅供参考。）",
				L"作者提示\t另外，三四区怪物伤害较高，请多存\n档多探路，最好先把所有NPC碰一遍。"
			};
			consts.setMsg(msg);
			break;
		}
	case 48:
		consts.setMsg(L"徘徊之影\t据说右上角那个仙子可以引导你进入\n一个异次元空间。");
		break;
	case 49:
		{
			const wchar_t* msg[50]={
				L"徘徊之影\t你可能要把本层所有高级卫兵杀死才\n能打开商店门前的机关门。",
				L"徘徊之影\t给两个提示：\n1. 建议把攻击加到203再打，不然十\n只卫兵加起来伤害很高的。\n2. 设计上，可以只用一次魔杖就能\n杀掉所有高级卫兵；当然你也可以选\n择多使用几次以节省黄钥匙。"
			};
			consts.setMsg(msg);
			break;
		}
	case 50:
		{
			int need=50+4*npctime;
			if (money<need)
				break;
			money-=need;
			if (select==1) atk+=4;
			if (select==2) def+=6;
			map_npc->visitNpc();
			break;
		}
	case 51:
		{
			int need=50*(npctime+1);
			if (money<need) {
				consts.setMsg(L"金币不足。");
				break;
			}
			money-=need;
			if (consts.fly<0) consts.fly=0;
			consts.fly++;
			map_npc->visitNpc();
			break;
		}
	case 52:
		{
			if (money<70) {
				consts.setMsg(L"金币不足。");
				break;
			}
			money-=70;
			hp+=500;
			map_npc->init(0);
			break;
		}
	case 53:
		{
			if (money<180) {
				consts.setMsg(L"金币不足。");
				break;
			}
			money-=180;
			hp+=2000;
			map_npc->init(0);
			break;
		}
	case 54:
		{
			if (npctime==0) {
				const wchar_t* msg[50]={
					L"仙子\t少年，你想进行试炼吗？",
					L"勇士\t试炼？什么是试炼......",
					L"仙子\t我给你简单介绍一下规则。请听好：",
					L"仙子\t1. 试炼之地位于异次元空间，共有3\n层。每过一层都可以回来获取我给你\n的奖励。",
					L"仙子\t2. 试炼之地的怪物都比较特殊：\n(1) 所有怪物的伤害值都是恒定的。\n(2) 所有怪物都拥有分裂属性，打掉\n后会向四个方向进行分裂。\n(3) 怪物打掉后无法获取金币。",
					L"仙子\t3. 试炼之地拥有钥匙、血瓶和对称飞\n等道具。所有道具都是能带出来的。",
					L"仙子\t警告：试炼之地比较考验路线，需一\n定的智商仔细琢磨才能找到最优解；\n当然直接用对称飞过关也是可以的。",
					L"仙子\t如果你想挑战一下的话就来找我！"
				};
				consts.setMsg(msg);
				map_npc->visitNpc();
				// consts.map_npc=map_npc;
			}
			else if (npctime<=3) {
				specialMove(npctime);
				map_npc->visitNpc();
				consts.msg=consts.MESSAGE_NONE;		
			}
			else {
				consts.setMsg(L"仙子\t你已通关所有试炼层！");
			}
			break;
		}
	case 55:
		{
			hp+=1500;
			if (consts.music)
				consts.hge->Effect_PlayEx(consts.he_GetItem, consts.volume);
			consts.setMsg(L"仙子\t恭喜通关试炼1层！\n这是你的奖励：生命+1500。");
			consts.map_npc=map_npc;
			break;
		}
	case 56:
		{
			consts.wand++; consts.fly+=2;
			if (consts.music)
				consts.hge->Effect_PlayEx(consts.he_GetItem, consts.volume);
			consts.setMsg(L"仙子\t恭喜通关试炼2层！\n这是你的奖励：魔杖+1，对称飞+2。");
			consts.map_npc=map_npc;
			break;
		}
	case 57:
		{
			atk+=10; def+=10;
			if (consts.music)
				consts.hge->Effect_PlayEx(consts.he_GetItem, consts.volume);
			consts.setMsg(L"仙子\t恭喜通关试炼3层！\n这是你的奖励：攻防+10。");
			consts.map_npc=map_npc;
			break;
		}
	case 70:
		{
			bool trueend=true;
			for (int i=0;i<=20;i++)
				if (map_floor[i].hasMonster()) {
					trueend=false;
					break;
				}

			if (trueend) {
				const wchar_t* msg[50]={
					L"勇士\t波动好像就是从这里散发开来的，看\n来就是这个水晶球了。",
					L"勇士\t（敲敲打打）\n好像从没见过这种材质呢，这就是异\n次元的物品吗？试试能不能带回去研\n究研究。",
					L"勇士\t（上前轻轻一摘）",
				};
				consts.map_npc=map_npc;
				consts.setMsg(msg);
			}
			else {
				const wchar_t* msg[50]={
					L"勇士\t波动好像就是从这里散发开来的，看\n来就是这个水晶球了。",
					L"勇士\t（敲敲打打）\n好像从没见过这种材质呢，这就是异\n次元的物品吗？试试能不能带回去研\n究研究。",
					L"勇士\t（勇士使出了洪荒之力）\n啊呀呀呀呀呀呀......",
					L"勇士\t不行，看来这个水晶球被封死在柱子\n上面了。没办法了，看来只能毁了它\n了。",
					L"勇士\t（拿起剑，狠狠劈下）\n为了我大青叶帝国的荣耀，什么牛鬼\n马蛇都统统去死吧！",
				};
				consts.map_npc=map_npc;
				consts.setMsg(msg);
			}
			break;
		}
	default:
		consts.setMsg(L"勇士\t这是啥？");
		break;
	}
	consts.lasttime=clock();
}
void c_hero::save(FILE* f)
{
	char* ss=toString();
	fprintf_s(f,"%s",ss);
	delete ss;
}
char* c_hero::toString()
{
	char* ss=new char[2000];
	sprintf_s(ss, 2000, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d ",
		hp/77,hp%77,atk/46,atk%46,def/51,def%51,money/22,money%22,yellowkey,bluekey,redkey,x,y,face,now_floor);
	return ss;
}
void c_hero::load(FILE* f)
{
	int h1,h2,a1,a2,d1,d2,m1,m2;
	fscanf_s(f,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
		&h1,&h2,&a1,&a2,&d1,&d2,&m1,&m2,&yellowkey,&bluekey,&redkey,&x,&y,&face,&now_floor);
	hp=77*h1+h2; atk=46*a1+a2; def=51*d1+d2; money=22*m1+m2;
}

void c_hero::setFlyFloor(int delta)
{
	if (delta==0) 
		fly_floor=now_floor;
	else
	{
		int tmpfloor=fly_floor+delta;
		if (tmpfloor>=0) fly_floor=tmpfloor;
	}
}