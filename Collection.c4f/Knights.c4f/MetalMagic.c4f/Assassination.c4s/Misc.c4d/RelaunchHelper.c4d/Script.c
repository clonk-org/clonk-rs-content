/*--Relauncher --*/

#strict 2

func Initialize() {
  SetAction("Do");
  Phase();
  return 1;
}

func Phase()
{
 var iSec=5-GetPhase();
 PlayerMessage(GetOwner(),"$TimeTo$",0,iSec);
}

func End()
{
 RemoveObject(0,1);
}

func RejectCollect()
{
 return 1;
}
