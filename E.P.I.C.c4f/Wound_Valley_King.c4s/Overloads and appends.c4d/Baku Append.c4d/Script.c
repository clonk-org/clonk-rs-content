/*-- Super-T-Flint --*/

#strict
#appendto CY8Z

public func BlowUp() { 
  var hus = FindObject2(Find_Category(C4D_Structure),Find_Distance(100,0,-20));
  var tower = FindObject2(Find_Category(C4D_Structure),Find_Or(Find_AtPoint(-10,0),Find_AtPoint(10,0)));
  if (hus) DoDamage(140,hus);
  if (tower) DoDamage(170,tower);
  var target = FindContents(CLNK,hus);
  if (target)
	Exit(target);
  var explo = CreateObject(ROCK);
  Explode(50,explo);
  CastObjects(FLNT,2,90);
  CastObjects(DFLM,4,90);
  Sound("F32");
  Explode(45);
}
