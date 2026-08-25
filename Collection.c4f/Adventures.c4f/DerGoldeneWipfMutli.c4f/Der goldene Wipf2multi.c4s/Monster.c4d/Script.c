/*-- Gold-Golem --*/

#strict
//#include CLNK

local target;
local target2;
local dir;
local pos;

func Initialize()
{
pos=GetX();
ScheduleCall(this(),"Special",RandomX(600,1000),0);
ScheduleCall(this(),"Move",RandomX(200,400),0);
ScheduleCall(this(),"CheckMoving",1,0);
ScheduleCall(this(),"Throw",RandomX(70,150),0);
  SetAction("Walk");
  SetDir(Random(2));
  return(1);
}

func CheckMoving() {
ScheduleCall(this(),"CheckMoving",1,0);
if(GetXDir()==0)Local(2)=0;
else(Local(2)=1);
}


func Timer2()
{
if(GetEnergy() > 0)Message("@<c f40000>%d",this(),GetEnergy());
if(GetY() <0)RemoveObject();

if(Local(0)==1)return(0);
//Wir suchen Leute die wir nicht mögen!

//Aber was wenn keiner da ist :(?
if(!FindObject(PLDN,-500,-300,1000,600,OCF_Alive())) { target=0; return(1); }

/*while(target=FindObject(CLNK,0,0,-1,-1,OCF_Alive(),0,0,0,target))
if(PathFree(GetX(),GetY(),GetX(target),GetY(target)))break;*/

target=FindObject(PLDN,0,0,-1,-1,OCF_Alive());


if(Local(2)==0) {
if(GetX(target) > GetX())SetDir(1);
else(SetDir(0));
}

  dir=GetDir(target)+1;
  if(dir==2)dir=-1;

//Wenn in der Nähe stinken wir ihn tot! Mundgeruchattacke!
if(Inside(GetY(target),GetY()-20,GetY()+30))if(ObjectDistance(target,this()) >30 && ObjectDistance(target,this()) < 55)Mundgeruch();


//Hoho! Da traut sich einer aber nah ran
if(ObjectDistance(target,this()) < 20)
  {
  CastParticles("PSpark",10,10,AbsX(GetX(target)),AbsY(GetY(target)),70,100,RGB(100,10,250),RGB(100,10,250));
  Punch(target,RandomX(5,10));
  
  
  Fling(target,3*dir,-2);
  }
}

func Mundgeruch()
{
//Nicht einatmen!
if(Local(0)!=0)return(0);
Sound("Gas",0,0,50);
var dir3=1;
if(GetDir()==DIR_Left())dir3=-1;
CreateObject(GAS_,12*dir3,-4,-1)->~Local(0)=dir3;
}

func Throw()
{
ScheduleCall(this(),"Throw",RandomX(70,150),0);

if(!PathFree(GetX(),GetY(),GetX(target),GetY(target))) { return(1); }
if(Local(0)==1)return(1);
if(Local(0)==0)if(ObjectDistance(target,this()) > 17 && ObjectDistance(target,this()) < 500) { SetAction("Throw"); }
}

func Threw()
{
var flint;
flint=CreateObject(UFLN,-12*dir,-16,-1);
SetXDir((GetX()-GetX(target)+RandomX(-5,5))*-1,flint);
SetYDir((GetY()-GetY(target)+RandomX(-5,5))*-1,flint);
}

func Move()
{
ScheduleCall(this(),"Move",RandomX(200,400),0);
if(Local(0)==1)return(1);
SetCommand(this(),"MoveTo",this(),AbsX(pos+RandomX(-40,40)));
}
  
func Special()
{
ScheduleCall(this(),"Special",RandomX(600,1000),0);
if(Local(0)==1)return(1);
Local(0)=1;
Local(0)=2;
Var(0)=Random(3);
if(Var(0)==0)
  {
   SetAction("Up");
   Local(1)=0;
   ScheduleCall(this(),"ClonkWarp",10,0);
   Sound("Scream2");
   }
 
 if(Var(0)==1)
  {
   SetAction("Up");
   Local(1)=0;
   ScheduleCall(this(),"Searcher",10,0);
   Sound("Scream2");
   }
   
    if(Var(0)==2)
  {
   SetAction("Up");
   Local(1)=0;
   ScheduleCall(this(),"Bomb",10,0);
   Sound("Scream2");
   }
}
func Bomb()
{
if(Local(1)==50) {
var dir3;
dir3=GetDir()+1;
if(dir3==2)dir3=-1;
/*CreateObject(BOMB,20*dir3,RandomX(-12,12),-1)->Local(0)=dir3;
CreateObject(BOMB,20*dir3,RandomX(-12,12),-1)->Local(0)=dir3;
CreateObject(BOMB,20*dir3,RandomX(-12,12),-1)->Local(0)=dir3;*/
for(var i; i <4; i++)
{
var k;
k=CreateObject(BOMB,20*-dir3,RandomX(-12,12),-1);
SetLocal(0,dir3,k);
}


 Local(0)=0; SetAction("Down"); return(1); }

ScheduleCall(this(),"Bomb",1,0);
CreateParticle("PSpark",RandomX(-20,20),16,0,RandomX(-2,-7),70,RGB(100,0,250));
Local(1)++;
}


func Searcher()
{
if(Local(1)==100) {
 CreateObject(SRCH,RandomX(-40,40),-40,-1);
 CreateObject(SRCH,RandomX(-40,40),-55,-1);
 Local(0)=0; SetAction("Down"); return(1); }

Local(1)++;

ScheduleCall(this(),"Searcher",1,0);

CastParticles("FSpark",4,5,-12,-16,20,50,RGB(100,10,250),RGB(100,10,250));
CastParticles("FSpark",4,5,12,-16,20,50,RGB(100,10,250),RGB(100,10,250));
}
  
func ClonkWarp()
{
//beamZ!
if(Local(1)==100) {
var u=RandomX(1,2);
 for(var i; i < u; i++)
 {
 var clnk;
 clnk=CreateObject(CLNK,RandomX(-20,20),-40,-1);
 RemoveObject(Contents(0,clnk));
 CreateContents(ROCK,clnk);
 }
 Local(0)=0; SetAction("Down"); return(1);
  }
 

ScheduleCall(this(),"ClonkWarp",1,0);

Local(1)++;
CastParticles("PSpark",4,5,-12,-16,20,50,RGB(100,10,250),RGB(100,10,250));
CastParticles("PSpark",4,5,12,-16,20,50,RGB(100,10,250),RGB(100,10,250));

CastParticles("PSpark",8,8,RandomX(-30,30),-50,60,100,RGB(100,10,250),RGB(100,10,250));
}


func Death()
{
//:C
Local(0)=1;
Message("@<c %x>Argh! Ich komme wieder!",this(),RGB(100,0,250));
ScheduleCall(this(),"Particle1",1,0);
ScheduleCall(this(),"Up",150,0);
}

func Particle1()
{
ScheduleCall(this(),"Particle1",1,0);
var cos,sin;
CreateParticle("PSpark",cos=Cos(Random(360),40),sin=Sin(Random(360),40),-cos/4,-sin/4,70,RGB(100,0,250));
CreateParticle("PSpark",cos=Cos(Random(360),40),sin=Sin(Random(360),40),-cos/4,-sin/4,70,RGB(100,0,250));
}

func Up()
{
for(var i; i < 8; i++)
RemoveVertex();
SetAction("Fly");
SetYDir(-50);
}

func QueryCatchBlow()
{
if(GetAction() eq "Up")return(true);
}


