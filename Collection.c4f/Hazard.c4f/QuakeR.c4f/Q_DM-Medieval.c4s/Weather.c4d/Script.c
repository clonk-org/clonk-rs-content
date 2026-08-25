/*-- Ph33r me! --*/

#strict

local raincolor;

protected func Initialize()
{
  Sound("Rain",1,0,100,0,1);
  SetSkyAdjust(RGBa(100,100,100,0),RGB(255,255,255));
  SetPosition();
  raincolor = RGB(255,255,255);
  return(1);
}

protected func Rain()
{
	for(var i = 0; i < 9; i++)
		CreateParticle("Raindrop",Random(LandscapeWidth()+200)-100,-10,GetWind(0,0,1)/2,160+RandomX(-4,4),300,raincolor);

if(!Random(150))if(!GetEffect("Thunder",this()))AddEffect("Thunder",this(),20,1,this());
}

func FxThunderStart(pTarget, iNumber)
{
 SetSkyAdjust(RGBa(100,100,100,190),RGB(255,255,255));
 CreateLightning(Random(LandscapeWidth()),0,180+RandomX(-10,10));
 Sound("Thunder_*",1);
}

func FxThunderTimer(pTarget, iNumber)
{
 var rgb, r, g ,b, a;
 rgb=GetSkyAdjust(0);
 SplitRGBaValue(rgb,r,g,b,a);
 a-=5;
 
 SetSkyAdjust(RGBa(100,100,100,a),RGB(255,255,255));
 if(a <= 0)return(-1);
 
}

global func RemoveWeather()
{
ClearParticles("Raindrop");
Sound("Rain",1,0,100,0,-1);
}
