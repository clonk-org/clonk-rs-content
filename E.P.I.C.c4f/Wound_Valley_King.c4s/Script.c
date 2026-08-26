/*-- Kleines Handgemenge --*/

#strict

/*static g_iCrewCount;*/
static g_iSpawnCount;
static iRelaunches;
static lorry1, lorry2,apo;

protected func Initialize() 
{
  iRelaunches = [];
  SetSkyParallax(1, 20,20, 0,0, SkyPar_Keep(),SkyPar_Keep());
  SetGamma(RGB(15,15,15),RGB(118,118,118),RGB(215,215,215));
/*   //Rauch 
   for(var i=0;i<180;++i)
   CreateParticle("Smoke",Random(LandscapeWidth()),Random(LandscapeHeight()),0,0,RandomX(900,1700)); */
   //Donner 
  CreateObject(_AMB,165,550,-1);
  apo = CreateObject(ET8Z,(LandscapeWidth()/2),0);
  lorry1 = CreateObject(LORY,0,0,-1);
  lorry2 = CreateObject(LORY,LandscapeWidth(),0,-1);
  ScriptGo(1);  
  return(1);
}


protected func InitializePlayer(int iPlr)
 {
  iRelaunches[iPlr] = 0;
    var iPlrID = GetPlayerID(iPlr), iColID = ScoreboardCol(TEAM);
  SetScoreboardData(SBRD_Caption,     SBRD_Caption,      "Score"                  ,SBRD_Caption);
   SetScoreboardData(iPlrID,          SBRD_Caption, GetTaggedPlayerName(iPlr)     ,iPlrID);
  SetScoreboardData(SBRD_Caption,     1,            "{{KILC}}"                    ,iColID);
   SetScoreboardData(iPlrID,          1,       Format("%d", iRelaunches[iPlr])    ,0);
  SetScoreboardData(SBRD_Caption,     2,            "{{RVLR}}"                    ,iColID);
   SetScoreboardData(iPlrID,          2,       Format("%d", LocalN("timestayed",apo)[iPlr])    ,0);
  SortScoreboard(iColID, true);
  DoScoreboardShow(1, iPlr);
  SetWealth(iPlr,25);
  var member = FindObject2(Find_Owner(iPlr),Find_ID(CLNK));
  var belt = CreateContents(938Z,member);
  AddCommand(member,"Call",belt,0,0,0,0,"Activate");
  SetLength(LocalN("timestayed",apo),iPlr);
  return(1);
 }

protected func RelaunchPlayer(int iPlr)
 {
    Log("%s dies.",GetPlayerName(iPlr));
    apo->ChangeScore(-25,iPlr);
    //neues Crewmitglied
    var x = Random(LandscapeWidth());                       
    var y = 200;
    while ((!(GetMaterial(x,y) == (Material( "Tunnel" )) )) && ( !(GetMaterial(x,y) == (Material( "Earth" ))) ))
	y++;
    var member = CreateObject(CLNK,x,y,iPlr);
    var spawner = CreateObject(EW8Z,x,y,iPlr);
    Enter(spawner,member);
    MakeCrewMember(member, iPlr);
    SelectCrew(iPlr, member, 1);
    SetCursor(iPlr,member);
    DoEnergy(100,member);
    iRelaunches[iPlr]++;
    var iPlrID = GetPlayerID(iPlr);
    SetScoreboardData(iPlrID, 1, Format("%d",iRelaunches[iPlr]), 0);
    return(1);
}

public func OnClonkDeath(object pClonk, int iKiller) {
}

protected func RemovePlayer(int iPlr) {
  var iPlrID = GetPlayerID(iPlr);
  SetScoreboardData(iPlrID, 1, "leaved", 0);
  SetScoreboardData(iPlrID, 2, "leaved", 0);
  return 1;
}

/*protected func InitializePlayer(iPlr)
{
  // Clonks auch für Spätjoiner
  if (g_iCrewCount) InitializePlayer2(iPlr);
}

protected func InitializePlayer2(iPlr)
{
  // Clonks an dieselbe Stelle wie den Erstclonk. Fällt am wenigsten auf
  var pHiRank = GetHiRank(iPlr);
  if (pHiRank) for (var i=1; i<g_iCrewCount; ++i)
    {
    var pClonk = CreateObject(CLNK, GetX(pHiRank), GetDefBottom(pHiRank), iPlr);
    ObjectSetAction(pClonk, "Walk");
    MakeCrewMember(pClonk, iPlr);
    DoEnergy(+500, pClonk);
    SetDir(GetDir(pHiRank), pClonk);
    SetCommand(pClonk, "Follow", pHiRank); // geht nicht :C
    }
}

protected func Script1()
{
  // Jetzt gibts erstmal Clonknachschub für alle: Nach Durchschnittsrang der besten Clonks
  var pHiRank, n, iRank;
  for (var i=0; i<GetPlayerCount(); ++i)
    if (pHiRank = GetHiRank(GetPlayerByIndex(i)))
      {
      iRank += GetRank(pHiRank);
      ++n;
      }
  iRank = (iRank+n/2)/n;
  g_iCrewCount = BoundBy(iRank/3, 1, 5);
  // Zusätzliche Clonks erzeugen
  if (g_iCrewCount)
    for (var i=0; i<GetPlayerCount(); ++i)
      InitializePlayer2(GetPlayerByIndex(i));
}*/

protected func Script30()
{
  var obj;
  if (obj = LocalN ("basement", FindObject2(Find_ID(ET8Z)))) 
	Explode (20, obj);
}

protected func Script50()
{
  // Mehr Spieler brauchen mehr Waffen
  for (var i=0; i<GetPlayerCount(); ++i)
    {
    // Typ bestimmen
    var id = getSpawnType();
    // Platz im Erdreich finden
    var x = 10 + Random(LandscapeWidth() - 20);
    var h = GetHorizonHeight(x);
    var y = h + 10 + Random(LandscapeHeight() - h - 20);
    if (GetMaterial(x, y) != Material("Earth")) continue;
    // Objekt per verzögertem Effekt erzeugen
    var effect = AddEffect("SpawnSparkle", 0, 1, 5, 0, 0);
    EffectVar(0, 0, effect) = id; EffectVar(1, 0, effect) = x; EffectVar(2, 0, effect) = y;
    }
  // Spawns mitzählen
  if(!Random(2))
  {
  	if (ContentsCount(0,lorry1) < 50)
		Enter(lorry1,CreateObject(AWeapon()));
  	if (ContentsCount(0,lorry2) < 50)
		Enter(lorry2,CreateObject(AWeapon()));
  }
  if (!Random(7))
  {
	if (ContentsCount(0,lorry1) < 50)
		Enter(lorry1,CreateObject(LOAM));
	if (ContentsCount(0,lorry2) < 50)
		Enter(lorry2,CreateObject(LOAM));
  }
  if (!Random(7))
  {
	if (ContentsCount(0,lorry1) < 50)
		Enter(lorry1,CreateObject(ES8Z));
	if (ContentsCount(0,lorry2) < 50)
		Enter(lorry2,CreateObject(ES8Z));
  }
  g_iSpawnCount++;    
}

protected func Script100()
{
  goto(50);
}
  
private func getSpawnType()
{
  var random = Random(13);
  if (random < 8) {
    return (AWeapon());
  }
  if (random < 10) {
    var id = [CP8Z,E98Z];
    return (id[Random(1 + Min(g_iSpawnCount, GetLength(id)))]);
  }
  var id = [578Z,548Z,CX8Z];
  return (id[Random(1 + Min(g_iSpawnCount, GetLength(id)))]);
}  

private func GetHorizonHeight(x)
{
  var y;
  while (y < LandscapeHeight() && !GBackSemiSolid(x, y))
    y += 10;
  while (y > LandscapeHeight() && GBackSemiSolid(x, y))
    --y;
  return(y);
}
  
 global func FxSpawnSparkleTimer(obj, effect, time)
{
  // Variablen auswerten
  var id = EffectVar(0, 0, effect); var x = EffectVar(1, 0, effect); var y = EffectVar(2, 0, effect);
  // Funken erzeugen
  CreateParticle("NoGravSpark", x + RandomX(-6,+6), y + RandomX(-6,+6), 0, -3, RandomX(30,50), RGB(64,64,255));
  // Objekt erzeugen
  if ((time >= 100) && !EffectVar(3, 0, effect)) 
    EffectVar(3, 0, effect) = CreateObject(id, x, y);
  // Fertig
  if (time >= 150) return(-1); 
 }
  

global func FxRejoinTimer(pTarget,iEffectNumber,iEffectTime)
{
 if(iEffectTime > 35*3)return -1;
 CreateParticle("PxSpark",GetX(pTarget),GetY(pTarget),Sin(iEffectTime*50,30),-Cos(iEffectTime*50,30),50,SetColorDw(GetColorDw(pTarget)),pTarget,1);
} 

global func FxRejoinDamage(pTarget)
{
 return;
}

global func AWeapon()
{
    var id = [CR8Z,STFN,STFN,CR8Z,FLNT,CY8Z,DZ8Z,E_8Z];
    return (id[Random(1 + Min(g_iSpawnCount, GetLength(id)))]);
}
