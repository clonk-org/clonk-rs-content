/*-- Gravitation rauf --*/

#strict

public func Activate(caster, real_caster) {
  {
    // Sonst erhöhen
    // Effekt global starten
    if (!AddEffect("GravChangeUSpell", 0, 100, 100, 0, MGDW, 2000))
      return(0, RemoveObject());
  }
  Sound("SOUNDX");
  return(1);
}

func GraviNorm()
{
  RemoveEffect("GravChangeUSpell");
  RemoveObject();
  return(1);
}

// Zauberklassen für den Magen
public func GetSpellClass(object pMage) { return(EART); }
public func GetSpellCombo(pMage) { return ("555"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
