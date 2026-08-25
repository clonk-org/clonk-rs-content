/*-- Holzhütte --*/

#strict
#include B070
#include _ZF2

BasementID: return(B280);

CanHost: return(100);
SleepLevel: return(35);

func Lightning()
{
  if(Random(2)) return(1);
  LaunchLightning(GetX(),0, -10,20, +5,15 );
  return(1);
}

public func SunRise()
{
  SetAction("IdleDay");
}

public func Dawn()
{
  SetAction("IdleNight");
}

public func Completion(){ SetEntrance(true); return(1);}
