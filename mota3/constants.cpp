#include "stdafx.h"

// ��ͼ��Ϣ
extern c_map_floor map_floor[50];

// ��ʿ��Ϣ
extern c_hero hero;

constants::constants()
{
	hge=NULL;
	volume=30;
	bgmvolume=100;
	ScreenLeft=240;
	music=true;
	map_width=13;map_height=13;
}

void constants::init()
{
	canfly=book=moving=opening=flooring=false;
	book=true;
	playtime=0.0;
	step=0;
	hard=0;
	time_move=time_open=time_animation=time_floor=0;
	msg=MESSAGE_START;
	wand=-1;
	for (int i=0;i<100;i++) sd[i].hp=0;
}
void constants::loadResources()
{
	ht_map=hge->Texture_Load("Res/map.png");
	ht_icon=hge->Texture_Load("Res/icon.png");
	ht_hero=hge->Texture_Load("Res/hero.png");
	ht_monster=hge->Texture_Load("Res/monster.png");
	ht_skin=hge->Texture_Load("Res/skin.png");
	ht_npc=hge->Texture_Load("Res/npc.png");
	ht_special=hge->Texture_Load("Res/special.png");
	he_GetItem=hge->Effect_Load("Res/item.ogg");
	he_OpenDoor=hge->Effect_Load("Res/door.ogg");
	he_Attack=hge->Effect_Load("Res/attack.ogg");
	he_Music=hge->Effect_Load("Res/bgm.mp3");
	hgef=new hgeFont("Res/font.fnt");
	//ht_map
	s_ground=new hgeSprite(ht_map,0,0,32,32);
	s_wall=new hgeSprite(ht_map,32,0,32,32);
	s_wall_hidden=new hgeSprite(ht_map,32,0,32,32);
	s_wall_hidden->SetColor(0xCCFFFFFF);
	s_wall2=new hgeSprite(ht_map,64,0,32,32);
	s_water=new hgeSprite(ht_map,32,32,32,32);
	s_sky=new hgeSprite(ht_map,0,32,32,32);
	s_lightning=new hgeSprite(ht_npc,0,128,32,32);
	s_barrier=new hgeSprite(ht_special,128,960,32,32);
	//ht_door
	s_yellowdoor=new hgeSprite(ht_map,0,64,32,32);
	s_bluedoor=new hgeSprite(ht_map,32,64,32,32);
	s_reddoor=new hgeSprite(ht_map,64,64,32,32);
	s_specialdoor=new hgeSprite(ht_map,96,64,32,32);
	s_downstair=new hgeSprite(ht_map,0,192,32,32);
	s_upstair=new hgeSprite(ht_map,32,192,32,32);
	s_shopleft=new hgeSprite(ht_map,0,224,32,32);
	s_shopright=new hgeSprite(ht_map,64,224,32,32);
	//ht_item
	s_yellowkey=new hgeSprite(ht_icon,0,0,32,32);
	s_bluekey=new hgeSprite(ht_icon,32,0,32,32);
	s_redkey=new hgeSprite(ht_icon,64,0,32,32);
	s_allkey=new hgeSprite(ht_icon,96,0,32,32);
	s_coin=new hgeSprite(ht_icon,0,64,32,32);
	s_fly=new hgeSprite(ht_icon,0,192,32,32);
	s_fly2=new hgeSprite(ht_icon,0,192,32,32);
	s_floor=new hgeSprite(ht_icon,32,160,32,32);
	s_level=new hgeSprite(ht_icon,64,128,32,32);
	s_storey=new hgeSprite(ht_icon,64,96,32,32);
	s_cross=new hgeSprite(ht_icon,96,160,32,32);
	s_stick=new hgeSprite(ht_icon,0,160,32,32);
	s_drink=new hgeSprite(ht_icon,32,192,32,32);
	s_heart=new hgeSprite(ht_icon,64,192,32,32);
	s_expr=new hgeSprite(ht_icon,96,192,32,32);
	s_time=new hgeSprite(ht_icon,32,96,32,32);
	s_step=new hgeSprite(ht_icon,32,128,32,32);
	s_damage=new hgeSprite(ht_icon,64,160,32,32);
	s_enemyinfo=new hgeSprite(ht_icon,32,64,32,32);
	s_bluejewel=new hgeSprite(ht_icon,64,32,32,32);
	s_redjewel=new hgeSprite(ht_icon,96,32,32,32);
	s_redpotion=new hgeSprite(ht_icon,64,64,32,32);
	s_bluepotion=new hgeSprite(ht_icon,96,64,32,32);
	s_sword1=new hgeSprite(ht_icon,0,32,32,32);
	s_shield1=new hgeSprite(ht_icon,32,32,32,32);
	s_sword2=new hgeSprite(ht_icon,0,96,32,32);
	s_shield2=new hgeSprite(ht_icon,0,128,32,32);
	s_sword3=new hgeSprite(ht_icon,96,96,32,32);
	s_shield3=new hgeSprite(ht_icon,96,128,32,32);
	//special
	s_sigh=new hgeSprite(ht_special,192,800,32,32);
	s_sighed=new hgeSprite(ht_special,192,832,32,32);
	s_up=new hgeSprite(ht_special,0,928,32,32);
	s_down=new hgeSprite(ht_special,32,928,32,32);
	s_left=new hgeSprite(ht_special,64,928,32,32);
	s_right=new hgeSprite(ht_special,96,928,32,32);
	s_portal=new hgeSprite(ht_special,192,928,32,32);
	s_flower=new hgeSprite(ht_special,224,736,32,32);
	s_box=new hgeSprite(ht_special,192,768,32,32);
	s_boxed=new hgeSprite(ht_special,224,768,32,32);
	s_atk=new hgeSprite(ht_special,224,832,32,32);
	s_def=new hgeSprite(ht_special,224,864,32,32);
	s_life=new hgeSprite(ht_special,224,896,32,32);
	hc_Music=hge->Effect_PlayEx(he_Music,bgmvolume,0,1,true);
}

void constants::destroy()
{
	hge->Texture_Free(ht_map);
	hge->Texture_Free(ht_icon);
	hge->Texture_Free(ht_hero);
	hge->Texture_Free(ht_skin);
	hge->Texture_Free(ht_monster);
	hge->Texture_Free(ht_npc);
	hge->Effect_Free(he_GetItem);
	hge->Effect_Free(he_Music);
	hge->Effect_Free(he_OpenDoor);
	hge->Effect_Free(he_Attack);
	delete hgef;
}

void constants::setMsg(const wchar_t* s[50])
{
	hint.clear();
	for (int i=0;i<50;i++) {
		if (s[i]==NULL) break;
		hint.push_back(wstring(s[i]));
	}

	lasttime=clock();
	nowcnt=0;
	msg=MESSAGE_HINT;
}

void constants::setMsg(const wchar_t* s) 
{
	hint.clear();
	hint.push_back(wstring(s));

	lasttime=clock();
	nowcnt=0;
	msg=MESSAGE_HINT;
}

void constants::goOn(c_hero* hero, c_map_floor* currFloor, float dt)
{
	playtime+=dt;
	if(moving) // �ƶ�
	{
		time_move+=dt;
		if(time_move>=0.03)
		{
			time_move-=0.03;
			if(hero->moveComplete())
				moving=false;
		}		
	}
	if(flooring) // ����¥
	{
		time_floor+=dt;
		if(time_floor>=0.4)
		{
			time_floor=0;
			flooring=false;
		}
	}
	if(opening) // ����
	{
		time_open+=dt;
		if(time_open>=0.05)
		{
			time_open-=0.05;
			if(map_openingdoor->open())
				opening=false;
		}		
	}

	time_animation+=dt;
	if(time_animation>=0.1) // �Ĵκ��ֻص�����״̬
	{
		time_animation-=0.1;
		currFloor->animation();
	}
}
void constants::finishHint()
{
	if (map_npc!=NULL && map_npc->getId()!=0) 
	{
		switch (map_npc->getId())
		{
		case 41:
			{
				book=true;
				hge->Effect_PlayEx(he_GetItem, volume);
				const wchar_t* msg[50]={
					L"��ù����ֲᡣ",
					L"�ǻ�֮Ӱ\t����ǹ����ֲᣬ����Խ�����ƶ�\n���������ϲ鿴����ĸ������ԡ�",
					L"��ʿ\t��лл��������æ�ˣ�",
					L"�ǻ�֮Ӱ\t��������������Ϊ�����һ��������\n����������������һЩ�ص㣬ϣ����\n����һЩ������",
					L"��ʿ\t���õģ���˵~",
					L"�ǻ�֮Ӱ\t�����������ص���ǹ��ﶼ�ǿ���\n�����ġ��������о����֣�������һ\n��ս���д�ܹ��������ⳡս��\n�������ˣ���������ǿյصĻ�����\n�ｫ�������������������Ĺ����\n����Ѫ����ԭ����ֵ�����Ǵ�����\n���Ľ��ȴ�����һ�롣",
					L"��ʿ\tԭ������������ұ��һ�룿��... \n����ô��",
					L"�ǻ�֮Ӱ\t�����ں�ڻ����ü�Ŷ��",
					L"��ʿ\t�����ٴ����ս��ҲҪ���ˣ�",
					L"�ǻ�֮Ӱ\t�ðɣ�������ע����һ�㣬��ο���\n��������������Ǳ���������ѵ㡣",
					L"��ʿ\t�õĺõģ��Ҽ����ˡ�������",
					L"�ǻ�֮Ӱ\t��... ��Ϊ�ұȽ�����û������͹�\n�ˣ���˲���֪������ʲô���ݡ�\n��������������������������ǻ�\n��Ӧ�û��кܶ࣬���������������\n��Ҳ��������ʸ�����Ϣ�ء�",
					L"��ʿ\t�õĺõģ�лл�����һ���͵ģ���\n������������������Ƕ����ѳ�����\n�����Ҵ���Ҷ�۹�����ҫ��",
					L"ϵͳ��ʾ\t������ݼ����£�\nS: �浵\nL: ����\nP: �鿴��ǰMAX\nM: ���ֿ���\nQ: ����������\n�����������ʹ�õĿ�ݼ����õ�ʱ\n�������ʾ��",
					L"ϵͳ��ʾ\t������Sky_��յ���ʹ��C++��д����\n�����뿪Դ�ڣ�\nhttps://github.com/ckcz123/mota3/\n\n�������⣬���ڷ������½��лظ���\n������лл֧�֣�",
					L"�ǻ�֮Ӱ\t���Ͱ�С���ӣ��ҿ����㣡"
				};
				setMsg(msg);
				break;
			}
		case 42:
			{
				// ������
				if (map_floor[hero.getNowFloor()].getinfo(1,0)->getNpc()->getId()==43)
				{
					if (wand>=0)
					{
						map_npc->init(0);
						map_floor[hero.getNowFloor()].getinfo(1,0)->getNpc()->init(0);
						msg=MESSAGE_NONE;
					}
					else
					{
						wand=0;
						hge->Effect_PlayEx(he_GetItem, volume);
						const wchar_t* msg[50]={
							L"�������ħ�ȣ�",
							L"�ǻ�֮Ӱ\t�������о�����������ħ�ȣ�������\n��ĳһ���յ��Ϸ���һ����ǣ�����\n����Ͳ����ڱ�Ǵ���������\n��������ֻ����ʮ�Σ��õĺõĻ�Ҳ\n���ܴ��������̽��Ӵ~\n\n[X]��ʹ�á�\n",
							L"��ʿ\tлл�����������Ҵ�æ����",
							L"�ǻ�֮Ӱ\t����������������һ�������������\n�������ܵģ�",
							L"��ʿ\t�õģ��һ���͵ģ�",
							L"�ǻ�֮Ӱ\t���������뿪�������ɣ��������Ѿ�\n���ˣ�ֻʣ����һ������û����ɢ��\n�㻹���ᣬδ����·���ܳ������뿪\n�ɣ�Ȣ���ù����ƽ�����������\n�������ɣ��ⲻ����Ӧ�����ĵط���",
							L"�ܿ�\t����......",
							L"�ǻ�֮Ӱ\t���ߣ��������ߣ��뿪��������",
							L"�ܿ�\t����......\n\n���ޣ��ã��������...",
							L"�ǻ�֮Ӱ\t���ٻ����ˣ�ȥ�ɣ�ȥ��......\n\n�ҵ�ִ��Ҳ����ɢ�ˣ���ʿ��ף��\n���ˣ�"
						};
						setMsg(msg);
						return;
					}

				}

				break;
			}
		case 43:
			{
				map_floor[hero.getNowFloor()].getinfo(1,0)->getNpc()->init(43);
				map_npc->init(0);
				msg=MESSAGE_NONE;
				break;
			}

		default:
			break;
		}
		
	}
	else
		msg=MESSAGE_NONE;
	map_npc=NULL;
}
void constants::printInfo()
{
	s_step->Render(ScreenLeft+map_width*32+24, 340);
	hgef->printf(ScreenLeft+map_width*32+68, 340, HGETEXT_LEFT, "%d", step);
	int ptm=playtime;
	s_time->Render(ScreenLeft+map_width*32+24, 298);
	if (ptm>=3600)
		hgef->printf(ScreenLeft+map_width*32+68, 298, HGETEXT_LEFT, "%02d : %02d : %02d", ptm/3600, (ptm/60)%60, ptm%60);
	else hgef->printf(ScreenLeft+map_width*32+68, 298, HGETEXT_LEFT, "%02d : %02d", ptm/60, ptm%60);

	GfxFont *f=new GfxFont(L"����", 24, true);
	if (hard==1) {
		f->SetColor(0xFF00FF00);
		f->Print(64, 356, L"���Ѷ�");
	}
	else if (hard==2) {
		f->SetColor(0xFF96CDCD);
		f->Print(64, 356, L"��ͨ�Ѷ�");
	}
	else if (hard==3) {
		f->SetColor(0xFFFF0000);
		f->Print(64, 356, L"�����Ѷ�");
	}
	delete f;

}

void constants::save(FILE* f) 
{
	fprintf_s(f, "%d %d %d %d %d %d %.2f\n", map_floornum, canfly?1:0, book?1:0, wand, step, hard, playtime);
}

void constants::load(FILE* f)
{
	int _fly, _book;
	fscanf_s(f, "%d %d %d %d %d %d %f", &map_floornum, &_fly, &_book, &wand, &step, &hard, &playtime);
	canfly=_fly==1;
	book=_book==1;
	moving=opening=flooring=false;
}