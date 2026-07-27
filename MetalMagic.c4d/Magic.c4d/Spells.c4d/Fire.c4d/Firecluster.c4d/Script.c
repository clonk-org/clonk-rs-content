/* Feuerkluster */

#strict 2

func Activate(pCaster,pRealcaster) {
  var pClonk;
  if(pRealcaster) pClonk=pRealcaster;
  else pClonk=pCaster;

  // Nicht im Gebäude zaubern
  if(Contained(pClonk))
  {
    Sound("Error");
    PlayerMessage(GetOwner(pClonk), "$must_be_in_free$", pClonk);
    RemoveObject();
    return 0;
  }

  // Zielen wenn möglich
  if (pClonk->~DoSpellAim(this)) return 1;
  
  var angle=-90;
  if(GetDir(pClonk) == DIR_Right) angle=90;
  ActivateAngle(pClonk,angle);
  
  RemoveObject();
  return 1;
}

// Aktivierung nach Zielen (iAngle: -90=links; 0=oben; +90=rechts)
public func ActivateAngle(object pCaller, int iAngle)
  {
  
  for(var cnt=0;cnt<3;cnt++)
  {
  var f=CreateObject(FRST,AbsX(GetX(pCaller))+Cos(iAngle-90,20),AbsY(GetY(pCaller))+Sin(iAngle-90,20),GetOwner(pCaller));
  f->Activate(pCaller,iAngle);
  }
  AddEffect("RecallMFCSAiming",pCaller,20,1,0,GetID(),iAngle);
  
  // Objekt löschen
  RemoveObject();
  return 1;
  } 
//ScheduleCall(0,Format("ClrMessage(%s,Object(%d))",sMessageSplit,ObjectNumber(pObject)),iSpeed);

public func HoldAimer(pCaller)
{
 return true;
}

protected func FxRecallMFCSAimingStart(pTarget,iEffectNumber,bTemp,par1){
EffectVar(0,pTarget,iEffectNumber)=par1;
EffectVar(1,pTarget,iEffectNumber)=0;
EffectVar(2,pTarget,iEffectNumber)=0;
return 1;}

protected func FxRecallMFCSAimingTimer(pTarget,iEffectNumber)
{
 if(GetAction(pTarget) == "Magic")return 0;
 
 if(!EffectVar(1,pTarget,iEffectNumber))
 {
   pTarget->~DoMagic(MFCS);
   EffectVar(1,pTarget,iEffectNumber)=1;
 }
 
 if(!EffectVar(2,pTarget,iEffectNumber) && GetAction(pTarget) == "AimMagic")
 { 
   var pAimer=FindObject(AIMR,0,0,0,0,0,0,pTarget,NoContainer());
   if(pAimer)
   {
   LocalN("iAngle",pAimer)=EffectVar(0,pTarget,iEffectNumber);
   LocalN("pArrow",pAimer)->~UpdateAngle(EffectVar(0,pTarget,iEffectNumber));
   pAimer->~UpdateAngle(EffectVar(0,pTarget,iEffectNumber));
   pTarget->~AimingAngle(EffectVar(0,pTarget,iEffectNumber));
   EffectVar(2,pTarget,iEffectNumber)=1;
   }
 }
 if(!WildcardMatch(GetAction(pTarget),"*Magic*") && !WildcardMatch(GetAction(pTarget),"*Aim*"))return -1;
 
 if(EffectVar(2,pTarget,iEffectNumber) && EffectVar(1,pTarget,iEffectNumber))return -1;
}

/* Zaubercombo */

public func GetSpellStaff(object pMage) { return SMFR; }
public func GetSpellStaffCombo(pMage) { return ("333"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
