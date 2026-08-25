/*-- Neues Objekt --*/

#strict

func Initialize() {
  ScheduleCall(this(),"Particle1",1,0);
  return(1);
}

func Particle1()
{
if(Local(0)==150) { Particle2(); return(1); }
ScheduleCall(this(),"Particle1",1,0);
Local(0)++;
CreateParticle("PSpark",RandomX(AbsX(GetX(Global(51)))-10,AbsX(GetX(Global(51)))+10),RandomX(AbsY(GetY(Global(51)))+10,AbsY(GetY(Global(51)))-2),0,RandomX(-2,-5),70,RGB(10,10,250));
}

func Particle2()
{
if(Local(0)==330) { Particle3(); return(1); }
ScheduleCall(this(),"Particle2",1,0);
Local(0)++;
CreateParticle("PSpark",AbsX(GetX(Global(51))),RandomX(AbsY(GetY(Global(51)))+10,AbsY(GetY(Global(51)))-2),0,0,240,RGB(20,20,250));
}

func Particle3()
{
Sound("Magic1",0,Global(51));
DrawParticleLine("PSpark",AbsX(GetX(Global(51))),AbsY(GetY(Global(51))),AbsX(LandscapeWidth()),AbsY(LandscapeHeight()/2),1,140,RGB(20,20,250),RGB(20,20,250));
RemoveObject(Global(51));
//RemoveObject();
}


