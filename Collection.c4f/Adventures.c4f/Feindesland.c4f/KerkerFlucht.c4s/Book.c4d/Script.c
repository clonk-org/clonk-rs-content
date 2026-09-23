#strict

protected func Activate()
{
Message("$MsgPrisonPlans$");
GainMissionAccess("KerkerFlucht");
GameOver();
return(1);
}