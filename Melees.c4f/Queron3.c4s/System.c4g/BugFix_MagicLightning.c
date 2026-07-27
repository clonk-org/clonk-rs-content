/* Blitz-Zauber: Wird Rückbelegt --*/

#strict

#appendto MLGT

func Activate(pCaster,pRealcaster) {
  var pClonk;
  if(pRealcaster) pClonk=pRealcaster;
  else pClonk=pCaster;

  var iResult;
  if (iResult = CheckEffect("LightningNSpell", 0, 125)) return(iResult!=-1 && RemoveObject());
  
  // Zielen wenn möglich
  if (pClonk->~DoSpellAim(this(), pCaster)) return(1);

  // Blitz erzeugen
  var obj = CreateObject(LGTS, -GetX(), -GetY(), GetOwner(pClonk));
  obj->Launch(GetX(pCaster)+GetVertex(0, VTX_X, pCaster), GetY(pCaster)+GetVertex(0, VTX_Y, pCaster), (!GetDir(pCaster))*1800);
  Sound("Thunder*");
  RemoveObject();
  return(1);
}
