/*-- Drain --*/

#strict 2

local amount;
local drain;
local iMaterial;
local iR;

public func SetWaterR(r)
{
 iR=r;
}

protected func Initialize()
{
  SetAction("Drain");
  amount = 2;
  iMaterial = Material("Water"); 
  return 1;
}

private func Draining()
{
  if(GBackSolid()) SetPosition(GetX(), GetY()-1);   
  var i;
  var mat;
  if(!drain)
  {
    i=amount;
    var xDir=Cos(iR-90+RandomX(-5,5),40+RandomX(-3,3));
    var yDir=Sin(iR-90+RandomX(-5,5),40+RandomX(-3,3));
    
    while(i--) InsertMaterial(iMaterial,0,-1,xDir,yDir);
    return;
  }
  mat = iMaterial;
  //mat = GetMaterial(GetX(drain)-GetX(), GetX(drain)-GetX());
  i = drain->ExtractMaterialAmount(0, 0, mat, amount);
  Message("%d %s", this, i, MaterialName(mat));
  while(i--) InsertMaterial(mat);
  return 1;
}
