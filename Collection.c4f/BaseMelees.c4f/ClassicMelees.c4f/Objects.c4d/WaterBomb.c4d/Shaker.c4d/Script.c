/*-- Shaker --*/

#strict

local iIntensity, iCount, pAim;
local iObjX, iObjY;

func Activate(pAimBy,iIntensityBy)
{
  pAim=pAimBy;
  iIntensity=iIntensityBy;
  iObjX=GetX(pAim);
  iObjY=GetY(pAim);
  iCount=0;
  SetAction("Shaking");
}

func Shake()
{
  ++iCount;
  var xdir=RandomX(-iIntensity,iIntensity);
  var ydir=RandomX(-iIntensity,iIntensity);
  SetPosition(iObjX+xdir,iObjY+ydir,pAim);
  if(iCount%10==0) iIntensity--;
  if(iIntensity==0) RemoveObject();
}
