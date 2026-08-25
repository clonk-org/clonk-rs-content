#strict

Initialize:
  SetLocal(2,300);
  Sound("Thunder");
  SetAction("Timer");
  return(1);

Timer:
  CastParticles("Flash", 2, 170, 0,0, 10, 500+Random(800), RGBa(255,255,255,50+Random(120)));
  SetLocal(2,Local(2)-1);
//  Sound("KistallExplode");
//  if(!Random(160))  Sound("Thunder");
//  if(!Random(10))  if(Local(2)<0)  return(RemoveObject());
  if(!Random(3))  ObjectCall(CreateObject(KSP_,0,0,-1),"Turn",Random(360));
  return(1);