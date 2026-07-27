/* angepasste Tastenkombos */

#strict 2

#appendto CBMU

// (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)

private func GetSpellCombo(id idSpell, id idClass)
{
	var combo;
	if (idSpell == MFRB) combo="666";
	//else if (idSpell == MBRG) combo="333";
	else if (idSpell == MSSH) combo="222";
	else if (idSpell == FHSK) combo="111";
	if (combo)
	{
		if (!idSelectedElement)
		{
		  if (idClass == MISC)
		    combo = Format("3%s", combo);
		  else
		    combo = Format("%s%s", idClass->GetClassCombo(), combo);
		}
		return combo;
	}
	else
		return inherited(idSpell, idClass, ...);
}
