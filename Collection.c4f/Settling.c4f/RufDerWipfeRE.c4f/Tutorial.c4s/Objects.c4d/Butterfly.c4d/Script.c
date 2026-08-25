/*-- Schmetterling --*/

#strict 2

local init;

protected func Initialize()
{
  SetAction("Fly");
  MoveToTarget();
  if(Random(3))
    SetGraphics(Format("%d",Random(3)));
  ScheduleCall(0,"TakeOff",3,2);
}
  
/* TimerCall */  
 
private func Activity()
{
  // Unterwasser
  if(InLiquid()) return SetComDir(COMD_Up);
  // Im Winter?
  if(GetTemperature()<0)
    if(GetAction()!="SitInv")
      if(GetAction()=="Sit")
        SetAction("SitInv");
      else
        SitDown();
  // Sitzt oder fliegt grad zum Boden: warten
  if(GetAction()=="Sit"||GetAction()=="TakeDown") return true;
  // Neues Ziel
  if(!GetCommand()||!Random(5))  MoveToTarget(); 
}
  
/* Bewegung */

private func Flying()
{
  // Richtung anpassen
  if(GetXDir()>0) SetDir(DIR_Right);
  else SetDir(DIR_Left);  
  // Aktion ändern
  if(!Random(3)) SetAction("Flutter");
}
  
private func Fluttering()
{
  // Richtung anpassen
  if(GetXDir()>0) SetDir(DIR_Right);
  else SetDir(DIR_Left);  
  // Aktion ändern
  if(!Random(7)) SetAction("Fly");
}
  
  
/* Kontakt */  
  
protected func ContactBottom()
{
  if(GetAction()=="TakeDown")
  {
    SetXDir(0);
    SetYDir(0);
    SetAction("Sit");
    SetCommand(this,"None");
    SetComDir(COMD_Stop);
  }
  if(GetAction()!="Sit")
  {
    SetCommand(this,"None");
    SetComDir(COMD_Up);
  }
  return(1);  
}
  
func SitDown()
{
  SetYDir(0);
  SetComDir(COMD_Down);
  SetAction("TakeDown");
  SetCommand(0,"None");
}
  
func TakeOff()
{
  SetComDir(COMD_Up);
  SetAction("Fly");
}
  
private func MoveToTarget()
{
  var iX,iY;
  iX=Random(LandscapeWidth());
  iY=Random(GetHorizonHeight(iX)-60)+30;
  SetCommand(this,"MoveTo",0,iX,iY);
}
  
private func GetHorizonHeight(int iX)
{
  var iY=0;
  while(iY<LandscapeHeight()&& !GBackSemiSolid(iX,iY))
    iY+=10;
  return(iY);
}

// RdW - Callbacks

public func Dawn()
{
  ScheduleCall(this,"SitDown",Random(350));
}  

public func SunRise()
{
  if((GetTemperature()>0&&GetAction()=="SitInv")||GetAction()=="Sit")
    ScheduleCall(this,"TakeOff",Random(150));
}  
