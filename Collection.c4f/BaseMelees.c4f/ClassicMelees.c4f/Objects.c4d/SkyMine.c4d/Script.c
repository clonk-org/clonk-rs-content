/*-- Ballon Mine--*/

#strict
#include _BLN

local Triggered;

private func Wind2Float()
{
  //Luftfahrzeug in der Nähe dann Explosion auslösen. Nur Raketen, Balons, Helis und Luftschiffe.
  for(var pObj in FindObjects(Find_Category(C4D_Vehicle), Find_Distance(150), Find_Exclude(this()), Find_Or(Find_ID(1RKT),Find_ID(BALN),Find_ID(BLMP),Find_ID(_HEL))))
	//Nur fliegende Objekte triggern
	if(!Triggered) TriggerMine();
  return(_inherited(...));
}

private func TriggerMine()
{
Triggered=1;
Explode(15);
CastObjects(_SKA,24,Random(100)+25,0,-100);
}