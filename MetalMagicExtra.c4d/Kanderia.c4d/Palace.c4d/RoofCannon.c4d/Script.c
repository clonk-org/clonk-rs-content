/*-- Turmgeschütz --*/

#strict 2
#include CPW2 // Burgteilfunktion

/* Initialisierung */

protected func Initialize()
{
  // Tür ist immer offen
  SetEntrance(1);
  // Transferzone
  UpdateTransferZone();
  //Kanonenhelfer erzeugen
  if(!FindObject(RFCH,0,0,0,0,0,"Attach",this))
  {
  
  var o=CreateObject(RFCH,0,0,-1);
  ObjectSetAction(o,"Attach",this);
  //SetActionData(256*1 +4,o);
  
  }
//zeitverzögert was machen
ScheduleCall(this,"TimeInitialize",1,0);

//erstmal auf turm anpassen
FindCastleLikeTowerAndFit();
// Basisimplementation
return _inherited();
}


protected func TimeInitialize()
{
if(basement)RemoveObject(basement);
}

//Spezi ausrichtung
protected func FindCastleLikeTowerAndFit()
{
var towerwidth=40;
var maybe=10, heigth=80, xval=0;
var lowestvertex=39;

for(var castlepart;castlepart=FindObject(0,-maybe,0,maybe*2,heigth,0,0,0,NoContainer(),castlepart);)
{
if(GetDefWidth(GetID(castlepart))!=towerwidth)continue;
if(castlepart==this)continue;
if(!castlepart->~CastlePartTop())continue;
SetPosition(GetX(castlepart)+xval,GetY(castlepart)+(castlepart->CastlePartTop())-lowestvertex,this);
break;
}

var o;
if(o=FindObject(CPB1,-25,0,50,45,0,"Attach") && false)RemoveObject(o);
if(o=FindObject(CPB2,-25,0,50,45,0,"Attach") && false)RemoveObject(o);
}
/* Ausrichtung von Burgbauteilen */

public func CastlePartWidth() { return (44); }
public func CastlePartHeight() { return (46); }
public func CastlePartTop() { return (-20); }
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
  LocalN("collection",pCase)=0;
  Schedule("LocalN(\"collection\")=1",2,0,pCase);
  pCase->SetEntrance(1);
  //Schedule("SetEntrance(0)",2,0,pCase);
  FinishCommand(Par(),1);
  TransferContents(pCase,1);
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

private func TransferContents(object pTarget,above)
{
  // Von oben runterzählen, damit Sachen wie TransferContents(this) funktionieren.
  var i;
  for (i = ContentsCount() - 1; i >= 0; --i)
  {
    var obj = Contents(i);
    if (!obj) continue;
    Enter(pTarget, obj);
    if(!above)AddCommand(obj, "Exit");
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
  return FindObject(RFCH,-1,-1,-1,-1,0,"Attach",this);
}

private func FindStaircaseBelow()
{
  return FindStaircase(0, CastlePartHeight()/2 + 10) ;
}

/* Status */

public func IsStaircase() { return 1; }
public func StaircaseExit() { return -5; } // Oberer Ausgang (-25)

/* Transfer */

public func UpdateTransferZone()
{
  /*// Nur mit darüberliegendem Niedergang
  if (FindStaircaseAbove() )
    if (GetOCF() & OCF_Fullcon )
      return SetTransferZone(-8, -40, 16, 61);
  // Ohne darüberliegenden Niedergang (kein Auf- oder Abstieg möglich)*/
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
 // if (!pCase)
   // return HandleTransferDescendPlatform(pObj, tx, ty);
    
  if (Contained(pObj) != pCase)
    return AddCommand(pObj, "Enter", pCase);
  
  return pCase->ContainedDown(pObj);
} 

//Kaputtet
//Halb kaputt?
protected func Damage()
{
ScheduleCall(FindObject(RFCH,0,0,0,0,0,"Attach",this),"Check4Target",1,0);

return _inherited(Par(),Par(1),Par(2));
}
//Gesamt kaputt
protected func Destruction()
{
RemoveCannonHelper();
return _inherited(Par(),Par(1),Par(2));
}
//anbrennen
protected func Incineration()
{
RemoveCannonHelper();
return _inherited(Par(),Par(1),Par(2));
}
//anbrennen, auch so
protected func IncinerationEx()
{
RemoveCannonHelper();
return _inherited(Par(),Par(1),Par(2));
}
//Funktion
protected func RemoveCannonHelper()
{
if(Var()=FindObject(RFCH,0,0,0,0,0,"Attach",this))Var()->LooseTarget();
}
/* Forschung */

public func GetResearchBase() { return CPW2; }



