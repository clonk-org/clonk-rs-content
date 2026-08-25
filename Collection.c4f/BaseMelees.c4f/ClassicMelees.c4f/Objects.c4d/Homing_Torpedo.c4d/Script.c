/*-- Zielsuchender Torpedo --*/

#strict

#include TRP1

/* Effekte und Zielsuche */

private func Swimming() {
  var iDir=(GetDir()*2-1);
  Bubble(-16*iDir,0);
  Bubble(-16*iDir,0);
  if (GetActTime() > 25) SetYDir(0);
}

private func Homeing() {
  var pTarget = GetTarget();
  if (!pTarget) return();
  var i =  Cos( Angle( GetX(), GetY(), GetX(pTarget), GetY(pTarget), 10 ), 10 );
  var iDir = -1+2*GetDir();
  var ihyp = Sqrt(GetXDir()**2+GetYDir()**2);
  var j = !GetDir();
  var TargetAngle = Angle(GetX(), GetY(), GetX(pTarget), GetY(pTarget)) - 90 +180*j;
  SetR(TargetAngle);
  SetXDir( Cos(GetR(),ihyp)*iDir, 0, 10); 
  SetYDir( Sin(GetR(),ihyp)*iDir, 0, 10);
}

func CheckTarget()
{
 var angle=GetR();
 var target=0;
 for(var sub in FindObjects(Find_ID(SUB1),Find_Distance(500),Sort_Distance()))
 {
  if(!Inside(Angle(GetX(),GetY(),GetX(sub),GetY(sub)),angle-30,angle+30))continue;
  target=sub;
  return(target);
 }
}

private func GetTarget() {
  var pTarget;
  while(pTarget = FindObject(SUB1,0,0,-1,-1,0,0,0,NoContainer(),pTarget))
    if ((GetX()-GetX(pTarget))*(1-2*GetDir()) > 0)
      if (Abs(GetY()-GetY(pTarget)) < 50)
        if (ObjectDistance(pTarget) < 200)
          if (GetDefCoreVal("Mass", "DefCore", GetID(pTarget)) > 99) break;
  return(pTarget);
}
/* Forschung */

public func GetResearchBase() { return(TRP1); }

