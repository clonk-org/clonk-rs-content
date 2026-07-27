/*-- Feuerklumpen --*/

#strict 2

protected func Activate(pCaster, pRealcaster)
  {
  var pFireball, pClonk, iDir, pCombo, fExplode;
  // der Clonk kriegt den Feuerball
  if(pRealcaster) pClonk=pRealcaster;
  else pClonk=pCaster;
  
  if (pCombo = FindContents(FLNT, pCaster))
    {
    fExplode = true;
    RemoveObject(pCombo);
    }

  pFireball=CreateObject(FRBL,0,0,GetOwner(pCaster));
  if (fExplode) pFireball->SetExplodeOnHit();
  // Feuerball ins Inventar des Clonks packen: wenn voll, gleich verschieﬂen
  if(!Collect(pFireball,pClonk))
    {
    iDir=GetDir(pClonk)*2-1;
    SetSpeed(iDir*RandomX(30,40),-RandomX(50,60),pFireball);
    // feuerball direkt benutzen ist gef‰hrlich
    Fling(pClonk,iDir,-RandomX(2,3));
    }
  
  RemoveObject();
  return 1;
  }

/* Zaubercombo */

public func GetSpellStaff(object pMage) { return SMFR; }
public func GetSpellStaffCombo(pMage) { return ("46"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
