/*-- Drain --*/

#strict

local amount;
local drain;
local iMaterial;
protected func Initialize()
{
  SetAction("Drain");
  amount = 2;
  iMaterial = 1;
  return(1);
}

private func Draining()
{
  if(GBackSolid(0,-1)) SetPosition(GetX(), GetY()+1);	
  var i;
  var mat;
  // Je nach Index Material setzen
  if(iMaterial==1) mat = Material("Water");
  if(iMaterial==2) mat = Material("Acid");
  if(iMaterial==3) mat = Material("DuroLava");
  if(iMaterial==4) mat = Material("Oil");
  if(iMaterial==5) mat = Material("Water2");
  if(!drain)
  {
    i=amount;
    while(i--) InsertMaterial(mat,0,-1,RandomX(-6,6),40);
    return();
  }
  i = drain->ExtractMaterialAmount(0, 0, drain->GetMaterial(), amount);
  Message("%d %s", this(), i, MaterialName(mat));
  while(i--) InsertMaterial(mat);
  return(1);
}
