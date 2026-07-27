/*-- Messages --*/

// script v1.6 by miniwipf
#strict 2

global func CastMsg(string szMsg, int dwClr)
{
	if (!this) return 0;
	var obj = CreateContents(_MSG);
	// rausspringen
	obj->SetAction("Rise");
	obj->Exit(,,,,, -3);
	var nr = AddEffect("IntMsgFlow", this, 2, 3, obj,, szMsg, dwClr);
	// Effekt übernommen? -> Befehlsziel ist nicht mehr obj -> überflüssig!
	if (GetEffect(, this, nr, 4) != obj) RemoveObject(obj);
	return 1;
}

/* IntMsgFlow */

// 0: string szMsg
protected func FxIntMsgFlowStart(object pTarget, int iNr, int iTemp, string szMsg, int dwClr)
	{
	if (iTemp) return;
	EffectVar(0, pTarget, iNr) = szMsg;
	EffectVar(1, pTarget, iNr) = dwClr;
	// Instanz bewegen
	SetComDir(COMD_Up);
	SetAction("Rise");
	Exit(,,,,, -3);
	return 1;
	}

protected func FxIntMsgFlowTimer(object pTarget, int iNr, int iTime)
	{
	// einfach ausfaden
	var a = BoundBy(255-iTime*8, 0, 255);
	if (!a) return -1;
	// nur Alpha setzen
	var dwClrMod = EffectVar(1, pTarget, iNr);
  if (!dwClrMod) dwClrMod = 0xffffff | a<<24; // wenn nix angegeben => weiss!
 	else dwClrMod = dwClrMod & 16777215 | a<<24; // untere 24 Bits (=16777215) für RGB
	// stehende Nachricht
	return Message("@<c %x>%s</c>", this, dwClrMod, EffectVar(0, pTarget, iNr));
	}

protected func FxIntMsgFlowStop(object pTarget, int iNr, int iReason, bool fTemp)
	{
	if (fTemp || iReason) return;
	return RemoveObject(); // Msg-Objekt löschen
	}
