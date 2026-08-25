/*--Clonk --*/

#strict
#appendto CLNK

func Departure (object pContainer) 
{
FindObject(RTVW)->ScheduleCall(0,"AddView",1);
 return(_inherited(...));
}

func Entrance (object pContainer) 
{
var pObj;
while (pObj = FindObject (VIEW, 0, 0, 0, 0, 0, 0, this(), 0, 0))
 RemoveObject(pObj);
return(_inherited(...));
}
