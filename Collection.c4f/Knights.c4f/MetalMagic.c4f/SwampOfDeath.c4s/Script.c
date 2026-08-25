/*-- Sumpf des Todes --*/

#strict
static iDifficulty;

static g_iRelaunchX;
static g_iRelaunchY;

static fInitialized;

func Initialize() {
  SetSkyParallax(0, 100, 15);
  AI_InitSystem();
  var obj;
  while(obj = FindObject(BRDG, 0, 0, -1, -1, 0, 0, 0, 0, obj))
  {
    AddEffect("NoDamage", obj, 400, 0, 0, 0);
  }
  for(obj in FindObjects(Find_Func("IsTree")))
  {
    AddEffect("NoDamage", obj, 400, 0, 0, 0);
  }
  // Nekromanten erzeugen
  var pNekro = CreateObject(MAGE, 97, 323, -1);
  SetGraphics(0, pNekro, _SSR, 1, 6, 0, 0);
  SetGraphics(0, pNekro, MAGE, 2, 6, 0, 4);
  SetColorDw(RGB(1), pNekro);
  SetClrModulation(RGBa(255,255,255,255), pNekro);
  SetClrModulation(RGBa(255,255,255,255), pNekro, 2);
  SetName("$NameNekro$", pNekro);
  AddEffect("Nekromancer", pNekro, 1, 0, 0, 0);
  // Regel
  CreateObject(NOUD); // Keine Geister/Skelette von Paladinen
  CreateObject(NBTR); // Keine brennenden Bäume
  // Nebel
  for(var i=0;i<280;++i)
    CreateParticle("Fog",Random(LandscapeWidth()),Random(LandscapeHeight()),0,0,RandomX(900,1700),RGB(50,Random(200)+50,50));
  return();
}

global func FxNekromancerDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
  // Erst wenn er kein Heiligtum mehr hat, dann Schaden bekommen
  if(FindObject2(Find_ID(SNHD), Find_Func("IsUndeadHome"))) return();
  return(iDmgEngy);
}

global func FxNekromancerStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(fTemp) return();
  Schedule("GameOver()", 36);
  Message("$MsgEnd$");
}

global func FxNoDamageDamage (pTarget, iNumber, iDmgEngy, iCause)
{
  return(0);   
}

global func ShiftMap(iX, iY)
{
  for(var obj in FindObjects(Find_InRect(0,0,LandscapeWidth(),LandscapeHeight())))
    SetPosition(GetX(obj)+iX, GetY(obj)+iY, obj);
}

global func SetDTRotation (int r, int xoff, int yoff, object obj, int iLayer) {
  var fsin=-Sin(r, 1000), fcos=Cos(r, 1000);
  // set matrix values
  SetObjDrawTransform (
    +fcos, +fsin, (1000-fcos)*xoff - fsin*yoff,
    -fsin, +fcos, (1000-fcos)*yoff + fsin*xoff,
    obj, iLayer
  );
}

protected func InitializePlayer(int player, X, Y, foo, iTeam)
{
  if(!FindObject(REAC)) CreateObject(REAC,10,10,-1); // Clonk entlassen
  if(!FindObject(TACC)) CreateObject(TACC,10,10,-1); // Teamkonto
  if(!FindObject(_ETG)) CreateObject(_ETG,10,10,-1); // Löscher
  if(!FindObject(STES)) CreateObject(STES,10,10,-1); // Stäbe laden Mana
  if(!FindObject(SURR)) CreateObject(SURR,10,10,-1); // Aufgeben
  if(!FindObject(_ETG)) CreateObject(_ETG,10,10,-1); // Löscher
  if(!FindObject(EA69)) CreateObject(EA69, 10, 10, -1); // KI
  if(!FindObject(MGES)) CreateObject(MGES, 10, 10, -1); // Zauberenergie
  var pClonk = Global(player) = GetHiRank(player);
  SetPlrViewRange(200, GetHiRank(player));
  SetFoW(1, player);
  var pSaintHood = FindObject2(Find_ID(SNHD), Find_Func("IsPlayerHome"));
  if(!fInitialized)
  {
    SetOwner(player, pSaintHood);
    DoWealth(player, 200);
    fInitialized = 1;
  }
  SetPosition(GetX(pSaintHood), GetY(pSaintHood)+4, GetHiRank(player));
  CreateContents(SPER, GetHiRank(player));
  CreateContents(EFLN, GetHiRank(player));
  CreateContents(CNKT, GetHiRank(player));
  MMSetPlayerChurch(player,scLightchurch);
  if (!iDifficulty)
  {
    iDifficulty = -1;
    CreateMenu(WIPF, pClonk, pClonk, 0, "$MnuChooseDifficulty$");
    AddMenuItem("Empty", "", NONE, pClonk, 0,0, "");
    AddMenuItem("$MnuLevelItmEasy$", "SetDifficulty(1)", FISH, pClonk, 0,0, "$MnuLevelItmEasyDesc$");
    AddMenuItem("$MnuLevelItmNormal$", "SetDifficulty(2)", WIPF, pClonk, 0,0, "$MnuLevelItmNormalDesc$");
    AddMenuItem("$MnuLevelItmHard$", "SetDifficulty(3)", MONS, pClonk, 0,0, "$MnuLevelItmHardDesc$");
    SelectMenuItem(2, pClonk);
  }
  return(1);
}

global func SetDifficulty(int iSetDifficulty)
{
  // Schwierigkeitsstufe setzen
  iDifficulty = iSetDifficulty;

  // Message über dem Clonk ausgeben
  if (iSetDifficulty == 1) var strDifficulty = "$MnuLevelItmEasy$";
  if (iSetDifficulty == 2) var strDifficulty = "$MnuLevelItmNormal$";
  if (iSetDifficulty == 3) var strDifficulty = "$MnuLevelItmHard$";
  Message(strDifficulty, GetHiRank(0));

  // Gegner anpassen
  AI_SetDifficulty(iDifficulty);
}

protected func SetCastleOwner(int iPlr)
 {
 var x=1;
 if(Global(iPlr) == 2) x=(LandscapeWidth()/3)*2;
 
 for(var obj=0;obj=FindObject(0,x,0,LandscapeWidth()/3,LandscapeHeight(),0,0,0,NoContainer(),obj);)
 {
  if(~GetCategory(obj) & C4D_Structure() && !obj->~CastlePartWidth() && GetID(obj) != BANR)continue;
  if(GetOwner(obj) != -1)continue;
  SetOwner(iPlr,obj);
 }
}

protected func RelaunchPlayer(int player)
{
  if(player<0) return();

 var pSaintHood = FindObject2(Find_ID(SNHD), Find_Func("IsPlayerHome"));
 
 if(!pSaintHood) return();

  Sound("Trumpet");

 // Den Spieler wiederbeleben
 var pClonk = CreateObject(KNIG, 0,0, player);
 MakeCrewMember(pClonk, player);
 SetPlrViewRange(200, pClonk);
 pClonk->GrabObjectInfo(Global(player));
 SetPortrait(Format("%d", Random(3)+1), pClonk, KNIG);
 DoEnergy(100, pClonk);
 Global(player) = pClonk;
 
 SetCursor(player,pClonk);
 SetPosition(GetX(pSaintHood), GetY(pSaintHood)+4, pClonk);
 SetWealth(player, GetWealth(player)*2/3);
 CreateContents(SPER, pClonk);
 CreateContents(EFLN, pClonk);
 
 return(1);
}

protected func RemovePlayer(int player)
{
 // Spielervariable zurücksetzen, damit Laufzeitbeitritt mit Aufgeben funzt
 Global(player) = 0;
 return(1);
}
