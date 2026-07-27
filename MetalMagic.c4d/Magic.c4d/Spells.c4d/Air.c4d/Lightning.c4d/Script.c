/*-- Blitz-Zauber --*/

#strict 2

func Activate(pCaster,pRealcaster) {
  var pClonk, pComboObj, fBlast;
  if(pRealcaster) pClonk=pRealcaster;
  else pClonk=pCaster;

  // Nicht im Gebäude zaubern
  if(Contained(pClonk))
  {
    Sound("Error");
    PlayerMessage(GetOwner(pClonk), "$must_be_in_free$", pClonk);
    RemoveObject();
    return 0;
  }

  var iResult;
  if (iResult = CheckEffect("LightningNSpell", 0, 125)) return iResult!=-1 && RemoveObject();
  
  // Zielen wenn möglich
  if (pClonk->~DoSpellAim(this)) return 1;

  // Dann gleich Blitz erzeugen
  var obj = CreateObject(LGTS, -GetX(), -GetY(), GetOwner(pClonk));
  obj->Launch(GetX(pCaster)+GetVertex(0, VTX_X, pCaster), GetY(pCaster)+GetVertex(0, VTX_Y, pCaster), (!GetDir(pCaster))*1800);
  Sound("Thunder*");
  RemoveObject();
  return 1;
}

// Aktivierung nach Zielen (iAngle: -90=links; 0=oben; +90=rechts)
public func ActivateAngle(object pCaller, int iAngle)
{
  // Blitz erzeugen
  var obj = CreateObject(LGTS, -GetX(), -GetY(), GetOwner(pCaller));
  obj->Launch(GetX(pCaller), GetY(pCaller), (900-iAngle*10));
  Sound("Thunder*");

  // Objekt löschen
  RemoveObject();
  return 1;
}

public func IsAimerSpell() { return 1; }

public func GetSpellStaff(object pMage) { return SMAR; }
public func GetSpellStaffCombo(pMage) { return ("33"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)

public func GetSpellClass(object pMage) { return AIR1; }
public func GetSpellCombo(pMage) { return ("444"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
