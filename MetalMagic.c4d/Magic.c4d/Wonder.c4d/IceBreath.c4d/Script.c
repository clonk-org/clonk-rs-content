/* Eis */

#strict 2

func Activate(pCaster,pRealcaster)
{    
  var pClonk = pRealcaster;
  if(!pClonk) pClonk = pCaster;
  
  // has to stand on the ground
  if(!GetContact(pClonk,-1,CNAT_Bottom)/* !pClonk->~IsWalking()*/)
  {
   PlayerMessage(pClonk->GetOwner(), "$OnTheGround$", pClonk);
   Sound("Error", 0, 0, 0, pClonk->GetOwner());
   return 0;
  }
  
  AddEffect("Breathing",pClonk,250,2,0,GetID());
  RemoveObject();
  return 1;
}


func FxBreathingTimer(pTarget, iEffectNumber, iEffectTime)
{
  if(iEffectTime > 35*1) return -1;
  if(GetAction(pTarget) == "Walk")SetXDir(0,pTarget);
  var dir=1;
  if(GetDir(pTarget) == DIR_Left)dir=-1;
  var h=CreateObject(MHIC,GetX(pTarget)+6*dir,GetY(pTarget)-5,GetOwner(pTarget));
  h->SetController(GetOwner(pTarget));
  h->SetSpeed(40*dir,RandomX(-10,10));
  h->Except(pTarget);

  return 1;
}


// Ist ein Paladinwunder
//public func IsWonder()   { return 1; }

// Nur für die Lichtkirche
//public func NeedChurch() { return scLightchurch; }

// Priorität, für Einordung ins Ringmenü
public func GetWonderPriority() { return 20; }

public func GetSpellClass(object pMage) { return(MISC); }
public func GetSpellCombo(pMage) { return ("444"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
