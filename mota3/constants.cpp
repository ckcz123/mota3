#include "stdafx.h"

// 地图信息
extern c_map_floor map_floor[50];

// 勇士信息
extern c_hero hero;

extern Http http;

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
	playtime=0.0;
	step=0;
	hard=0;
	time_move=time_open=time_animation=time_floor=0;
	msg=MESSAGE_START;
	wand=-1;
	starttime=0;
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
	if(moving) // 移动
	{
		time_move+=dt;
		if(time_move>=0.03)
		{
			time_move-=0.03;
			if(hero->moveComplete())
				moving=false;
		}		
	}
	if(flooring) // 上下楼
	{
		time_floor+=dt;
		if(time_floor>=0.4)
		{
			time_floor=0;
			flooring=false;
		}
	}
	if(opening) // 开门
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
	if(time_animation>=0.1) // 四次后又回到自身状态
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
				if (music)
					hge->Effect_PlayEx(he_GetItem, volume);
				const wchar_t* msg[50]={
					L"获得怪物手册。",
					L"徘徊之影\t这个是怪物手册，你可以将鼠标移动\n到怪物身上查看怪物的各项属性。",
					L"勇士\t啊谢谢，这个帮大忙了！",
					L"徘徊之影\t不客气。下面我为你介绍一下我们摸\n索出来的这座塔的一些特点，希望能\n给你一些帮助。",
					L"勇士\t哎好的，您说~",
					L"徘徊之影\t这座塔最大的特点就是怪物都是可以\n重生的。经我们研究发现，当你在一\n场战斗中打败怪物后，如果这场战斗\n不是无伤，且你身后是空地的话，怪\n物将在你身后重生。重生后的怪物的\n攻防血都是原本数值，但是打掉后得\n到的金币却变成了一半。",
					L"勇士\t原属性重生？金币变成一半？这... \n这怎么打？",
					L"徘徊之影\t你现在后悔还来得及哦。",
					L"勇士\t不，再大的挑战我也要攻克！",
					L"徘徊之影\t好吧，反正你注意这一点，如何克制\n怪物的重生或许是本塔的最大难点。",
					L"勇士\t好的好的，我记下了。还有吗？",
					L"徘徊之影\t嗯... 因为我比较弱，没闯几层就挂\n了，因此并不知道其他什么内容。\n不过，在这座塔里，像我这样的徘徊\n者应该还有很多，如果你碰见了他们\n，也许可以问问更多信息呢。",
					L"勇士\t好的好的，谢谢您！我会加油的！我\n会毁了这座塔，将你们都解脱出来，\n捍卫我大青叶帝国的荣耀！",
					L"系统提示\t本塔快捷键如下：\nS: 存档\nL: 读档\nP: 查看当前MAX\nM: 音乐开关\nQ: 返回主界面\n其他各项道具使用的快捷键在拿到时\n会给予提示。",
					L"系统提示\t本塔由Sky_天空的梦使用C++编写而成\n，代码开源在：\nhttps://github.com/ckcz123/mota3/\n\n如有问题，请于发布帖下进行回复和\n反馈，谢谢支持！",
					L"徘徊之影\t加油吧小伙子，我看好你！"
				};
				setMsg(msg);
				break;
			}
		case 46:
			{
				wand=10;
				if (music)
					hge->Effect_PlayEx(he_GetItem, volume);
				const wchar_t* msg[50]={
					L"获得神秘魔杖！\n你可以在任意空地放置一个标记，怪\n物将不能在标记处进行重生。\n\n[X]键使用。",
					L"徘徊之影\t目前你只能使用10次，不过你可以来\n找我增加它的使用次数，每次增加需\n100金币。",
					L"勇士\t好的，谢谢您！"
				};
				setMsg(msg);
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

	// 怪物手册
	if (book)
		s_enemyinfo->Render(24, 288);
	// 飞行器
	if (canfly) 
		s_floor->Render(71, 288);
	// 魔杖
	if (wand>=0) {
		s_wand->Render(118, 288);
		GfxFont* f=new GfxFont(L"楷体", 14, true);
		f->Print(142, 308, L"%d", wand);
		delete f;
	}

	GfxFont *f=new GfxFont(L"楷体", 24, true);
	if (hard==1) {
		f->SetColor(0xFF00FF00);
		f->Print(64, 356, L"简单难度");
	}
	else if (hard==2) {
		f->SetColor(0xFF96CDCD);
		f->Print(64, 356, L"普通难度");
	}
	else if (hard==3) {
		f->SetColor(0xFFFF0000);
		f->Print(64, 356, L"困难难度");
	}
	delete f;

}

void constants::upload()
{
	thread t1(&constants::doUpload, this);
	t1.detach();
}

void constants::doUpload()
{
	char url[200];
	// 开始时间、难度、当前层数、生命、攻击、防御、金钱、黄钥匙、蓝钥匙、魔杖次数、游戏时间、步数
	sprintf_s(url, "/service/mota/mota3.php?action=upload&starttime=%ld&hard=%d&floor=%d&hp=%d&atk=%d&def=%d&money=%d&yellow=%d&blue=%d&wand=%d&playtime=%.0f&step=%d",
		starttime, hard, hero.getNowFloor(), hero.getHP(), hero.getAtk(), hero.getDef(), hero.getMoney(), hero.yellow(), hero.blue(), wand, playtime, step);

	char* output=http.get(http.server, http.port, url, NULL);

	currentmax=0;
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
	for (int i=0; i<10; i++)
		rd[i].init();
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
		stream >> currentmax;
		for (int i=0; i<currentmax; i++) {
			stream >> rd[i].hp >> rd[i].atk >> rd[i].def >> rd[i].money >> rd[i].yellow >> rd[i].blue;
			string s1, s2;
			stream >> s1 >> s2;
			size_t outsize;
			mbstowcs_s(&outsize, rd[i].t1, 20, s1.c_str(), 20);
			mbstowcs_s(&outsize, rd[i].t2, 20, s2.c_str(), 20);
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
	fscanf_s(f, "%d %d %d %d %d %d %f %ld", &map_floornum, &_fly, &_book, &wand, &step, &hard, &playtime, &starttime);
	canfly=_fly==1;
	book=_book==1;
	moving=opening=flooring=false;
}