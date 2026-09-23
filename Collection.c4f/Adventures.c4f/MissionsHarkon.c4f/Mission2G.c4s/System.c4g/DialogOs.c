/* Dialoge */

#strict 2

#appendto DG69

local iNextDlg;

func DlgOsStart()
{
  if(iNextDlg) return(Call(Format("DlgOs%d",iNextDlg)));
  DlgWait("Os0");
}

func DlgOs0()
{
  DlgMsg("$TxtOs01$", dlg_npc);
  DlgWait("Os1");
}

func DlgOs1()
{
  DlgMsg("$TxtOs02$", dlg_plr);
  DlgWait("Os2");
}

func DlgOs2()
{
  DlgMsg("$TxtOs03$", dlg_npc);
  DlgWait("Os3");
}

func DlgOs3()
{
  DlgMsg("$TxtOs04$", dlg_plr);
  DlgWait("Os4");
}

func DlgOs4()
{
  DlgMsg("$TxtOs05$", dlg_npc);
  DlgWait("Os6");
}

func DlgOs5()
{
  DlgMsg("$TxtOs06$", dlg_npc);
  DlgWait("Os6");
}

func DlgOs6()
{
  DlgMsg("$TxtOs07$", dlg_plr);
  DlgWait("Os7");
}

local g_DlgOsQuest;

func DlgOs7()
{
  DlgMsg("$TxtOs08$", dlg_npc);
  DlgWait("Os9");
	iNextDlg = 8;
	SetLocalNDialogs("g_DlgOsQuest", 1);
}

func DlgOs8()
{
  DlgMsg("$TxtOs09$", dlg_npc);
  DlgWait("Os9");
}


func DlgOs9()
{
  DlgMsg("", dlg_plr);
	if(dlg_plr->FindContents(BHLG))
    DlgAddOption(NONE, "$TxtOs10$","DlgOs11", 0, 0);
  DlgAddOption(NONE, "$TxtOs11$","DlgOs10", 0, 0);
}

func DlgOs10() { DlgWaitStop(); }

func DlgOs11()
{
	SetLocalNDialogs("g_DlgOsQuest", 0);
	var pHoligon = dlg_plr->FindContents(BHLG);
	pHoligon->RemoveObject();
	DlgMsg("$TxtOs12$", dlg_npc);
  DlgWait("Os12");
}

func DlgOs12()
{
  dlg_npc->SetClrModulation(RGBa(255,255,255,255));
  dlg_npc->SetObjectLayer(dlg_npc);
  SetObjectLayer(this);
  FindObject2(Find_ID(WNTK), Find_ActionTarget(dlg_npc))->RemoveObject();
  dlg_npc->CastObjects(BONE,4,10);
	var pGhost = CreateObject(GHST);
	pGhost->Activate(271);
	DlgWait("Os13", DG69_Wait_Menu);
}

func DlgOs13()
{
  DlgMsg("$TxtOs13$", dlg_plr);
  DlgWaitStop();
}