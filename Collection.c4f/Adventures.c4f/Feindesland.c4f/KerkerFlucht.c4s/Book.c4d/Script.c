#strict

protected func Activate()
{
Message("Durch diese Gefängnisspläne können viele Soldaten befreit werden... :D");
GainMissionAccess("KerkerFlucht");
GameOver();
return(1);
}