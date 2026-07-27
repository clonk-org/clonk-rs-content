/* Fischhaut: Keine (Grafik)fehler; Inventargrösse entspricht der des Zaubernden */

#strict

#appendto FHSK

public func Revaluation(target, target1) // target=clonk, target1=zauberturm/zauberbuch
  {
  var id2 = GetID(target);
  
  // Aufwertungsziel
  var idRevaluationTarget;
  if(GetPhysical("Magic", 0, target)) idRevaluationTarget = FCLK;  // Zauberbegabte Clonks werden zum Fischclonk
  else idRevaluationTarget = ACLK; // Sonst Aquaclonk

  // Keine Aufwertung zum gleichen Clonktyp
  if (idRevaluationTarget == GetID(target))
    return(NoRevaluation(target, target1, Format("$MsgIsRev$", GetName(target), )));
  // Aufwertung möglich? (z.B. nicht vom König zum Ritter)
  if (idRevaluationTarget->~IsRevaluationBlocked(target))
    return(NoRevaluation(target, target1, Format("$MsgBlocked$", GetName(target))));
	
  // Verwandlung
  AddEffect("FishSkinPSpell", target, 101, 800, 0, GetID(), id2, idRevaluationTarget);
	
  RemoveObject(); 
  return(1);
  }

/* FishSkinPSpell */

// 0: id2
// 1: portrait
// 2: portraitID
// 3: idRevaluationTarget
// 4: action
public func FxFishSkinPSpellStart(target, no, temp, id2, idRevaluationTarget)
{
	//Log("Fishclonk start, temp=%d", temp);
	if (!temp)
		{
		// Effekt
  	Sound("Magic1");
		// Alte ID des Clonks speichern
		EffectVar(0, target, no) = id2;
		// Portrait speichern
		EffectVar(1, target, no) = GetPortrait(target);
		EffectVar(2, target, no) = GetPortrait(target, 1);
		// Neue ID auch (für tmp-Calls)
		EffectVar(3, target, no) = idRevaluationTarget;
		}
  // Umwandlung
  if (!target->~RedefineFx(EffectVar(3, target, no)))
    if (GetOCF(target) & OCF_Alive())
      {
      ChangeDef(idRevaluationTarget, target);
      ObjectSetAction(target, "Walk");
      }
    else return(0, RemoveObject());
  // Clonkspezifische Aktionen nach Umwandlung (z.B. Zauberenergie für Magier)
  target->~OnRevaluation();
  // Fix: Klettern & Hangeln wieder aktivieren
  SetPhysical("CanScale", 1, 2, target);
  SetPhysical("CanHangle", 1, 2, target);
  // Clonk kann (muss) unter Wasser atmen
  SetPhysical("BreatheWater", 1, 2, target);
  // Grafik verändern
  SetGraphics("FishClonk", target, FHSK);
  // Portrait
  SetPortrait("FishClonk1", target, FHSK);
	return 1;
}

public func FxFishSkinPSpellStop(target, no, reason, temp)
{
	if (!temp)
		{
	  // Magie -> hören
  	Sound("DeEnergize", 0, target);
  	}
  // Clonk ist nun kein Fisch mehr
  SetPhysical("BreatheWater", 0, 2, target);
  // Clonk wieder zurückverwandeln, wenn er noch ein Aquaclonk ist (als er zB nicht Aufwertung benutzt hat)
  var id2 = EffectVar(0, target, no);
  if(GetID(target) == FCLK || GetID(target) == ACLK)
    {
    // Umwandlung
    if (!target->~RedefineFx(id2))
      if (GetOCF(target) & OCF_Alive())
        {
        ChangeDef(id2, target);
        ObjectSetAction(target, "Walk");
        }
    // Fix: Klettern & Hangeln wieder aktivieren
    SetPhysical("CanScale", 1, 2, target);
    SetPhysical("CanHangle", 1, 2, target);
    // Grafik zurücksetzen
    SetGraphics(0, target, GetID(target));
    // Portrait zurücksetzen
    SetPortrait(EffectVar(1, target, no), target, EffectVar(2, target, no));
    }
	return 1;
}
