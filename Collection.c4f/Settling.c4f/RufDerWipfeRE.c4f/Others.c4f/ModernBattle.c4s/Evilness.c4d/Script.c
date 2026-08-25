/*-- Motorengezwitscher --*/

#strict

protected func Initialize()
{
  SetCategory(3145729);
  Local( )=80;
  Local(1)=80;
  return(1);
}

private func EvilWar()
{
  if(!Random(5))
  {
    var iSize=RandomX(500,5000);
    CreateParticle("PSpark",Random(LandscapeWidth()),Random(LandscapeHeight()),0,0,iSize*4,RGB(255,255,255),this);
  }
}  
