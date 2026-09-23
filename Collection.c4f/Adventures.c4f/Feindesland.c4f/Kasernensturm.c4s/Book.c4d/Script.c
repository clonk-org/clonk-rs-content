#strict

protected func Activate()
{
Message("$MsgStopHim$");
RemoveObject();
return(1);
}