#strict

local pgegner,ptroll,ptroll2;

Initialize:
  SetPosition(0,0,this());
  CreateObject(W_03,980,280,-1);
  ptroll=CreateObject(MMO_,810,320,-1);
  ptroll2=CreateObject(MMO_,810,320,-1);
  ObjectCall(ptroll,"Gegner2",0,(GetPlayerCount()*4) +2 );
  ObjectCall(ptroll2,"Gegner2",0,(GetPlayerCount()*4) +2 );
  SetAction("Check");
  pgegner=(GetPlayerCount()*2) +gGrad; //Anzahl der Orks

  var i=0;

  i=pgegner;
  while(i>0)  if(--i)  if(!Local(30+i)) { SetLocal(30+i,CreateObject(KNEN,810,320,-1)); ObjectCall(Local(30+i),"Gegner6",0,GetPlayerCount() +1 ); }

  RemoveObject();
  return(1);