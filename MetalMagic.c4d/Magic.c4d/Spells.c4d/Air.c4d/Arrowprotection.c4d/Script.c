/* Schweben */

#strict 2

public func Activate(oCaller, oClonk)
{                            
  if(!oClonk) oClonk = oCaller;
  // Effekt prüfen
  var iChkEff;
  if (iChkEff = CheckEffect("FloatPSpell", this, 180)) return iChkEff!=-1 && RemoveObject();
  
  AddEffect("FloatPSpell",oClonk,280,2,0,GetID());
  
  RemoveObject(this);
  
  return true;
}

func FxFloatPSpellStart(pClonk, iEffectNumber, iTemp)
{
  if(iTemp) return;
  EffectVar(0,pClonk,iEffectNumber)=200;
  EffectVar(1,pClonk,iEffectNumber)=CreateObject(AIPA);
  ObjectSetAction(EffectVar(1,pClonk,iEffectNumber), "Exist", pClonk);
}

func FxFloatPSpellTimer(pClonk, iEffectNumber, iEffectTime)
{
  if(!EffectVar(0,pClonk,iEffectNumber)) return -1;
  EffectVar(0,pClonk,iEffectNumber)--;
  
  var iSearchRadius = GetRange();
  var pObj;
  CreateParticle("Aura",GetX(pClonk),GetY(pClonk), RandomX(-1,1),RandomX(-1,1), 12*iSearchRadius,RGB(50,255,180));
  var i=2;
  while(i--)
  {
    var angle = RandomX(-180, 180);
    CreateParticle("PSpark",GetX(pClonk)+Sin(angle, iSearchRadius),GetY(pClonk)-Cos(angle, iSearchRadius),
            -Sin(angle, iSearchRadius/3),Cos(angle, iSearchRadius/3),RandomX(50,70), 
            RGBa(150,155,80, 75),pClonk);
  }
  CastParticles("PSpark", 3, iSearchRadius/3, GetX(pClonk), GetY(pClonk)+8, 50, 70, RGBa(150,155,80, 75), RGBa(150,155,80, 75), pClonk);
  // Schnelle Objekte in der Umgebung suchen
  for(var pObj in FindObjects(Find_OCF(OCF_HitSpeed1),Find_Layer(GetObjectLayer(pClonk)), Find_Distance(iSearchRadius, GetX(pClonk),GetY(pClonk)),
   Find_Or(Find_Func("IsDangerousToAirTravel"), Find_Func("IsArrow"))))
     if( pObj->~IsDangerousToAirTravel() || pObj->~IsArrow())
      EffectVar(1,pClonk,iEffectNumber)->~DoStop(pObj, pClonk);
  return 1;
}

func FxFloatPSpellStop(pClonk, iEffectNumber, iReason, iTemp)
{
  if(iTemp) return;   
  RemoveObject(EffectVar(1,pClonk,iEffectNumber));
}

private func FxFloatPSpellEffect(string szNewEffectName, object pTarget, int iNumber) 
{ 
  if(szNewEffectName == "FloatPSpell") { return -2; }
}

private func FxFloatPSpellAdd(object pTarget, int iNumber, string szNewEffectName, int iNewEffectTimer, int iChange0, int iChange1, int iChange2, int iTime) {
  // Effekt übernehmen
  EffectVar(0,pTarget,iNumber) += 300;
}

protected func FxFloatPSpellInfo() { return "{{AIPA}} $SpellInfo$"; }

private func DoStop(object pObj, object pCaller)
 {
  if(!pObj->~IsArrow() || (pObj->~IsArrow() && GetCategory(pObj)&C4D_Object))
  {
   // Hat schon einen Effekt? beenden
   if(GetEffect("StopNSpell", pObj)) return;
   pObj->~MagicStop(pCaller);
   // In der Liste speichern
   Local()++;
   Local(Local()) = pObj;
   // StillhalteEffekt
   AddEffect("StopNSpell", pObj, 200, 1, 0, GetID());
   return 1;
  }
  
  var oneProtected=false;
  for(var clnk in FindObjects(Find_Distance(GetRange(),AbsX(GetX(pCaller)),AbsY(GetY(pCaller))),Find_Layer(GetObjectLayer(pCaller)),Find_OCF(OCF_Alive),Find_Allied(GetOwner(pCaller))))  
  {
   var iSkip=false;
   var iEffect;
   for(var i=0;iEffect=GetEffect("IntNoArrowHit",pObj,i);i++)
   {
    if(EffectVar(0,pObj,GetEffect("IntNoArrowHit",pObj,i)) != clnk)continue;
    iSkip=true;
    break;
   }
   if(iSkip){continue;}
   
   oneProtected=true;
   
   pObj->~NoArrowHit(clnk,40);
   clnk->~CreateParticle("PSpark",0,0,0,0,(GetCon(clnk)*100)/90,GetColorDw(pCaller),clnk,0);
  }
  if(oneProtected)
      pObj->~CreateParticle("PSpark",0,0,0,0,50,GetColorDw(pCaller),pObj,0);
  return 1;
 }

func FxStopNSpellTimer(pTarget, iNumber, iEffectTime)
{
  // Beenden    
  if(iEffectTime>600) return -1;
  // Stoppen
  SetXDir(0,pTarget);   
  SetYDir(0,pTarget);
  // Ist der Zauber zu weit weg? beenden
  if( !(pTarget->FindObject(AIPA, -100, -100, 200, 200))) return -1;
}

public func GetRange() { return 100; }

public func GetSpellStaff(object pMage) { return SMAR; }
public func GetSpellStaffCombo(pMage) { return ("26"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)

