/*-- Zapstock --*/

#strict
#include BAS3

local time;

protected func Initialize()
{
Schedule("inhab()",100);
return(1);
}

public func ProduceDelay() { return(1000); }

protected func Check()
{
  if(GetCon() < 100)  return();

  // Der Stock nestet wenns mehr als 20 Zaps und eine Wabe im Stock sind
  if (ObjectCount2(Find_ID(ZAP1), Find_Distance(50)) > 20)
  {
  var i=20;
  while(i--) RemoveObject(FindObject(ZAP1,0,0,-1,-1));
  var Feed = FindContents(KHNY);
  if (!Feed) return();
  RemoveObject(Feed);
  PlaceObjects(ZAPN,1,"Earth",-100,-50,200,100,0,0);
  }

  // ... Zaps vermehren sich wenn Waben im Stock sind (max 10)
  var cnt = ContentsCount(KHNY);
  if(cnt < 1)  return();
  var add = 10 * Min(10,cnt) / 10;
  time += add;
  if(time > ProduceDelay())  { time -= ProduceDelay(); LocalN("beestick",CreateObject(ZAP1,0,0,NO_OWNER())) = this(); }
}

private func PlaceHoney()
{
  if (ContentsCount(KHNY) < 25) CreateContents(KHNY,this(),1);
  Sound("bsss");
}

protected func Collection() 
{
  if (GetID(Par(0)) == ZAPN)
	{
	Schedule("inhab()",100);
	RemoveObject(Par(0));
	}
  else return(0);
}

protected func inhab()
{
  var i= RandomX(5,15),bee;
  while(i--)
    {
    bee= CreateObject(ZAP1,-11+Random(22),-11+Random(22),NO_OWNER());
    LocalN("beestick",bee)= this();
    }
  i= RandomX(2,8);
  while(i--)
    {
    bee= CreateObject(ZAP1,0,0,NO_OWNER());
    Enter(this(),bee);
    LocalN("beestick",bee)= this();
    }
  return(1);
}