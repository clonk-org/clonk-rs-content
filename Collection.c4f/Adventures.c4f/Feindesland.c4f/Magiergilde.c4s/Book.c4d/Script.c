#strict

protected func Activate()
{
Message("Von dem hier versteh ich kein Wort! :(");
GainMissionAccess("Magier");
GameOver();
return(1);
}