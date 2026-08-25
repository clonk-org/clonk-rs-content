/*-- Greifenkampf --*/

#strict

static g_MaxKills;

static aDeaths;
static aKills;
static aGriffons;

static g_No_Relaunch_Removal;

func Initialize() {
  // Kapelle, Altäre und Zelte
  PlaceBuilding(CHPL, 1, 1);
  PlaceBuilding(ALTR, 2, 4);
  PlaceBuilding(TENT, 5, 20);
  
  // Statuen
  PlaceBuilding(IDOL, 0, 5);
  PlaceBuilding(GIDL, 0, 3);
  
  // Truhen, Grünzeug, Speerständer und Gras
  PlaceItems(CHST, 5, 20);
  PlaceItems(_VEG,10, 20);
  PlaceItems(SPRC, 5, 10);
  PlaceGrass(GRAS,100, 200);
  
  // Bäume und Bäume und Bäume
  PlaceTree(TRE1, 5, 10);
  PlaceTree(TRE2, 5, 10);
  PlaceTree(TRE3, 5, 10);
  PlaceTree(TRE7, 5, 10, 20);
  PlaceTree(TRE5, 5, 10, 20);
  PlaceTree(TRE6, 5, 10, 20);
  
  // Arrays initialisieren;
  aGriffons = [];
  aDeaths = [];
  aKills = [];
  g_MaxKills = 5;

  // Truhen füllen
  for(var chest in FindObjects(Find_ID(CHST)))
  {
    while(Contents(0,chest))RemoveObject(Contents(0,chest));
    FillChest(chest);
  }
  // Speerständer füllen
  for(var pObj in FindObjects(Find_ID(SPRC)))
  {
    var i = RandomX(3, 5);
    while(i--) CreateContents(SPER, pObj);
  }

  // Objekte im Erdreich
  PlaceInEarth(GOLD,  5,10);
  PlaceInEarth(ROCK,  8,12);
  PlaceInEarth(FLNT, 10,15);
  PlaceInEarth(SFLN, 10,15);
  PlaceInEarth(XARP,  4, 8);
  PlaceInEarth(SBLP,  4, 8);

  // Hübsches Gamma
  SetGamma(0, RGB(60,80,80), RGB(200,200,200));

  // Scorboard
  SetScoreboardData(SBRD_Caption, SBRD_Caption, "$Score$");
  SetScoreboardData(SBRD_Caption, ScoreboardCol(KILL), Format("%d{{SWOR}}",g_MaxKills), ScoreboardCol(KILL));
  SetScoreboardData(SBRD_Caption, ScoreboardCol(LIFE), "{{SKUL}}", ScoreboardCol(LIFE));

  // Script
  ScriptGo(1);
}

protected func Script5()
{
  if(ObjectCount2(Find_ID(BIRD))<10 && !Random(3))
  {
    PlaceBird();
  }
  goto(1);
}

protected func PlaceBird()
{
  var iError, pObj;
  while(1)
  {
    pObj = PlaceAnimal(BIRD);
    if(iError>20) return();
    if(FindObject2(Find_Distance(1000, GetX(pObj), GetY(pObj)), Find_Exclude(pObj), Find_OCF(OCF_CrewMember)))
    {
      RemoveObject(pObj);
      iError++;
      continue;
    }
    return;
  }
}

protected func PlaceInEarth(id idID, int iMin, int iMax)
{
  var i = RandomX(iMin,iMax)*3, pObj;
  var iError;
  while(i--)
  {
    pObj = PlaceInMaterial(idID,RandomMat(),0);
    if(FindObject2(Find_Distance(10, GetX(pObj), GetY(pObj)), Find_Exclude(pObj), Find_Category(C4D_Object)) ||
       FindObject2(Find_Distance(60, GetX(pObj), GetY(pObj)), Find_Exclude(pObj), Find_ID(idID)))
    {
      RemoveObject(pObj);
      iError++;
      i++;
    }
    if(iError>20) return();
  }
}

protected func RandomMat()
{
  var rnd = Random(3);
  if(rnd==0) return Material("Earth");
  if(rnd==1) return Material("Ashes");
  if(rnd==2) return Material("FlyAshes");
}

protected func PlaceItems(id idID, int iMin, int iMax)
{
  var i = RandomX(iMin,iMax), pObj;
  var iError;
  while(i--)
  {
    pObj = PlaceAnimal(idID);
    if(FindObject2(Find_Distance(100, GetX(pObj), GetY(pObj)), Find_ID(idID), Find_Exclude(pObj))
    || Stuck(pObj) || FindObject2(Find_AtPoint(GetX(pObj), GetY(pObj)), Find_Category(C4D_Structure)))
    {
      RemoveObject(pObj);
      iError++;
      i++;
    }
    if(iError>20) return();
  }
}

protected func PlaceTree(id idID, int iMin, int iMax, int iOffset)
{
  var i = RandomX(iMin,iMax), pObj;
  var iError;
  while(i--)
  {
    pObj = PlaceAnimal(idID);
    SetPosition(GetX(pObj), GetY(pObj)+iOffset, pObj);
    if(FindObject2(Find_Distance(80, GetX(pObj), GetY(pObj)), Find_Func("IsTree"), Find_Exclude(pObj)))
    {
      RemoveObject(pObj);
      iError++;
      i++;
    }
    if(iError>20) return();
  }
}

protected func PlaceGrass(id idID, int iMin, int iMax)
{
  var i = RandomX(iMin,iMax), pObj;
  var iError;
  while(i--)
  {
    pObj = PlaceAnimal(idID);
    if(FindObject2(Find_Distance(10, GetX(pObj), GetY(pObj)), Find_ID(idID), Find_Exclude(pObj)))
    {
      RemoveObject(pObj);
      iError++;
      i++;
    }
    if(iError>20) return();
  }
}

protected func PlaceBuilding(id idID, int iMin, int iMax)
{
  var i = RandomX(iMin,iMax);
  var iError;
  var pObj;
  while(i--)
  {
    Var(0)=Random(LandscapeWidth()); Var(1)=Random(LandscapeHeight());
    if(FindConstructionSite(idID, 0, 1))
    {
      if(FindObject2(Find_Distance(150, Var(0), Var(1)), Find_ID(idID)))
      {
        iError++;
        i++;
      }
      else
      {
        pObj = CreateConstruction(idID, Var(0), Var(1), -1, 100, 1);
	if(idID == TENT) SetColorDw(HSL(Random(255),255,128), pObj);
      }
    }
    else
    {
      iError++;
      i++;
    }
    if(iError>20) return();
  }
}

protected func FillChest(object pChest)
{
 var count=4+Random(6);
 while(count--)
 CreateContents(RandChestID(),pChest);
}

protected func RandChestID()
{
  var iItems = [BRED, SPER, MUSK, BLTP, BOW1, FARP, XARP, STFN, EFLN, AXE1, SWOR, TENP, SBLP, PBLP, COKI];
  return(iItems[Random(GetLength(iItems))]);
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

  // Mit allen Spielern verfeinden
  for(var i=0; i < GetPlayerCount(); i++)
    if(iPlr != GetPlayerByIndex(i))
    {
      SetHostility(GetPlayerByIndex(i), iPlr, 1);
      SetHostility(iPlr, GetPlayerByIndex(i), 1);
    }
  // Kirche wählen lassen
  if(!MMGetPlayerChurch(iPlr))
    Enter(CreateObject(DSHP, 100, 100, iPlr),GetCrew(iPlr));
  // Joinen lassen
  else JoinPlayer(iPlr);
  return(1);
}

func OnChurchChoose(iPlr)
{
  // Nach Kirchenwahl joinen
  RemoveObject(Contained(GetCrew(iPlr)), 1);
  JoinPlayer(iPlr);
  // Ein bischen Startkarma
  GetCrew(iPlr)->DoKarmaEnergy(20);
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
   
    DoWealth(iDeathCausedBy, 30);
    var pObj = GetCursor(iDeathCausedBy);
    if(GetCrew(iDeathCausedBy))
    	GetCrew(iDeathCausedBy)->DoKarmaEnergy(30);
    
    if(!FindContents(EFLN, pObj))
    {
      var idID = GetID(Contents(0, pObj));
      CreateContents(EFLN, pObj);
      ShiftContents (pObj, 0, idID);
    }
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

protected func RemovePlayer(iPlr)
{
  // Relaunchzähler ium Scoreboard ausleeren
  var iPlrID = GetPlayerID(iPlr);
  SetScoreboardData(iPlrID,       ScoreboardCol(SWOR),       0, -1);
  SetScoreboardData(iPlrID,       ScoreboardCol(SKUL),       0, -1);
  SortScoreboard(ScoreboardCol(KILL), true);
}

protected func JoinPlayer(iPlr)
{
  // Spawnposition finden
  var x=-1, y=-1;
  GetSpawnPosition(x,y);
  if(x==-1) { x=100;y=400; }
 
  // Relaunchen mit Relauncher!
  var crew=GetCrew(iPlr);
  var help=CreateObject(DSHP,x,y,iPlr);
  SetYDir(0,help);
  SetPosition(x,y,crew);
  Enter(help,crew);
  AddEffect("Relaunch",help,20,4,0,0);
 
  // Auswahl der Waffen
  Schedule(Format("OpenWeaponMenu(Object(%d))", ObjectNumber(crew)), 10);

  // Ein bischen Energie
  DoEnergy(100,crew);
 
  // Anwählen
  SelectCrew(iPlr,crew,1);
}

global func GiveWeapons(idID, pClonk)
{
  // Jeder bekommt ein paar Flints :-)
  CreateContents(SFLN,pClonk);
  CreateContents(SFLN,pClonk);
  // Eine Waffe seiner Wahl
  if(idID==MUSK) // Musktete mit Kugeln
  {
    CreateContents(BLTP,pClonk);
    CreateContents(MUSK,pClonk);
  }
  if(idID==BOW1) // Bogen mit Pfeilen
  {
    CreateContents(ARWP,pClonk);
    CreateContents(ARWP,pClonk);
    CreateContents(BOW1,pClonk);
  }
  if(idID==LANC) // Lanze mit Schwert und Schild
  {
    CreateContents(SWOR,pClonk);
    CreateContents(SHIE,pClonk)->Activate(pClonk);
    CreateContents(LANC,pClonk)->Activate(pClonk);
  }
}

protected func GetSpawnPosition(&x,&y)
{
  for(var cnt=0;cnt<10000 && x==-1;cnt++)
  {
    // Wir plazieren einen Greif und schauen ob er Platz hat
    var pWipf=PlaceAnimal(GRFN);
    if(Stuck(pWipf) || GetY(pWipf)<80) { RemoveObject(pWipf); continue; }
  
    // Auch kein Gegner im Umkreis?
    if(FindObject2(Find_OCF(OCF_CrewMember()), Find_NoContainer(), Find_Distance(1000, GetX(pWipf), GetY(pWipf)))) { RemoveObject(pWipf); continue; }
  
    // Gut, dann Position speichern und weg mit dem Restgreif
    x=GetX(pWipf);
    y=GetY(pWipf)+27;
    RemoveObject(pWipf);

    // Fertig!
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
  var pClonk = FindObject(PLDN, 0, 0, 0, 0, 0, 0, 0, pTarget);
  var iSelection = -1;
  if((GetMenu(pClonk)==WIPF)) iSelection = GetMenuSelection(pClonk);
  if(pTarget) RemoveObject(pTarget,1);
  if(iSelection>=0)
  {
    OpenWeaponMenu(pClonk, iSelection);
  }
  var pGriffon;
  if(MMGetPlayerChurch(GetOwner(pClonk))==scBloodchurch) pGriffon = pClonk->CreateObject(SARN,0,5,GetOwner(pClonk));
  else pGriffon = pClonk->CreateObject(GRFN,0,5, GetOwner(pClonk));
  pGriffon->CastParticles("FSpark", 50,50, 0,0, 100, 200, RGBa(128,128,255,0), RGBa(255,255,255,127));
  for(var i=0; i<5; i++) pGriffon->Smoke(0,25,50);
  Sound("Poff", 0, pGriffon);
  pGriffon->SetAction("Fly");
  aGriffons[GetOwner(pClonk)] = pGriffon;
  pGriffon->ActivateEntrance(pClonk);
  SetCommand(pGriffon, "Follow", pClonk);
   AddEffect("Radar",pClonk,23,35,0,0, pGriffon);
}

global func OpenWeaponMenu(pClonk, iSelection)
{
  CloseMenu(pClonk);
  CreateMenu (WIPF, pClonk, pClonk, 0, "");
  AddMenuItem ("$MsgMusket$", "GiveWeapons", MUSK, pClonk, 0, pClonk, "$DescMusket$");
  AddMenuItem ("$MsgBow$"   , "GiveWeapons", BOW1, pClonk, 0, pClonk, "$DescBow$");
  AddMenuItem ("$MsgLance$" , "GiveWeapons", LANC, pClonk, 0, pClonk, "$DescLance$");
  SelectMenuItem(iSelection, pClonk);
}

global func FxRadarStart(pTarget, iNumber, fTmp, pGriffon)
{
  if(fTmp) return();
  var obj = EffectVar(0, pTarget, iNumber) = CreateObject(DSHP, 0, 0, GetOwner(pTarget));
  SetVisibility(VIS_Owner(),obj);
  obj->SetAction("Follow", pTarget);
  EffectVar(2, pTarget, iNumber) = pGriffon;
}

global func FxRadarTimer(pTarget, iNumber)
{
  for(var obj in FindObjects(Find_Hostile(GetOwner(pTarget)), Find_OCF(OCF_Alive(), OCF_CrewMember())))
  {
    var iAngle = Angle(GetX(pTarget), GetY(pTarget), GetX(obj), GetY(obj));
    CreateParticle("Arrow",Sin(iAngle, 30)+GetX(pTarget),-Cos(iAngle, 30)+GetY(pTarget),Sin(iAngle, 5),-Cos(iAngle, 5),100,GetColorDw(obj), EffectVar(0, pTarget, iNumber));
  }
  var pEnemy, pGriffon;
  // Reitet ein Gegner auf dem Greifen?
  if(pEnemy = FindObject2(Find_ID(PLDN), Find_Hostile(GetOwner(pTarget)), Find_ActionTarget(EffectVar(2, pTarget, iNumber)), Find_Func("IsRiding")))
  {
    // Dann Eigentum der Tiere tauschen
    pGriffon = EffectVar(2, pTarget, iNumber);
    EffectVar(2, pTarget, iNumber) = EffectVar(2, pEnemy, GetEffect("Radar", pEnemy));
    EffectVar(2, pEnemy, GetEffect("Radar", pEnemy)) = pGriffon;
    SetCommand(pGriffon, "Follow", pEnemy);
    SetOwner(GetOwner(pEnemy),pGriffon);
    SetCommand(EffectVar(2, pTarget, iNumber), "Follow", pTarget);
    SetOwner(GetOwner(pTarget),EffectVar(2, pTarget, iNumber));
  }
  if(EffectVar(1, pTarget, iNumber)) EffectVar(1, pTarget, iNumber)--;
  if(!pTarget->~IsRiding() && !EffectVar(1, pTarget, iNumber))
  {
    pGriffon = EffectVar(2, pTarget, iNumber);
    if(!pGriffon) return();
    if(!GetAlive(pGriffon)) return();
    pGriffon->ResetControl();
    SetCommand(pGriffon, "Follow", pTarget);
    EffectVar(1, pTarget, iNumber)+=100;
  }
}

global func FxRadarStop(pTarget, iNumber, iReason, fTmp)
{
  if(fTmp) return();
  if(EffectVar(0, pTarget, iNumber)) RemoveObject(EffectVar(0, pTarget, iNumber));
  if(EffectVar(2, pTarget, iNumber))  EffectVar(2, pTarget, iNumber)->Kill();
}
