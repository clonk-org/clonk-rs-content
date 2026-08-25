/*-- Neues Script --*/

#strict

#appendto CLNK

protected func Death()
{
ScheduleCall(this(),"RemoveMe",150,0);
return(_inherited());
}

func RemoveMe()
{
RemoveObject();
}

func ReturnLocal()
{
Local(700)=0;
}
