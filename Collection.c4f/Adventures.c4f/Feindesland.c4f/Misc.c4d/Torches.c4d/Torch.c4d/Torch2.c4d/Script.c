/*-- Fackel --*/

#strict

public Initialize:
      CreateObject(LI9Z,0,0,GetOwner())->LI9Z::Aufleuchten();
return(1);

public ControlDigDouble:  return(Loesch());

public Check4Clonk:
var clnk;
     if(GetCon(GetActionTarget())<50) return(ControlDigDouble());
     if(clnk=FindObject(0,-100,-100,200,200,OCF_CrewMember()))
       if(!FindObjectOwner(LI9Z,GetOwner(clnk),-10,-10,20,20))
         CreateObject(LI9Z,0,0,GetOwner(clnk))->LI9Z::Aufleuchten();
     if(InLiquid()) Loesch();
return(1);

private func Loesch() {
     var light;
     Sound("Pshshsh");
     while(light=FindObject(LI9Z,-10,-10,20,20,0,0,0,0,light))
        RemoveObject(light);
     CreateObject(FK9Z,0,8);
     RemoveObject();
     }