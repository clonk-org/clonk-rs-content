#strict

local time,pgegner;

Initialize:
  Start();
  return(1);

Start:
  SetPosition(0,0,this());
  SetAction("Check");
  time=45;
  pgegner=(GetPlayerCount()*2) +gGrad; //Anzahl der Gegner

  if(!Local(3)) { SetLocal(3,CreateObject(MMO_,440,300,-1)); ObjectCall(Local(3),"Gegner2",0,GetPlayerCount() ); }
  return(1);

Timer:
  var i=0;
//  time=time-1;

//Gegner
  i=pgegner;
  if(time>0)  while(i>0)  if(--i)  if(!Local(30+i)) { SetLocal(30+i,CreateObject(KNEN,440,300,-1)); ObjectCall(Local(30+i),"Gegner6",-1+(Random(2)),GetPlayerCount() ); time=time-4; }

  if(!Random(9)) if(!Local(3)) { SetLocal(3,CreateObject(MMO_,440,300,-1)); ObjectCall(Local(3),"Gegner2",0,GetPlayerCount() ); time=time-20; }

  i=pgegner;
  if(time<0)  while(i>0)  if(--i)  if(Local(30+i))  return(0);
  if(!Local(3))  if(time<0)  Ende();
  return(1);

Ende:
  GameCall("Teil2");
  RemoveObject();
  return(1);