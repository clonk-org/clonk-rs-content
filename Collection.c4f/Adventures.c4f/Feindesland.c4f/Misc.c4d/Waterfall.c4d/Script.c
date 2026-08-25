/*-- Drain --*/

#strict

local amount;
local drain;
local iMaterial;
protected func Initialize()
{
  SetAction("Drain");
  amount = 2;
  iMaterial = Material("Water"); 
  return(1);
}

private func Draining()
{
  if(GBackSolid()) SetPosition(GetX(), GetY()-1);	
  var i;
  var mat;
  if(!drain)
  {
    i=amount;
    while(i--) InsertMaterial(iMaterial);
    return();
  }
  mat = iMaterial;
  //mat = GetMaterial(GetX(drain)-GetX(), GetX(drain)-GetX());
  i = drain->ExtractMaterialAmount(0, 0, mat, amount);
  Message("%d %s", this(), i, MaterialName(mat));
  while(i--) InsertMaterial(mat);
  return(1);
}
