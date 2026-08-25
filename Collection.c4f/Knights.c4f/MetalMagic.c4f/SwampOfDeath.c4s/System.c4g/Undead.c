/*-- Untote beschwören --*/

#strict 2
#appendto RUND

protected func FxUndeadSpellStart(object pTarget, int iEffectNumber, int iTemp, int iController) {
  var iColor;
  // temporäre aufrufe ignorieren
  if(iTemp) return 0;
  // wiedererwecken mit einer passenden Action
  if(!ObjectSetAction(pTarget,"FlatUp",0,0,1))
    if(!ObjectSetAction(pTarget,"KneelUp",0,0,1))
      if(!ObjectSetAction(pTarget,"Walk",0,0,1))
        return -1;
  SetAlive(1,pTarget);

  iColor = GetColorDw(pTarget);
  // Zeichenart ändern
  SetObjectBlitMode(4,pTarget);
  // Hautfarbe färben :)
  SetClrModulation(RGB(128,255,128),pTarget);
  // Farbe so lassen: die Farbänderung durch SetOwner muss rückgängig gemacht werden
  SetOwner(iController,pTarget);
  SetColorDw(iColor,pTarget);
  // neue Physicals (temporär natürlich):
  // Geister können nicht hangeln und nicht klettern
  ResetPhysical(pTarget);
  SetPhysical("CanScale",0,PHYS_Temporary,pTarget);
  SetPhysical("CanHangle",0,PHYS_Temporary,pTarget);

  // Laufgeschwindigkeit, Schwimmgeschwindigkeit und Sprunghöhe werden reduziert
  DoPhysical("Jump",-10000,PHYS_Temporary,pTarget);
  DoPhysical("Walk",-20000,PHYS_Temporary,pTarget);
  DoPhysical("Swim",-10000,PHYS_Temporary,pTarget);
  DoEnergy(+100,pTarget);
  DoBreath(+100,pTarget);

  // Ein Untoter kann sehr gut töten
  DoPhysical("Fight",+60000,PHYS_Temporary,pTarget);
  
  pTarget->SetAI();

  // zum Manschaftsmitglied machen
  // MakeCrewMember(pTarget,iController);
  // Fertig
  return 1;
}


protected func FxUndeadSpellStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  // Wenn Untoter Clonk stirbt, nicht entfernen um andere Untoteneffekte zu sperren
  // (Untote können nicht wiedererweckt werden)
  if(iReason==4)
  {
    pTarget->CastObjects(BONE,4,10);	  
    RemoveObject(pTarget);	  
    return 1;
  }
}
