/* Symbol */

#strict 2

local idThisSpell, pPaladin;

protected func AttachTargetLost() { return RemoveObject(); }

public func SetSymbolGfx(id idSpell, object pTarget, object pPala)
{ 
  // Bild und Name setzen
  SetGraphics(0, this, idSpell, GFX_Overlay, GFXOV_MODE_IngamePicture);
  SetName(GetName(0, idSpell));
  // Werte speichern
  idThisSpell = idSpell;
  pPaladin = pPala;
  // Anwählbar
  SetCategory(GetCategory() | C4D_MouseSelect);

  // Default-Aktivität
  SetAction("Symbol64x64", pTarget);

  // Fertig
  return 1;
}

protected func RecheckSymbol()
{
  // Wundersymbol-Timer: Färbung danach, ob der Paladin genug Karmaenergie besitzt
  if(GetEffect(Format("Int%iDelay", idThisSpell), pPaladin))
  {
    SetGraphics(0, this, idThisSpell, 2, GFXOV_MODE_IngamePicture);
    SetClrModulation(RGBa(0,80,100,100), 0, 2);
    SetClrModulation(1, 0, GFX_Overlay);
    SetObjectBlitMode(GFX_BLIT_Mod2, 0, GFX_Overlay);
  }
  else if(pPaladin->~GetKarmaEnergy() < GetValue(0,idThisSpell))
  {
    SetGraphics(0, this, idThisSpell, 2, GFXOV_MODE_IngamePicture);
    SetClrModulation(RGBa(150,80,0,100), 0, 2);
    SetClrModulation(1, 0, GFX_Overlay);
    SetObjectBlitMode(GFX_BLIT_Mod2, 0, GFX_Overlay);
  }
  else
  {
    SetGraphics(0, this, 0, 2);
    SetClrModulation(16777215, 0, GFX_Overlay);
    SetObjectBlitMode(0, 0, GFX_Overlay);
  }
  return true;
}

protected func MouseSelection(iByPlr) { return GetActionTarget()->MouseSelection(this); }
