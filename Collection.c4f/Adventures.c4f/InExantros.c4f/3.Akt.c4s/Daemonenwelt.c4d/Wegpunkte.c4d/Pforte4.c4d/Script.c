#strict

local time,pgegner;

Initialize:
  Start();
  return(1);

Start:
  SetPosition(0,0,this());
  SetAction("Check");

  SetLocal(3,CreateObject(TROL,2170,330,-1)); //Der Troll als letzter Wächter
  ObjectCall(Local(3),"Gegner1",0,2+GetPlayerCount() );

  return(1);

Timer:
  if(!Local(3))  return(Ende());
  return(1);

Ende:
  GameCall("Teil4");
  RemoveObject();
  return(1);