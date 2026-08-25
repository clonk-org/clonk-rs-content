/* Steinfalle */

#strict 2

local delay;
local idID;
local fActive;

func Initialize()
{
  if(Stuck()) Var(0)=1;
  while(Stuck()) SetPosition(GetX(),GetY()+1);
  if(Var(0)) SetPosition(GetX(),GetY()+5);
  delay=Random(40);
  SetAction("Delaydings");
  return 1;
}

func RockFall()
{
  if(!fActive) return;
  if(Stuck()) return;
  if(!idID) idID = ROCK;
  var pRock = CreateObject(idID,0,0,-1);
  SetXDir(RandomX(-5,5), pRock);
  SetMass(0, pRock);
  Local(0,pRock) = 1;
  return 1;
}

func Activate() { fActive = 1; }
