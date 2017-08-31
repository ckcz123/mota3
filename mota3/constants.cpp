#include "stdafx.h"

// ��ͼ��Ϣ
extern c_map_floor map_floor[50];

// ��ʿ��Ϣ
extern c_hero hero;

extern Http http;

constants::constants()
{
	hge=NULL;
	volume=30;
	bgmvolume=100;
	ScreenLeft=240;
	music=true;
	showdamage=true;
	map_width=13;map_height=13;
}

void constants::init()
{
	canfly=book=moving=opening=flooring=ended=false;
	lefttime=100.0;
	playtime=0.0;
	step=0;
	hard=0;
	time_move=time_open=time_animation=time_floor=0;
	msg=MESSAGE_START;
	wand=-1;
	fly=-1;
	starttime=0;
	for (int i=0;i<1000;i++) sd[i].hp=0;
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
	ht_special2=hge->Texture_Load("Res/special2.png");
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
	s_wall3=new hgeSprite(ht_map,96,0,32,32);
	s_water=new hgeSprite(ht_map,32,32,32,32);
	s_sky=new hgeSprite(ht_map,0,32,32,32);
	s_lightning=new hgeSprite(ht_npc,0,128,32,32);
	s_barrier=new hgeSprite(ht_special,128,960,32,32);
	s_bg=new hgeSprite(ht_skin,0,0,128,128);
	s_bg->SetColor(0xCCFFFFFF);
	s_bg_font=new hgeSprite(ht_skin,0,0,128,128);
	s_bg_font->SetColor(0x72FFFFFF);
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
	s_wand=new hgeSprite(ht_special,224,800,32,32);
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
	hge->Texture_Free(ht_special);
	hge->Texture_Free(ht_special2);
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

	if (!ended && lefttime<80 && lefttime>0)
		lefttime-=dt;
	if (lefttime<0) {
		lefttime=0;
		normalEnd();
	}
}
void constants::normalEnd()
{
	ended=true;
	const wchar_t* msg[50]={
		L"�����ˣ���ʿ���ջ���û���ӳ�����",
		L"ս���ϣ������ǵĲ���֮��������ʧ\n�ˣ��۹�����ʿ��һ��������������\n���ˣ�����취���������Ԫ֮�š�\n��Ҷ�۹��ָֻ���������ƽ����",
		L"����Ϊ��ʿ���һ��������Լ�\n����ʿ��׿Խ���ס���Ԥ�����ǣ���\nʿ�����ֽ�����ʷ�ĳ��������䣬��\n���������ϴ��̡�",
		L"Ȼ���������ˣ�ʲô���������ˡ�",
		L"���ҳɹ����𣿵۹���ʤ���𣿺���\n���뿴����...��",
		L"NORMAL END."
	};
	setMsg(msg);
}
void constants::goodEnd()
{
	ended=true;
	bool trueend=true;
	for (int i=0;i<map_floornum;i++)
		if (map_floor[i].hasMonster()) {
			trueend=false;
			break;
		}

	if (trueend) {
		const wchar_t* msg[50]={
			L"��ʿЯ����ˮ����Ƭ�ӳ�����������\n�ص��˵۹���",
			L"ս���ϣ������ǵĲ���֮��������ʧ\n�ˣ��۹�����ʿ��ѹ�����������һ\n��������ǰ�ƽ������ڿ�����ʤ����\n��⡣",
			L"��Ϊ�۹�����������ʿ֮һ����ʿһ\n���ȳ嵽�����Ԫ����ǰ��",
			L"ͻȻ�������ǿ�ʼ����һ�еؽ��з�\n�ˣ���ʿ��ľ��֧���ܿ��������ˡ�",
			L"��Ԯ������������ʿ��������ȥ��\n�ƣ�Ȼ���������ƹ����������졣",
			L"�������������ѣ��عⷵ�յ���ʿ��\n���Ŵ������ѱ���յ����Ԫ֮�ţ�\n���������ᡣ",
			L"������ʶ����֮�ʣ�ͻȻ����һ����\nůϮ����ˮ����Ƭ������ҫ�۵Ĺ�â\n������һ�����Լ��˿�ȫ����ʧ�ˣ�\n����Ҳ�ָ������۷�ʱ�̣�"
			L"��ʿ��ȻҲӵ����������������",
			L"ս�������󣬹�������ʿ�ַ���һ��\n����أ�������һ������Լ�����\nʿ���ⳡս���еĵ�׿Խ���ס�",
			L"......�����......",
			L"ӵ��һ�����أ�ϥ�¶�Ů��Ⱥ����\n����һ�����ٻ��������Ѱ׷��Բ�\n�������ӵ���ʿ�����Լ�����ˮ��ժ\n�£�����ر������۾���",
			L"������һ�����Ѿ������ź��ˡ���",
			L"TRUE END."
		};
		setMsg(msg);
	}
	else {
		const wchar_t* msg[50]={
			L"��ʿЯ����ˮ����Ƭ�ӳ�����������\n�ص��˵۹���",
			L"ս���ϣ������ǵĲ���֮��������ʧ\n�ˣ��۹�����ʿ��ѹ�����������һ\n��������ǰ�ƽ������ڿ�����ʤ����\n��⡣",
			L"��Ϊ�۹�����������ʿ֮һ����ʿһ\n���ȳ嵽�����Ԫ����ǰ��",
			L"ͻȻ�������ǿ�ʼ����һ�еؽ��з�\n�ˣ���ʿ��ľ��֧���ܿ��������ˡ�",
			L"��Ԯ������������ʿ��������ȥ��\n�ƣ�Ȼ���������ƹ����������졣",
			L"�������������ѣ��عⷵ�յ���ʿ��\n���Ŵ������ѱ���յ����Ԫ֮�ţ�\n���������ᡣ",
			L"ս�������󣬹���Ϊ��ʿ���һ��\n������Լ�����ʿ��׿Խ���ס���\nԤ�����ǣ���ʿ�����ֽ�����ʷ�ĳ�\n�������䣬�����������ϴ��̡�",
			L"���۹�������Ӯ�ˣ�����Ӯ��... ֻ\n�ǣ���������ôһ���ź��İ�...��",
			L"GOOD END."
		};
		setMsg(msg);
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
				if (music)
					hge->Effect_PlayEx(he_GetItem, volume);
				upload();
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
					L"ϵͳ��ʾ\t������ݼ����£�\nS/L: ��/����\nZ: ת��\n-: ����/�ر�����\nP: �鿴��ǰMAX\nM: ����/�ر�����\nQ: ����������\n�����������ʹ�õĿ�ݼ����õ�ʱ\n�������ʾ��",
					L"ϵͳ��ʾ\t������Sky_��յ���ʹ��C++��д����\n�����뿪Դ�ڣ�\nhttps://github.com/ckcz123/mota3/\n\n�������⣬���ڷ������½��лظ���\n������лл֧�֣�",
					L"�ǻ�֮Ӱ\t���Ͱ�С���ӣ��ҿ����㣡"
				};
				setMsg(msg);
				break;
			}
		case 46:
			{
				wand=5;
				if (music)
					hge->Effect_PlayEx(he_GetItem, volume);
				const wchar_t* msg[50]={
					L"�������ħ�ȣ�\n�������ǰ���ǿյأ������������\nǰ������һ�����ӡ�\n\n[X]��ʹ�á�",
					L"�ǻ�֮Ӱ\tĿǰ��ֻ��ʹ��5�Σ������������\n������������ʹ�ô�����ÿ��������\n80��ҡ�",
					L"��ʿ\t�õģ�лл����"
				};
				setMsg(msg);
				break;
			}
		case 70:
			{
				map_npc->init(0);
				map_floor[hero.getNowFloor()].getinfo(0,6)->getNpc()->init(0);

				bool trueend=true;
				for (int i=0;i<map_floornum;i++)
					if (map_floor[i].hasMonster()) {
						trueend=false;
						break;
					}

				if (trueend) {
					const wchar_t* msg[50]={
						L"�����ˮ����Ƭ��",
						L"��ʿ\t��ô���׾�ժ�����ˣ�������ʲô��\n��...",
						L"��ͻȻ�����洫��һ���𶯣�",
						L"��ʿ\t��ô�ˣ���������������",
						L"���𶯸и�ǿ���ˣ�",
						L"��ʿ\t�Բۣ����������������Ҫ�尡��\n���ܰ���",
						L"��ʿ\t���ܰ����ò����״���������ҿ�\n�����������"
					};
					setMsg(msg);
				}
				else {
					const wchar_t* msg[50]={
						L"��ˮ�����ػ������һ�أ�",
						L"��ʿ\t�ţ�����Ӧ�þͿ����˰ɡ���֪����\n������������Ƿ��ڡ�",
						L"��ͻȻ�����洫��һ���𶯣�",
						L"��ʿ\t��ô�ˣ���������������",
						L"���𶯸и�ǿ���ˣ�",
						L"��ʿ\t�Բۣ����������������Ҫ�尡��\n���ܰ���",
						L"��ʿ\t���ܰ����ò����״���������ҿ�\n�����������"
					};
					setMsg(msg);
				}
				map_npc=new c_map_npc();
				map_npc->init(71);
				return;
			}
		case 71:
			{
				delete map_npc;
				map_npc=NULL;
				lefttime=10.0;
				this->msg=MESSAGE_NONE;
				break;
			}
		default:
			break;
		}
		map_npc=NULL;
		
	}
	else if (ended) {
		//upload();
		msg=MESSAGE_WIN;
		lasttime=clock();
	}
	else
		msg=MESSAGE_NONE;
}
void constants::printInfo()
{
	if (lefttime<80)
		hgef->printf(ScreenLeft+16,16,HGETEXT_LEFT,"%.2f",lefttime);
	s_step->Render(ScreenLeft+map_width*32+24, 340);
	hgef->printf(ScreenLeft+map_width*32+68, 340, HGETEXT_LEFT, "%d", step);
	int ptm=playtime;
	s_time->Render(ScreenLeft+map_width*32+24, 298);
	if (ptm>=3600)
		hgef->printf(ScreenLeft+map_width*32+68, 298, HGETEXT_LEFT, "%02d : %02d : %02d", ptm/3600, (ptm/60)%60, ptm%60);
	else hgef->printf(ScreenLeft+map_width*32+68, 298, HGETEXT_LEFT, "%02d : %02d", ptm/60, ptm%60);

	// �����ֲ�
	if (book)
		s_enemyinfo->Render(24, 288);
	// ������
	if (canfly) 
		s_floor->Render(71, 288);
	// ħ��
	if (wand>=0) {
		s_wand->Render(118, 288);
		GfxFont* f=new GfxFont(L"����", 14, true);
		f->Print(142, 308, L"%d", wand);
		delete f;
	}
	if (fly>=0) {
		s_fly2->Render(165, 288);
		GfxFont* f=new GfxFont(L"����", 14, true);
		f->Print(189, 308, L"%d", fly);
		delete f;
	}

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
	else if (hard==4) {
		f->SetColor(0xFFFF0000);
		f->Print(64, 356, L"ج���Ѷ�");
	}
	delete f;

}

void constants::upload()
{
	currentmax=0;
	thread t1(&constants::doUpload, this);
	t1.detach();
}

void constants::doUpload()
{
	char url[200];
	// ��ʼʱ�䡢�Ѷȡ���ǰ��������������������������Ǯ����Կ�ס���Կ�ס�ħ�ȴ�������Ϸʱ�䡢����
	sprintf_s(url, "/service/mota/mota3.php?action=upload&starttime=%ld&hard=%d&floor=%d&hp=%d&atk=%d&def=%d&money=%d&yellow=%d&blue=%d&wand=%d&fly=%d&playtime=%.2f&step=%d&lefttime=%.2f",
		starttime, hard, ended?map_floornum-1:hero.getNowFloor(), hero.getHP(), hero.getAtk(), hero.getDef(), hero.getMoney(), hero.yellow(), hero.blue(), wand, fly, playtime, step, lefttime);

	char* output=http.get(http.server, http.port, url, NULL);

	if (output!=NULL && hero.getNowFloor()==map_floornum-1) {
		string text(output);
		stringstream stream;
		stream << text;

		string s1;
		stream >> s1 >> currentmax;
		if (currentmax<=0) currentmax=-1;
		else {
			size_t outsize;
			mbstowcs_s(&outsize, rank, 20, s1.c_str(), 20);
		}
		delete output;
	}
	else
		currentmax=-1;
}

void constants::getRank()
{
	msg=MESSAGE_RANK;
	currentmax=0;
	for (int x=0;x<4;x++)
		for (int i=0; i<10; i++)
			rd[x][i].init();
	nowcnt=1;
	thread t2(&constants::doGetRank, this);
	t2.detach();
}

void constants::doGetRank()
{
	char* output=http.get(http.server, http.port, "/service/mota/mota3.php?action=top", NULL);
	if (output!=NULL) {
		string text(output);
		stringstream stream;
		stream << text;

		for (int x=0;x<4;x++) {
			stream >> currentmax;
			for (int i=0; i<currentmax; i++) {
				stream >> rd[x][i].hp >> rd[x][i].atk >> rd[x][i].def >> rd[x][i].money >> rd[x][i].yellow >> rd[x][i].blue;
				string s1, s2;
				stream >> s1 >> s2;
				size_t outsize;
				mbstowcs_s(&outsize, rd[x][i].t1, 20, s1.c_str(), 20);
				mbstowcs_s(&outsize, rd[x][i].t2, 20, s2.c_str(), 20);
			}
		}
		delete output;
		if (currentmax==0) currentmax=1;
	}
	else
		currentmax=-1;
}

void constants::save(FILE* f) 
{
	fprintf_s(f, "%d %d %d %d %d %d %.2f %ld\n", map_floornum, canfly?1:0, book?1:0, wand, step, hard, playtime, starttime);
}

void constants::load(FILE* f)
{
	int _fly, _book;
	fscanf_s(f, "%d %d %d %d %d %d %f %ld", &nowcnt, &_fly, &_book, &wand, &step, &hard, &playtime, &starttime);
	canfly=_fly==1;
	book=_book==1;
	moving=opening=flooring=false;
}