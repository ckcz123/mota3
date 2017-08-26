#include "stdafx.h"

// ��ͼ��Ϣ
c_map_floor map_floor[50];

// ��ʿ��Ϣ
c_hero hero;

// ������Ϣ
constants consts;

// HTTP
Http http;


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
	for (int i=0;i<100;i++) {
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
	fclose(loadfile);
	consts.setMsg(L"�����ɹ���");
}
void showMessage(const wchar_t *_s) // ��ʾ��ʾ
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
	hgeSprite *s_temp;
	s_temp=new hgeSprite(consts.ht_skin, 0, 0, 128, 128);
	s_temp->SetColor(0xCCFFFFFF);
	s_temp->RenderStretch(left, top, right, bottom);
	GfxFont *f=new GfxFont(L"����", 22);

	// get name
	wchar_t* pos=s;
	while (*pos!=L'\t' && *pos!=L'\0') *pos++;
	if (*pos==L'\t') {
		*pos=L'\0';
		s_temp->RenderStretch(left, top-40, left+25+f->GetTextSize(s).cx, top-2);
		f->Print(left+12, top-33, L"%s", s);
		pos++;
	}
	else pos=s;

	f->Print(left+12, top+12, L"%s", pos);

	delete f;
	delete s_temp;
}
void showMax(const wchar_t *s)
{
	hgeSprite *s_temp;
	s_temp=new hgeSprite(consts.ht_skin, 0, 0, 128, 128);
	s_temp->SetColor(0xBBFFFFFF);
	s_temp->RenderStretch(16+consts.ScreenLeft, consts.map_height*32-400, consts.map_width*32+consts.ScreenLeft-16, consts.map_height*32-8);
	GfxFont *f=new GfxFont(L"����", 22);
	f->Print(16+consts.ScreenLeft+8, consts.map_height*32-400+8, L"%s", s);
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
	if(consts.hge->Input_GetKeyState(HGEK_Q) && consts.isFree()) consts.msg=consts.MESSAGE_RESTART;
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
	if (consts.isFree() && consts.hge->Input_GetKeyState(HGEK_P)) consts.getRank();
	if(consts.hge->Input_GetKeyState(HGEK_G) && consts.isFree()) {
		if (consts.canfly) { 
			if (!hero.nearStair()) consts.setMsg(L"ֻ����¥�ݱ߲���ʹ����¥����");
			else consts.msg=consts.MESSAGE_FLYING;
		}
	}
	if(consts.hge->Input_GetKeyState(HGEK_M) && consts.isFree()) {
		consts.music=!consts.music;
		consts.setMsg(consts.music?L"�����ѿ���":L"�����ѹر�");
		if (consts.music) consts.hge->Channel_SetVolume(consts.hc_Music, consts.bgmvolume);
		else consts.hge->Channel_SetVolume(consts.hc_Music, 0);
	}

	// ʹ��ħ��
	if (consts.isFree() && consts.hge->Input_GetKeyState(HGEK_X) && consts.wand>0) {
		c_map_point* point=map_floor[hero.getNowFloor()].getinfo(hero.getY(), hero.getX());
		if (point->isGround()) {
			point->setSpecial(209);
			consts.wand--;
		}
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
	}
	if (consts.msg==consts.MESSAGE_TEXT) {
		if(consts.hge->Input_GetKeyState(HGEK_ENTER) && clock()-consts.lasttime>200) {
			consts.nowcnt++;
			consts.lasttime=clock();
		}
		if (consts.nowcnt==10) {
			consts.setMsg(L"��ʿ\t���������ˣ����ȥ!");
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
		if (consts.nowcnt>=consts.hint.size()) {
			consts.finishHint();
		}
	}

	// �浵
	if(consts.msg==consts.MESSAGE_SAVE)
	{
		if(consts.hge->Input_GetKeyState(HGEK_DOWN) && clock()-consts.lasttime>200) {
			consts.wanttosave++;
			if (consts.wanttosave>=100) consts.wanttosave=99;
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
			if (consts.wanttosave>=100) consts.wanttosave=9;
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

	// ʤ��
	if (consts.msg==consts.MESSAGE_WIN && consts.hge->Input_GetKeyState(HGEK_ENTER)) init();
	// ���¿�ʼ
	if (consts.msg==consts.MESSAGE_RESTART) {
		if (consts.hge->Input_GetKeyState(HGEK_ENTER))
			init();
		else if (consts.hge->Input_GetKeyState(HGEK_ESCAPE))
			consts.msg=consts.MESSAGE_NONE;
	}

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
		c_map_npc* map_npc=map_floor[hero.getNowFloor()].getinfo(hero.nextY(),hero.nextX())->getNpc();
		int npcid=map_npc->getId(), npctimes=map_npc->getVisit();
		
		// �̵�
		if (npcid==47) {
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
		else if (npcid==48) {
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

	if ((consts.msg==consts.MESSAGE_FLYING || consts.msg==consts.MESSAGE_NPC || consts.msg==consts.MESSAGE_RANK) && consts.hge->Input_GetKeyState(HGEK_ENTER))
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
		float len=f->GetTextSize(L"[2] ��ͨ����һ�ƣ�����5%%��").cx;
		float left=16*consts.map_width+consts.ScreenLeft-len/2;
		float height=consts.map_height*32*0.6;
		f->Print(left, height, L"[1] �򵥣���һ��һ��������15%%��");
		f->Print(left, height+28, L"[2] ��ͨ����һ�ƣ�����5%%��");
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
			L"���Դ������ֺ�ս�ֶ�Ȼ�����˱�\n���������Ǿ�Ȼӵ���˲���֮����",
			L"���۵۹�����ʿ����ʲô����նɱ��\n���һתͷȴ�ֻᷢ�ֹ���ö˶�\n�������ֻ���Լ����ϵ�Ѫ������\n֤���ⳡս��ȷʵ��������",
			L"�������ʣ������������ĳ��֣��ŵ�\n�¹�����ӵ���˲���֮����������\n��ˣ��۹�������ǲ�¸ҵ���ʿ����\n�취�ƹ����ߣ�͵�����У�һ��Ҫ��\n�������������ܡ�",
			L"Ȼ������ȥ����ȴ��Ҳû�л�����\n���㣬��Ϊ��1127����ʿ���¸ҵؽ�\n����ʹ������취��������������",
			L"���ǵĹ��£�Ҳ��������ʼ��......"
		};

		GfxFont *f=new GfxFont(L"����", 24);
		f->SetKerningHeight(8);
		f->Render(consts.ScreenLeft+8, 32*consts.map_height*0.3, msg[consts.nowcnt]);
		delete f;

		f=new GfxFont(L"����", 24, true);
		if (consts.hard==1) {
			f->SetColor(0xFF00FF00);
			f->Print(64, 356, L"���Ѷ�");
		}
		else if (consts.hard==2) {
			f->SetColor(0xFF96CDCD);
			f->Print(64, 356, L"��ͨ�Ѷ�");
		}
		else if (consts.hard==3) {
			f->SetColor(0xFFFF0000);
			f->Print(64, 356, L"�����Ѷ�");
		}
		delete f;
		consts.hge->Gfx_EndScene();

		return false;
	}
	if (consts.msg==consts.MESSAGE_LOAD && consts.hard==0) {
		wchar_t ss[200];
		savedata* sd=&consts.sd[consts.wanttosave];
		if (sd->hp<0)
			wsprintf(ss,L"��ȡ�浵 %d\n(��Ч�Ĵ浵)\n\n[��] [��] ���Ķ���λ��\n[ENTER] ȷ�϶���\n[ESC] ȡ��", consts.wanttosave+1);
		else
			wsprintf(ss,L"��ȡ�浵 %d\n(%s/F%d/HP%d/A%d/D%d)\n\n[��] [��] ���Ķ���λ��\n[ENTER] ȷ�϶���\n[ESC] ȡ��",
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

	float mx,my; // ���λ��
	consts.hge->Input_GetMousePos(&mx,&my);
	int x=(mx-consts.ScreenLeft)/32,y=my/32;
	if (consts.book && x>=0 && x<consts.map_width)map_floor[hero.getNowFloor()].printMonsterInfo(x,y);

	switch (consts.msg)
	{
	case consts.MESSAGE_RESTART:
		showMessage(L"���뷵����������\n\n[ENTER] ȷ��\n[ESC] ȡ��");
		break;
	case consts.MESSAGE_WIN:
		{
			wchar_t ss[200];
			wsprintf(ss, L"��ϲͨ�أ����ķ����� %d��\n", hero.getHP());

			if (consts.hard!=3) {
				wcscat_s(ss, L"�򵥻���ͨ�ѶȽ�����¼���ĳɼ���\n");
			}
			// uploading...
			else if (consts.currentmax==0) {
				wcscat_s(ss, L"�����ϴ��ɼ�... ���Ժ�\n");
			}
			// error
			else if (consts.currentmax<0) {
				wcscat_s(ss, L"�ɼ��ϴ�ʧ�ܣ������������á�\n");
			}
			else {
				wchar_t tmp[200];
				wsprintf(tmp, L"��ǰ����%s����ǰMAX %d��\n", consts.rank, consts.currentmax);
				wcscat_s(ss, tmp);
			}
			wcscat_s(ss, L"��P���ɲ鿴��ǰMAX��¼��Ϣ����\n��ӭ��ͼ���������½�����ҫ��\n\n[ENTER] ���¿�ʼ");
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
				wsprintf(ss,L"�浵���ļ� %d\n(�հ�)\n\n[��] [��] ���Ĵ浵λ��\n[ENTER] ȷ�ϴ浵\n[ESC] ȡ��", consts.wanttosave+1);
			else
				wsprintf(ss,L"�浵���ļ� %d\n(%s/F%d/HP%d/A%d/D%d)\n\n[��] [��] ���Ĵ浵λ��\n[ENTER] ȷ�ϴ浵\n[ESC] ȡ��",
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
				wsprintf(ss,L"��ȡ�浵 %d\n(��Ч�Ĵ浵)\n\n[��] [��] ���Ķ���λ��\n[ENTER] ȷ�϶���\n[ESC] ȡ��", consts.wanttosave+1);
			else
				wsprintf(ss,L"��ȡ�浵 %d\n(%s/F%d/HP%d/A%d/D%d)\n\n[��] [��] ���Ķ���λ��\n[ENTER] ȷ�϶���\n[ESC] ȡ��",
						consts.wanttosave+1, consts.getHardText(sd->hard), sd->now_floor, sd->hp,
						sd->atk, sd->def);
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
	case consts.MESSAGE_RANK:
		{
			wchar_t ss[1200];
			if (consts.currentmax<0) {
				wsprintf(ss, L"��ȡMAX��¼ʧ�ܣ������������ӣ�\n���������½��з�����\n\n[ENTER] ȷ��");
				showMessage(ss);
			}
			else if (consts.currentmax==0) {
				wsprintf(ss, L"������ȡMAX��¼�����Ժ�...\n�������Ҫ�����ӡ�\n\n[ENTER] ȡ��");
				showMessage(ss);
			}
			else {
				wcscpy_s(ss, L"");

				for (int i=0; i<10; i++) {
					wchar_t tmp[100];
					wsprintf(tmp, L"TOP%2d: %-8d", i+1, consts.rd[i].hp);
					wcscat_s(ss, tmp);
					if (consts.rd[i].hp>0) {
						wsprintf(tmp, L"  (%s %s)", consts.rd[i].t1, consts.rd[i].t2);
						wcscat_s(ss, tmp);
					}
					if (i<=2) {
						wsprintf(tmp, L"\n[%d, %d, %d, %d, %d, %d]",
							consts.rd[i].hp, consts.rd[i].atk, consts.rd[i].def, consts.rd[i].money, consts.rd[i].yellow, consts.rd[i].blue);
						wcscat_s(ss, tmp);
					}
					wcscat_s(ss, L"\n");
				}

				wcscat_s(ss, L"\n[ENTER] ȷ��");
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

		// 6¥�̵�
		if (id==47) {
			int need=25+2*times;
			wchar_t s[200];
			wsprintf(s, L"̰��֮��\t�¸ҵ���ʿ��������%d��ҾͿ��ԣ�\n\n[1] ����+400\n[2] ����+2\n[3] ����+3\n[ESC] �뿪", need);
			showMessage(s);
		}
		// 7¥����
		if (id==48) {
			if (times>0) {
				showMessage(L"�ǻ�֮Ӱ\t100���һ�Σ�������ħ��ʹ�ô�����\n\n[ENTER] ��Ҫ\n[ESC] �뿪");
			}
		}


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
	consts.hge->System_SetState(HGE_TITLE,"������ By Sky_��յ���");
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