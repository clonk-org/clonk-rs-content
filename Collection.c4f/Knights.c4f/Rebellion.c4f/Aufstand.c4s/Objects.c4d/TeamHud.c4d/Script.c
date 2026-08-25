/*-- Team-Hud --*/

#strict

/* 
  Anmerkung:
    Wenn der Spieler eliminiert wird, 
    muss das Team-Hud gelöscht werden!
*/


local clonk;

static const _THD_PortraitLayer = 1,
             _THD_BorderLayer = 2,
             _THD_EnergyLayer = 3;
             
local iArmor, pSelection;

protected func Initialize()
{
  SetCategory(C4D_StaticBack | C4D_Foreground | C4D_MouseIgnore | C4D_Parallax | C4D_IgnoreFoW);
  SetVisibility(VIS_Owner | VIS_Allies);
}

public func BindTo(object pClonk)
{
  clonk = pClonk;

  if(pSelection) RemoveObject(pSelection);
  pSelection = CreateObject(_THD);
  pSelection->SetPosition(GetX(), GetY());
  pSelection->SetVisibility(VIS_None);
  pSelection->SetGraphics("BorderSelected", 0, _THD);
  pSelection->SetObjDrawTransform(1000, 0, -1000, 0, 1000, -1000);

  var szPortraitGraphics;

  if(GetID(clonk) == CLNK)
    if(clonk->IsFarmer())
      szPortraitGraphics = "Farmer"; // Farmer
    else 
      szPortraitGraphics = Format("CLNK%d", Random(2));  // Normaler Clonk
  else if(GetID(clonk) == KNIG)
    if(clonk->~HasArmor())
      if(LocalN("idArmorType", clonk) == BRMR)
      {
        szPortraitGraphics = "BlackKNIG"; // Schwarze Rüstung
        iArmor = 2;
      }
      else
      {
        szPortraitGraphics = "ArmoredKNIG"; // Normale Rüstung
        iArmor = 1;
      }
    else
    {
      szPortraitGraphics = Format("KNIG%d", Random(2)); // Normaler Ritter
      iArmor = 0;
    }
  else if(GetID(clonk) == KING)
      szPortraitGraphics = "KING"; // König

  var owner = GetOwner(clonk);
  SetOwner(owner);
  pSelection->SetOwner(owner);
  
  SetGraphics(szPortraitGraphics, 0, _THD, _THD_PortraitLayer, GFXOV_MODE_Base);
  SetGraphics("Border", 0, _THD, _THD_BorderLayer, GFXOV_MODE_Base);
  SetGraphics("Energy", 0, _THD, _THD_EnergyLayer, GFXOV_MODE_Base);

  return(1);
}

protected func Update()
{
  if(GetOwner() == NO_OWNER) return(RemoveObject());
  if(!clonk) return();
  if(!GetAlive(clonk))
  {
    clonk = 0;
    if(pSelection) RemoveObject(pSelection);
    SetClrModulation(RGBa(200,200,200,128));
    SetClrModulation(RGBa(200,200,200,128), 0, _THD_PortraitLayer);
    SetClrModulation(RGBa(200,200,200,128), 0, _THD_BorderLayer);
    SetGraphics(0, 0, 0, _THD_EnergyLayer);
    return();
  }
  // Clonk ausgewählt?
  if(clonk == GetCursor(GetOwner(clonk))) //if(IsSelected(clonk))
    pSelection->SetVisibility(VIS_Owner);
  else
    pSelection->SetVisibility(VIS_None);
  // Bei Rittern immer das Rüstungsbild überprüfen
  if(GetID(clonk) == KNIG)
  {
    var armor;
    if(clonk->~HasArmor())
      if(LocalN("idArmorType", clonk) == BRMR)
        armor = 2;
      else 
        armor = 1;
    else 
      armor = 0;
    if(armor != iArmor) BindTo(clonk);
  }
  // Energie des Clonks von 0-1000
  var energy = 1000*1000 * GetEnergy(clonk) / GetPhysical("Energy", 0, clonk);
  // Höhe der Energieleiste setzen
  return(SetObjDrawTransform(1000, 0, 0, 0, energy, 30*(1000-energy)/2, this(), _THD_EnergyLayer));
}

/*
private func IsSelected(object pClonk)
{
  var obj, i;
  while(obj=GetCursor(GetOwner(pClonk), i++))
    if(obj == pClonk)
      return(true);
}
*/

