/*-- Fehler --*/

#strict
#appendto MWP2

/* Warp */

#strict

public func Activate(caster, real_caster) {
  return(1);
}

private func Warp(clonk)
{
  return(1);
}

public func GetSpellClass(object pMage) { return(AIR1); }
public func GetSpellCombo(pMage) { return ("11"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)

public func GetResearchBase () { return (MGWP); }
