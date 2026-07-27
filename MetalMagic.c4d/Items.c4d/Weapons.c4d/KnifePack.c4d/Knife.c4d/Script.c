/* --- Wurfmesser --- */
#strict 2

#include ARRW

/* Pfeileigenschaften */
public func PackTo() { return KNFP; }
public func IsArrow() { return 0; }

local depart;
local pLauncher;

local iOldX, iOldY;

protected func Departure(pObj) {
  if(depart) return;
  pLauncher = pObj;
  // Wurfparameter berechnen
  var iX, iY, iR, iXDir, iYDir, iRDir;
  // Messerwurf (wie Speerwurf)
  iX = 10; if (!GetDir(pObj)) iX = -iX;
  iY = -6;
  iR = 150; if(!GetDir(pObj)) iR = 180 - iR;
  iXDir = 5; if(!GetDir(pObj)) iXDir = -iXDir;
  iYDir = -2;
  iRDir = 4; if(!GetDir(pObj)) iRDir = -iRDir;
  depart=1;
  // Effekt zum Deaktivieren des Pfeilpackens
  AddEffect("IntNoPacking", pObj, 1);
  // Rauswerfen
  Enter(pObj);
  Exit(this, iX, iY, iR, iXDir, iYDir, iRDir);
  // Effekt wieder entfernen
  RemoveEffect("IntNoPacking", pObj);
  // Schussaktion und Sicht setztn
  SetAction("Shot");
  SetPlrView(GetOwner(pObj), this);
  SetCategory(4);
  // Koordianten speichern
  iOldX=GetX();
  iOldY=GetY();
}

protected func Hit() {
  if(GetAction()=="Shot")
  {
    var target = FindTarget();
    if(target)
      return HitTarget(target);
    SetCategory(16);
  }
  Sound("KnifeHit");
  SetAction("Idle");
  depart=0;
  return;
}

/* Ziel prüfen */
private func InFlight()
{
  var target = FindTarget();
  if(target)
    return HitTarget(target);
}

protected func FindTarget()
{
  var obj;
 
  var x = GetX()-iOldX;
  var y = GetY()-iOldY;

  iOldX=GetX();
  iOldY=GetY();

  if(GetActTime()<5) return;

  var obj;
  var length = Abs(x/4);
  if(Abs(GetXDir()) < Abs(GetYDir())) length = Abs(y/4);

  var cX,cY;
  for(var cnt=length;cnt;cnt--)
  {
    cX=-x*cnt/length;
    cY=-y*cnt/length;
    if(obj=FindObject2(Find_AtPoint(cX,cY),Find_Layer(GetObjectLayer()),Find_NoContainer(),Find_OCF(OCF_Alive, OCF_Living)))
      if(pLauncher != obj)
        return obj;
  }
 
  return 0;
}

/* Treffer */

private func HitTarget(pObject)
{
  Sound("KnifeHit");
  var iDamage = 25;
  iDamage = iDamage * (100 - pObject->~IsShielded()) / 100; 
  DoEnergy(-iDamage,pObject);
  if(iDamage > Random(60))
    Fling(pObject,GetXDir()/80,GetYDir()/80-2);
  SetXDir(0); SetYDir(0);
  SetAction("Idle");
  depart=0;
  SetCategory(16);
}
