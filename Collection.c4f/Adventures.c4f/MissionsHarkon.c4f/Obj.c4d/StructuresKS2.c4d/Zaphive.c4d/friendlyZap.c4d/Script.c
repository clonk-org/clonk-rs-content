/*-- Zap --*/

#strict

local beestick;

protected func Initialize()
{
  SetAction("Fly");
  SetPhase(Random(6));
}

private func Flying()
{
  var xdir,ydir;
  if(Random(4)) return(1);
  // Geschwindigkeit zufällig bestimmen
  xdir=Random(13)-6;
  ydir=Random(9)-4;
  if(GBackLiquid(xdir,ydir)) return(SetSpeed());
  if(beestick)
   if(ObjectDistance(beestick)> 25) 
    if(!Random(8))
     {
     xdir+=BoundBy((GetX(beestick)-GetX()),-11,11);
     ydir+=BoundBy((GetY(beestick)-GetY())/3,-7,7);
     }
  SetSpeed(xdir,ydir);
}

protected func Check()
{
  // Verschüttet oder im Wasser?: Sterben
  if(GBackSemiSolid()) return(Death());

  // einfach so Sterben
  if(!Random(1600)) return(Death());

  // In den Bienenstock gehen...
  var stick;
  if(!Random(100) && (stick= FindObject(KZHV,-10,-10,20,20,OCF_Fullcon(),0,0,NoContainer())))
    {
    // Nur in den eigenen
    if(stick== beestick) Enter(stick);
    }

  // ... und wieder raus
  if(!Random(80) && Contained()) Exit();

  // Summen
  if(!Random(50)) Sound("Bsss");

  if(!Random(1000) && beestick) beestick= 0;
}

public func CatchBlow() { RemoveObject(); }
public func Damage()    { RemoveObject(); }
protected func Death()  { RemoveObject(); }
