/*-- Bolzen --*/

#appendto BOLT
#strict 2

private func HitTarget(pObject)
{
  Sound("ArrowHit");
  var iDamage = BoltStrength();
  // Im oberen Drittel getroffen?
  if( (GetY()<GetY(pObject)+GetDefHeight(GetID(pObject))/3+GetDefOffset(GetID(pObject), 1) && !pObject->~HasArmor())
		|| iStory == 2)
  {
    // Headshot!! -> nimmt ganze Energie weg
    iDamage = GetPhysical("Energy", 0, pObject)*15/10000;
    Sound("Punch3");
    Message("!", pObject);
  }
  iDamage = iDamage * (100 - pObject->~IsShielded()) / 100;
  DoEnergy(-iDamage,pObject);
  if(Random(3)) Fling(pObject,GetXDir()/80,GetYDir()/80-2);
  RemoveObject();
}
