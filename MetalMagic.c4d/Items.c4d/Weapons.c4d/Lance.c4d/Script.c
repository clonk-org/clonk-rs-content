/*-- Lanze --*/

#strict 2

static const LANC_LanceLayer=8; // Der Layer der Lanzengrafik

/* Status */
public func IsLance()
{
  return 1;
}

/* Anlegen */
public func Activate(pClonk)
{           
  [$CtrlActivateDesc$]        
  // Der Clonk kann dieses Objekt nicht verwenden
  if (!pClonk->~CanUse(GetID())) return 0;
  // Ritter trägt schon eine Lanze
  if (pClonk->~HasLance()) return 0;
  // Anlegen
  Sound("Connect");
  ChangeDef(LNCA);
  LocalN("iVis") = GetVisibility();
  SetVisibility(GetVisibility() | VIS_OverlayOnly);
  SetObjDrawTransform(1000, 0, 0, 0, 1000,-1000*12);
  SetGraphics(0, pClonk, 0, LANC_LanceLayer, 5, 0, 0, this);
  LocalN("pClonk") = pClonk;
  SetAction("Lance",pClonk);
  return 1;
}
  
/* Aufschlag*/  
protected func Hit()
{
  Sound("WoodHit*");
}

protected func CollectCheck()
{
  // Die Lanze kann durch Anfassen eingesammelt werden
  var pObj;
  if (pObj = FindObject(0,0,0,0,0, OCF_CrewMember, "Push", this))
    if (!pObj->~RejectCollect(GetID(this), this))
      Enter(pObj);
}

/* Kann geschmiedet werden */
public func IsAnvilProduct() { return 1; }
