#strict
#include TREE

/* Initialisierung */

func Initialize()
{
  SetAction("Tree2");
  SetPhase(Random(5));
  if(!Random(5)) DoCon(RandomX(10,30));
  return(1);
}

/*


protected func Initialize(string szV)
  {
  var i=Random(5);
  if (!Random(3))
    {
    SetAction("Tree1");
    SetSolidMask(i*80,0,80,80);
    }
  else
    {
    SetAction("Tree2");
    if (!Random(3)) DoCon(-Random(40));
    }
  SetPhase(i);
  i=120+Random(136);
  if (!Random(3)) SetClrModulation(RGB(i,i,i));
  }

public func IsTree() { return(true); }

protected func Timer() {
  if(FindObject(CLNK, 0, 1))
    if(!Random(5))
     Sound("crackle*");
}

*\
