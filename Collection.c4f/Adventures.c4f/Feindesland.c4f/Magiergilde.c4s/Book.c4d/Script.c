#strict

protected func Activate()
{
Message("$MsgNotAWord$");
GainMissionAccess("Magier");
GameOver();
return(1);
}