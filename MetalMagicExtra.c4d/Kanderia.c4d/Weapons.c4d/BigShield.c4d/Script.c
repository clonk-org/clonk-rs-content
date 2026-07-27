/*-- Schild --*/

#strict 2

#include SHIE

/* Intialisierung */
public func Initialize()
{
  //if(MMGetPlayerChurch(GetOwner())==scBloodchurch) SetGraphics("Dark");
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
  if(pKnight->~WearShield(this, "BigShield"))
    return 1;
  // sonst altes Anlegen  
  Sound("Connect");
  SetOwner(GetOwner(pKnight));
  ChangeDef(SHA2);
  //if(MMGetPlayerChurch(GetOwner())==scBloodchurch) SetGraphics("Dark");
  SetAction("Shield",pKnight);
  return 1;
}
