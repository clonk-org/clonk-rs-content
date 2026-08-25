/*-- Ph33r me! --*/

#strict


protected func Initialize()
{
  AddEffect("Rain", this(), 20, 1, 0, GetID());
  AddEffect("Dark", this(), 20, 1, 0, GetID(), 255);
  SetGamma(RGB(0,0,0), RGB(100,100,100), RGB(200,200,200));
  return(1);
}

func FxRainTimer(pTarget)
{
    CreateParticle("Raindrop",Random(LandscapeWidth()),0,GetWind(0,0,1)/20,60+RandomX(-2,10),300,RGB(255,255,255));
}

func FxDarkFogTimer(pTarget)
{
    CreateParticle("Fog",RandomX(-50,100),Random(LandscapeHeight()-50),RandomX(3,10),0,RandomX(1000,1500),RGB(50,50,50));
}

func FxRefreshStart(pTarget, iNumber, iTemp, iBrightness)
{
    EffectVar(0, pTarget, iNumber) = iBrightness;
}

func FxRefreshTimer(pTarget, iNumber, iTime)
{
    if(iTime >= EffectVar(0, pTarget, iNumber)-25)
        return(-1);
        
    SetSkyAdjust(HSL(0, 0, EffectVar(0, pTarget, iNumber)-iTime));
}

func FxDarkTimer(pTarget, iNumber, iTime)
{
    if(iTime >= 225)
        return(-1);
        
    SetSkyAdjust(HSL(0, 0, 255-iTime));
    SetMatAdjust(HSL(0, 0, 255-iTime*2/3));
}

func FxDarkStop(pTarget)
{
    AddEffect("Thunder", pTarget, 20, 3, 0, GetID());
    AddEffect("DarkFog", pTarget, 20, 10, pTarget);
}


func FxThunderTimer(pTarget, iNumber, iTime)
{
    CreateParticle("DarkCloud",RandomX(0,LandscapeWidth()),10,0,0,800+Random(50),RGB(10,10,10));
    
    if(iTime % 100 == 0)
    {
        if(!Random(3))
            return(0);
            
        LaunchLightning(Random(LandscapeWidth()), 0, -20, 41, +5, 15);
        LaunchLightning(Random(LandscapeWidth()), 0, -20, 41, +5, 15);
        Sound("Thunder*");
        
        AddEffect("Refresh", pTarget, 20, 1, 0, GetID(), 100);
    }
    
}
