/*-- Zombiewipf --*/

local clonk,ix,iy;

#strict

Initialize:
  SetAction("Walk");
  ix=GetX();
  iy=GetY();
  return(1);

Timer:
  // Geräusche machen
  if (!Random(10)) Sound("Snuff*");

  // Nur im Gehen oder Schwimmen
  if (GetAction() ne "Walk" && GetAction() ne "Swim") return();

  // Auftauchen, wenn der Atem ausgeht
  if (InLiquid() && GetBreath() <= 25 && GetComDir() != COMD_Up())
   SetComDir(COMD_Up());
  if(GetAction()S="Dead")  return(0);
  if(GetAction()S="HangOn")  return(0);

  if(!GBackSolid(ix-GetX(),iy+15-GetY()))  iy=iy+10; //Kein Startpunkt in der Luft!

  //if(Distance(GetX(),GetY(),ix,iy)>350)  return(SetPosition(ix,iy,this()));
  if(!Random(2))  if(Distance(GetX(),GetY(),ix,iy)>250)  SetCommand(this(),"MoveTo",0,ix,iy);

  if(!Random(2))  if(GetAction()S="Walk")  while(SetVar(0,FindObject(0,-50,-50,100,100,OCF_Prey(),0,0,NoContainer(),Var(0))))  if(GetAlive(Var(0)))  Sprung(Var(0));

  if(!Random(3))  SetCommand(this(), "MoveTo", 0,GetX()-200+Random(401),GetY()-50+Random(101));
  if(!Random(2))  if(GetAction()S="Walk")  while(SetVar(0,FindObject(0,-20,-15,19,30,OCF_Prey(),0,0,NoContainer(),Var(0))))  if(GetAlive(Var(0)))  return(SetDir(DIR_Left()),SetAction("Punch"));
  if(!Random(2))  if(GetAction()S="Walk")  while(SetVar(0,FindObject(0,  1,-15,19,30,OCF_Prey(),0,0,NoContainer(),Var(0))))  if(GetAlive(Var(0)))  return(SetDir(DIR_Right()),SetAction("Punch"));
  if(!Random(3))  if(FindObject(0,-200,-75,400,150,OCF_Prey(),0,0,NoContainer()))  if(SetVar(0,FindObject(0,0,0,-1,-1,OCF_Prey(),0,0,NoContainer())))  SetCommand(this(), "MoveTo", Var(0));

  // Nichts machen
  if (Random(2)) return();

  // Springen
  if (GetAction() eq "Walk")
   if (!Random(3)) return(Jump());

  // Hinsetzen
  if (GetAction() eq "Walk")
    if (!Random(4)) return(SetAction("Sit"));

  // Umdrehen
  if (Random(2)) return(TurnRight());
  return(TurnLeft());

  return(1);

/* Kontakt */

Sprung:
  if (SEqual(GetAction(),"Attack"))                          return(0);
//  if (Not(Frei()))                                            return(0);
  if (Not(PathFree(GetX(),GetY(),GetX(Par(0)),GetY(Par(0))))) return(0);
  SetXDir(SetVar(0,Sub(GetX(Par(0)),GetX())));
  SetYDir(Sub(GetY(Par(0)),GetY(),10));
  SetAction("Attack",Par(0));
  SetLocal(2);
  Sound("WAttack*");
  if (!Random(5)) Sound("Snuff*");
  if (GreaterThan(Var(0))) return(SetDir(DIR_Right()));
  return(SetDir(DIR_Left()));

PAttack:
  while(SetVar(0,FindObject(0,-16,-10,32,20,OCF_Prey(),0,0,NoContainer(),Var(0))))  if(GetAlive(Var(0)))  return(Punching(Var(0)),SetXDir(-10+Random(21)),SetYDir(-5+Random(11)));
  return(1);

Fight:
  if(GetDir()==DIR_Left())  while(SetVar(0,FindObject(0,-20,-15,19,30,OCF_Prey(),0,0,NoContainer(),Var(0))))  if(GetAlive(Var(0)))  return(Punching(Var(0)));
  if(GetDir()==DIR_Right())  while(SetVar(0,FindObject(0, 1,-15,19,30,OCF_Prey(),0,0,NoContainer(),Var(0))))  if(GetAlive(Var(0)))  return(Punching(Var(0)));
  return(1);

Punching:
  SetAction("Walk");
  Sound("WAttack*");
  Punch(Par(0),2+Random(2));
//  if(!Random(3))  HangOn(Par(0));
  return(1);

CheckHangOn:
  if(Local(0)==0)  return(SetAction("Walk"));
  if(!Local(0)==0)  if(GetDir()==DIR_Left())  if(GetX(Local(0))-GetX()>0)  SetDir(DIR_Right());
  if(!Local(0)==0)  if(GetDir()==DIR_Right())  if(GetX(Local(0))-GetX()<0)  SetDir(DIR_Left());
  Sound("WAttack*");
  DoEnergy(-Random(2),Local(0));
  SetXDir(0,Local(0));
  ObjectSetAction(Local(0),"GetPunched");
  if(!Random(3))  ObjectCall(Local(0),"Hurt");
  if(!Local(0)==0)  if(Distance(GetX(Local(0)),GetY(Local(0)),GetX(),GetY())>15)  return(SetAction("Walk"),SetLocal(0,0));
  if(!Random(40))  return(SetAction("Walk"));
  return(1);

HangOn:
  SetLocal(0,Par(0));
  if(GetEnergy(Par(0))<1)  return(0); //Schon Tot :(
  SetAction("HangOn");
  return(1);

Sitting:
  if(!Random(5)) SetAction("Walk");
  return(1);

protected func ContactLeft() { return(TurnRight()); }
protected func ContactRight() { return(TurnLeft()); }

/* Aktionen */

public func TurnRight()
{
  if (Stuck() || (GetAction() ne "Walk" && GetAction() ne "Swim" && GetAction() ne "Sit")) return();
  if (GetXDir() < 0) SetXDir(0);
  SetDir(DIR_Right());
  SetComDir(COMD_Right());
  if (GetAction() eq "Sit") SetAction("Walk");
  return(1);
}

public func TurnLeft()
{
  if (Stuck() || (GetAction() ne "Walk" && GetAction() ne "Swim" && GetAction() ne "Sit")) return();
  if (GetXDir() > 0) SetXDir(0);
  SetDir(DIR_Left());
  SetComDir(COMD_Left()); 
  if (GetAction() eq "Sit") SetAction("Walk");
  return(1);
}

/* Einwirkungen */

Hurt:
  if(GetAction() eq "Dead") return(0);
  if(!Random(3))  Sound("WHurt*");
  return(1);

Death:
  Sound("WDead");
  SetDir(0);
  ChangeDef(_DWP);
  SetAction("Dead");
  return(1);
