/* Steinschlag: Kein "Muss im freien sein!"-Error und wird rückbelegt */

#strict 2

#appendto MARK

func Activate(pCaster, pRealcaster)
{
  var pCaller;
  if(pRealcaster) pCaller=pRealcaster; // realcaster ist der clonk und caster das Zauberbuch
  else pCaller=pCaster;
	
  // Nicht im Gebäude zaubern
  if(Contained(pCaller))
  {
    Sound("Error");
    PlayerMessage(GetOwner(pClonk), "$must_be_in_free$", pCaller);
    RemoveObject();
    return 0;
  }

  // Effekt prüfen
  var iResult;
  if (iResult = CheckEffect("BigRockNSpell", 0, 125)) return iResult!=-1 && RemoveObject();
  // Zauber
  Sound("Magic*");

  id = BIRK;
  
  // Zielen wenn möglich
  if (pCaller->~DoSpellAim(this, pCaster)) return 1;
  // Clonk kann nicht zielen:
  ActivateAngle(pCaller, GetDir(pCaller)*180-90);
//  RemoveObject();
  return 1;
}
