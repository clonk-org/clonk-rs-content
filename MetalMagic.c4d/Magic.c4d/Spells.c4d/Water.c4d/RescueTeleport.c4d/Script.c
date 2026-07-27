/* Rettungsteleport */

#strict 2

func Activate(pCaster,pRealcaster)
{
  var pClonk=pRealcaster;
  if(!pClonk) pClonk=pCaster;

  // Auswählen wenn möglich
  if (pCaster->~DoSpellSelect(this,300)) return 1;

  // Clonk kann nicht auswählen: Clonk bekommst selbst den Bonus
  if (SelectorTarget(pClonk))
   return ActivateTarget(pClonk, pClonk);

  // ansonsten geht nicht
  RemoveObject();
  return 0;
}

public func ActivateTarget(object pCaller, object pTarget)
{
  // Nähchste Basis finden
  var pBase = GetActionTarget(0, FindObject2(Find_ID(FLAG), Find_Allied(GetOwner(pCaller)), Find_Action("FlyBase")));
  if(!pBase) { RemoveObject(); return 0; }
  // Geräuschkulisse!
  Sound("Electric");Sound("Crystal2", 0, pTarget);
  // Beamen
  DrawParticleLine("FSpark", GetX(pTarget)-GetX(), GetY(pTarget)-GetY(), GetX(pBase)-GetX(), GetY(pBase)-GetY(), 8, 50, RGB(200,200,255), RGB(0,255,255));
  while(Contents(0, pTarget)) pTarget->Exit(Contents(0, pTarget), 0, 10);
  Enter(pBase, pTarget);
  // Kleiner Effekt beim Zauberer
  if(pCaller != pTarget)
    CastParticles ("FSpark", 10, 10, 0, 0, 40, 50, RGB(200,200,255), RGB(0,255,255));
  // Objekt löschen
  return RemoveObject();
}

public func SelectorTarget(object pTarget)
{
  // nur außerhalb von Gebäuden
  if(Contained(pTarget)) return;
  // muss ein Mannschaftsmitglied sein
  if(!(GetOCF(pTarget)&OCF_CrewMember)) return;
  // muss verbündet sein oder NTMG muss aktiviert sein
  if(Hostile(GetOwner(),GetOwner(pTarget)) && !ObjectCount(NTMG)) return;
  return 1;
}

/* Zaubercombo */

public func GetSpellStaff(object pMage) { return SMWT; }
public func GetSpellStaffCombo(pMage) { return ("56"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
