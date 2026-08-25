/*-- Captain-Anzeige --*/

#strict

local iAlpha, iAlphaDir;

protected func AttachTargetLost() 
{
  RemoveObject();
  return true;
}

public func RestoreAlpha()
{
  if(ActIdle() || !GetAlive(GetActionTarget())) RemoveObject();

  iAlpha+=iAlphaDir;
  if((iAlpha > 400  && iAlphaDir > 0) ||
     (iAlpha < 100 && iAlphaDir < 0)) iAlphaDir*=-1;
  
  SetClrModulation(RGBa(255, 255, 255, BoundBy(iAlpha,0,255)));
  return true;
}

global func MakeCaptain(object pClonk)
{
  var pCaptain = CreateObject(_CAP, 0,0, GetController(pClonk));
  SetVisibility(VIS_God | VIS_Owner | VIS_Allies, pCaptain);
  pCaptain->SetAction("Hang", pClonk);
  pCaptain->LocalN("iAlpha") = 300;
  pCaptain->LocalN("iAlphaDir") = -10;
  pCaptain->RestoreAlpha();
  return pCaptain;
}

global func IsCaptain(object pClonk)
{
  if(FindObject(_CAP,,,,,, "Hang", pClonk))
    return true;
  return false;
}

