/*-- Eiskrähe --*/

#strict 2

#appendto ICCR

/* TimerCall mit KI-Steuerung */

protected func Activity()
{
  // Geräusche machen
  if (!Random(25)) Sound("Raven*");

  // Nichts machen
  if (Random(2) || GetAction() != "Fly") return 1;

  // Nichts machen
  if (!Random(4)) return 1;

  // Umdrehen
  if (Random(2)) return TurnRight();
  return TurnLeft();
}

protected func ContactTop() { return; }

protected func ContactBottom() { return; }

protected func Death()
{
  CastParticles("PSpark", 10, 20, 0, 0, 50, 70, RGBa(255, 255, 100), RGBa(100, 100, 0));
  Sound("Poff");
  if(Local())
  {
    Local()->CastParticles("PSpark", 10, 20, 0, 0, 50, 70, RGBa(255, 255, 100), RGBa(100, 100, 0));
    RemoveObject(Local());
  }
  DropRider();
  RemoveObject();
  return 1;
}
