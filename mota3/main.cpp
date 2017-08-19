#include "stdafx.h"

// 地图信息
c_map_floor map_floor[50];

// 勇士信息
c_hero hero;

// 常量信息
constants consts;


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
	for (int i=0;i<10;i++) {
		char s[100]="";
		sprintf_s(s,"Save/save%d.dat",i);
		int err=fopen_s(&savefile,s,"r");
		if (err!=0)
			consts.sd[i].hp=-1;
		else {
			tmpcon.load(savefile);
			tmphero.load(savefile);
			consts.sd[i].init(tmphero.getHP(), tmphero.getAtk(), tmphero.getDef(), tmphero.getNowFloor());
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
	for(int i=31;i<=consts.special_floornum;i++)map_floor[i].save(savefile);
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
	for(int i=0;i<consts.map_floornum;i++)map_floor[i].load(loadfile);
	for(int i=31;i<=consts.special_floornum;i++)map_floor[i].load(loadfile);
	fclose(loadfile);
	consts.setMsg(L"读档成功！");
}
void showMessage(const wchar_t *s, const wchar_t *_name=NULL) // 显示提示
{
	int l=consts.lines, 
		left=16+consts.ScreenLeft, right=consts.map_width*32+consts.ScreenLeft-16,
		top=consts.map_height*32-12-25*l, bottom=consts.map_height*32-8;
	hgeSprite *s_temp;
	s_temp=new hgeSprite(consts.ht_skin, 0, 0, 128, 128);
	s_temp->SetColor(0xCCFFFFFF);
	s_temp->RenderStretch(left, top, right, bottom);
	GfxFont *f=new GfxFont(L"楷体", 22);
	f->Print(left+8, top+8, L"%s", s);

	wchar_t name[200]=L"";
	if (_name!=NULL) {
		wcscpy_s(name, _name);
	}
	else if (consts.name.length()!=0) {
		wcscpy_s(name, consts.name.c_str());
	}
	if (wcslen(name)!=0) {
		s_temp->RenderStretch(left, top-40, left+100, top-2);
		f->Print(left+8, top-33, L"%s", name);
	}

	delete f;
	delete s_temp;
}
void init()
{
	// 初始化
	consts.init();
	hero.init();
	// 读入地图
	FILE *map_infile;
	fopen_s(&map_infile,"Res/map.dat","r");
	for(int i=0;i<consts.map_floornum;i++)
	{
		int ch[30][30];
		for(int j=0;j<consts.map_height;j++)for(int k=0;k<consts.map_width;k++)fscanf_s(map_infile,"%d",&ch[j][k]);
		map_floor[i].init(i,ch);
	}
	for(int i=31;i<=consts.special_floornum;i++)
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
	if(consts.hge->Input_GetKeyState(HGEK_Q) && consts.isFree()) consts.msg=consts.MESSAGE_QUIT;
	if(consts.hge->Input_GetKeyState(HGEK_R) && consts.isFree()) consts.msg=consts.MESSAGE_RESTART;
	if(consts.hge->Input_GetKeyState(HGEK_S) && consts.isFree()) {
		if (hero.getNowFloor()>=30 && !hero.nearStair()) {
			consts.setMsg(L"只有在楼梯边才能存档！");
		}
		else {
			loadsave();
			consts.msg=consts.MESSAGE_SAVE;
		}
	}
	if(consts.hge->Input_GetKeyState(HGEK_L) && consts.isFree()) {
		loadsave();
		consts.msg=consts.MESSAGE_LOAD;
	}
	if(consts.hge->Input_GetKeyState(HGEK_F) && consts.isFree()) {
		if (hero.getNowFloor()>=30) consts.setMsg(L"这里不能使用飞行器！");
		else if (consts.canfly) { 
			if (hero.getNowFloor()==21) consts.setMsg(L"飞行器好像在这层楼失效了！");
			else consts.msg=consts.MESSAGE_FLYING;
		}
		else if (consts.lefttime<80) consts.setMsg(L"飞行器好像遗失了！");
		else consts.setMsg(L"你需要先获得飞行器！");
	}
	if(consts.hge->Input_GetKeyState(HGEK_M) && consts.isFree()) {
		consts.music=!consts.music;
		consts.setMsg(consts.music?L"音乐已开启":L"音乐已关闭");
		if (consts.music) consts.hge->Channel_SetVolume(consts.hc_Music, consts.bgmvolume);
		else consts.hge->Channel_SetVolume(consts.hc_Music, 0);
	}
	if (consts.msg==consts.MESSAGE_START) {
		if (consts.hge->Input_GetKeyState(HGEK_1)) {
			consts.lasttime=clock();
			consts.msg=consts.MESSAGE_CHOOSE_HARD;
		}
		else if (consts.hge->Input_GetKeyState(HGEK_2)) {
			consts.msg=consts.MESSAGE_LOAD;
		}
		else if (consts.hge->Input_GetKeyState(HGEK_3)) {
			return true;
		}
	}
	if (consts.msg==consts.MESSAGE_CHOOSE_HARD) {
		if (consts.hge->Input_GetKeyState(HGEK_1) && clock()-consts.lasttime>200) {
			consts.hard=1;
			consts.msg=consts.MESSAGE_TEXT;
			consts.nowcnt=0;
		}
		else if (consts.hge->Input_GetKeyState(HGEK_2) && clock()-consts.lasttime>200) {
			consts.hard=2;
			consts.msg=consts.MESSAGE_TEXT;
			consts.nowcnt=0;
		}
		else if (consts.hge->Input_GetKeyState(HGEK_3) && clock()-consts.lasttime>200) {
			consts.hard=3;
			consts.msg=consts.MESSAGE_TEXT;
			consts.nowcnt=0;
		}
	}
	if (consts.msg==consts.MESSAGE_TEXT) {
		if(consts.hge->Input_GetKeyState(HGEK_ENTER) && clock()-consts.lasttime>200) {
			consts.nowcnt++;
			consts.lasttime=clock();
		}
		if (consts.nowcnt==10) {
			consts.msg=consts.MESSAGE_NONE;
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
		if (consts.nowcnt>=consts.hint.size())
			consts.msg=consts.MESSAGE_NONE;
	}

	// 存档
	if(consts.msg==consts.MESSAGE_SAVE)
	{
		if(consts.hge->Input_GetKeyState(HGEK_DOWN) && clock()-consts.lasttime>200) {
			consts.wanttosave++;
			if (consts.wanttosave>=10) consts.wanttosave=9;
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
			if (consts.wanttosave>=10) consts.wanttosave=9;
			consts.lasttime=clock();
		}
		else if(consts.hge->Input_GetKeyState(HGEK_UP) && clock()-consts.lasttime>200) {
			consts.wanttosave--;
			if (consts.wanttosave<0) consts.wanttosave=0;
			consts.lasttime=clock();
		}
		else if(consts.hge->Input_GetKeyState(HGEK_ESCAPE))
			consts.msg=consts.MESSAGE_NONE;
		else if(consts.hge->Input_GetKeyState(HGEK_ENTER)) {
			if (consts.sd[consts.wanttosave].hp<0)
				consts.setMsg(L"无效的存档。");
			else load(consts.wanttosave);
		}
	}

	// 胜利or失败
	if ((consts.msg==consts.MESSAGE_WIN || consts.msg==consts.MESSAGE_LOSE) && consts.hge->Input_GetKeyState(HGEK_ENTER)) return true;
	// 退出
	if (consts.msg==consts.MESSAGE_QUIT && consts.hge->Input_GetKeyState(HGEK_SPACE)) return true;
	// 重新开始
	if (consts.msg==consts.MESSAGE_RESTART && consts.hge->Input_GetKeyState(HGEK_SPACE)) init();

	// 正在飞行
	if(consts.msg==consts.MESSAGE_FLYING)
	{
		if(consts.hge->Input_GetKeyState(HGEK_UP) && clock()-consts.lasttime>100)
		{
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
		int npcid=map_floor[hero.getNowFloor()].getinfo(hero.nextY(),hero.nextX())->getNpcID();

		if (npcid>=40 && npcid<=44)
			hero.npc();

		if (npcid>=45 && npcid<=50)
		{
			if(consts.hge->Input_GetKeyState(HGEK_1) && clock()-consts.lasttime>200) {
				hero.npc(1);consts.lasttime=clock();
			}
			else if(consts.hge->Input_GetKeyState(HGEK_2) && clock()-consts.lasttime>200) {
				hero.npc(2);consts.lasttime=clock();
			}
			else if(consts.hge->Input_GetKeyState(HGEK_3) && clock()-consts.lasttime>200) {
				hero.npc(3);consts.lasttime=clock();
			}
		}

		if (npcid==51 || npcid==52 || npcid>=55)
			hero.npc();

		if(npcid==53 || npcid==54)
		{
			if(consts.hge->Input_GetKeyState(HGEK_1)) hero.npc(1);
			else hero.npc();
		}
	}

	if ((consts.msg==consts.MESSAGE_FLYING || consts.msg==consts.MESSAGE_NPC
			|| consts.msg==consts.MESSAGE_QUIT || consts.msg==consts.MESSAGE_RESTART) && consts.hge->Input_GetKeyState(HGEK_ENTER))
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
		float len=f->GetTextSize(L"[1] 简单难度（嗯").cx;
		float left=16*consts.map_width+consts.ScreenLeft-len/2;
		float height=consts.map_height*32*0.6;
		f->Print(left, height, L"[1] 简单（减伤15%%）");
		f->Print(left, height+28, L"[2] 普通（减伤5%%）");
		f->Print(left, height+56, L"[3] 困难");
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
			L"然而，进去的人却再也没有回来。\n而你，作为第127个勇士，勇敢地接\n受了使命，想办法闯入了这座塔。",
			L"而我们的故事，也就这样开始了......"
		};

		GfxFont *f=new GfxFont(L"楷体", 24);
		f->SetKerningHeight(8);
		f->Render(consts.ScreenLeft+8, 32*consts.map_height*0.3, msg[consts.nowcnt]);
		delete f;
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
	if(consts.battling)consts.monster_battling->printInfo();
	else if (consts.book && x>=0 && x<consts.map_width)map_floor[hero.getNowFloor()].printMonsterInfo(x,y);

	switch (consts.msg)
	{
	case consts.MESSAGE_QUIT:
		showMessage(L"你想退出游戏吗？\n\n[ENTER] 继续游戏\n[SPACE] 退出游戏");
		break;
	case consts.MESSAGE_RESTART:
		showMessage(L"你想重新开始吗？\n\n[ENTER] 继续游戏\n[SPACE] 重新开始");
		break;;
	case consts.MESSAGE_WIN:
		if (consts.trueend) 
			showMessage(L"在仙子的祝福下，YY和XX\n同学顺利地从塔中逃生。从此\n，两人相亲相爱，度过了幸福\n的一生。\n\nTRUE END.");
		else 
			showMessage(L"YY和XX同学顺利地从塔中\n逃生，从此过上了快乐幸福的\n生活。\n\nHAPPY END.");
		break;
	case consts.MESSAGE_LOSE:
		showMessage(L"塔倒了，YY和XX同学最终\n还是没有冲出来；但是直到最\n后他们的手还是拉在一起的，\n紧紧也没分开。\n\nNORMAL END.");
		break;
	case consts.MESSAGE_HINT:
		showMessage(consts.hint.at(consts.nowcnt).c_str());
		break;
	case consts.MESSAGE_SAVE:
		{
			wchar_t ss[200];
			if (consts.sd[consts.wanttosave].hp<0)
				wsprintf(ss,L"存档到文件 %d\n(空白)\n\n[↑] [↓] 更改存档位置\n[ENTER] 确认存档\n[ESC] 取消", consts.wanttosave+1);
			else
				if (consts.sd[consts.wanttosave].now_floor<30)
					wsprintf(ss,L"存档到文件 %d\n(F%d/HP%d/A%d/D%d)\n\n[↑] [↓] 更改存档位置\n[ENTER] 确认存档\n[ESC] 取消",
						consts.wanttosave+1, consts.sd[consts.wanttosave].now_floor, consts.sd[consts.wanttosave].hp,
						consts.sd[consts.wanttosave].atk, consts.sd[consts.wanttosave].def);
				else
					wsprintf(ss,L"存档到文件 %d\n(F???/HP%d/A%d/D%d)\n\n[↑] [↓] 更改存档位置\n[ENTER] 确认存档\n[ESC] 取消",
					consts.wanttosave+1, consts.sd[consts.wanttosave].hp,
					consts.sd[consts.wanttosave].atk, consts.sd[consts.wanttosave].def);
			showMessage(ss);
			break;
		}
	case consts.MESSAGE_LOAD:
		{
			wchar_t ss[200];
			if (consts.sd[consts.wanttosave].hp<0)
				wsprintf(ss,L"读取存档 %d\n(无效的存档)\n\n[↑] [↓] 更改读档位置\n[ENTER] 确认读档\n[ESC] 取消", consts.wanttosave+1);
			else
				if (consts.sd[consts.wanttosave].now_floor<30)
					wsprintf(ss,L"读取存档 %d\n(F%d/HP%d/A%d/D%d)\n\n[↑] [↓] 更改读档位置\n[ENTER] 确认读档\n[ESC] 取消",
						consts.wanttosave+1, consts.sd[consts.wanttosave].now_floor, consts.sd[consts.wanttosave].hp,
						consts.sd[consts.wanttosave].atk, consts.sd[consts.wanttosave].def);
				else
					wsprintf(ss,L"读取存档 %d\n(F???/HP%d/A%d/D%d)\n\n[↑] [↓] 更改读档位置\n[ENTER] 确认读档\n[ESC] 取消",
						consts.wanttosave+1, consts.sd[consts.wanttosave].hp,
						consts.sd[consts.wanttosave].atk, consts.sd[consts.wanttosave].def);
			showMessage(ss);
			break;
		}
	case consts.MESSAGE_FLYING:
		{
			wchar_t ss[100];
			wsprintf(ss,L"我要飞往 %d 楼\n[↑] [↓] 更改楼号\n[ENTER] 确认飞行\n[ESC] 取消",hero.getFlyFloor());
			showMessage(ss);
			break;
		}
	default:
		break;
	}
	if (consts.msg==consts.MESSAGE_NPC) {
		int npcid=map_floor[hero.getNowFloor()].getinfo(hero.nextY(),hero.nextX())->getNpcID();
		int npctime=map_floor[hero.getNowFloor()].getinfo(hero.nextY(),hero.nextX())->getNpcVisit();
		if (npcid==45)
			showMessage(L"你想用经验提升你的能力吗？\n[1] 提升一级 （100经验）\n[2] 攻击+5 （30经验）\n[3] 防御+5 （30经验）\n[ENTER] 退出");
		if (npcid==46)
			showMessage(L"你想购买钥匙吗？\n[1] 黄钥匙 （10金币）\n[2] 蓝钥匙 （50金币）\n[3] 红钥匙 （100金币）\n[ENTER] 退出");
		if (npcid==47)
			showMessage(L"你可以用25金币：\n[1] 生命+400\n[2] 攻击+4\n[3] 防御+4\n[ENTER] 退出");
		if (npcid==48)
			showMessage(L"你可以用100金币：\n[1] 生命+2000\n[2] 攻击+20\n[3] 防御+20\n[ENTER] 退出");
		if (npcid==49)
			showMessage(L"你想用经验提升你的能力吗？\n[1] 提升三级 （270经验）\n[2] 攻击+17 （95经验）\n[3] 防御+17 （95经验）\n[ENTER] 退出");
		if (npcid==50)
			showMessage(L"你有多余的钥匙要出售吗？\n[1] 黄钥匙 （7金币）\n[2] 蓝钥匙 （35金币）\n[3] 红钥匙 （70金币）\n[ENTER] 退出");
		if (npcid==53 && npctime==0)
			showMessage(L"500点经验换取120点攻击力，\n换吗？\n[1] 换\n[ENTER] 不要");
		if (npcid==54 && npctime==0)
			showMessage(L"500点金币换取120点防御力，\n换吗？\n[1] 换\n[ENTER] 不要");
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
	consts.hge->System_SetState(HGE_TITLE,"魔塔");
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
