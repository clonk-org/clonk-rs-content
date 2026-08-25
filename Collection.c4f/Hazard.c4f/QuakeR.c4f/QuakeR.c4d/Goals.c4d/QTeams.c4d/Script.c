/* Überladung von Teams für Medals */

#strict
#include TEAM //Alles weird
#appendto TEAM

// Hazard-Backwards-Compatibility!
func DoEvaluation(int plr)
{
	var pMedal;
	while (pMedal=FindObject2(Find_Owner(plr),Find_Func("IsMedal"),Sort_Func("MedalCount")))
	{
		EvalMedals(plr,GetID(pMedal));
	}
	AddEvaluationData(Format("{{PIWP}}$Kills$: %d", aKill[plr]), GetPlayerID(plr));
	AddEvaluationData(Format("{{KAMB}}$Death$: %d", aDeath[plr]), GetPlayerID(plr));
}
