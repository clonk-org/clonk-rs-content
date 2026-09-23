/* Dialoge */

#strict 2

#appendto DG69

local iNextDlg;

func DlgMerlinStart()
{
  if(iNextDlg) return(Call(Format("DlgMerlin%d",iNextDlg)));
  DlgWait("Merlin0");
}

func DlgMerlin0()
{
  DlgMsg("$TxtMerlin01$", dlg_npc);
  DlgWait("Merlin1");
}

func DlgMerlin1()
{
  DlgMsg("$TxtMerlin02$", dlg_plr);
  DlgWait("Merlin2");
}

func DlgMerlin2()
{
  DlgMsg("$TxtMerlin03$", dlg_npc);
  DlgWait("Merlin3");
}

func DlgMerlin3()
{
  DlgMsg("$TxtMerlin04$", dlg_plr);
  DlgWait("Merlin4");
}

func DlgMerlin4()
{
  DlgMsg("$TxtMerlin05$", dlg_npc);
  DlgWait("Merlin6");
	iNextDlg = 5;
}

func DlgMerlin5()
{
  DlgMsg("$TxtMerlin06$", dlg_npc);
  DlgWait("Merlin6");
}

func DlgMerlin6()
{
  DlgMsg("", dlg_plr);
	if(GetWealth(GetOwner(dlg_plr))>=10)
    DlgAddOption(NONE, "$TxtMerlin07$","DlgMerlin8", 0, 0);
  DlgAddOption(NONE, "$TxtMerlin08$","DlgMerlin7", 0, 0);
	if(g_DlgOsQuest)
		DlgAddOption(NONE, "$TxtMerlin09$","DlgMerlin13", 0, 0);
}

func DlgMerlin7() { DlgWaitStop(); }

func DlgMerlin8()
{
	DoWealth(GetOwner(dlg_plr), -10);
	Sound("Uncash");
	DlgMsg("$TxtMerlin10$", dlg_npc);
  DlgWait("Merlin9");
}

func DlgMerlin9()
{
  DlgMsg("$TxtMerlin11$", dlg_npc);
	var pObj = CreateObject(SCRL);
	pObj->SetSpell(MBOT);
	pObj->Enter(dlg_plr);
  DlgWait("Merlin10");
}

func DlgMerlin10()
{
  DlgMsg("$TxtMerlin12$", dlg_plr);
	iNextDlg = 11;
	DlgWaitStop();
}

func DlgMerlin11()
{
	if(g_DlgOsQuest) return DlgMerlin12();
  DlgMsg("$TxtMerlin13$", dlg_npc);
	DlgWaitStop();
}

func DlgMerlin12()
{
	DlgMsg("$TxtMerlin09$", dlg_plr);
  DlgWait("Merlin13");
}

func DlgMerlin13()
{
  DlgMsg("$TxtMerlin14$", dlg_npc);
	DlgWaitStop();
}
