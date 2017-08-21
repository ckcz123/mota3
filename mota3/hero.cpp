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

	// ����
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
					L"�ǻ�֮Ӱ\t˭֪���أ�Ҳ���и�ʲôˮ���ɡ�\n���������˺��ӣ��㻹���ᣬ����\n������Ҫ�������Լ�����������",
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
		{
			// �Ѿ�����
			if (map_floor[now_floor].getinfo(1,0)->getNpc()->getId()==43) {
				consts.setMsg(L"�ǻ�֮Ӱ\tлл������ҵĶ��ӣ�\n\n����֮ǰ��˵���Ұ��������������\n�о��ɹ��͸����ˣ����պá�");
				consts.map_npc=map_npc;
			}
			else {
				if (npctime==0) {
					const wchar_t* msg[50]={
						L"�ǻ�֮Ӱ\t�������������ҵĶ�����",
						L"��ʿ\t��ô�ˣ�����ʲô�ˣ�",
						L"�ǻ�֮Ӱ\t�Ҽ���ǰΪ����������ˣ���������\n����ð�գ�ϣ���ܵõ��������ͽ�\n����ȴ���������������û�뵽��\nǰ���죬�ҵĶ���Ҳ�����ˣ�����Ȼ\n���ˣ�",
						L"�ǻ�֮Ӱ\t��֪��������������ģ���������ֻ\nϣ����ƽƽ�����������һ�û����˵\n�ϻ������ͱ�ץ�������������ˣ�",
						L"�ǻ�֮Ӱ\t��ϧ������һ��ִ����ѣ�û�а취\n������ֻ���������ؿ���������ô��\n���š�",
						L"�ǻ�֮Ӱ\t��ʿ����ʿ����ȥ�Ⱦ����ɣ�������\n�ţ��������Ű����ٲ�������Ҫ����\n���Ҳ�����������Ҫ������������ȥ\n�����ȳ�����ֻ�ܿ����˰���",
						L"�ǻ�֮Ӱ\t����Ҷ�۹������壬�ҷ��ģ������\n��Ȼ���ذ������˳������Ҿ�����\nһ���ö��������⼸���ҵ���Ѫ��",
						L"��ʿ\t�õģ��𼱱𼱣���һ��������ȳ�\n���ģ�������",
						L"�ǻ�֮Ӱ\t��.. ������ֻ�ܿ�����..."
					};
					consts.setMsg(msg);
					map_npc->setVisit(1);
				}
				else {
					consts.setMsg(L"�ǻ�֮Ӱ\t��������Ҷ��Ӿȳ����ɣ���������\n�أ�����Ҷ�۹��������ҷ��ģ����\n����ȫ�ȳ������Ҿ�������ö�����");
				}


			}
			break;
		}
	case 43:
		{
			// �Ѿ�����
			if (map_floor[now_floor].getinfo(1,0)->getNpc()->getId()==43) {
				consts.setMsg(L"�ܿ�\t����... ����ô�����������...");
			}
			else {
				const wchar_t* msg[50]={
					L"�ܿ�\t�����������ˣ����ǲ�����ɱ�ҵģ�\n��ɱ���Ұ����Ѿ��ܹ���ط��ˣ�",
					L"��ʿ\t����������ˣ��������������У���\n�취ɱ��������������",
					L"�ܿ�\t���裿�������ţ������ģ�\n\n���裬���裡",
					L"��ʿ\t�����������أ���ȥ�����ɡ�",
					L"�ܿ�\t�ţ������ȥ������лл�㣡\n\n���裬���裡"
				};
				consts.setMsg(msg);
				consts.map_npc=map_npc;
			}
			break;
		}
	case 44:
		consts.setMsg(L"�ǻ�֮Ӱ\t��������һЩ�Ƚ����׿�����ǽ�ڣ�\n�ҵ�����Ҳ��������Ĺ���֮·��Ϊ\n���ɡ�");
		break;
	case 45:
		consts.setMsg(L"�ǻ�֮Ӱ\t�����ֻ����ţ�����������Ҫ������\n������ȫ��������ŻῪ����");
		break;
	case 46:
		consts.setMsg(L"�ǻ�֮Ӱ\t������ÿ��㶼��һ��СͷĿһ����\n���ڣ�����СͷĿҲ���������ġ�");
		break;
	default:
		consts.setMsg(L"��ʿ\t����ɶ��");
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