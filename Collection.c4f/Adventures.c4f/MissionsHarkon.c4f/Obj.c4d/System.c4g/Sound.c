/*-- Soundeffekte --*/

//Stellt das Echosystem zur Verfügung.

#strict 2


/* Echo */

global func Sound(string szSound, bool fGlobal, object pObj, int iLevel, int iAtPlayer, int iLoop, int fMultiple, int iCustomFalloffDistance)
{
	if(!iCustomFalloffDistance) iCustomFalloffDistance = 300;
	if(iCustomFalloffDistance == -1) iCustomFalloffDistance = 0;
  return inherited(szSound, fGlobal, pObj, iLevel, iAtPlayer, iLoop, fMultiple, iCustomFalloffDistance);
}