/*-- Airstrike --*/

#strict

// wurde der Airstrike aktiviert?
local launched;

// Rauchschwade zeigt das Ziel für die Bomber
func Activate(pObject)
{
  SetOwner(GetController(pObject),this());
  if(!PathFree(GetX(),GetY(),GetX(),0)) {return(Message("$RecievingBlocked$",this()));}
  Exit(this());
    
  launched = true;
  SetVisibility(VIS_Allies()+VIS_Owner(),this());
  SetAction("Waiting");
  SetDir(GetDir(pObject));
  
  ScheduleCall(this(),"DoSmoke",1,300); 
  ScheduleCall(this(),"CallSupport",200);
  
  CreateObject(LGFL,0,0,-1)->Init(this, 0, 0, 100, GetPlrColorDw(GetOwner()), 1);
  Sound("Fusing",0,this,100,0,1);
}

func DoSmoke()
{
  Smoke(0,0,20,GetPlrColorDw(GetOwner()));
  if(!Random(10))
    Sparkle(2);
}

func CallSupport()
{
  ScheduleCall(this(),"Boom",15,2);
}

func Boom()
{
  var dir = GetDir()*2-1;
  Boom2(-300*dir,dir);
  ScheduleCall(this,"Boom2",5*1,0,-200*dir,dir);
  ScheduleCall(this,"Boom2",5*2,0,-100*dir,dir);
  ScheduleCall(this,"Boom2",5*3,0,+000*dir,dir);
  ScheduleCall(this,"Boom2",5*4,0,+100*dir,dir);
  ScheduleCall(this,"Boom2",5*5,0,+200*dir,dir);
  FadeOut(this());
  Sound("Airstrike*");
}

func Boom2(iX,iDir)
{
  Schedule(Format("SetSpeed(%d,60,CreateObject(FLNT,%d,AbsY(-25),%d))",iDir*20,RandomX(iX,iX+100)-100*iDir,GetOwner(),iDir),2,RandomX(1,2),this());
}

func RejectEntrance()
{
  return(launched);
}
