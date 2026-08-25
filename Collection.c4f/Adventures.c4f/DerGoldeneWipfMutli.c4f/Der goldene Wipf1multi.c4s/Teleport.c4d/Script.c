/*-- Neues Objekt --*/

#strict
local clnk;
local target;

func Initialize() {
  Local(1)=0;
  return(1);
}


func Effekt()
{
Local(2)+=2;
  CreateParticle("PSpark",Cos(Local(2),30),Sin(Local(2),30),0,0,70,RGB(250,10,10));
  for(var i; i < 360; i+=5)
  CreateParticle("PSpark",Cos(i,30),Sin(i,30),0,0,70,RGB(10,10,250));
if(clnk=FindObject(0,-20,-20,40,40,OCF_CrewMember()))
{
 if(Local(700,clnk)==1)return(1);
 if(target==0)return(1);
 
 CreateParticle("MSpark",0,0,0,-3,150,RGB(10,10,250));
 CreateParticle("MSpark",0,0,0,-3,150,RGB(10,10,250));
 SetLocal(700,1,clnk);
 ScheduleCall(clnk,"ReturnLocal",250,0);
 SetPosition(GetX(target),GetY(target),clnk);
}
 

}

func AddTarget(boo)
{
target=boo;
}


