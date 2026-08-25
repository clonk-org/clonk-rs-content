/*-- Neues Objekt --*/

#strict

func Initialize() {
   
  return(1);
}

func Check()
{
if(FindObject(0,-10,-25,20,50,OCF_CrewMember())) {
SetPosition(GetX(),GetY(),FindObject(PLCE));
for(var i; i < 20; i++)
{
CreateParticle("PSpark",RandomX(-10,10),RandomX(-25,25),RandomX(-5,-5),RandomX(-5,5),70,RGB(Random(250),Random(250),Random(250)));
}
RemoveObject();
}
}
