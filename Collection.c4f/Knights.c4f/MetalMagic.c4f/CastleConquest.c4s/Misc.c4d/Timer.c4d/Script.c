/*-- Zeitanzeige --*/

#strict 2

local iTimer;

func Initialize() {
  SetVisibility(VIS_Allies);
  SetPosition(50,100,this);
  Local(0)=0;
  Local(1)=0;
  iTimer=60*25;
  return 1;
}

protected func Tick()
{
  if(iTimer > 0) iTimer--;
  var color = RGB(255,255,255);
  if(iTimer < 90) color=RGB(255,50,50);
 
  CustomMessage(Format("@%.2d:%.2d",iTimer/60,iTimer%60),this,-1,0,3,color);
 
  if(iTimer < 90)
  if(!Random(25))
  {
    Log("$Reinforcement$");
    Sound("Trumpet");
    GameCall("Reinforcement");
    RemoveObject();
  }
}
