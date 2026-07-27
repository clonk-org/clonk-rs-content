/*-- Schild --*/

#strict 2

/* Intialisierung */
public func Initialize()
{
  if(MMGetChurch(GetOwner())==scBloodchurch) SetGraphics("Dark");
}

public func OnChurchChange(iChurch)
{
  if(iChurch==scBloodchurch) SetGraphics("Dark");
  else SetGraphics();
}

/* Status */
public func IsShield()
{
  return 1;
}

public func EnterRack(iPos)
{
  if(iPos==0)
  {
    SetAction("Rack"); SetDir(0);
    SetObjDrawTransform(1000, 0,-3000, 0, 1000, 4000);
  }
  if(iPos==1)
  {
    SetAction("Rack"); SetDir(1);
    SetObjDrawTransform(1000, 0,3000, 0, 1000,1000);
  }
  if(iPos==2)
  {
    SetAction("Rack"); SetDir();
    SetObjDrawTransform(800, 50,-3000, 0, 1000,-9000);
  }
  if(iPos==3)
  {
    SetAction("Rack"); SetDir(2);
    SetObjDrawTransform(1000, 0,3000, 0, 1000,-10000);
  }
}

public func LeaveRack()
{
  SetDir(0,this);
  SetAction("Idle");
  SetObjDrawTransform();
}

public func GetShieldGraphics(pKnight)
{
  if(GetID()!=SHIE) return;
  if(MMGetChurch(GetOwner(pKnight))==scBloodchurch)
    return "ShieldDark";
  return "Shield";
}

/* Anlegen */
public func Activate(pKnight)
{      
  [$CtrlActivateDesc$]             
  // Der Clonk kann dieses Objekt nicht verwenden
  if (!pKnight->~CanUse(GetID())) return 0;
  // Ritter trägt schon ein Schild
  if (pKnight->~HasShield()) return 0;
  // Versuchen als Grafik anzulegen
  if(GetShieldGraphics())
   if(pKnight->~WearShield(this, GetShieldGraphics(pKnight)))
    return 1;
  // sonst altes Anlegen  
  Sound("Connect");
  SetOwner(GetOwner(pKnight));
  ChangeDef(SHIA);
  if(MMGetPlayerChurch(GetOwner())==scBloodchurch) SetGraphics("Dark");
  SetAction("Shield",pKnight);
  return 1;
}

/* Aufschlag */  
protected func Hit()
{
  Sound("MetalHit*");
}

/* Kann geschmiedet werden */
protected func IsAnvilProduct() { return 1; }
