/*--- Holzkugel ---*/

#strict
#include _CBL

public func ExplodeIt()	{
  Explode(2);
  CastObjects(FLAM, 3, 10);
  Sound("Inflame");
  Sound("Fuse");
			}

protected func Hit()
{
  if (fActivated == 0) return(1);
  var x = GetXDir();
  if(x > 0) x = 1;
  if(x < 0) x = -1;
  if(!GBackSolid(x, 0) && !GBackSolid(x*2, 0) && !GBackSolid(x*3, 0)) SetXDir(2 * GetXDir() / 3);
   else SetXDir(-2 * GetXDir() / 3);
  if(!Inside(GetXDir(), -1, 1) && !GBackSolid(0, -3)) SetYDir(RandomX(-50, -15));
  SetRDir(GetRDir() / 2);
  Sound("WoodHit*");
  CreateObject(FLAM);
  var c;
  var i=10;
  while(i--) CreateParticle("MSpark",Random(11)-5,Random(11)-5,0,-Random(10),c=20+Random(41),RGBa(255,Random(100),0,c*4,128));
}

public func HitTarget(pObject)
{
 return(1);
}

protected func Departure(pObj)
{
  if(GetID(pObj)==_HLA) {
	fActivated = 1;
	Incinerate();
	Schedule("ExplodeIt()", 150-Random(50));
			}
  if(GetID(pObj)==CCAN) {
	fActivated = 1;
	Incinerate();
	Schedule("ExplodeIt()", 150-Random(50));
			}
}

protected func RejectEntrance(pNewContainer)
  {
  // Einsammelbar?
  if (fActivated == 0) return(0);
  // Ansonsten ablehnen
  return(1);
  }