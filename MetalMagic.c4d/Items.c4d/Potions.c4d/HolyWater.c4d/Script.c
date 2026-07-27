/* Weihwasser */

#strict 2

protected func Hit() { Sound("Crystal*"); }

public func IsPotion() { return 1; }
public func BrewTime() { return 6*35; }

public func EffectDuration(){return 27*30*1;}

protected func Activate(object pClonk) {
  // Sound
  Sound("Drink");
  // Aktion
  pClonk->~Drink(this);
  // Effekt starten (zeitverzögert)
  Schedule("AddEffect(\"HolyWater\",this,210,10,0,HLWT,HLWT->EffectDuration())",80,0,pClonk);
  // fertig..
  RemoveObject();
  return 1;
}

private func FxHolyWaterStart (object pTarget, int iNumber, int iTemp, int iTime) {
  if(iTemp) return;

  // Zeit speichern
  EffectVar(0,pTarget,iNumber)=iTime;
  
  EffectVar(2,pTarget,iNumber)=0;
}

private func FxHolyWaterEffect(string szNewEffectName, object pTarget) {
  // gleichen Effekt zusammenfassen (Trank zweimal hintereinander getrunken)
  if(szNewEffectName == "HolyWater") return -2;
}

private func FxHolyWaterAdd(object pTarget, int iNumber, string szNewEffectName, int iNewEffectTimer, int iTime) {
  // Effekt übernehmen
  EffectVar(0,pTarget,iNumber) += iTime;
}

private func FxHolyWaterTimer(object pTarget, int iNumber, int iTime) { 

  // Langsam Karmaenergie füllen
  pTarget->~DoKarmaEnergy(1);  
  
  // Zeit abgelaufen!
  if(iTime >= EffectVar(0,pTarget,iNumber)) return -1;
}

private func FxHolyWaterStop(object pTarget, int iNumber, int iReason, bool fTemp) {
  if(fTemp) return;
}
