/*-- Ghost --*/

#strict

local iAngle;
local x, y;

func Initialize() {
  SetObjectBlitMode(GFX_BLIT_Additive,this());
  SetClrModulation(RGBa(255,255,150,50));
  
  SetAction("Released");
  
  //Schedule("Start()", 50, 0);
 // SetYDir(-20);
  x=GetX();
  y=0;
  Start();
  return(1);
}

func Start()
{
   SetAction("Floating");
   AddEffect("Fly", this(), 20, 1, this());
   SetComDir(COMD_Stop);
   iAngle=0;
}

func Speed()
{
    return 2;
}

func FxFlyTimer(pTarget, iNumber, iEffectTime)
{
    if(iEffectTime > 35*60)
    {
        /*if(y != -10)
        {
            y=-10;
            x=Random(LandscapeWidth());
        }
        if(GetY() < 0) return -1;*/
        
        if(!GetEffect("TheEnd", pTarget))
            AddEffect("TheEnd", pTarget, 20, 1, pTarget);
    }
    else
    if(!x || !Random(120))
    {
        x=RandomX(200, LandscapeWidth()-200);
        y=RandomX(200, LandscapeHeight()-200);
    }
    
    var a=Angle(GetX(), GetY(), x, y);
    
    var dir;
     if(a < iAngle)dir=-Speed();
    else dir=Speed();
 
    var dif=360-iAngle+a;
    var dif2=360-a+iAngle;
 if(dif < 180)dir=Speed();
 else
 if(dif2 < 180)dir=-Speed();
 


 
 iAngle=(iAngle+dir)%360;
 if(iAngle < 0)iAngle=360+iAngle;
     if(Inside(iAngle, a-1, a+2))iAngle=a;
    
    var nA=iAngle;//+Cos(iEffectTime*5, 20);
    SetXDir(Sin(nA, 3000), 0, 1000);
    SetYDir(-Cos(nA,3000), 0, 1000);
    SetR(iAngle);;
    
    CreateParticle("cFire3", -GetXDir()/4+RandomX(-3,3), -GetYDir()/4+RandomX(-3,3), 0, 0, 60, RGB(15,155,15));    
}

func FxFlyStop(pTarget)
{
    RemoveObject(pTarget);
    
}

func FxTheEndTimer(pTarget, iNumber, iTime)
{
    CreateParticle("cFire3", RandomX(-5,5), RandomX(-5,5), 0, 0, 50, RGB(15,155,15), pTarget);
    SetClrModulation(RGBa(255-iTime*2,255,150-iTime,50));
    
    if(iTime >= 125)
        return(-1);
}

func FxTheEndStop(pTarget, iNumber, iReason, fTemp)
{
    if(iReason)
        return(0);
    
    Sound("Poff", 0, this(), 100);
    CastParticles("cFire3", 30, 60, 0, 0, 70, 80, RGB(15,155,15), RGB(15,200, 15));
    CastObjects(GPRT, 3, 50);
    RemoveObject();
}
