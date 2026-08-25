/*-- Zielscheibe --*/

#strict 2

/* Initialisierung */

local iRingCounter;

protected func Initialize()
{
  SetAction("Stand");
  return 1;
}

func Ring()
{
  Sound("BellRing.wav");
  iRingCounter++;
  if(iRingCounter>4)
    SetAction("SwingEnd");
}

func StartSwing()
{
  iRingCounter = 0;
  if(GetAction() == "Stand")
    SetAction("Swing0");
  else if(GetAction() == "SwingEnd")
  {
    var iPhase = GetPhase();
    SetAction("Swing0");
    SetPhase(6-iPhase);
  }
}

