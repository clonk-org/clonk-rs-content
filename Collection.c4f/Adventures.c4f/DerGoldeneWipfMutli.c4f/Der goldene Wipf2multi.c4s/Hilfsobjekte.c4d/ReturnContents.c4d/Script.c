/*-- Neues Objekt --*/

#strict


func Initialize() {
   SetAction("Fly");
   ScheduleCall(this(),"Check2",5,0);
  return(1);
}

func Part()
{
CreateParticle("PSpark",0,0,0,0,70,RGB(0,250,250));
}

func Check2()
{
SetCommand(this(),"None");
}

func Check()
{

if(GetY() > LandscapeHeight()-8)SetYDir(-5);

if(Local(1)==1)
if(!Local(0))
{
SetCommand(this(),"MoveTo",FindObject(0,0,0,-1,-1,OCF_CrewMember()),0,0,0,0,1);
if(!GBackSolid()&&!GBackLiquid()){
CastParticles("PSpark",20,50,0,0,70,100,RGB(0,250,250),RGB(0,250,250));
var foo;
while(foo = Contents(0)) Exit(foo);
RemoveObject();
return(1);
}
}

if(!GetAlive(Local(0)))Local(0)=0;


AddCommand(this(),"MoveTo",Local(0),0,0,0,0,1);
if(Local(0)) {
Local(1)=1;
if(ObjectDistance(Local(0),this()) < 8)
{
var foo;
while(foo = Contents(0))Exit(foo); /*Enter(Local(0),foo);*/
/*for(var i=0; i < ContentsCount(0,this()); i++)
Exit(Contents(i,this()));*/
CastParticles("PSpark",20,50,0,0,70,100,RGB(0,250,250),RGB(0,250,250));
RemoveObject();
}
}
}



