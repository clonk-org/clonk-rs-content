/*-- Zapstock --*/

#strict
#include BAS3

local time;

protected func Initialize()
{
 
  var i= RandomX(5,15),bee;
  while(i--)
    {
    bee= CreateObject(KZAP,-11+Random(22),-11+Random(22),NO_OWNER());
    LocalN("beestick",bee)= this();
    }
  i= RandomX(2,8);
  while(i--)
    {
    bee= CreateObject(KZAP,0,0,NO_OWNER());
    Enter(this(),bee);
    LocalN("beestick",bee)= this();
    }
}

public func ProduceDelay() { return(1000); }

protected func Check()
{
  if(GetCon() < 100)  return();
  // Bienen vermehren sich...
  var rate= 60 /ContentsCount(KZAP);
  if(rate< 5) rate= 5;
  if(ContentsCount(KZAP)> 1)
   if(!Random(rate))
    {
    LocalN("beestick",CreateObject(KZAP,0,0,NO_OWNER()))= this();
    }
}
