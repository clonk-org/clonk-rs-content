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
  DlgMsg("Seid gegrüßt fremder Schleicher. Mein Name ist Merlin.", dlg_npc);
  DlgWait("Merlin1");
}

func DlgMerlin1()
{
  DlgMsg("Hallo Merlin, mein Name ist Harkon. Ich komme hier nur zufällig vorbei.", dlg_plr);
  DlgWait("Merlin2");
}

func DlgMerlin2()
{
  DlgMsg("Ach erzähl mir nichts Harkon. Du willst bestimmt Fenring besiegen, so wie du hier herumschleichst. Aber ich kann dir helfen.", dlg_npc);
  DlgWait("Merlin3");
}

func DlgMerlin3()
{
  DlgMsg("Ach ja, das wäre natürlich nett...", dlg_plr);
  DlgWait("Merlin4");
}

func DlgMerlin4()
{
  DlgMsg("Nett schon, aber nicht billig. Ich will dafür 10 Clunker.", dlg_npc);
  DlgWait("Merlin6");
	iNextDlg = 5;
}

func DlgMerlin5()
{
  DlgMsg("Hast du das Geld dabei?", dlg_npc);
  DlgWait("Merlin6");
}

func DlgMerlin6()
{
  DlgMsg("", dlg_plr);
	if(GetWealth(GetOwner(dlg_plr))>=10)
    DlgAddOption(NONE, "Ja, ich habe hier das Geld.","DlgMerlin8", 0, 0);
  DlgAddOption(NONE, "Nein, ich benötige deine Hilfe nicht.","DlgMerlin7", 0, 0);
	if(g_DlgOsQuest)
		DlgAddOption(NONE, "Ich suche Holigon für das Skelett Os.","DlgMerlin13", 0, 0);
}

func DlgMerlin7() { DlgWaitStop(); }

func DlgMerlin8()
{
	DoWealth(GetOwner(dlg_plr), -10);
	Sound("Uncash");
	DlgMsg("Wenn du Fenring besiegen willst brauchst du unbedingt einen Kristall um seine Blitze ab zu wehren. Sonst bist du sofort tot.", dlg_npc);
  DlgWait("Merlin9");
}

func DlgMerlin9()
{
  DlgMsg("Außerdem könnte dir vielleicht dieser Zauber hilfreich sein.", dlg_npc);
	var pObj = CreateObject(SCRL);
	pObj->SetSpell(MBOT);
	pObj->Enter(dlg_plr);
  DlgWait("Merlin10");
}

func DlgMerlin10()
{
  DlgMsg("Oh ja, vielen Dank Merlin. Ich werde deinen Rat befolgen.", dlg_plr);
	iNextDlg = 11;
	DlgWaitStop();
}

func DlgMerlin11()
{
	if(g_DlgOsQuest) return DlgMerlin12();
  DlgMsg("Ich wünsche dir noch viel Glück bei deiner Aufgabe.", dlg_npc);
	DlgWaitStop();
}

func DlgMerlin12()
{
	DlgMsg("Ich suche Holigon für das Skelett Os.", dlg_plr);
  DlgWait("Merlin13");
}

func DlgMerlin13()
{
  DlgMsg("Puh da habe ich keines gesehen abgesehen von den Gaben auf dem Altar gibt es hier wohl in dem ganzen Höhlensystem kein Holigon. Ich muss dich da leider enttäuschen.", dlg_npc);
	DlgWaitStop();
}
