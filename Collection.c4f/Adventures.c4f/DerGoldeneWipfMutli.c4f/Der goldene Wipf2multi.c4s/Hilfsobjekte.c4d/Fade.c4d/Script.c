

#strict

func Initialize() {
   ScheduleCall(this(),"Fade",1,0);
  return(1);
}

func Fade()
{
ScheduleCall(this(),"Fade",1,0);
if(Local(1)==254)return(RemoveObject());
if(Local(0)!=128)Local(0)+=2;
if(Local(1)!=254)Local(1)+=2;
SetGamma(RGB(0,0,0),RGB(128-Local(0),128-Local(0),128-Local(0)),RGB(255-Local(1),255-Local(1),255-Local(1)));
}

