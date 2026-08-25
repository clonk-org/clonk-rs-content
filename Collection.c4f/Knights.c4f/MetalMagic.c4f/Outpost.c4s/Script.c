/*-- Outpost --*/

#strict

static g_MaxKills;

static aDeaths;
static aKills;

static g_No_Relaunch_Removal;

func Initialize() {
  // Truhen füllen
  for(var chest in FindObjects(Find_ID(CHST)))
  {
    while(Contents(0,chest)) RemoveObject(Contents(0,chest));
    FillChest(chest);
  }

  // Aufzüge anpassen
  FindObject(ELEV)->CreateShaft(140);

  // Arrays initialisieren;
  aDeaths = [];
  aKills = [];
  g_MaxKills = 5;

  // Scorboard
  SetScoreboardData(SBRD_Caption, SBRD_Caption, "$Score$");
  SetScoreboardData(SBRD_Caption, ScoreboardCol(KILL), Format("%d{{SWOR}}", g_MaxKills), ScoreboardCol(KILL));
  SetScoreboardData(SBRD_Caption, ScoreboardCol(LIFE), "{{SKUL}}", ScoreboardCol(LIFE));

  CreateObject(RCHS, 10, 10, -1);

  CreateObject(MELE, 10, 10, -1);
}

protected func InitElevator(pElev, iLength)
{
  var pElevNew = CreateObject(ELEV, GetX(pElev), GetY(pElev)+28,-1);
  RemoveObject(pElev);
  pElevNew->CreateShaft(iLength);
  var pCase = LocalN("pCase", pElevNew);
  DigFreeRect(GetX(pCase) - 12, GetY(pCase) - 13, 24, 26);
}

protected func FillChest(object pChest)
{
 var count=4+Random(6);
 while(count--)
 CreateContents(RandChestID(),pChest);
}

protected func RandChestID()
{
 var rand=Random(11);
 if(!rand) return(BRED);
 if(!--rand) return(CRBW);
 if(!--rand) return(BOLT);
 if(!--rand) return(NBLT);
 if(!--rand) return(HOOK);
 if(!--rand) return(STFN);
 if(!--rand) return(EFLN);
 if(!--rand) return(METL);
 if(!--rand) return(TENP);
 if(!--rand) return(TKNF);
 return(COKI);
}

protected func InitializePlayer(int iPlr, int tx, int ty, object pBase, int iTeam)
{
  // Scoreboard-Überschrift initialisieren
  var iPlrID = GetPlayerID(iPlr);
  SetScoreboardData(iPlrID, SBRD_Caption, GetTaggedPlayerName(iPlr), GetPlayerID(iPlr));
  aDeaths[iPlr] = 0;
  aKills[iPlr] = 0;
  SetScoreboardData(GetPlayerID(iPlr), ScoreboardCol(LIFE), Format("%d", aDeaths[iPlr]), aDeaths[iPlr]);
  SetScoreboardData(GetPlayerID(iPlr), ScoreboardCol(KILL), Format("%d", aKills[iPlr]), aKills[iPlr]);
  
  // Spieler joinen
  JoinPlayer(iPlr);
  return(1);
}

protected func RelaunchPlayer(int iPlr, int iDeathCausedBy)
{
  if(g_No_Relaunch_Removal) return();
  // Tod mehr
  aDeaths[iPlr]++;
  SetScoreboardData(GetPlayerID(iPlr), ScoreboardCol(LIFE), Format("%d", aDeaths[iPlr]), aDeaths[iPlr]);
  // Selbstmord? Kill abziehen... böse, böse
  if(iPlr == iDeathCausedBy || iDeathCausedBy==-1)
  {
    aKills[iPlr]--;
    SetScoreboardData(GetPlayerID(iPlr), ScoreboardCol(KILL), Format("%d", aKills[iPlr]), aKills[iPlr]);
  }
  // Belohnung für Kill
  else if(iDeathCausedBy>=0)
  {
    aKills[iDeathCausedBy]++;
    SetScoreboardData(GetPlayerID(iDeathCausedBy), ScoreboardCol(KILL), Format("%d", aKills[iDeathCausedBy]), aKills[iDeathCausedBy]);
    if(aKills[iDeathCausedBy]>=g_MaxKills)
    {
      for(var i = 0; i < GetPlayerCount(); i++)
        if(GetPlayerByIndex(i)!=iDeathCausedBy) EliminatePlayer(GetPlayerByIndex(i));
      DoScoreboardShow(1, 0);
      return;
    }
  }

  // Sortieren nach Frags
  SortScoreboard(ScoreboardCol(LIFE), false);
  SortScoreboard(ScoreboardCol(KILL), true);
  DoScoreboardShow(1, 0);
  Schedule("DoScoreboardShow(-1, 0)", 38*3);
 
  var crew = CreateObject(ASAS,10,10,iPlr);
  MakeCrewMember(crew,iPlr); 
  JoinPlayer(iPlr);
}

protected func RemovePlayer(iPlr)
{
  // Relaunchzähler im Scoreboard ausleeren
  var iPlrID = GetPlayerID(iPlr);
  SetScoreboardData(iPlrID,       ScoreboardCol(KILL),       0, -1);
  SetScoreboardData(iPlrID,       ScoreboardCol(LIFE),       0, -1);
  SortScoreboard(ScoreboardCol(KILL), true);
}

protected func JoinPlayer(iPlr)
{
  var x = -1, y = -1;
  GetSpawnPosition(x,y);
  if(x==-1) { x = 100; y = 400;}
 
  var crew = GetCrew(iPlr);
  var help = CreateObject(DSHP,x,y,iPlr);
  SetYDir(0,help);
  SetPosition(x,y,crew);
  Enter(help,crew);
  AddEffect("Relaunch",help,20,4,0,0);
 
  // Auswahl der Waffen
  Schedule(Format("OpenWeaponMenu(Object(%d))", ObjectNumber(crew)), 10);
 
  DoEnergy(100,crew);
 
  SelectCrew(iPlr,crew,1);
}

global func GiveWeapons(idID, pClonk)
{
  if(idID==CRBW)
  {
    CreateContents(CRBW,pClonk);
    CreateContents(BOLT,pClonk);
    CreateContents(BOLT,pClonk);
  }
  if(idID==HOOK)
  {
    CreateContents(CRBW,pClonk);
    CreateContents(HOOK,pClonk);
  }
  if(idID==KNFP)
  {
    CreateContents(TKNF,pClonk);
    CreateContents(TKNF,pClonk);
  }
}

protected func GetSpawnPosition(&x,&y)
{
 for(var cnt=0;cnt<10000 && x==-1;cnt++)
 {
  var pWipf=PlaceAnimal(WIPF);
  if(GetY(pWipf)>500) { RemoveObject(pWipf); continue; }
  if(GBackSolid(GetX(pWipf)-10,GetY(pWipf)-5)
  || GBackSolid(GetX(pWipf)+10,GetY(pWipf)-5)
  || GBackSolid(GetX(pWipf),GetY(pWipf)-30)
  || GBackSolid(GetX(pWipf),GetY(pWipf)-5)) { RemoveObject(pWipf); continue; }
  
  if(FindObject2(Find_OCF(OCF_CrewMember()), Find_NoContainer(), Find_Distance(100, GetX(pWipf), GetY(pWipf)))) { RemoveObject(pWipf); continue; }
  
  x=GetX(pWipf);
  y=GetY(pWipf);
  
  RemoveObject(pWipf);
  break;
 }
}

global func FxRelaunchStart(pTarget)
{
 SetVisibility(VIS_Owner()|VIS_God(),pTarget);
 return(1);
}

global func FxRelaunchTimer(pTarget,iEffectNumber,iEffectTime)
{
 if(iEffectTime>90)return(-1);
 
 var angle=Random(360);
 var rand=Random(20);
 var cos=Cos(angle,rand);
 var sin=Sin(angle,rand);
 
 CreateParticle("PxSpark",GetX(pTarget)+cos,GetY(pTarget)+sin,-cos,-sin-10,50,RGB(10+iEffectTime,10+iEffectTime,10+iEffectTime),pTarget);
 
 return(1);
}

global func FxRelaunchStop(pTarget)
{
  var pClonk = FindObject(ASAS, 0, 0, 0, 0, 0, 0, 0, pTarget);
  var iSelection = -1;
  if((GetMenu(pClonk)==WIPF)) iSelection = GetMenuSelection(pClonk);
  if(pTarget) RemoveObject(pTarget,1);
  if(iSelection>=0)
  {
    OpenWeaponMenu(pClonk, iSelection);
  }
}

global func OpenWeaponMenu(pClonk, iSelection)
{
  CloseMenu(pClonk);
  CreateMenu (WIPF, pClonk, pClonk, 0, "");
  AddMenuItem ("$MsgCrossbow$", "GiveWeapons", CRBW, pClonk, 0, pClonk, "$DescCrossbow$");
  AddMenuItem ("$MsgHook$"   , "GiveWeapons", HOOK, pClonk, 0, pClonk, "$DescHook$");
  AddMenuItem ("$MsgDagger$" , "GiveWeapons", KNFP, pClonk, 0, pClonk, "$DescDagger$");
  SelectMenuItem(iSelection, pClonk);
}
