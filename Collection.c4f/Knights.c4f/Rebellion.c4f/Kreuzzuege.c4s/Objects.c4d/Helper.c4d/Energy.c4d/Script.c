/*-- Lebensanzeige --*/

#strict

local iLastHealth, iLastChange, iAlpha, fAllied;

public func AttachTo(object pClonk)
{
  // Attach-Aktion setzen
  SetAction("AttachToClonk", pClonk);
  // Variablen auf Anfangswerte setzen
  iLastHealth = GetEnergy(pClonk);
  iLastChange = FrameCounter()-50;
  SetClrModulation(RGBa(255,255,255,iAlpha=255));
  return(Check());
}

public func SetAllied(bool allied)
{
  if(fAllied = allied)
  {
    SetGraphics("Allied", this(), GetID());
    SetClrModulation(RGBa(255,255,255,iAlpha=50));
    SetVisibility(VIS_Allies | VIS_God);
  }
  else
    SetGraphics("", this(), GetID());
  return(true);
}

protected func AttachTargetLost()
{
  return(RemoveObject());
}

protected func Check()
{
  var pClonk = GetActionTarget();
  // Kein lebender Clonk?
  if(!pClonk) return(RemoveObject());
  if(!GetAlive(pClonk)) return(RemoveObject());
  // Werte berechnen
  var iHealth = GetEnergy(pClonk);
  var iMaxHealth = GetPhysical("Energy", 0, pClonk) / 1000;
  // Aktionsphase setzen
  var d = iHealth*1000/iMaxHealth;
  SetObjDrawTransform(d, 0, -0*18*(1000-d)/2, 0, 1000, -20000); 
  // Anm.: Um die Leiste nach links schrumpfen zu lassen, einfach das 0* entfernen!

  // Einblenden, wenn Schaden zugefügt
  if(iHealth < iLastHealth)
  {
    iLastHealth = iHealth;
    iLastChange = FrameCounter();
    iAlpha = 0;
  }
  // Ansonsten ausblenden
  else
    if(FrameCounter() - iLastChange > 35)
      iAlpha = BoundBy(iAlpha+20, 0, 255);

  // Alpha-Wert setzen
  if(!fAllied) SetClrModulation(RGBa(255,255,255, iAlpha));

  return(1);
}

