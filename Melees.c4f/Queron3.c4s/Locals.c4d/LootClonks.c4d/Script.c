/*-- Clonks plündern --*/

#strict 2

/* Regelscript */

protected func Activate(iByPlayer)
	{
  // Effekt für das Ausfaden des Clonks anhängen
  MessageWindow(GetDesc(), iByPlayer);
  return 1;
	}

public func OnClonkCreation(object pClonk)
	{
	// Damit wir schneller sind als OnClonkDeath()
	AddEffect("IntLootWhenDead", pClonk, 1,, this);
	}

public func FxIntLootWhenDeadStop(object pTarget, int iNr, int iReason, bool fTemp)
	{
	if (fTemp || iReason != 4) return 0; // nur wenn der Clonk stirbt
	// Verzögert die Definition anpassen und Inhalt wieder rein
	var nr = AddEffect("IntRecollectLoot", pTarget, 1, 1, this);
	EffectVar(0, pTarget, nr) = [];
	for (var i=0, n=0, obj; i < ContentsCount(, pTarget); i++)
		if (GetID(Contents(i, pTarget)) != MRCK) // getarnte Magi nicht
			EffectVar(0, pTarget, nr)[n++] = Contents(i, pTarget);
	// Aber zuerst mal in Ruhe sterben lassen..
	return 1;
	
	}

public func FxIntRecollectLootStop(object pTarget, int iNr, int iReason, bool fTemp)
{
	if (fTemp) return 0; // nur wenn sterbend
	// Clonk umwandeln
	var id2 = GetID(pTarget);
	pTarget->ChangeDef(CRPS);
	// Todesanimation fortsetzen
	pTarget->SetDeathAction(id2);
	// Immer noch angewählt
	//SetCursor(GetOwner(pTarget), pTarget);
	// Zeugs wieder aufsammeln
	for (var i=0; i < GetLength(EffectVar(0, pTarget, iNr)); i++)
		if (EffectVar(0, pTarget, iNr)[i]) Enter(pTarget, EffectVar(0, pTarget, iNr)[i]);
	// Wenn er nix hat gleich ausfaden
	pTarget->CheckContents();
	return 1;
}
