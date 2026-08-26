/*-- Kleines Handgemenge --*/

#strict

/*static g_iCrewCount;*/
static g_iSpawnCount;
static iRelaunches;

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
  ScriptGo(1);  
  return(1);
}


protected func InitializePlayer(int iPlr)
 {
  iRelaunches[iPlr] = 1;
    var iPlrID = GetPlayerID(iPlr), iColID = ScoreboardCol(TEAM);
  SetScoreboardData(SBRD_Caption,     SBRD_Caption,      "Score"                  ,SBRD_Caption);
   SetScoreboardData(iPlrID,          SBRD_Caption, GetTaggedPlayerName(iPlr)     ,iPlrID);
  SetScoreboardData(SBRD_Caption,     1,            "{{KILC}}"                    ,iColID);
   SetScoreboardData(iPlrID,          1,       Format("%d", iRelaunches[iPlr])    ,0);
  SortScoreboard(iColID, true);
  DoScoreboardShow(1, iPlr);
  SetWealth(iPlr,2);
  var member = FindObject2(Find_Owner(iPlr),Find_ID(CLNK));
  var belt = CreateContents(938Z,member);
  AddCommand(member,"Call",belt,0,0,0,0,"Activate");
  return(1);
 }

protected func RelaunchPlayer(int iPlr)
 {
   if(iRelaunches[iPlr] == 0) return 0;
  SetWealth(iPlr,GetWealth(iPlr)-1); 
  if(GetWealth(iPlr) <= 0)
   {
    Log("<c ffcc00><i>%s hat versagt. :(</i></c>",GetPlayerName(iPlr));
    EliminatePlayer(iPlr);
    return(1);
   }
   if(GetWealth(iPlr) <= 1)
   {
    Log("<c ffcc00><i>%s hat seinen letzten Versuch.</i></c>",GetPlayerName(iPlr));
    //neues Crewmitglied
    while(!(GetMaterial(x,y)==Material("Tunnel"))) // irgendwo im Tunnel plazieren
    {
     var x = Random(LandscapeWidth());                       
     var y = Random(LandscapeHeight()+150);
    }
    var member = CreateObject(CLNK,x,y,iPlr);
    
    MakeCrewMember(member, iPlr);
    //DigFree(GetX(member), GetY(member), 10);
    //DrawMaterialCircle("Tunnel", GetX(member), GetY(member), 15); // Zeichnet einen Kreis auch im festen Material
    DigFreeCircle(15,  GetX(member), GetY(member));
    SelectCrew(iPlr, member, 1);
    SetCursor(iPlr,member);
    DoEnergy(100,member);   
    var belt = CreateContents(938Z,member);
    AddCommand(member,"Call",belt,0,0,0,0,"Activate");
    AddEffect("Rejoin",member,1,1,0);
     iRelaunches[iPlr]--;
    var iPlrID = GetPlayerID(iPlr);
    SetScoreboardData(iPlrID, 1, Format("%d",iRelaunches[iPlr]), 0);
    return(1);       
   }
}

public func OnClonkDeath(object pClonk, int iKiller) {/*
  var iOwner = GetOwner(pClonk);
  if (!GetPlayerName(iOwner) || !pClonk) return();
  var sTypeName = GetName(0,GetID(pClonk));
  if (!sTypeName) return();
  
  if(iOwner == iKiller) {
    Log("%s begann Selbstmord.",GetTaggedPlayerName(iOwner));
  } else {
    if(Hostile(iOwner,iKiller)) {
            iKills[iKiller]++;
        var iPlrID = GetPlayerID(iKiller);
       /*
       // 1 Relaunch mehr für den 3fachen Killer
            if(iKills[iKiller] % 3 == 0) {
       
                iRelaunches[iKiller]++;
                SetScoreboardData(iPlrID, 1, Format("%d",iRelaunches[iKiller]), 0);
          }
            Log(Format("$Kill$",GetTaggedPlayerName(iOwner),GetTaggedPlayerName(iKiller)));
            SetScoreboardData(iPlrID, 2, Format("%d", iKills[iKiller]), 1);
        }
    }
    return 1;*/
}

protected func RemovePlayer(int iPlr) {
  var iPlrID = GetPlayerID(iPlr);
  SetScoreboardData(iPlrID, 1, 0, 0);
  SetScoreboardData(iPlrID, 2, 0, 0);
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

protected func Script100()
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
  g_iSpawnCount++;    
}

protected func Script150()
{
  goto(100);
}
  
private func getSpawnType()
{
  var random = Random(12);
  if (random < 7) {
    var id = [EH8Z,TFLN,FLNT,D18Z,STFN,CY8Z,DZ8Z,SFLN,EP8Z,E_8Z,DF8Z,CR8Z,FBMP,/*ZAPN*/,CQ8Z];
    return (id[Random(1 + Min(g_iSpawnCount, GetLength(id)))]);
  }
  if (random < 10) {
    var id = [CP8Z,E98Z];
    return (id[Random(1 + Min(g_iSpawnCount, GetLength(id)))]);
  }
  var id = [CNKT,DL8Z,578Z,548Z,CX8Z];
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


