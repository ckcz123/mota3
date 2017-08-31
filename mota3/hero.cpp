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
	max_floor=0;
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
			consts.setMsg(L"��ù����ֲᡣ\n����Խ��������ڹ����ϲ�\n������Ļ�����Ϣ�����ϵ���\n����Ϊ��������ơ�Ѫ������\n����������������õĽ��\n�����飬������ɵ��˺���");
			break;
		case 14:
			yellowkey++;bluekey++;redkey++;
			consts.setMsg(L"��ȡԿ�׺У�ȫԿ������+1");
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
			consts.setMsg(L"���������������+10��");
			break;
		case 21:
			def+=10;
			consts.setMsg(L"������ܣ�������+10��");
			break;
		case 25:
			atk+=20;
			consts.setMsg(L"���������������+20��");
			break;
		case 26:
			def+=20;
			consts.setMsg(L"������ܣ�������+20��");
			break;
		case 27:
			consts.canfly=true;
			consts.setMsg(L"���¥���������\n����Է�������ȵ�ǰ�����͵�¥��\n����ȵ�ǰ�������Ҵ���ͨ·�ɴ��\n¥�㡣\n[G]��ʹ�á�");
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
		if (!consts.ended && now_floor==0 && x==6 && y==12 && consts.lefttime<80)
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
void c_hero::specialMove()
{
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
bool c_hero::canBeat(c_monster* monster) // �ж��ܷ���
{
	return hp>getDamage(monster);
}
int c_hero::getDamage(c_monster* monster) // ��ܹ������hp
{
	int mon_def=monster->getDef(), mon_atk=monster->getAtk();
	if (atk<=mon_def) return MAX_DAMAGE;
	// ħ��
	int hero_def=monster->getSpecial()==2?0:def;
	if (hero_def>=mon_atk) return 0;
	// �ȹ�
	int damage=monster->getSpecial()==1?mon_atk-hero_def:0;
	int val=damage+(monster->getHp()-1)/(atk-mon_def)*(mon_atk-hero_def);

	if (consts.hard==1) return val*.8;
	if (consts.hard==2) return val*.92;
	return val;

}
void c_hero::beat(c_monster* monster)
{
	int d=getDamage(monster), id=monster->getId();
	hp-=d;
	if (consts.music)
		consts.hge->Effect_PlayEx(consts.he_Attack, consts.volume);

	money+=monster->getMoney();

	// ����
	if (d!=0 && consts.lefttime>=80)
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

	// ���꣬����һЩ�¼�

	if (now_floor==2 && id==22) {
		// 2¥�������״����Ҳ�������
		if (!map_floor[now_floor].hasMonster(22)) {
			map_floor[now_floor].getinfo(3,6)->openSpecial();
		}
	}
	if (now_floor==4 && x<=2 && y>=3 && y<=5)
	{
		// 4¥����
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
		// 5¥����
		map_floor[now_floor].getinfo(3,6)->openSpecial();
		consts.upload();
	}
	if (now_floor==6 && x>=4 && x<=6 && y>=6 && y<=8) {
		// 6¥�м俪��
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
		// 6¥�̵꿪��
		map_floor[now_floor].getinfo(2,9)->openSpecial();
	}
	if (now_floor==8 && id==18 && !map_floor[now_floor].hasMonster(18)) {
		// 8¥����
		map_floor[now_floor].getinfo(7,1)->openSpecial();
	}
	if (now_floor==10 && !map_floor[now_floor].hasMonster()) {
		// 10¥������
		map_floor[now_floor].getinfo(3,6)->openSpecial();
		consts.upload();
	}
	if (now_floor==12 && id==23 && !map_floor[now_floor].hasMonster(23)) {
		// 12¥������
		map_floor[now_floor].getinfo(2, 10)->openSpecial();
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
					L"������\t����һ���𣿿��ȥ�����������\n���Σ�յģ�",
					L"��ʿ\t���ã����������ǡ�����",
					L"������\t����˵�����������Ƕ���֮ǰ���Դ�\n������������ʿ������ʧ���ˡ�\n�����в��ʣ���Ը��ʹ���ȥ��ִ��\n���������������¾�Ȼ������������\n�ͳ�������������ӡ�\n����Խ����ǻ�֮Ӱ�ɡ�",
					L"��ʿ\tǰ�����...",
					L"�ǻ�֮Ӱ\t���ӣ����뿪��ɡ�������̫Σ����\n������һֱ��ɢ���Ų����������Ƕ�\n�Ǵ����İ���",
					L"��ʿ\t������������ʲô��",
					L"�ǻ�֮Ӱ\t˭֪����......\n���������˺��ӣ��㻹���ᣬ����\n������Ҫ�������Լ�����������",
					L"��ʿ\t�������ǵĹ��������Ѿ�����Σ��֮\n���ˣ��ұ���Ҫ�������������Ū��\n����������²�����",
					L"�ǻ�֮Ӱ\t�����Դ��Ҵ���������Ѿ��кܶ�\n������������ô˵������ȴû��һ��\n�ɹ��ġ�Ҳ�գ���Ҳ֪�����۹�����\nʿ�ǲ�����ȴ�ġ�û�취�����Ұ���\n������ɣ�ϣ���ܰ�����һЩ��"
				};
				consts.setMsg(msg);
				consts.map_npc=map_npc;
			}
			else {
				consts.setMsg(L"�ǻ�֮Ӱ\t���Ͱ�С���ӣ��ҿ����㣡");
			}
			break;
		}
	case 42:
		consts.setMsg(L"�ǻ�֮Ӱ\t��������һЩ�Ƚ����׿�����ǽ�ڣ�\n�ҵ�����Ҳ��������Ĺ���֮·��Ϊ\n���ɡ�");
		break;
	case 43:
		consts.setMsg(L"�ǻ�֮Ӱ\t�����ֻ����ţ�����������Ҫ������\n������ȫ��������ŻῪ����");
		break;
	case 44:
		consts.setMsg(L"�ǻ�֮Ӱ\t�������������̵꣬6¥��12¥����\n���̵�������������ħ�������ֹ�ϵ��");
		break;
	case 45:
		{
			int need=25+2*npctime;
			if (money<need) break;
			money-=need;
			if (select==1) hp+=400;
			if (select==2) atk+=2;
			if (select==3) def+=3;
			map_npc->visitNpc();
			break;
		}
	case 46:
		{
			if (npctime==0) {
				consts.setMsg(L"�ǻ�֮Ӱ\t�㾹Ȼ�ҵ������ˣ����Ҿ�����һ��\n�ö�����~");
				map_npc->visitNpc();
				consts.map_npc=map_npc;
				break;
			}
			else {
				if (money<80) {
					consts.setMsg(L"��Ҳ��㡣");
					break;
				}
				money-=80;
				consts.wand++;
				break;
			}
		}
	case 47:
		consts.setMsg(L"�ǻ�֮Ӱ\tע�����Կ����������̽·��");
		break;
	case 48:
		{
			const wchar_t* msg[50]={
				L"�ǻ�֮Ӱ\t���Ȼͨ����ǰ���������������\n���Ѿ��Ա��������ֺ�ˢǮ��������\n�ɶ��Ƚ���Ϥ�˰ɡ�\n\n��ô����ӭ����������",
				L"��ʿ\t�����... ������... �治��ħ����\n�����Ե��𣿣�"
			};
			consts.setMsg(msg);
			break;
		}
	case 49:
		consts.setMsg(L"�ǻ�֮Ӱ\t�����Ҫ�ѱ������и߼�����ɱ����\n�ܴ��̵���ǰ�Ļ����š�");
		break;
	case 50:
		{
			int need=50+4*npctime;
			if (money<need) {
				consts.setMsg(L"��Ҳ��㡣");
				break;
			}
			money-=need;
			if (select==1) hp+=800;
			if (select==2) atk+=4;
			if (select==3) def+=6;
			map_npc->visitNpc();
			break;
		}
	case 51:
		{
			int need=150*(npctime+1);
			if (money<need) {
				consts.setMsg(L"��Ҳ��㡣");
				break;
			}
			money-=need;
			if (consts.fly<0) consts.fly=0;
			consts.fly++;
			map_npc->visitNpc();
			break;
		}
	case 70:
		{
			bool trueend=true;
			for (int i=0;i<consts.map_floornum;i++)
				if (map_floor[i].hasMonster()) {
					trueend=false;
					break;
				}

			if (trueend) {
				const wchar_t* msg[50]={
					L"��ʿ\t����������Ǵ�����ɢ�������ģ���\n���������ˮ�����ˡ�",
					L"��ʿ\t�����ô��\n�����û�������ֲ����أ��������\n��Ԫ����Ʒ�������ܲ��ܴ���ȥ��\n���о���",
					L"��ʿ\t����ǰ����һժ��",
				};
				consts.map_npc=map_npc;
				consts.setMsg(msg);
			}
			else {
				const wchar_t* msg[50]={
					L"��ʿ\t����������Ǵ�����ɢ�������ģ���\n���������ˮ�����ˡ�",
					L"��ʿ\t�����ô��\n�����û�������ֲ����أ��������\n��Ԫ����Ʒ�������ܲ��ܴ���ȥ��\n���о���",
					L"��ʿ\t����ʿʹ���˺��֮����\n��ѽѽѽѽѽѽ......",
					L"��ʿ\t���У��������ˮ���򱻷���������\n�����ˡ�û�취�ˣ�����ֻ�ܻ�����\n�ˡ�",
					L"��ʿ\t�����𽣣��ݺ����£�\nΪ���Ҵ���Ҷ�۹�����ҫ��ʲôţ��\n���߶�ͳͳȥ���ɣ�",
				};
				consts.map_npc=map_npc;
				consts.setMsg(msg);
			}
			break;
		}
	default:
		consts.setMsg(L"��ʿ\t����ɶ��");
		break;
	}
	consts.lasttime=clock();
}
void c_hero::save(FILE* f)
{
	fprintf_s(f,"%d %d %d %d %d %d %d %d %d %d %d %d %d\n",
		hp,atk,def,money,redkey,bluekey,yellowkey,x,y,face,now_floor,max_floor,fly_floor);
}
void c_hero::load(FILE* f)
{
	fscanf_s(f,"%d %d %d %d %d %d %d %d %d %d %d %d %d",
		&hp,&atk,&def,&money,&redkey,&bluekey,&yellowkey,&x,&y,&face,&now_floor,&max_floor,&fly_floor);
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