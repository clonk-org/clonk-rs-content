#strict

Initialize:
  SetAction("Check");
  return(1);

Dreh:
  SetCategory(C4D_Background());
  return(1);

Timer:
  if(!Local(2))  SetR(GetR()-4);
  if(Local(2))  SetR(GetR()+4);
  if(!Random(3))  CastParticles("MSpark", 1, 200, 0,0, 10, 150+Random(230), RGBa(10,10,255,50+Random(120)));
  if(!Random(4))  CastParticles("Flash", Random(2), 200, 0,0, 10, 150+Random(230), RGBa(255,255,255,50+Random(120)));
  return(1);

Kaputt:
  CastParticles("Flash", 10, 170, 0,0, 10, 4000, RGBa(255,255,255,50+Random(120)));
  CreateObject(KIE_,0,0,-1);
  RemoveObject();
  return(1);