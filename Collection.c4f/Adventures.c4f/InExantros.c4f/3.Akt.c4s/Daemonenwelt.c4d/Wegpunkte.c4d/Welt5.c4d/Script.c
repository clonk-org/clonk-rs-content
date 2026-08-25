#strict

local ix;

Initialize:
  ix=10;
  SetAction("Check");
  return(1);

Timer:
//Sound
  if(!Random(80))  Sound("Angst*",1);
/*
//Grün
  if(Local(0)==0)  ix=ix-Random(3);
  if(Local(0)==0)  if(ix<0)  SetLocal(0,1);

  if(Local(0)==1)  ix=ix+Random(3);
  if(Local(0)==1)  if(ix>20)  SetLocal(0,0);
  SetGamma(0, RGB(50, 170+ix, 40-(ix/2)), 16777215, 3);

*/
//Rot
  if(Local(0)==0)  ix=ix-Random(3);
  if(Local(0)==0)  if(ix<0)  SetLocal(0,1);

  if(Local(0)==1)  ix=ix+Random(3);
  if(Local(0)==1)  if(ix>20)  SetLocal(0,0);
  SetGamma(0, RGB(100+(ix*2), 70-ix, 60-(ix/2)), 16777215, 3);
  return(1);