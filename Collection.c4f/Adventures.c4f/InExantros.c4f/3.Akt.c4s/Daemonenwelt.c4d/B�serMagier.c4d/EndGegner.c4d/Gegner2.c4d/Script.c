#strict

local pgegner;

Initialize:
  return(1);

Los:
  SetPosition(0,0,this());
  var i=0;
  pgegner=10+(GetPlayerCount()*3)+gGrad; //Anzahl der Gegner
  i=pgegner;
  while(i>0)  if(--i)  if(!Local(30+i)) { SetLocal(30+i,CreateObject(W_00,2700+Random(700),500,-1)); }

  Message("<c ff0000> <Dunkle Gestalt>| Hahaha,| wie wärs mit einer| feuerigen Ladung Lava.| Hahaha</c>");
  Sound("Boss_02",1);
  return(1);

Timer:

  var i=pgegner;
  while(i>0)  if(--i)  if(Local(30+i))  RemoveObject(Local(30+i));

  Ende();
  return(1);

Ende:
//  SetVar(0,Random(2));
//  if(Var(0)==0)  ObjectCall(FindObject(ENDE),"Next"); //Neuer Trick
//  if(Var(0)==1)  ObjectCall(FindObject(ENDE),"Next2"); //Magier persönlich
  ObjectCall(FindObject(ENDE),"Next2"); //Magier persönlich
  RemoveObject();
  return(1);