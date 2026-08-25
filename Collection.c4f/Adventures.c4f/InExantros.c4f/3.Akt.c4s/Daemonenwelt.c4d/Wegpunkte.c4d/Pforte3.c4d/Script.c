#strict

local time,pgegner;

Initialize:
  Start();
  return(1);

Start:
  SetPosition(0,0,this());
  SetAction("Check");
  pgegner=GetPlayerCount(); //Anzahl der Gegner

  SetLocal(3,CreateObject(MMO_,1780,330,-1));
  ObjectCall(Local(3),"Gegner2",0,GetPlayerCount() );
  return(1);

Timer:
  if(Local(3))  return(0);
  Ende();
  return(1);

Ende:
  GameCall("Teil3");
  RemoveObject();
  return(1);