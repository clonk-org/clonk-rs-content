/*-- Zap --*/

#strict
#appendto ZAP1

local beestick;
local flower;
local pollen;

private func Flying()
{
  var xdir,ydir;
  if(flower && !pollen)
   {
   xdir+=BoundBy((GetX(flower)-GetX()),-11,11);
   ydir+=BoundBy((GetY(flower)-GetY())/3,-7,7);
   }
  else if(beestick)
   if(ObjectDistance(beestick)> 25) 
    if(!Random(8))
     {
     xdir+=BoundBy((GetX(beestick)-GetX()),-11,11);
     ydir+=BoundBy((GetY(beestick)-GetY())/3,-7,7);
     }
  SetSpeed(xdir,ydir);
  return(_inherited());
}

protected func Check()
{
  //...Keinen Blütenstaub dabei und noch keine Blume gefunden, dann mal los?
  if (!pollen && !flower) flower = FindFlower(200);

  //...oder vermehren
  if(beestick)
  if(ObjectDistance(beestick) < 50)
  if(!Random(80))
  {
  LocalN("beestick",CreateObject(ZAP1)) = LocalN("beestick",this());
  }

  // In die Blume gehen...
  var flow;
  if(!Random(25) && !pollen && (flow = FindObject(KFLR,-10,-10,20,20,OCF_Fullcon(),0,0,NoContainer())))
    {
    Enter(flow);
    flower = 0;
    pollen = 1;
    }

  // In den Bienenstock gehen...
  var stick;
  if(!Random(25) && pollen && (stick = FindObject(KZHV,-10,-10,20,20,OCF_Fullcon(),0,0,NoContainer())))
    {
    // Nur in den eigenen
    if(stick == beestick)
	{
	Enter(stick);
	// Und Pollen abladen...
	beestick -> PlaceHoney();
	pollen = 0;
	}
    }

  // ... und wieder raus
  if(!Random(25) && Contained()) Exit();

  if(!Random(1000) && beestick) beestick= 0;

  return(_inherited());
}

private func Sting(obj) {
  if(GetID(obj)==CLNK)
	if (obj->IsZapmaster())
	{
	  Sound("bsss");
	  if(!Random(3)) RemoveObject();
	  return();
	}
  Punch(obj,3+Random(2));
  Sound("Sting");
  if(!Random(3)) RemoveObject();
}

public func FindFlower(maxdist) {  // maximale Distanz zur "Blume"
  var obj;
  // neues Ziel bestimmen falls es nicht zu weit entfernt ist
  if(obj=FindObject(KFLR,RandomX(-maxdist,maxdist),0,-1,-1))
    if(ObjectDistance(obj)<maxdist)
      if(!GBackSemiSolid(GetX(obj)-GetX(),GetY(obj)-GetY()))
        return(obj);
}

// Immer nur 2 Zaps in Blumen oder Stock
protected func RejectEntrance(pIntoObj)
{
if (ContentsCount(ZAP1,pIntoObj) > 2) return(1);
}