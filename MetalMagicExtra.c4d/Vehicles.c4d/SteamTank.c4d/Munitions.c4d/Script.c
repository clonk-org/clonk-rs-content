/*-- Munitionspack --*/

#strict 2
local iWasted;

protected func Initialize()
{
 iWasted=5;
 UpdatePicture();
}

/* Auftreffen */

protected func Hit()
{
  Sound("MetalHit*");
}
  
public func UnpackOne(pTo)
{
 var shot=CreateContents(GetShotID(),pTo);
 DoSomething(shot);
 if(!--iWasted)RemoveObject();
 UpdatePicture();
 return shot;
}

/* Grafik anpassen */
public func UpdatePicture()
{
  if(iWasted > 1)
  {
    SetGraphics(0,0,GetNumberID(iWasted/10),1,GFXOV_MODE_Picture);    
    SetObjDrawTransform(400,0,-12000,0,400,+10000, this, 1);
  }
  else SetGraphics(0,0,0,1,0);  

  SetGraphics(0,0,GetNumberID(Mod(iWasted,10)),2,GFXOV_MODE_Picture);   
  SetObjDrawTransform(400,0,-5000,0,400,+10000, this, 2);
}

/* ID des passenden Zahlobjektes ausgeben */
private func GetNumberID(i)
{
  return C4Id(Format("SNB%d", i));
}

protected func DoSomething(pShot)
{
 LocalN("explosion_base",pShot)=19;
}

/* Forschung */  

public func GetResearchBase() { return STTK; }

/* Kann in der Chemiefabrik hergestellt werden */
public func IsChemicalProduct() { return 1; }

public func IsSteamTankAmmo(){return 1;}
public func GetShotID(){return METO;}
