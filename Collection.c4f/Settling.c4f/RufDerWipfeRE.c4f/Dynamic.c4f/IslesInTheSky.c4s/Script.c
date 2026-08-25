#strict

func Initialize() {

  SetSkyParallax (0,17,19,0,0,0,0); 
  SetGamma(RGB(5,5,5),RGB(122,122,122),RGB(235,235,235));

  // Nebel
  for(var i=0;i<50;++i) 
    CreateParticle("Fog",Random(LandscapeWidth()),Random(LandscapeHeight()),0,0,RandomX(900,1700));
}

func AntiLagConfig()
{
  ClearParticles("Fog");
}

func InitializePlayer(iPlr)
{
}