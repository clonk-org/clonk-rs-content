/* Freeze: Killverfolgung und Unhide für Assas */

#strict 2

global func FxFreezeStart(pTarget, iEffectNumber, iTemp, iFromController)
{
  // Physicals herunter setzen
  SetPhysical("Dig"   , GetPhysical("Dig"   , 0, pTarget)*3/4, PHYS_StackTemporary, pTarget);
  SetPhysical("Walk"  , GetPhysical("Walk"  , 0, pTarget)*3/4, PHYS_StackTemporary, pTarget);
  SetPhysical("Jump"  , GetPhysical("Jump"  , 0, pTarget)*3/4, PHYS_StackTemporary, pTarget);
  SetPhysical("Swim"  , GetPhysical("Swim"  , 0, pTarget)*3/4, PHYS_StackTemporary, pTarget);
  SetPhysical("Scale" , GetPhysical("Scale" , 0, pTarget)*3/4, PHYS_StackTemporary, pTarget);
  SetPhysical("Hangle", GetPhysical("Hangle", 0, pTarget)*3/4, PHYS_StackTemporary, pTarget);
  SetPhysical("Float" , GetPhysical("Float" , 0, pTarget)*3/4, PHYS_StackTemporary, pTarget);
  // Bei einer temporären Änderung aufhöhren
  if(iTemp) return;
  // Assas aufdecken
  var nr = GetEffect("Hide", pTarget);
  if (nr) RemoveEffect(, pTarget, nr);
  // Toller Sound
  Sound("Crystal2", 0, pTarget);
  // Zeit einstellen
  EffectVar(0, pTarget, iEffectNumber)+=35;
  // ID speichern
  var id = GetID(pTarget);
  if(GetDefHeight(id)>GetDefWidth(id)) EffectVar(1, pTarget, iEffectNumber) = GetDefHeight(id);
  else EffectVar(1, pTarget, iEffectNumber) = GetDefWidth(id);
  // Verursacher speichern
  EffectVar(2, pTarget, iEffectNumber) = iFromController;
  return 1;    
}

global func FxFreezeTimer(pTarget, iEffectNumber, iEffectTime)
{
  // In Lava schwimmend? Dann schnell auftauen (nur für Monster und andere nicht brennbare Lebewesen wichtig)
  if(GetMaterialVal("Incindiary", "Material", pTarget->GetMaterial()))
    return -1;
  // Partikeleffekte für den betroffenen
  var id = GetID(pTarget);
  CreateParticle("NoGravSpark",GetX(pTarget)+RandomX(-GetDefWidth(id)/2, GetDefWidth(id)/2),GetY(pTarget)+RandomX(-GetDefHeight(id)/2, GetDefHeight(id)/2),0,RandomX(-2, -4),RandomX(18,28),RGB(10,120,210), pTarget);
  CreateParticle("Freeze",GetX(pTarget),GetY(pTarget),0,0,EffectVar(1, pTarget, iEffectNumber)*5+20,RGB(0,40,80),pTarget);
  // Runterzählen
  EffectVar(0, pTarget, iEffectNumber)-=1;
  if(EffectVar(0, pTarget, iEffectNumber)<=0) return FX_Execute_Kill;
  // Ein bischen tut Einfrieren auch weh
  if(!Random(10))
  	DoEnergy(-1, pTarget, false, 0, EffectVar(2, pTarget, iEffectNumber) + 1);
  return FX_OK;
}
