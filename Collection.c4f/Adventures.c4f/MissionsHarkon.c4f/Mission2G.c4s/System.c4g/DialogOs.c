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
  DlgMsg("Hallo, was willst du hier? Soll wieder jemand in diese Grube geworfen werden?.", dlg_npc);
  DlgWait("Os1");
}

func DlgOs1()
{
  DlgMsg("Ehm nein, ich bin Harkon und ich ähm wollte mich hier mal umsehen.", dlg_plr);
  DlgWait("Os2");
}

func DlgOs2()
{
  DlgMsg("Du gehörst also nicht zu den Schurken von Fenring? Könntest du mir dann vielleicht helfen?", dlg_npc);
  DlgWait("Os3");
}

func DlgOs3()
{
  DlgMsg("Naja das mache ich gerne. Was ist den dein Belang?", dlg_plr);
  DlgWait("Os4");
}

func DlgOs4()
{
  DlgMsg("Ich bin tot und muss trotzdem noch hier arbeiten, das ist nicht fair! Sollten Tote nicht endlich mal ruhen dürfen? Befreie meine Seele!", dlg_npc);
  DlgWait("Os6");
}

func DlgOs5()
{
  DlgMsg("Dazu musst du mir nur einen heiligen Stein bringen, er wird mich direkt ins Himmelreich bringen, in die Arme von Sawelô...", dlg_npc);
  DlgWait("Os6");
}

func DlgOs6()
{
  DlgMsg("Oder in die Hölle.", dlg_plr);
  DlgWait("Os7");
}

local g_DlgOsQuest;

func DlgOs7()
{
  DlgMsg("Was hast du gesagt? Hast du schon einen Holigonstein?", dlg_npc);
  DlgWait("Os9");
	iNextDlg = 8;
	SetLocalNDialogs("g_DlgOsQuest", 1);
}

func DlgOs8()
{
  DlgMsg("Hallo Harkon, da bist du ja wieder, hast du schon meinen Holigonstein gefunden?", dlg_npc);
  DlgWait("Os9");
}


func DlgOs9()
{
  DlgMsg("", dlg_plr);
	if(dlg_plr->FindContents(BHLG))
    DlgAddOption(NONE, "Ja, ich habe dir einen mitgebracht.","DlgOs11", 0, 0);
  DlgAddOption(NONE, "Nein, ich habe leider keinen dabei.","DlgOs10", 0, 0);
}

func DlgOs10() { DlgWaitStop(); }

func DlgOs11()
{
	SetLocalNDialogs("g_DlgOsQuest", 0);
	var pHoligon = dlg_plr->FindContents(BHLG);
	pHoligon->RemoveObject();
	DlgMsg("Oh hab vielen Dank edler Retter! Ich werde bei Sawelô ein gutes Wort für dich einlegen.", dlg_npc);
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
  DlgMsg("Jetzt ist er weg der Os. Möge Sawelô ihn aufgenommen haben.", dlg_plr);
  DlgWaitStop();
}