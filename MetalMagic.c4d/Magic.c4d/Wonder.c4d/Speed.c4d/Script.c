/* Geschwindigkeit */

#strict 2
static const MSPD_Duration=500;
func Activate(pCaster,pRealcaster)
{ 
  var pClonk=pRealcaster;
  if(!pClonk) pClonk=pCaster;
  
  if(GetEffect("SpeedPSpell",pClonk))
  {
   RemoveObject();
   return 0;
  }
  
  AddEffect("SpeedPSpell",pClonk,250,2,0,GetID());
  RemoveObject();
  return 1;
}

global func MSPD_Physical(cnt)
{
 if(cnt==1) return "Walk";
 if(cnt==2) return "Swim";
 if(cnt==3) return "Hangle";
 if(cnt==4) return "Scale";
}

func FxSpeedPSpellStart(pTarget, iEffectNumber, iEffectTime)
{
 EffectVar(0,pTarget,iEffectNumber)=GetObjectBlitMode(pTarget);
 
 for(var cnt=1;cnt<=4;cnt++)
 {
   EffectVar(cnt*2,pTarget,iEffectNumber)=GetPhysical(MSPD_Physical(cnt),0,pTarget);
   SetPhysical(MSPD_Physical(cnt),EffectVar(cnt*2,pTarget,iEffectNumber)+RandomX(20000,20300),0,pTarget);
 }
}

func FxSpeedPSpellTimer(pTarget, iEffectNumber, iEffectTime)
{
  if(iEffectTime > MSPD_Duration) return -1;

  //Er kann auch schnelelr schlagen und so
  if(WildcardMatch(pTarget->GetAction(),"*Fight*"))pTarget->~Fighting();
  //er kann auch schneller schieﬂen! *froi*
  if (!(iEffectTime % 3)) // aber nicht zu schnell! (#1424)
    if(WildcardMatch(GetAction(pTarget),"*Load*"))
      if(GetPhase(pTarget)!=GetActMapVal("Length",GetAction(pTarget),GetID(pTarget))-1)
        SetPhase(GetPhase(pTarget)+1,pTarget);

  var obj;
  if(!Contained(pTarget) && !WildcardMatch(GetAction(pTarget),"TwoHandSword*"))
  {    
    obj=CreateObject(MSPD,AbsX(GetX(pTarget)),AbsY(GetY(pTarget))+32,-1);
    SetClrModulation(RGBa(255,255,255,255),obj,0);
    SetGraphics(0,obj,0,1,GFXOV_MODE_Object,0,GFX_BLIT_ClrSfc_OwnClr,pTarget);
    SetClrModulation(RGBa(255,200,200,100),obj,1);
    Schedule(Format("RemoveObject(Object(%d))",ObjectNumber(obj)),8,0,obj);
  }

  return 1;
}

func FxSpeedPSpellStop(pTarget, iEffectNumber, iEffectTime)
{
  for(var cnt=1;cnt<=4;cnt++)
    SetPhysical(MSPD_Physical(cnt),EffectVar(cnt*2,pTarget,iEffectNumber),0,pTarget);
}

func DoesMagic(pObj) { return WildcardMatch(GetAction(pObj), "*Magic"); }

protected func FxSpeedPSpellInfo() { return "{{MSPD}} $SpellInfo$"; }

// Ist ein Paladinwunder
//public func IsWonder()   { return 1; }

// Nur f¸r die Blutkirche
public func NeedChurch() { return scBloodchurch ; }

// Priorit‰t, f¸r Einordung ins Ringmen¸
public func GetWonderPriority() { return 20; }

public func GetSpellClass(object pMage) { return(MISC); }
public func GetSpellCombo(pMage) { return ("111"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
