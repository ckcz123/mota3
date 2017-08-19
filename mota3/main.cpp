#include "stdafx.h"

// ��ͼ��Ϣ
c_map_floor map_floor[50];

// ��ʿ��Ϣ
c_hero hero;

// ������Ϣ
constants consts;


// encode & decode file
bool fileConvert(char* from, char* to, bool encode=true) {
	char* pwd = "f32&*9h=)";

	// ����ļ����ڣ���ɾ��
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

	/*����/�����㷨��ʼ*/
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
	consts.setMsg(L"�浵�ɹ���");
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
	consts.setMsg(L"�����ɹ���");
}
void showMessage(const wchar_t *s, const wchar_t *_name=NULL) // ��ʾ��ʾ
{
	int l=consts.lines, 
		left=16+consts.ScreenLeft, right=consts.map_width*32+consts.ScreenLeft-16,
		top=consts.map_height*32-12-25*l, bottom=consts.map_height*32-8;
	hgeSprite *s_temp;
	s_temp=new hgeSprite(consts.ht_skin, 0, 0, 128, 128);
	s_temp->SetColor(0xCCFFFFFF);
	s_temp->RenderStretch(left, top, right, bottom);
	GfxFont *f=new GfxFont(L"����", 22);
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
	// ��ʼ��
	consts.init();
	hero.init();
	// �����ͼ
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
			consts.setMsg(L"ֻ����¥�ݱ߲��ܴ浵��");
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
		if (hero.getNowFloor()>=30) consts.setMsg(L"���ﲻ��ʹ�÷�������");
		else if (consts.canfly) { 
			if (hero.getNowFloor()==21) consts.setMsg(L"���������������¥ʧЧ�ˣ�");
			else consts.msg=consts.MESSAGE_FLYING;
		}
		else if (consts.lefttime<80) consts.setMsg(L"������������ʧ�ˣ�");
		else consts.setMsg(L"����Ҫ�Ȼ�÷�������");
	}
	if(consts.hge->Input_GetKeyState(HGEK_M) && consts.isFree()) {
		consts.music=!consts.music;
		consts.setMsg(consts.music?L"�����ѿ���":L"�����ѹر�");
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

	// ��ʾ��Ϣ
	if (consts.msg==consts.MESSAGE_HINT)
	{
		if(consts.hge->Input_GetKeyState(HGEK_ENTER) && clock()-consts.lasttime>200) {
			consts.nowcnt++;
			consts.lasttime=clock();
		}
		if (consts.nowcnt>=consts.hint.size())
			consts.msg=consts.MESSAGE_NONE;
	}

	// �浵
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

	// ����
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
				consts.setMsg(L"��Ч�Ĵ浵��");
			else load(consts.wanttosave);
		}
	}

	// ʤ��orʧ��
	if ((consts.msg==consts.MESSAGE_WIN || consts.msg==consts.MESSAGE_LOSE) && consts.hge->Input_GetKeyState(HGEK_ENTER)) return true;
	// �˳�
	if (consts.msg==consts.MESSAGE_QUIT && consts.hge->Input_GetKeyState(HGEK_SPACE)) return true;
	// ���¿�ʼ
	if (consts.msg==consts.MESSAGE_RESTART && consts.hge->Input_GetKeyState(HGEK_SPACE)) init();

	// ���ڷ���
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
		GfxFont *f=new GfxFont(L"����", 25);
		float len=f->GetTextSize(L"[1] ��ʼ��Ϸ").cx;
		float left=16*consts.map_width+consts.ScreenLeft-len/2;
		float height=consts.map_height*32*0.6;
		f->Print(left, height, L"[1] ��ʼ��Ϸ");
		f->Print(left, height+28, L"[2] ��ȡ��Ϸ");
		f->Print(left, height+56, L"[3] �˳���Ϸ");
		delete f;
		f=new GfxFont(L"����", 40);
		f->SetColor(0xFFFF0000);
		len=f->GetTextSize(L"������").cx;
		f->Print(16*consts.map_width+consts.ScreenLeft-len/2, height*0.4, L"������");
		delete f;
		consts.hge->Gfx_EndScene();
		return false;
	}
	if (consts.msg==consts.MESSAGE_CHOOSE_HARD) {
		GfxFont *f=new GfxFont(L"����", 25);
		float len=f->GetTextSize(L"[1] ���Ѷȣ���").cx;
		float left=16*consts.map_width+consts.ScreenLeft-len/2;
		float height=consts.map_height*32*0.6;
		f->Print(left, height, L"[1] �򵥣�����15%%��");
		f->Print(left, height+28, L"[2] ��ͨ������5%%��");
		f->Print(left, height+56, L"[3] ����");
		delete f;
		consts.hge->Gfx_EndScene();
		return false;
	}
	if (consts.msg==consts.MESSAGE_TEXT) {
		wchar_t* msg[50]={
			L"����Ҷ�۹����ԣ�3358��ԭ��Ӧ�ú�\n����һ��ƽ����",
			L"���ǣ������ڱ߾��ϵ����Ԫ֮��ȴ\n���۹������˴��鷳��\n��ԴԴ���ϵĹ�������г��֣�����\nͼ����۹���������",
			L"���˵��ǣ��������������£��Ҳ���\nǿ���������ױ�նɱ�������Ȼ����\n�������٣�ȴ���ǵ۹�ռ�����Ϸ硣",
			L"ֱ����һ��...",
			L"��һ�죬��һ����ͻȻ�������˷���\n��һ�࣬��˭Ҳ��֪��������ô����\n�ģ��ͺ���ͻȻð���������",
			L"���Դ������ֺ�ս�ֶ�Ȼ�����˱�\n���������Ǿ�Ȼӵ���˲���֮��",
			L"���۵۹�����ʿ����ʲô����նɱ��\n���һתͷȴ�ֻᷢ�ֹ���ö˶�\n�������ֻ���Լ����ϵ�Ѫ������\n֤���ⳡս��ȷʵ��������",
			L"�������ʣ������������ĳ��֣��ŵ�\n�¹�����ӵ���˲���֮���������\n��ˣ��۹�������ǲ�¸ҵ���ʿ����\n�취�ƹ����ߣ�͵�����У�һ��Ҫ��\n�������������ܡ�",
			L"Ȼ������ȥ����ȴ��Ҳû�л�����\n���㣬��Ϊ��127����ʿ���¸ҵؽ�\n����ʹ������취��������������",
			L"�����ǵĹ��£�Ҳ��������ʼ��......"
		};

		GfxFont *f=new GfxFont(L"����", 24);
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

	float mx,my; // ���λ��
	consts.hge->Input_GetMousePos(&mx,&my);
	int x=(mx-consts.ScreenLeft)/32,y=my/32;
	if(consts.battling)consts.monster_battling->printInfo();
	else if (consts.book && x>=0 && x<consts.map_width)map_floor[hero.getNowFloor()].printMonsterInfo(x,y);

	switch (consts.msg)
	{
	case consts.MESSAGE_QUIT:
		showMessage(L"�����˳���Ϸ��\n\n[ENTER] ������Ϸ\n[SPACE] �˳���Ϸ");
		break;
	case consts.MESSAGE_RESTART:
		showMessage(L"�������¿�ʼ��\n\n[ENTER] ������Ϸ\n[SPACE] ���¿�ʼ");
		break;;
	case consts.MESSAGE_WIN:
		if (consts.trueend) 
			showMessage(L"�����ӵ�ף���£�YY��XX\nͬѧ˳���ش������������Ӵ�\n�����������మ���ȹ����Ҹ�\n��һ����\n\nTRUE END.");
		else 
			showMessage(L"YY��XXͬѧ˳���ش�����\n�������Ӵ˹����˿����Ҹ���\n���\n\nHAPPY END.");
		break;
	case consts.MESSAGE_LOSE:
		showMessage(L"�����ˣ�YY��XXͬѧ����\n����û�г����������ֱ����\n�����ǵ��ֻ�������һ��ģ�\n����Ҳû�ֿ���\n\nNORMAL END.");
		break;
	case consts.MESSAGE_HINT:
		showMessage(consts.hint.at(consts.nowcnt).c_str());
		break;
	case consts.MESSAGE_SAVE:
		{
			wchar_t ss[200];
			if (consts.sd[consts.wanttosave].hp<0)
				wsprintf(ss,L"�浵���ļ� %d\n(�հ�)\n\n[��] [��] ���Ĵ浵λ��\n[ENTER] ȷ�ϴ浵\n[ESC] ȡ��", consts.wanttosave+1);
			else
				if (consts.sd[consts.wanttosave].now_floor<30)
					wsprintf(ss,L"�浵���ļ� %d\n(F%d/HP%d/A%d/D%d)\n\n[��] [��] ���Ĵ浵λ��\n[ENTER] ȷ�ϴ浵\n[ESC] ȡ��",
						consts.wanttosave+1, consts.sd[consts.wanttosave].now_floor, consts.sd[consts.wanttosave].hp,
						consts.sd[consts.wanttosave].atk, consts.sd[consts.wanttosave].def);
				else
					wsprintf(ss,L"�浵���ļ� %d\n(F???/HP%d/A%d/D%d)\n\n[��] [��] ���Ĵ浵λ��\n[ENTER] ȷ�ϴ浵\n[ESC] ȡ��",
					consts.wanttosave+1, consts.sd[consts.wanttosave].hp,
					consts.sd[consts.wanttosave].atk, consts.sd[consts.wanttosave].def);
			showMessage(ss);
			break;
		}
	case consts.MESSAGE_LOAD:
		{
			wchar_t ss[200];
			if (consts.sd[consts.wanttosave].hp<0)
				wsprintf(ss,L"��ȡ�浵 %d\n(��Ч�Ĵ浵)\n\n[��] [��] ���Ķ���λ��\n[ENTER] ȷ�϶���\n[ESC] ȡ��", consts.wanttosave+1);
			else
				if (consts.sd[consts.wanttosave].now_floor<30)
					wsprintf(ss,L"��ȡ�浵 %d\n(F%d/HP%d/A%d/D%d)\n\n[��] [��] ���Ķ���λ��\n[ENTER] ȷ�϶���\n[ESC] ȡ��",
						consts.wanttosave+1, consts.sd[consts.wanttosave].now_floor, consts.sd[consts.wanttosave].hp,
						consts.sd[consts.wanttosave].atk, consts.sd[consts.wanttosave].def);
				else
					wsprintf(ss,L"��ȡ�浵 %d\n(F???/HP%d/A%d/D%d)\n\n[��] [��] ���Ķ���λ��\n[ENTER] ȷ�϶���\n[ESC] ȡ��",
						consts.wanttosave+1, consts.sd[consts.wanttosave].hp,
						consts.sd[consts.wanttosave].atk, consts.sd[consts.wanttosave].def);
			showMessage(ss);
			break;
		}
	case consts.MESSAGE_FLYING:
		{
			wchar_t ss[100];
			wsprintf(ss,L"��Ҫ���� %d ¥\n[��] [��] ����¥��\n[ENTER] ȷ�Ϸ���\n[ESC] ȡ��",hero.getFlyFloor());
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
			showMessage(L"�����þ����������������\n[1] ����һ�� ��100���飩\n[2] ����+5 ��30���飩\n[3] ����+5 ��30���飩\n[ENTER] �˳�");
		if (npcid==46)
			showMessage(L"���빺��Կ����\n[1] ��Կ�� ��10��ң�\n[2] ��Կ�� ��50��ң�\n[3] ��Կ�� ��100��ң�\n[ENTER] �˳�");
		if (npcid==47)
			showMessage(L"�������25��ң�\n[1] ����+400\n[2] ����+4\n[3] ����+4\n[ENTER] �˳�");
		if (npcid==48)
			showMessage(L"�������100��ң�\n[1] ����+2000\n[2] ����+20\n[3] ����+20\n[ENTER] �˳�");
		if (npcid==49)
			showMessage(L"�����þ����������������\n[1] �������� ��270���飩\n[2] ����+17 ��95���飩\n[3] ����+17 ��95���飩\n[ENTER] �˳�");
		if (npcid==50)
			showMessage(L"���ж����Կ��Ҫ������\n[1] ��Կ�� ��7��ң�\n[2] ��Կ�� ��35��ң�\n[3] ��Կ�� ��70��ң�\n[ENTER] �˳�");
		if (npcid==53 && npctime==0)
			showMessage(L"500�㾭�黻ȡ120�㹥������\n����\n[1] ��\n[ENTER] ��Ҫ");
		if (npcid==54 && npctime==0)
			showMessage(L"500���һ�ȡ120���������\n����\n[1] ��\n[ENTER] ��Ҫ");
	}
	consts.hge->Gfx_EndScene();
	return false;
}
int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{
	
	//hge��ʼ��
	consts.hge=hgeCreate(HGE_VERSION);
	consts.hge->System_SetState(HGE_FRAMEFUNC,frameFunc);
	consts.hge->System_SetState(HGE_RENDERFUNC,renderFunc);
	consts.hge->System_SetState(HGE_USESOUND,true);
	consts.hge->System_SetState(HGE_TITLE,"ħ��");
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
