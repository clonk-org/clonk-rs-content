#strict

local time,pgegner;

Initialize:
  return(1);

Los:
  SetPosition(0,0,this());
  var i=0;
  pgegner=(GetPlayerCount()*2)+(gGrad/3); //Anzahl der Gegner
  i=pgegner;
  while(i>0)  if(--i)  if(!Local(30+i)) { SetLocal(30+i,CreateObject(KNEN,Par(0),Par(1),-1)); ObjectCall(Local(30+i),"Gegner6",0,GetPlayerCount()+1 ); }

  SetAction("Check");
  return(1);

Timer:
  var i=pgegner;
  while(i>0)  if(--i)  if(Local(30+i))  return(0);
  Ende();
  return(1);

Ende:
//  SetVar(0,Random(2));
//  if(Var(0)==0)  ObjectCall(FindObject(ENDE),"Next"); //Neuer Trick
//  if(Var(0)==1)  ObjectCall(FindObject(ENDE),"Next2"); //Magier persönlich
  ObjectCall(FindObject(ENDE),"Next2"); //Magier persönlich
  RemoveObject();
  return(1);