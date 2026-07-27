/* RedefineFx */

// Verwandelt einen Clonk und speichert die alten Werte bis zur Rückverwandlung

#strict 2

global func RedefineFx(id idTo, object pObj)
{
	if (!pObj && this) pObj = this;
	else if (!pObj) return 0;
  // Aktivitätsdaten sichern
  var phs = GetPhase(pObj), act = GetAction(pObj);
  // Umwandeln
  ChangeDefFx(idTo, pObj);
  // Aktivität wiederherstellen
  var chg = pObj->SetAction(act);
  if (!chg) pObj->SetAction("Walk");
  if (chg) pObj->SetPhase(phs);
  // Fertig
  return 1;
}

global func ChangeDefFx(id idTo, object pObj)
{
	if (!pObj && this) pObj = this;
	else if (!pObj) return 0;
	// idTo == im Stack vorhandene ID? -> Rückverwandlung
	var nr;
	for (var i=0; i < GetEffectCount("IntSaveDefChange", pObj); i++)
		{
		if (EffectVar(0, pObj, GetEffect("IntSaveDefChange", pObj, i)) == idTo)
			nr = GetEffect("IntSaveDefChange", pObj, i);
		}
	// Verwandlung
	if (!nr) AddEffect("IntSaveDefChange", pObj, 1,,,, idTo);	// Hinverwandlung -> Variablen speichern
	var r = ChangeDef(idTo, pObj);
	if (nr) RemoveEffect(, pObj, nr); // Rückverwandlung -> Variable wiederherstellen
	return r;
}

global func GetLocalByIndex2(int iIndex, object pObj)
{
	// Variablentyp statt Name anschauen!
	for (var i=2; GetObjectVal("LocalNamed",, pObj, i); i+=3)
		{
		if (!iIndex--) return GetObjectVal("LocalNamed",, pObj, i-1);
		if (GetObjectVal("LocalNamed",, pObj, i) == "a") // Achtung Array -> überspringen!
			i += GetObjectVal("LocalNamed",, pObj, i+1)*2;
		}
	return 0;
}

/* IntRedefineClonk */

global func FxIntSaveDefChangeStart(object pTarget, int iNr, int iTemp, id idTo, bool fTmp)
	{
	if (iTemp) return 0;
	// Ursprungs-ID
	EffectVar(0, pTarget, iNr) = GetID(pTarget);
	// Alle lokalen Variablen
	EffectVar(1, pTarget, iNr) = [];
	var n = GetLocalCount(pTarget);
	for (var i=0; i < n; i++)
		EffectVar(1, pTarget, iNr)[i] = LocalN(GetLocalByIndex2(i, pTarget), pTarget);
	return 1;
	}

global func FxIntSaveDefChangeStop(object pTarget, int iNr, int iReason, bool fTemp)
	{
	if (fTemp) return 0;
	// lokale Variablen wiederherstellen
	var n = GetLocalCount(pTarget);
	for (var i=0; i < n; i++)
		LocalN(GetLocalByIndex2(i, pTarget), pTarget) = EffectVar(1, pTarget, iNr)[i];
	return 1;
	}
