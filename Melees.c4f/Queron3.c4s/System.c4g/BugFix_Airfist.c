/* Luftklaue wird beim Zaubern von Zauberbüchern aus zurückserstattet */

#strict 2

#appendto AFST

func Activate(pCaster,pRealcaster) {
  var pClonk;
  if(pRealcaster) pClonk=pRealcaster;
  else pClonk=pCaster;

  // Noch Delay?
  if(GetEffect("AFST_Delay", pCaster))
  {
    PlayerMessage(GetOwner(pClonk), "$StillDelay$", pClonk);
    return 0;
  }
  AddEffect("AFST_Delay", pCaster, 1, 35);

  // Nicht im Gebäude zaubern
  if(Contained(pClonk))
  {
    Sound("Error");
    PlayerMessage(GetOwner(pClonk), "$must_be_in_free$", pClonk);
    RemoveObject();
    return 0;
  }

  // Zielen wenn möglich
  if (pClonk->~DoSpellAim(this, pCaster)) return 1;
  
  // Sonst einfach ungezielt loslassen
  var angle=-90;
  if(GetDir(pClonk) == DIR_Right) angle=90;
  ActivateAngle(pClonk,angle);
  
  RemoveObject();
  return 1;
}
