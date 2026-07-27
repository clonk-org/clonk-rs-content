#strict 
local target;

local iAngle;

public func Activate(caster1, caster2) {
  // Effekt prüfen
  var iResult;
  if (iResult = CheckEffect("ToadjumpUSpell", 0, 125)) return(iResult!=-1 && RemoveObject());
	
  // Tatsächliches Ziel ermitteln
  target = caster1;

  // caster2 auf den Container vom Zauberer setzen falls nicht übergeben
  if (!caster2) caster2 = Contained(caster1);

  // Wirklichen Caster überprüfen
  if (caster2) {
    // Container lebt?
    if (GetOCF(caster2) & OCF_Alive()) {
      // Zielobjekt = Container
      target = caster2;
    }
    // Sonst Fehlermeldung
    else 
      return(CastError("$NotInFree$", target));
  }
  
  // Nur auf festem Grund
  if (!GBackSolid(0, AbsY(GetDefBottom(target))))
    return(CastError("$NotWalking$", target, 1));
  
  // Zielen wenn möglich
  if (target->~DoSpellAim(this())) 
  // Clonk soll gleich schräg nach oben zielen	  
  {  AddEffect("IntAim",target,1,1,0,GetID(), GetDir(target)*80-40); return(1); }
  
  // Einen Frame verzögert springen
  SetAction("Jump");

  iAngle = -45+90*GetDir(target);
  
  // Fertig
  return(1);
}

// Aktivierung nach Zielen (iAngle: -90=links; 0=oben; +90=rechts)
public func ActivateAngle(object pCaller, int iAimAngle)
{
  iAngle = iAimAngle;
  // Einen Frame verzögert springen
  SetAction("Jump");
  return(1);
}

protected func DoJump() {
  var iSpeed=GetPhysical("Jump",0,target)/512; //78 bei standardphysicals
  
  // Sound
  Sound("ToadJump");

  // Aktivität setzen
  Jump(target);  SetSpeed(Sin(iAngle, iSpeed), -(Cos(iAngle, iSpeed)*4/3), target);
  
  ObjectSetAction(target, "Jump");
  
  // Zerstören
  RemoveObject();
}

protected func CastError(s, caster, silent) {
  // Meldung ausgeben
  if (!silent) Message(s, caster, GetName(caster));
  // Sound
  if (!silent) Sound("Error");
  // Action zurücksetzen
  if (GetAction(caster) eq "Magic") ObjectSetAction(caster, "Walk");
  // Selbstzerstörung
  RemoveObject();
  // Fehlschlag
  return(0);
}

func FxIntAimStart(object pTarget, int iNumber, int iTemp, int iAngle) {
  if(iTemp) return();

  // Richtung speichern
  EffectVar(0,pTarget,iNumber) = iAngle;
}

func FxIntAimStop(object pTarget, int iNumber, int iTime) {
  // Clonk gleich in die passende Richtung zielen lassen	
  var iAngle = EffectVar(0, pTarget, iNumber);
  if(!LocalN("pAimer", pTarget)) return();
  LocalN("iAngle", LocalN("pAimer", pTarget)) = iAngle;
  LocalN("pArrow", LocalN("pAimer", pTarget))->~UpdateAngle(iAngle);
  LocalN("pTargetObject", LocalN("pAimer", pTarget))->~AimingAngle(iAngle);
}

// Zielspruch (für Artefaktzauber)
public func IsAimerSpell() { return(1); }

public func GetSpellClass(object pMage) { return(AIR1); }
public func GetSpellCombo(pMage) { return ("553"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
