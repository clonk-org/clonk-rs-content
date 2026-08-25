/*-- Zeitanzeige --*/

#strict 2

local iTimer;
local fStop;

func Initialize() {
  SetPosition(50,120,this);
  Local(0)=0;
  Local(1)=0;
  iTimer=10*60+1;
  Tick();
  return 1;
}

protected func Tick()
{
  if(iTimer > 0 && !fStop) iTimer--;
  var color = RGB(255,255,255);
  if(iTimer < 90)color=RGB(255,50,50);

  CustomMessage(Format("@%.2d:%.2d",iTimer/60,iTimer%60),this,-1,0,0,color); 
 
  if(!iTimer)
  {
    Sound("Trumpet");
    GameCall("TimeUp");
    RemoveObject();
  }
}

public func Start() { fStop = 0; }
public func Stop()  { fStop = 1; }
