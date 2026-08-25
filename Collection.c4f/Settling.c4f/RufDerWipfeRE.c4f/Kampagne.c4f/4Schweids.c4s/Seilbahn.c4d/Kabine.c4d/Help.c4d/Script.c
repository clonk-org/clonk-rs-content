#strict

local sx,sy,ex,ey,gt,t,dir,sup,sdown;

Travel:
  sup=Par(0); sdown=Par(1);
  dir=-1;
  SearchNewTarget();
  SetPosition(ex,ey);
  SetAction("Fly");

  ObjectCall(CreateObject(_G5A),"Connect",this(),sup);
  ObjectCall(CreateObject(_G5A),"Connect",this(),sdown);

  return(1);

Fly:
  if(!EnergyCheck(1000,sdown)) return(SetSpeed(0,0));
  if(!dir) return(SetSpeed(0,0));
  if(Distance(GetX(),GetY(),ex,ey)<10 || t>=gt) {
		SetPosition(ex,ey);
		SearchNewTarget();
		dir=0;
		Sound("Seilbahn");
		SetSpeed(0,0); }
  Drive();
  return(1);

func SetSpeed(x,y)
{
  SetXDir(x);
  SetYDir(y);
  return(1);
}

Drive:
  SetSpeed(
		(ex - GetX()) / (gt - t++),
		(ey - GetY()) / (gt - t++));
  return(1);

SearchNewTarget:
  sx=GetX(); sy=GetY();
  if(dir==-1) {ex=GetX(sdown)+14; ey=GetY(sdown)+15;}
  if(dir==+0) {ex=GetX(); ey=GetY();}
  if(dir==+1) {ex=GetX(sup)-14; ey=GetY(sup)+15;}
  gt=Distance(sx,sy,ex,ey)/10;
  t=0;
  return(1);

DriveUp: return(dir=1);
DriveDown: return(dir=-1);