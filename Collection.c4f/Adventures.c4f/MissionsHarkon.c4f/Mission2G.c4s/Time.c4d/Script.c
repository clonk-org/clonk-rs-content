/*-- Story --*/

#strict 2

func Initialize() { FramesLeft = -1; }

protected func MenuQueryCancel() { return 1; }

func Start()
{
	ScheduleCall(this, "DoStart", 1, 1);
}

local pClonk;

func DoStart()
{
  // Starte in einer anderen Section
  SaveObjects();
  LoadScenarioSection("Map", 3);
  // Alles wiederherstellen
  RestoreSectObjs();


  pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));
  pClonk->SetPosition(76, 88);
  pClonk->SetDir(1);
  
  StartFilm();

  PosCam(pClonk, 0, 1);
  g_pCamera->SetPlrViewRange(40);

  FilmFadeIn();
  SetNext("Text1", 1);
}

func Text1()
{
  // Auftrag
  DialogMessage(pClonk, "Hierher hat sich also Fenring zurückgezogen. Ich muss mich wohl durchschlagen, ihn finden und dann besiegen.", "EndIntro", 0, 0, "Dialog1.ogg");
}

func EndIntro()
{
	StopFilm();
	StartSzen();
}

local pMessenger;

func DoStartSzen()
{
	DoPlayMusic(["Cavern of Time.ogg", "Strange Vision.ogg", "Chee Zee Cave.ogg"]);
  SetPosition();
  CallOnCondition("ScriptFight", this, "CheckEnterHall");
//  CallOnPlayerNear("ScriptFight", Object(4365), this, 40);
}

func CheckEnterHall()
{
  for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
  {
    var pPlayer = GetCursor(GetPlayerByIndex(i, C4PT_User));
    if(pPlayer->GetY() > 787 && pPlayer->GetY() < 850 && pPlayer->GetX() < 586 && pPlayer->GetX() > 468)
    {
      pClonk = pPlayer;
      return true;
    }
  }
}

local pClonk;
local pFenring;

func ScriptFight()
{
//  pClonk = pPlayer;
  pFenring = FindObject(_FNG);
  pFenring->SetPortrait("Fenring1", 0, _PRT);
  pFenring->SetColorDw(RGB(61,85,61));
  pFenring->SetName("Fenring");
  StartFilm();

  PosCam(pClonk, 0, 1);
  g_pCamera->SetPlrViewRange(40);
  
  SetCommand(pClonk, "MoveTo", 0, 550, 990);

  CallOnCondition("FightFlingPlayerDown", this, "CheckPlayerScale");
  CallOnCondition("ScriptFight2", this, "CheckTouchGround");
}

func CheckPlayerScale() { if(pClonk->GetY() > 795) return true; }

func FightFlingPlayerDown()
{
  pClonk->SetXDir(0);
  pClonk->SetAction("Tumble");
  pClonk->SetCommand(pClonk, "None");
  pClonk->SetComDir(COMD_Stop);
  pClonk->SetDir(1);
}

func CheckTouchGround() { if( (GetContact(pClonk, -1) & CNAT_Bottom) && pClonk->GetY() > 900) return true; }

func ScriptFight2()
{
  pClonk->UnHide();
  pClonk->SetAction("FlatUp");
  SetNext("ScriptFight2b", 1);
}
  
func ScriptFight2b()
{
  DialogMessage(pFenring, "So, du bist also doch letztendlich zu mir durchgedrungen, elender Meuchler...", "ScriptFight3", 0, 0, "Dialog2.ogg");
}

func ScriptFight3()
{
  DialogMessage(pClonk, "Bei Teiwaz, Mist, das war wohl nicht gerade sehr unauffällig.", "ScriptFight4", 0, 0, "Dialog3.ogg");
}

func ScriptFight4()
{
  DialogMessage(pFenring, "Oh ja, diesen Fehler wirst du nur einmal machen, Abschaum.", "ScriptFight5", 0, 0, "Dialog4.ogg");
}

func ScriptFight5()
{
  pFenring->LocalN("pEnemy") = pClonk;
  pFenring->CreateObject(MLGT)->Activate(pFenring);
  pFenring->AI_DoMagic(MCLT, 0, 0, pClonk);
  pFenring->SetAction("Magic");
  if(FindContents(CRYS, pClonk)) g_lightningCommand = "ScriptFight6";
  else
  {
    g_lightningCommand = "ScriptFightDead";
  }
}

func ScriptFightDead()
{
  StopFilm();
  pClonk->SetAction("Dead");
  for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
    GetCursor(GetPlayerByIndex(i, C4PT_User))->OpenRelauchMenu("Fenring hat Harkon getötet.");
}

func ScriptFight6()
{
  pClonk->CastObjects(SPRK, 10, 20);
  pClonk->PoffClonk();
  for(var i = 0; i < 5; i++)
    pFenring->CreateParticle("MSpark", RandomX(-5,5), RandomX(-15,0), 0, 0, 1000, RGB(255,255,200));
  for(var pPlayer in FindObjects(Find_ID(ASAS)))
  {
    pPlayer->SetPosition(694, 900);
    while(Contents(0, pPlayer)) RemoveObject(Contents(0, pPlayer));
  }
  SetNext("ScriptFight7", 1);
}

func ScriptFight7()
{
  DialogMessage(pFenring, "Verdammt, wo ist der Mistkerl hin? Ich bin geblendet.", "ScriptFight8", 0, 0, "Dialog5.ogg");
}

func ScriptFight8()
{
  StopFilm();
  FindObject(_FNG)->StartFight();
  CallOnDeath("ScriptFightWin", pFenring, this);
	DoPlayMusic(["Megalo Spirit Village.ogg", "No one Escape.ogg"]);
}

func ScriptFightWin()
{
  Message("", pFenring);
  StartFilm();
  PosCam(pFenring, 0, 1);
  g_pCamera->SetPlrViewRange(40);
  DialogMessage(pFenring, "Args, ich kann nicht mehr. Du hast mich besiegt...", "ScriptFightWin2", 0, 0, "Dialog6.ogg");
}

func ScriptFightWin2()
{
  DialogMessage(pClonk, "Ich werde dich vor den großen Cra bringen. Er wird entscheiden, was mit einem Verräter wie dir passiert!", "ScriptFightWin3", 0, 0, "Dialog7.ogg");
}

func ScriptFightWin3()
{
  FilmFadeOut();
  SetNext("ScriptFightWin4", 4);
}

func ScriptFightWin4()
{
  FinishMission();
}

local NextFunc;
local FramesLeft;

func Timer()
{
	if(!FramesLeft)
	{
		FramesLeft = -1;
		Call(NextFunc);
	}
	if(FramesLeft > 0) FramesLeft--;
}

func SetNext(next, frames) { NextFunc = next; FramesLeft = frames; }