/* Symbol */

#strict 2

local pTarget;
local pStaff;
local iTime;
local fFixedSymbol;

public func SetFixedSymbol(idPict, pNewTarget, iX, iY, szGraphic)
{
  fFixedSymbol = 1;
  pTarget = pNewTarget;
  SetGraphics(szGraphic, this, idPict,1,GFXOV_MODE_IngamePicture);
  SetPosition(iX,iY);
  SetVisibility(VIS_Owner);
}

public func SetStaff(pStaff, pNewTarget)
{
  pTarget = pNewTarget;
  Enter(this, pStaff);
  SetGraphics(0, this, GetID(pStaff),3,GFXOV_MODE_IngamePicture);
  SetGraphics(0, this,          ABLA,1,GFXOV_MODE_IngamePicture);
  SetGraphics(0, this,          ABLA,2,GFXOV_MODE_IngamePicture);
  SetClrModulation(pStaff->~GetStaffColor(), this, 1);
  SetClrModulation(GetClrModulation(this, 1), this, 2);
  SetPosition(20,-25);
  SetVisibility(VIS_Owner);
}

protected func Timer()
{
  var pCursor = GetCursor(GetOwner(pTarget));
  if(pCursor == pTarget || GetActionTarget(0, pCursor)==pTarget)
    SetVisibility(VIS_Owner);
  else SetVisibility(VIS_None);
  if(fFixedSymbol) return;
  SetPosition(20+40*Min(ContentsCount(0, pTarget),1),-25);
  if(!Mod(iTime,35)) if(FindObject(STES)) DoMagicEnergy(1, pTarget);
  SetDTRotation(iTime++, Abs(Sin(iTime, 200))+800, Abs(Sin(iTime, 200))+800, 0, 1);
  SetDTRotation(-iTime, Abs(Sin(90-iTime, 200))+800, Abs(Sin(90-iTime, 200))+800, 0, 2);
}

public func SetDTRotation (int r, int xoff, int yoff, object obj, int iLayer) {
  var fsin=-Sin(r, 1000), fcos=Cos(r, 1000);
  // set matrix values
  SetObjDrawTransform (
    +fcos*xoff/1000, +fsin, 0*1000,//(1000-fcos)*xoff - fsin*yoff,
    -fsin, +fcos*yoff/1000, 0*1000,//(1000-fcos)*yoff + fsin*xoff,
    obj, iLayer
  );
}
