#appendto MMTR

#strict

local explosion_base;

public func CastMeteor(id idObj,int iX,int iDir,int iRange,int iExplo, int iOwner) { 
  // Standardhagel besteht aus Meteoriten
  if(!idObj) idObj=METO;
  // Log("%d is Inside %d,%d",AbsX(iX+RandomX(-iRange/2,+iRange/2)),iX-iRange/2,iX+iRange/2);
  // Einen Meteoriten gen Erdreich schleudern
  var iY=AbsX(0);
  if (!GetScenTopOpen()) iY += GetDefHeight(idObj);
  var pMeteor = CreateObject(idObj,AbsX(iX+RandomX(-iRange/2,+iRange/2)),iY, -1);
 // SetSpeed(iDir*RandomX(5,30)+RandomX(-20,+20),15,pMeteor);
 SetSpeed(iDir*RandomX(5,30)+RandomX(-20,+20),200,pMeteor); // RAMBA ZAMBA!!
  // Controller korrigieren
  SetController(iOwner,pMeteor);
  // Explosionsradius anpassen
  if(iExplo && idObj==METO) pMeteor->METO::SetExplosionBase(iExplo);
}
