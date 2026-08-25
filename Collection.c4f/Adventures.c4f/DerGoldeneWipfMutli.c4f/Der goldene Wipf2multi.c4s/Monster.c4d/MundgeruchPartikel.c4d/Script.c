/*-- Neues Objekt --*/

#strict
local dir3;

func Initialize() {
   SetAction("Fly");
   ScheduleCall(this(),"Randoom",1,0);
  return(1);
}

func Randoom()
{
if(!Random(2))RemoveObject();
}

func Check()
{
//CreateParticle("FSpark",0,0,0,0,50,RGB(100,0,250));
CreateParticle("PSpark",0,0,0,0,50,RGB(100,0,250));
if(Local(1)==20)RemoveObject();
Local(1)++;
var clnk;
if(clnk=FindObject(0,0,0,-1,-1,OCF_CrewMember()))if(ObjectDistance(clnk,this()) < 10)if(!Random(3))Punch(clnk,Random(2));
SetYDir(RandomX(-25,35));
SetXDir(RandomX(0,30)*Local(0));
}
