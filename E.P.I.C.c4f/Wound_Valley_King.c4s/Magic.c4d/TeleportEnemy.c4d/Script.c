#strict

public func Activate(oCaller, oClonk)
{                            
  if(!oClonk) oClonk = oCaller;
  var targets = FindObjects(Find_Exclude(oClonk),Find_OCF(OCF_CrewMember),Find_Distance(200));
  for(var x in targets)
	Teleporter(x);
  return(1);
}

private func Teleporter(target) 
{
	CastObjects(MSTB,10,25, GetX(target)-GetX(), GetY(target)-GetY());
  	CastParticles("MSpark", 200, 50, GetX(target)-GetX(),GetY(target)-GetY(), 10, 50, RGBa(100,255,100,128), RGBa(0,255,0,0));
	Sound("Plopp");

	var x = Random(LandscapeWidth());                       
	var y = 250;
	var checkX,checkY,matCheck1,matCheck2;

	checkX = (GetX() - x);
	checkY = (GetY() - y);
	matCheck1 = (!(GetMaterial(checkX,checkY) == Material("Tunnel")));
	matCheck2 = (!(GetMaterial(checkX,checkY) == Material("Earth")));

	while ((matCheck1) && (matCheck2))
	{
		y++;
		checkX = (x-GetX());
		checkY = (y-GetY());
		matCheck1 = (!(GetMaterial(checkX,checkY) == Material("Tunnel")));
		matCheck2 = (!(GetMaterial(checkX,checkY) == Material("Earth")));
	}
	SetPosition(x,(y-15),target);
}