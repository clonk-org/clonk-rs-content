/*-- Blitz --*/

#strict 2

local alpha,r,g,b;


/* Initialisierung */

protected func Initialize()
{
 if(GBackLiquid())return RemoveObject();
 
 r=255;
 g=255;
 b=255;
 
  var z = Random(20);
  if (z <= 9)
    SetGraphics(Format("0%d", z));
  else if (z <= 18)
    SetGraphics(Format("%d", z));
  alpha = 0;
  return 1;
}


/* ausblenden */

private func Fading()
{
 if(Random(3))
 {
  var z = Random(20);
  if (z <= 9)
    SetGraphics(Format("0%d", z));
  else if (z <= 18)
    SetGraphics(Format("%d", z));
    }
    
  alpha += 30;
  if (alpha >= 255)
    return RemoveObject();
  SetClrModulation(RGBa(r,g,b,alpha));
  // nicht herunterfallen
  SetYDir(-GetGravity(),0,500);
  return 1;
}

public func ActivateColor(rn,gn,bn)
{
 r=rn;
 g=gn;
 b=bn;
}
