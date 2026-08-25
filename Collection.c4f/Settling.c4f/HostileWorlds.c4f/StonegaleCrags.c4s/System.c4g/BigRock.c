// Bigger, tougher rocks

#strict 2 

#appendto RCK2

protected func Initialize()
{
  Roll();
  if (!Random(3)) 
    DoCon(-Random(20));
  return(1);
}

protected func ContactTop()
{
  SetXDir(5 - Random(11));
  if (!Random(10)) Destruct();
  Crash(-4,-14);
  Crash(0,-16);
  Crash(4,-14);
  ShakeViewPort(GetCon() / 5, this());  
  Roll();
  return(1);
}
  
protected func ContactBottom()
{
  SetXDir(50 - Random(101));
  SetYDir(-45 - Random(10));
  Crash(-4,14);
  Crash(0,16);
  Crash(4,14);  
  Roll();
  ShakeViewPort(GetCon() / 5, this());  
  if (!Random(10)) CrashFree();
  if (!Random(20)) Destruct();
  return(1);
}
  
protected func ContactLeft()
{
  SetXDir(10 + Random(10));
  SetYDir(-40 - Random(10));
  Crash(-14,-4);
  Crash(-16,0);
  Crash(-14,4);
  Roll();
  ShakeViewPort(GetCon() / 5, this());  
  if (!Random(15)) Destruct();
  return(1);
}
  
protected func ContactRight()
{
  SetXDir(-10 - Random(10));
  SetYDir(-40 - Random(10));
  Crash(14,-4);
  Crash(16,0);
  Crash(14,4); 
  Roll();
  ShakeViewPort(GetCon() / 5, this());  
  if (!Random(15)) Destruct();
  return(1);
}

protected func CastSplitRock()
{
  var pRock = CreateObject(GetID(), 0, 0, -1);
  SetXDir(-50 + Random(101), pRock);
  SetYDir(-60 + Random(80), pRock);
  DoCon(GetCon() - 110 - Random(GetCon()), pRock);
  SetMass(GetMass(pRock) - (GetMass(pRock)-GetCon()), pRock);
  if (GetMass(pRock) <= 5) SetMass(5, pRock);
  return(1);
}
