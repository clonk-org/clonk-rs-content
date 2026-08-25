/*-- Zeitanzeige --*/

#strict

local iTimer,iMode;

func Initialize() {
  var iX=80;
  var iY=120;
  GameCall("TimerPosition",this,iX,iY);
  if(Local()){ iX=Local(); iY=Local(1);}
  SetPosition(iX,iY,this());
  Local(0)=0;
  Local(1)=0;
  Set(60*5);
  Tick();
  return(1);
}

func Set(Timer,dwClrA,dwClrB,sGraph){
  SetGraphics(sGraph);
  SetObjectBlitMode(GFX_BLIT_ClrSfc_OwnClr);
  if(!dwClrA)
  {
    if(!sGraph)
      dwClrA=RGB(55,55,55);
    else
      dwClrA=RGB(255,255,255);
  }
  if(!dwClrB)
   dwClrB=RGB(255,255,255);   
  SetClrModulation(dwClrA);
  SetColorDw(dwClrB);
  iMode=1;
  iTimer=Timer+1;
  if(Timer==-1)
  {
    iMode=-1;
    iTimer=1;
  }
  Tick();
  return true;
}

func Tick()
{
  if(Abs(iTimer) > 0) iTimer-=iMode;
  var color = RGB(255,255,255);
  if(iTimer < 10)color=RGB(255,50,50);
 
  if(iTimer>60)
    Message("@<c %x>%.2d:%.2d ",this(),color,iTimer/60,iTimer%60);
  else
    Message("@<c %x>%.2d  ",this(),color,iTimer%60);
 
  if(iMode==1)
    if(iTimer < 10)
      Sound("Warning",1,0,100-iTimer*10);
  if(!iTimer)
  {
    Sound("Trumpet");
    GameCallEx("TimeUp");
    RemoveObject();
  }
}
