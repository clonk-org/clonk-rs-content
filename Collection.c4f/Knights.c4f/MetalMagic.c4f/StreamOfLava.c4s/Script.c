/*-- Tanko  --*/

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

  // Arrays initialisieren;
  aDeaths = [];
  aKills = [];
  g_MaxKills = 5;

  // Scorboard
  SetScoreboardData(SBRD_Caption, SBRD_Caption, "$Score$");
  SetScoreboardData(SBRD_Caption, ScoreboardCol(KILL), Format("%d{{SWOR}}", g_MaxKills), ScoreboardCol(KILL));
  SetScoreboardData(SBRD_Caption, ScoreboardCol(LIFE), "{{SKUL}}", ScoreboardCol(LIFE));

  if(!ObjectCount(MELE)) CreateObject(MELE,10,10,-1);
  if(!ObjectCount(NPRY)) CreateObject(NPRY,12,12,-1);
  if(!ObjectCount(NOUD)) CreateObject(NOUD,13,13,-1);
  if(!ObjectCount(_ETG)) CreateObject(_ETG,14,14,-1);

  CreateObject(RCHS,10,10,-1);

  for(var obj in FindObjects(Find_ID(BRDG)))
    AddEffect("NoDamage", obj, 400, 0, 0, 0);

  // Spawnpunkte starten
  for(var pObj in FindObjects(Find_ID(IDOL)))
  {
    pObj->Initialize();
  }

  // Gamma
  SetGamma(0, RGB(128,110,80), RGB(255,255,155));
}

global func FxNoDamageDamage (pTarget, iNumber, iDmgEngy, iCause) { return 0; }

protected func FillChest(object pChest)
{
 var count=4+Random(6);
 while(count--)
 CreateContents(RandChestID(),pChest);
}

protected func RandChestID()
{
 var rand = Random(6);
 if(!rand) return(BRED);
 if(!--rand) return(BOW1);
 if(!--rand) return(XARP);
 if(!--rand) return(EFLN);
 if(!--rand) return(TENP);
 return(BOW1);
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
  
  // Ein bischen FoW...
  SetFoW(1, iPlr);

  if(!MMGetPlayerChurch(iPlr))
    Enter(CreateObject(DSHP, 100, 100, iPlr),GetCrew(iPlr));
  else JoinPlayer(iPlr);
  return(1);
}

protected func RemovePlayer(iPlr)
{
  // Relaunchzähler im Scoreboard ausleeren
  var iPlrID = GetPlayerID(iPlr);
  SetScoreboardData(iPlrID,       ScoreboardCol(KILL),       0, -1);
  SetScoreboardData(iPlrID,       ScoreboardCol(LIFE),       0, -1);
  SortScoreboard(ScoreboardCol(KILL), true);
}

func OnChurchChoose(iPlr)
{
  RemoveObject(Contained(GetCrew(iPlr)), 1);
  DoKarmaEnergy(20,iPlr);
  JoinPlayer(iPlr);
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
    DoKarmaEnergy(30,iDeathCausedBy);
    DoWealth(iDeathCausedBy, 30);
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

  // Keine Respawns mehr?
  /*if(0)//!aDeaths[iPlr])
  {
    SetScoreboardData(GetPlayerID(iPlr), ScoreboardCol(LIFE), "{{SKUL}}", 0);
    return(0);
  }*/
 
  // Neuer Paladin
  var crew=CreateObject(PLDN,10,10,iPlr);
  MakeCrewMember(crew,iPlr);
  JoinPlayer(iPlr);
}

protected func JoinPlayer(iPlr)
{
  var crew = GetCrew(iPlr);
  SetPlrViewRange(300, crew);
  var pTank = CreateObject(STTK, Random(LandscapeWidth()), 0, iPlr);
  SetPhysical("Jump", 60000, 2, pTank);
  CreateContents(STM1, pTank);
  Enter(pTank, crew);
 
  DoEnergy(100,crew);
 
  SelectCrew(iPlr,crew,1);
}
