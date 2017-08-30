#include "stdafx.h"

// 地图信息
c_map_floor map_floor[50];

// 勇士信息
c_hero hero;

// 常量信息
constants consts;

// HTTP
Http http;


// encode & decode file
bool fileConvert(char* from, char* to, bool encode=true) {
	char* pwd = "f32&*9h=)";

	// 如果文件存在，先删除
	remove(to);

	FILE *fp1, *fp2;
	register char ch;
	int j = 0;
	int j0 = 0;
	int err = fopen_s(&fp1, from, "r");
	if (fp1 == NULL || err!=0) return false;
	err = fopen_s(&fp2, to, "w");
	if (fp2 == NULL || err!=0) {
		fclose(fp1);
		return false;
	}

	while (pwd[++j0]);

	ch = fgetc(fp1);

	/*加密/解密算法开始*/
	while (!feof(fp1)) {
		if (j0 > 7)
			j0 = 0;
		if (encode)
			ch += pwd[j0++];
		else
			ch -= pwd[j0++];
		fputc(ch, fp2);
		ch = fgetc(fp1);
	}
	fclose(fp1);
	fclose(fp2);
	return true;
}


void loadsave()
{
	FILE *savefile;
	constants tmpcon;
	c_hero tmphero;
	for (int i=0;i<1000;i++) {
		char s[100]="";
		sprintf_s(s,"Save/save%d.dat",i);
		int err=fopen_s(&savefile,s,"r");
		if (err!=0)
			consts.sd[i].hp=-1;
		else {
			tmpcon.load(savefile);
			tmphero.load(savefile);
			consts.sd[i].init(tmphero.getHP(), tmphero.getAtk(), tmphero.getDef(), tmphero.getNowFloor(), tmpcon.hard);
			fclose(savefile);
		}
	}
	consts.lasttime=clock();
}

void save(int id)
{
	FILE *savefile;
	char s[100]="";
	sprintf_s(s,"Save/save%d.dat",id);
	fopen_s(&savefile,s,"w");
	consts.save(savefile);
	hero.save(savefile);
	for(int i=0;i<consts.map_floornum;i++)map_floor[i].save(savefile);
	fclose(savefile);
	consts.setMsg(L"存档成功！");
}
void load(int id)
{
	FILE *loadfile;
	char s[100]="";
	sprintf_s(s,"Save/save%d.dat",id);
	fopen_s(&loadfile,s,"r");
	consts.load(loadfile);
	hero.load(loadfile);
	for(int i=0;i<consts.nowcnt;i++)map_floor[i].load(loadfile);
	if (consts.map_floornum<consts.nowcnt)
		consts.map_floornum=consts.nowcnt;
	consts.nowcnt=0;
	fclose(loadfile);
	consts.setMsg(L"读档成功！");
}
void showMessage(const wchar_t *_s) // 显示提示
{

	wchar_t s[500];
	wcscpy_s(s, _s);

	// cal lines
	int l=1;
	for (wchar_t* p=s; *p!=L'\0';p++) {
		if (*p==L'\n') l++;
	}
	if (l<6) l=6;

	int left=16+consts.ScreenLeft, right=consts.map_width*32+consts.ScreenLeft-16,
		top=consts.map_height*32-12-28*l, bottom=consts.map_height*32-8;
	consts.s_bg->RenderStretch(left, top, right, bottom);
	GfxFont *f=new GfxFont(L"楷体", 22);

	// get name
	wchar_t* pos=s;
	while (*pos!=L'\t' && *pos!=L'\0') *pos++;
	if (*pos==L'\t') {
		*pos=L'\0';
		consts.s_bg->RenderStretch(left, top-40, left+25+f->GetTextSize(s).cx, top-2);
		f->Print(left+12, top-33, L"%s", s);
		pos++;
	}
	else pos=s;

	f->Print(left+12, top+12, L"%s", pos);

	delete f;
}
void showMax(const wchar_t *s)
{
	consts.s_bg->RenderStretch(16+consts.ScreenLeft, consts.map_height*32-400, consts.map_width*32+consts.ScreenLeft-16, consts.map_height*32-8);
	GfxFont *f=new GfxFont(L"楷体", 22);
	f->Print(16+consts.ScreenLeft+8, consts.map_height*32-400+8, L"%s", s);
	delete f;
}
void init()
{
	// 初始化
	consts.init();
	hero.init();
	// 读入地图
	FILE *map_infile;
	fopen_s(&map_infile,"Res/map.dat","r");
	fscanf_s(map_infile, "%d", &consts.map_floornum);
	for(int i=0;i<consts.map_floornum;i++)
	{
		int ch[30][30];
		for(int j=0;j<consts.map_height;j++)for(int k=0;k<consts.map_width;k++)fscanf_s(map_infile,"%d",&ch[j][k]);
		map_floor[i].init(i,ch);
	}
	fclose(map_infile);
}
bool frameFunc()
{
	float dt=consts.hge->Timer_GetDelta();

	if(consts.isFree() && consts.hge->Input_GetKeyState(HGEK_DOWN) && hero.canMove(0))consts.moving=true;
	if(consts.isFree() && consts.hge->Input_GetKeyState(HGEK_LEFT) && hero.canMove(1))consts.moving=true;
	if(consts.isFree() && consts.hge->Input_GetKeyState(HGEK_RIGHT) && hero.canMove(2))consts.moving=true;
	if(consts.isFree() && consts.hge->Input_GetKeyState(HGEK_UP) && hero.canMove(3))consts.moving=true;
	if(consts.isFree() && consts.hge->Input_GetKeyState(HGEK_Z) && clock()-consts.lasttime>200) {
		hero.turn();
		consts.lasttime=clock();
	}
	if(consts.hge->Input_GetKeyState(HGEK_Q) && consts.isFree()) consts.msg=consts.MESSAGE_RESTART;
	if(consts.hge->Input_GetKeyState(HGEK_S) && consts.isFree()) {
		loadsave();
		consts.msg=consts.MESSAGE_SAVE;
	}
	if(consts.hge->Input_GetKeyState(HGEK_L) && consts.isFree()) {
		loadsave();
		consts.msg=consts.MESSAGE_LOAD;
	}
	if (consts.isFree() && consts.hge->Input_GetKeyState(HGEK_P)) consts.getRank();
	if(consts.hge->Input_GetKeyState(HGEK_G) && consts.isFree()) {
		if (consts.canfly) { 
			if (consts.lefttime<80.) consts.setMsg(L"传送器好像失效了！");
			else if (!hero.nearStair()) consts.setMsg(L"只有在楼梯旁才能使用传送器！");
			else {
				hero.setFlyFloor();
				consts.msg=consts.MESSAGE_FLYING;
			}
		}
	}
	if(consts.hge->Input_GetKeyState(HGEK_M) && consts.isFree()) {
		consts.music=!consts.music;
		consts.setMsg(consts.music?L"音乐已开启":L"音乐已关闭");
		if (consts.music) consts.hge->Channel_SetVolume(consts.hc_Music, consts.bgmvolume);
		else consts.hge->Channel_SetVolume(consts.hc_Music, 0);
	}
	if(consts.hge->Input_GetKeyState(HGEK_MINUS) && consts.isFree()) {
		consts.showdamage=!consts.showdamage;
		consts.setMsg(consts.showdamage?L"显伤已开启":L"显伤已关闭");
	}

	// 使用魔杖
	if (consts.isFree() && consts.hge->Input_GetKeyState(HGEK_X) && consts.wand>0) {
		hero.useWand();
	}

	if (consts.msg==consts.MESSAGE_START) {
		if (consts.hge->Input_GetKeyState(HGEK_1)) {
			consts.lasttime=clock();
			consts.msg=consts.MESSAGE_CHOOSE_HARD;
		}
		else if (consts.hge->Input_GetKeyState(HGEK_2)) {
			loadsave();
			consts.msg=consts.MESSAGE_LOAD;
		}
		else if (consts.hge->Input_GetKeyState(HGEK_3)) {
			return true;
		}
	}
	if (consts.msg==consts.MESSAGE_CHOOSE_HARD) {
		if (consts.hge->Input_GetKeyState(HGEK_1) && clock()-consts.lasttime>200) {
			consts.hard=1;
			hero.getYellowKey();
			hero.getYellowKey();
			hero.getBlueKey();
			consts.msg=consts.MESSAGE_TEXT;
			consts.nowcnt=0;
			consts.starttime=time(NULL);
		}
		else if (consts.hge->Input_GetKeyState(HGEK_2) && clock()-consts.lasttime>200) {
			consts.hard=2;
			hero.getYellowKey();
			consts.msg=consts.MESSAGE_TEXT;
			consts.nowcnt=0;
			consts.starttime=time(NULL);
		}
		else if (consts.hge->Input_GetKeyState(HGEK_3) && clock()-consts.lasttime>200) {
			consts.hard=3;
			consts.msg=consts.MESSAGE_TEXT;
			consts.nowcnt=0;
			consts.starttime=time(NULL);
		}
		else if (consts.hge->Input_GetKeyState(HGEK_4) && clock()-consts.lasttime>200) {
			consts.hard=4;
			consts.msg=consts.MESSAGE_TEXT;
			consts.nowcnt=0;
			consts.starttime=time(NULL);

			// 噩梦难度：所有纯血门均不可开启
			map_floor[4].getinfo(1,4)->init(0,0,0,4);
			map_floor[5].getinfo(12,1)->init(0,0,0,4);
			map_floor[6].getinfo(4,1)->init(0,0,0,4);
			map_floor[8].getinfo(11,8)->init(0,0,0,4);
			map_floor[10].getinfo(12,9)->init(0,0,0,4);
			map_floor[12].getinfo(8,1)->init(0,0,0,4);

		}
	}
	if (consts.msg==consts.MESSAGE_TEXT) {
		if(consts.hge->Input_GetKeyState(HGEK_ENTER) && clock()-consts.lasttime>200) {
			consts.nowcnt++;
			consts.lasttime=clock();
		}
		if (consts.nowcnt==10) {
			consts.setMsg(L"勇士\t就是这里了，冲进去!");
			consts.nowcnt=0;
		}
	}

	// 提示消息
	if (consts.msg==consts.MESSAGE_HINT)
	{
		if(consts.hge->Input_GetKeyState(HGEK_ENTER) && clock()-consts.lasttime>200) {
			consts.nowcnt++;
			consts.lasttime=clock();
		}
		if (consts.nowcnt>=consts.hint.size()) {
			consts.finishHint();
		}
	}

	// 存档
	if(consts.msg==consts.MESSAGE_SAVE)
	{
		if(consts.hge->Input_GetKeyState(HGEK_DOWN) && clock()-consts.lasttime>200) {
			consts.wanttosave++;
			if (consts.wanttosave>=1000) consts.wanttosave=999;
			consts.lasttime=clock();
		}
		else if(consts.hge->Input_GetKeyState(HGEK_UP) && clock()-consts.lasttime>200) {
			consts.wanttosave--;
			if (consts.wanttosave<0) consts.wanttosave=0;
			consts.lasttime=clock();
		}
		else if(consts.hge->Input_GetKeyState(HGEK_ESCAPE))
			consts.msg=consts.MESSAGE_NONE;
		else if(consts.hge->Input_GetKeyState(HGEK_ENTER))
			save(consts.wanttosave);
	}

	// 读档
	if(consts.msg==consts.MESSAGE_LOAD)
	{
		if(consts.hge->Input_GetKeyState(HGEK_DOWN) && clock()-consts.lasttime>200) {
			consts.wanttosave++;
			if (consts.wanttosave>=1000) consts.wanttosave=999;
			consts.lasttime=clock();
		}
		else if(consts.hge->Input_GetKeyState(HGEK_UP) && clock()-consts.lasttime>200) {
			consts.wanttosave--;
			if (consts.wanttosave<0) consts.wanttosave=0;
			consts.lasttime=clock();
		}
		else if(consts.hge->Input_GetKeyState(HGEK_ESCAPE)) {
			if (consts.hard==0) consts.msg=consts.MESSAGE_START;
			else consts.msg=consts.MESSAGE_NONE;
		}
		else if(consts.hge->Input_GetKeyState(HGEK_ENTER)) {
			if (consts.sd[consts.wanttosave].hp>=0)
				load(consts.wanttosave);
		}
	}

	// 胜利
	if (consts.msg==consts.MESSAGE_WIN && clock()-consts.lasttime>300) {
		if (consts.hge->Input_GetKeyState(HGEK_P))
			consts.getRank();
		else if (consts.hge->Input_GetKeyState(HGEK_ENTER)) init();
	}
	// 重新开始
	if (consts.msg==consts.MESSAGE_RESTART) {
		if (consts.hge->Input_GetKeyState(HGEK_ENTER))
			init();
		else if (consts.hge->Input_GetKeyState(HGEK_ESCAPE))
			consts.msg=consts.MESSAGE_NONE;
	}

	// 正在飞行
	if(consts.msg==consts.MESSAGE_FLYING)
	{
		if(consts.hge->Input_GetKeyState(HGEK_UP) && clock()-consts.lasttime>100)
		{
			if (hero.getFlyFloor()<hero.getNowFloor()+(hero.nearUpStair()?1:0)
				|| map_floor[hero.getFlyFloor()].isLinked())
				hero.setFlyFloor(1);
			consts.lasttime=clock();
		}
		if(consts.hge->Input_GetKeyState(HGEK_DOWN) && clock()-consts.lasttime>100)
		{
			hero.setFlyFloor(-1);
			consts.lasttime=clock();
		}
		if(consts.hge->Input_GetKeyState(HGEK_ESCAPE))
		{
			consts.msg=consts.MESSAGE_NONE;
		}
		if(consts.hge->Input_GetKeyState(HGEK_ENTER))
		{
			hero.fly();
			consts.msg=consts.MESSAGE_NONE;
		}
	}

	if(consts.msg==consts.MESSAGE_NPC)
	{
		c_map_npc* map_npc=map_floor[hero.getNowFloor()].getinfo(hero.nextY(),hero.nextX())->getNpc();
		int npcid=map_npc->getId(), npctimes=map_npc->getVisit();

		// 逃跑过程中
		if (consts.lefttime<80) {
			// 商店
			if (npcid==45||npcid==50) {
				consts.setMsg(L"贪婪之神\t塔都要倒了你还想来买东西不成？");
			}
			else consts.setMsg(L"徘徊之影\t孩子恭喜你成功了！塔快倒了，快跑\n吧，我们已是亡魂，不用管我们了。");
		}
		
		// 商店
		else if (npcid==45) {
			if(consts.hge->Input_GetKeyState(HGEK_1) && clock()-consts.lasttime>200) {
				hero.npc(1);
			}
			else if(consts.hge->Input_GetKeyState(HGEK_2) && clock()-consts.lasttime>200) {
				hero.npc(2);
			}
			else if(consts.hge->Input_GetKeyState(HGEK_3) && clock()-consts.lasttime>200) {
				hero.npc(3);
			}
			else if (consts.hge->Input_GetKeyState(HGEK_ESCAPE)) {
				consts.msg=consts.MESSAGE_NONE;
			}
			
		}
		else if (npcid==46) {
			if (npctimes==0) {
				hero.npc();
			}
			else {
				if (consts.hge->Input_GetKeyState(HGEK_ENTER)) {
					hero.npc();
				}
				else if (consts.hge->Input_GetKeyState(HGEK_ESCAPE)) {
					consts.msg=consts.MESSAGE_NONE;
				}
			}
		}
		else {
			hero.npc();
		}
	}

	if (consts.msg==consts.MESSAGE_RANK) {
		if(consts.hge->Input_GetKeyState(HGEK_DOWN) && consts.currentmax>0 && clock()-consts.lasttime>100)
		{
			consts.nowcnt++; if (consts.nowcnt>4) consts.nowcnt=4;
			consts.lasttime=clock();
		}
		if(consts.hge->Input_GetKeyState(HGEK_UP) && consts.currentmax>0 && clock()-consts.lasttime>100)
		{
			consts.nowcnt--; if (consts.nowcnt==0) consts.nowcnt=1;
			consts.lasttime=clock();
		}
		if(consts.hge->Input_GetKeyState(HGEK_ENTER))
		{
			if (consts.ended)
				init();
			else 
				consts.msg=consts.MESSAGE_NONE;
			consts.nowcnt=0;
		}
	}

	if ((consts.msg==consts.MESSAGE_FLYING || consts.msg==consts.MESSAGE_NPC) && consts.hge->Input_GetKeyState(HGEK_ENTER))
		consts.msg=consts.MESSAGE_NONE;

	consts.goOn(&hero, &map_floor[hero.getNowFloor()], dt);
	return false;
}
bool renderFunc()
{
	consts.hge->Gfx_BeginScene();
	consts.hge->Gfx_Clear(0);

	if (consts.msg==consts.MESSAGE_START) {
		GfxFont *f=new GfxFont(L"楷体", 25);
		float len=f->GetTextSize(L"[1] 开始游戏").cx;
		float left=16*consts.map_width+consts.ScreenLeft-len/2;
		float height=consts.map_height*32*0.6;
		f->Print(left, height, L"[1] 开始游戏");
		f->Print(left, height+28, L"[2] 读取游戏");
		f->Print(left, height+56, L"[3] 退出游戏");
		delete f;
		f=new GfxFont(L"楷体", 40);
		f->SetColor(0xFFFF0000);
		len=f->GetTextSize(L"重生塔").cx;
		f->Print(16*consts.map_width+consts.ScreenLeft-len/2, height*0.4, L"重生塔");
		delete f;
		consts.hge->Gfx_EndScene();
		return false;
	}
	if (consts.msg==consts.MESSAGE_CHOOSE_HARD) {
		GfxFont *f=new GfxFont(L"楷体", 25);
		float len=f->GetTextSize(L"[2] 普通（送一黄，减伤5%%）").cx;
		float left=16*consts.map_width+consts.ScreenLeft-len/2;
		float height=consts.map_height*32*0.6;
		f->Print(left, height-32, L"请选择难度：");
		f->Print(left, height, L"[1] 简单（送二黄一蓝，减伤20%%）");
		f->Print(left, height+28, L"[2] 普通（送一黄，减伤8%%）");
		f->Print(left, height+56, L"[3] 困难（正常模式）");
		f->Print(left, height+84, L"[4] 噩梦（所有纯血门均不可开启）");
		delete f;
		consts.hge->Gfx_EndScene();
		return false;
	}
	if (consts.msg==consts.MESSAGE_TEXT) {
		wchar_t* msg[50]={
			L"对青叶帝国而言，3358年原本应该和\n往常一样平静。",
			L"但是，出现在边境上的异次元之门却\n给帝国带来了大麻烦。\n有源源不断的怪物从门中出现，并企\n图侵入帝国的领土。",
			L"幸运的是，怪物们智力低下，且并不\n强悍，很容易被斩杀，因此虽然怪物\n数量不少，却仍是帝国占据着上风。",
			L"直到那一天...",
			L"那一天，有一座塔突然出现在了防线\n另一侧，而谁也不知道它是怎么出现\n的，就好像突然冒出在了那里。",
			L"而自从塔出现后，战局陡然发生了变\n化。怪物们竟然拥有了不死之身！",
			L"无论帝国的武士们用什么方法斩杀了\n怪物，一转头却又会发现怪物好端端\n就在那里，只有自己身上的血迹才能\n证明这场战斗确实发生过。",
			L"毫无疑问，正是这座塔的出现，才导\n致怪物们拥有了不死之身的能力。\n因此，帝国决定派遣勇敢的武士，想\n办法绕过防线，偷入塔中，一定要找\n到这座塔的秘密。",
			L"然而，进去的人却再也没有回来。\n而你，作为第1127个勇士，勇敢地接\n受了使命，想办法闯入了这座塔。",
			L"我们的故事，也就这样开始了......"
		};

		GfxFont *f=new GfxFont(L"楷体", 24);
		f->SetKerningHeight(8);
		f->Render(consts.ScreenLeft+8, 32*consts.map_height*0.3, msg[consts.nowcnt]);
		delete f;

		f=new GfxFont(L"楷体", 24, true);
		if (consts.hard==1) {
			f->SetColor(0xFF00FF00);
			f->Print(64, 356, L"简单难度");
		}
		else if (consts.hard==2) {
			f->SetColor(0xFF96CDCD);
			f->Print(64, 356, L"普通难度");
		}
		else if (consts.hard==3) {
			f->SetColor(0xFFFF0000);
			f->Print(64, 356, L"困难难度");
		}
		else if (consts.hard==4) {
			f->SetColor(0xFFFF0000);
			f->Print(64, 356, L"噩梦难度");
		}
		delete f;
		consts.hge->Gfx_EndScene();

		return false;
	}
	if (consts.msg==consts.MESSAGE_LOAD && consts.hard==0) {
		wchar_t ss[200];
		savedata* sd=&consts.sd[consts.wanttosave];
		if (sd->hp<0)
			wsprintf(ss,L"读取存档 %d\n(无效的存档)\n\n[↑] [↓] 更改读档位置\n[ENTER] 确认读档\n[ESC] 取消", consts.wanttosave+1);
		else
			wsprintf(ss,L"读取存档 %d\n(%s/F%d/HP%d/A%d/D%d)\n\n[↑] [↓] 更改读档位置\n[ENTER] 确认读档\n[ESC] 取消",
			consts.wanttosave+1, consts.getHardText(sd->hard), sd->now_floor, sd->hp,
			sd->atk, sd->def);
		showMessage(ss);
		
		consts.hge->Gfx_EndScene();

		return false;
	}

	map_floor[hero.getNowFloor()].show();
	hero.setMaxFloor();
	hero.show();
	hero.printInfo();
	consts.printInfo();

	float mx,my; // 鼠标位置
	consts.hge->Input_GetMousePos(&mx,&my);
	int x=(mx-consts.ScreenLeft)/32,y=my/32;
	if (consts.book && x>=0 && x<consts.map_width)map_floor[hero.getNowFloor()].printMonsterInfo(x,y);

	switch (consts.msg)
	{
	case consts.MESSAGE_RESTART:
		showMessage(L"你想返回主界面吗？\n\n[ENTER] 确认\n[ESC] 取消");
		break;
	case consts.MESSAGE_WIN:
		{
			wchar_t ss[200];
			wsprintf(ss, L"恭喜通关！您的分数是 %d。\n", hero.getHP());

			// uploading...
			if (consts.currentmax==0) {
				wcscat_s(ss, L"正在上传成绩... 请稍后\n");
			}
			// error
			else if (consts.currentmax<0) {
				wcscat_s(ss, L"成绩上传失败，请检查网络设置。\n");
			}
			else {
				wchar_t tmp[200];
				wsprintf(tmp, L"当前难度下排名%s，当前MAX %d。\n", consts.rank, consts.currentmax);
				wcscat_s(ss, tmp);
			}
			wcscat_s(ss, L"（P键可查看各难度MAX记录信息。）\n欢迎截图到发布帖下进行炫耀！\n\n[ENTER] 重新开始");
			showMessage(ss);
			break;
		}
	case consts.MESSAGE_HINT:
		showMessage(consts.hint.at(consts.nowcnt).c_str());
		break;
	case consts.MESSAGE_SAVE:
		{
			wchar_t ss[200];
			savedata* sd=&consts.sd[consts.wanttosave];
			if (sd->hp<0)
				wsprintf(ss,L"存档到文件 %d\n(空白)\n\n[↑] [↓] 更改存档位置\n[ENTER] 确认存档\n[ESC] 取消", consts.wanttosave+1);
			else
				wsprintf(ss,L"存档到文件 %d\n(%s/F%d/HP%d/A%d/D%d)\n\n[↑] [↓] 更改存档位置\n[ENTER] 确认存档\n[ESC] 取消",
				consts.wanttosave+1, consts.getHardText(sd->hard), sd->now_floor, sd->hp,
				sd->atk, sd->def);
			showMessage(ss);
			break;
			break;
		}
	case consts.MESSAGE_LOAD:
		{
			wchar_t ss[200];
			savedata* sd=&consts.sd[consts.wanttosave];
			if (sd->hp<0)
				wsprintf(ss,L"读取存档 %d\n(无效的存档)\n\n[↑] [↓] 更改读档位置\n[ENTER] 确认读档\n[ESC] 取消", consts.wanttosave+1);
			else
				wsprintf(ss,L"读取存档 %d\n(%s/F%d/HP%d/A%d/D%d)\n\n[↑] [↓] 更改读档位置\n[ENTER] 确认读档\n[ESC] 取消",
						consts.wanttosave+1, consts.getHardText(sd->hard), sd->now_floor, sd->hp,
						sd->atk, sd->def);
			showMessage(ss);
			break;
		}
	case consts.MESSAGE_FLYING:
		{
			wchar_t ss[100];
			wsprintf(ss,L"只能飞往低层或有通路可达的高层。\n\n我要飞往 %d 楼\n[↑] [↓] 更改楼号\n[ENTER] 确认飞行\n[ESC] 取消",hero.getFlyFloor());
			showMessage(ss);
			break;
		}
	case consts.MESSAGE_RANK:
		{
			wchar_t ss[1200];
			if (consts.currentmax<0) {
				wsprintf(ss, L"拉取MAX记录失败，请检查网络连接，\n或在贴吧下进行反馈。\n\n[ENTER] 确定");
				showMessage(ss);
			}
			else if (consts.currentmax==0) {
				wsprintf(ss, L"正在拉取MAX记录，请稍后...\n这可能需要几秒钟。\n\n[ENTER] 取消");
				showMessage(ss);
			}
			else {
				wsprintf(ss, L"%s难度下MAX数据：\n", consts.getHardText(consts.nowcnt));

				for (int i=0; i<10; i++) {
					record* r=&consts.rd[consts.nowcnt-1][i];
					wchar_t tmp[100];
					wsprintf(tmp, L"TOP%2d: %-8d", i+1, r->hp);
					wcscat_s(ss, tmp);
					if (consts.rd[consts.nowcnt-1][i].hp>0) {
						wsprintf(tmp, L"  (%s %s)", r->t1, r->t2);
						wcscat_s(ss, tmp);
					}
					if (i<=2) {
						wsprintf(tmp, L"\n[%d, %d, %d, %d, %d, %d]",
							r->hp, r->atk, r->def, r->money, r->yellow, r->blue);
						wcscat_s(ss, tmp);
					}
					wcscat_s(ss, L"\n");
				}

				wcscat_s(ss, L"[↑][↓] 其他难度   [ENTER] 确定");
				showMax(ss);
			}

			break;
		}
	default:
		break;
	}
	if (consts.msg==consts.MESSAGE_NPC) {
		c_map_npc* npc=map_floor[hero.getNowFloor()].getinfo(hero.nextY(),hero.nextX())->getNpc();
		int id=npc->getId(), times=npc->getVisit();

		// 6楼商店
		if (id==45) {
			int need=25+2*times;
			wchar_t s[200];
			wsprintf(s, L"贪婪之神\t勇敢的武士啊，给我%d金币就可以：\n\n[1] 生命+400\n[2] 攻击+2\n[3] 防御+3\n[ESC] 离开", need);
			showMessage(s);
		}
		// 7楼奸商
		if (id==46) {
			if (times>0) {
				showMessage(L"徘徊之影\t100金币一次，可增加魔杖使用次数。\n\n[ENTER] 我要\n[ESC] 离开");
			}
		}
		// 12楼商店
		if (id==50) {
			int need=50+4*times;
			wchar_t s[200];
			wsprintf(s, L"贪婪之神\t勇敢的武士啊，给我%d金币就可以：\n\n[1] 生命+800\n[2] 攻击+4\n[3] 防御+6\n[ESC] 离开", need);
			showMessage(s);
		}
	}
	consts.hge->Gfx_EndScene();
	return false;
}
int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{
	
	//hge初始化
	consts.hge=hgeCreate(HGE_VERSION);
	consts.hge->System_SetState(HGE_FRAMEFUNC,frameFunc);
	consts.hge->System_SetState(HGE_RENDERFUNC,renderFunc);
	consts.hge->System_SetState(HGE_USESOUND,true);
	consts.hge->System_SetState(HGE_TITLE,"重生塔 By Sky_天空的梦");
	consts.hge->System_SetState(HGE_WINDOWED,true);
	consts.hge->System_SetState(HGE_HIDEMOUSE,false);
	consts.hge->System_SetState(HGE_SCREENHEIGHT,32*consts.map_height);
	consts.hge->System_SetState(HGE_SCREENWIDTH,32*consts.map_width+2*consts.ScreenLeft);
	if(consts.hge->System_Initiate())
	{
		consts.loadResources();
		init();
		consts.hge->System_Start();
		consts.destroy();
	}
	consts.hge->System_Shutdown();
	consts.hge->Release();
	return 0;
}
