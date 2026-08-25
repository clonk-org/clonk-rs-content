/*-- Wand --
Das Objekt enthält und benutzt 2 gleiche Glockensounds,
da die Engine scheinbar bei szenariointerner Musik (auch bei auf einen
anderen Ordner bezogene Musik) das gleichzeitige Abspielen desselben Sounds
unterbindet */

#strict 2
#include CPW2 // Burgteilfunktion

/* Initialisierung */

local hour, chimes, bell;

protected func Initialize()
{
  // Tür ist immer offen
  SetEntrance(1);
  // Transferzone
  UpdateTransferZone();
  //Damit der Turm richtig sitzt -> hier nicht
  //SetPosition(GetX(), GetY()+16);
  SetAction("BellRest");
  AddEffect("Clock", this, 1, 1000, this);
  chimes=1;
  bell=1;
  // Basisimplementation
  _inherited();
  // Fertig
  return 1;
}

/* Ausrichtung von Burgbauteilen */

public func CastlePartWidth() { return (40); }
public func CastlePartHeight() { return (73); }
public func CastlePartTop() { return (-28); }
public func CastlePartBasement() { return 0; }

/* Bei Veränderung der Burg */

public func CastleChange()
{
  UpdateTransferZone();
}

/* Bewegung im Treppenhaus */

public func ContainedUp()
{
  [$TxtUp$]
  var pCase = FindStaircaseAbove();
  if (!pCase) return 0;  
  TransferContents(pCase);
  return 1;
}

public func ContainedDown()
{
  [$TxtDown$]
  var pCase = FindStaircaseBelow();
  if (!pCase) return 0;  
  TransferContents(pCase);
  return 1;
}

public func ContainedLeft()
{
  [$TxtExit$]
  // Inhalt raus
  TransferContents(this);
  return 1;
}

public func ContainedRight()
{
  [$TxtExit$]
  // Inhalt raus
  TransferContents(this);
  return 1;
}

private func TransferContents(object pTarget)
{
  // Von oben runterzählen, damit Sachen wie TransferContents(this) funktionieren.
  var i;
  for (i = ContentsCount() - 1; i >= 0; --i)
  {
    var obj = Contents(i);
    if (!obj) continue;
    Enter(pTarget, obj);
    AddCommand(obj, "Exit");
  }
}

/* Treppenhaus */

private func FindStaircase(int ox, int oy)
{
  // Anderes Treppenhaus in entsprechender Richtung suchen
  var obj;
  while(obj = FindObject(0, ox,oy, 0,0, OCF_Fullcon, 0,0, NoContainer(), obj) )
    if (obj->~IsStaircase())
      return obj;
  // Keins gefunden
  return 0;
}

private func FindStaircaseAbove()
{
  return FindStaircase(0, -CastlePartHeight()/2 - 10) ;
}

private func FindStaircaseBelow()
{
  return FindStaircase(0, CastlePartHeight()/2 + 10) ;
}

/* Status */

public func IsStaircase() { return 1; }
public func StaircaseExit() { return -25; } // Oberer Ausgang

/* Transfer */

public func UpdateTransferZone()
{
  // Nur mit darüberliegendem Niedergang
  if (FindStaircaseAbove() )
    if (GetOCF() & OCF_Fullcon )
      return SetTransferZone(-8, -40, 16, 61);
  // Ohne darüberliegenden Niedergang (kein Auf- oder Abstieg möglich)
  return SetTransferZone();
}

public func ControlTransfer(object pObj, int tx, int ty)
{
  // Aufstieg
  if (Inside(tx - GetX(), -20, 20) && Inside(ty - GetY(), -50, -20) )
    if (Contained(pObj) == this || GetY(pObj) > GetY() - 20)
      return HandleTransferAscend(pObj, tx, ty) ;

  // Abstieg
  if (Inside(tx - GetX(), -20, 20) && Inside(ty - GetY(), -20, 37) )
    if (Contained(pObj) == this || GetY(pObj) < GetY() - 20)
      return HandleTransferDescend(pObj, tx, ty) ;

  // Transfer nicht möglich
  return 0;
}

private func HandleTransferAscend(object pObj, int tx, int ty)
{
  // Treppenhaus betreten
  if (Contained(pObj) != this)
    return AddCommand(pObj, "Enter", this);

  // Aufstieg
  return ContainedUp(pObj);
}
  
private func HandleTransferDescend(object pObj, int tx, int ty)
{
  // Darüberliegendes Treppenhaus suchen
  var pCase = FindStaircaseAbove();
  if (!pCase)
    return HandleTransferDescendPlatform(pObj, tx, ty);
    
  if (Contained(pObj) != pCase)
    return AddCommand(pObj, "Enter", pCase);
  
  return pCase->ContainedDown(pObj);
} 

private func HandleTransferDescendPlatform(object pObj, int tx, int ty)
{
  return 0;
}

/* Forschung */

public func GetResearchBase() { return CPW2; }


/* Nette Glockenfunktionen */
//Die Glocke schlägt je nach Stunde(hour) unterschiedlich oft(chimes)
//Bei 12 angekommen, beginnt wieder mit Stunde 1
private func ChimeCheck()
{ 
  if (chimes <= 0) 
   {
    SetAction("BellRest");
    chimes= 0;
   }
  else
   {
    SetAction("BellSwing");
    chimes -= 1;
   }
  return(1);
}

protected func FxClockStart(object pTarget, int iEffectNumber, int iTemp) {
hour= 1;
chimes= hour;
SetAction("BellIdle");
}


protected func FxClockTimer(object pTarget, int iEffectNumber, int iTime) {
hour += 1;
if (hour > 12) hour = 1;
chimes= hour;
SetAction("BellIdle");
}

protected func FxClockStop(object pTarget, int iEffectNumber, int iReason, bool fTemp) {
}

private func SoundBell()
{
  //kleiner Hack der notwendig wurde, damit man die Glocke bei jedem Schlag hört
  if (bell==1)
   {
    var Klangstein= CreateContents(WOOD,this);
    Sound("DarkBell",1,Klangstein);
    bell=2;
    RemoveObject(Klangstein);
   }
  if (bell==2)
   {
    var Klangstein2= CreateContents(WOOD,this);
    Sound("DarkBell1",1,Klangstein2);
    bell=1;
    RemoveObject(Klangstein2);
   }
  return(1);
}
