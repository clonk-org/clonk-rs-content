/*-- Lebensamulett --*/

#strict 2

local moved;

protected func Initialize()
{
  AddEffect("Color",this,20,1,this);
}

protected func Hit()
{
  Sound("Crystal*");
  return 1;
}

protected func Regeneration()
{
  if(~GetOCF(Contained()) & OCF_CrewMember) return;
  if(GetEnergy(Contained()) == GetPhysical("Energy",1,Contained())/1000) return;

  if(GetXDir(Contained()) || GetYDir(Contained()) || GetProcedure(Contained()) == "FIGHT") moved = 15;
  if(moved) moved--;
  if(moved) return;

  // Effekte
  CreateParticle("PSpark",0,-1,0,-2,200,RGB(255,0,0),Contained());
  // nur Clonks
  if(~GetOCF(Contained()) & OCF_CrewMember)return;
  // Lädt die Lebensenergie des tragenden Clonks auf
  DoEnergy(+1,Contained());
  return 1;
}

public func Activate(caller)
{
  [$TxtActivate$]
  // wenn Träger nicht brennt Fehlermeldung
  if(!OnFire(caller)) return PlayerMessage(GetOwner(caller),"$TxtExtinguish$",this);
  // Löschen
  Extinguish(caller);
  // ein wenig Energie
  DoEnergy(+10, Contained());
  // Effekte
  Sound("Energize");
  CastParticles("Fire",10,20,0,0,50,90,RGB(255,255,255),RGB(255,255,255),caller);
  CastParticles("Fire2",10,20,0,0,50,90,RGB(255,255,255),RGB(255,255,255),caller);
  // Amulett wird entfernt
  RemoveObject();
  return 1;
}
  
protected func LightningStrike()
{
  if(~GetOCF(Contained()) & OCF_CrewMember) return;
  var cnt=10+Random(10);
  while(cnt-- && GetEnergy(Contained())>5) DoEnergy(-1,Contained());
  return 1;
}

// Routineabfragen für Herstellung
public func IsWizardTowerProduct() { return 1; }

protected func FxColorStart(pTarget, iEffectNumber)
{
  EffectVar(0, pTarget, iEffectNumber)=128;
  EffectVar(1, pTarget, iEffectNumber)=0;
  EffectVar(3, pTarget, iEffectNumber)=0;
}

protected func FxColorTimer(pTarget, iEffectNumber)
{
  if(!pTarget) return -1;

  if(Contained(pTarget))
  {
    if(pTarget != Contained(pTarget)->FindObject(GetID(pTarget), 0, 0, 0, 0, 0, 0, 0, Contained(pTarget)))
      return 1;
  }
 
  if(!EffectVar(3, pTarget, iEffectNumber)) EffectVar(3, pTarget, iEffectNumber) = RandomX(-4,4);
 
  EffectVar(1, pTarget, iEffectNumber) += EffectVar(3, pTarget, iEffectNumber);
  var cos = Cos(EffectVar(1, pTarget, iEffectNumber),EffectVar(3, pTarget, iEffectNumber)*50);
  if(cos>240) EffectVar(3, pTarget, iEffectNumber)=-4;
  if(cos<64) EffectVar(3, pTarget, iEffectNumber)=+4;
 
  if(Contained(pTarget))
  {
    var i, pObj;
    while(pObj = Contents(i++, Contained(pTarget))) if(GetID(pObj)==GetID(pTarget))
      SetColorDw(RGB(BoundBy(Abs(cos),64,254),0,0),pObj);
  }
  else SetColorDw(RGB(BoundBy(Abs(cos),64,254),0,0),pTarget);
  return 1;
}




