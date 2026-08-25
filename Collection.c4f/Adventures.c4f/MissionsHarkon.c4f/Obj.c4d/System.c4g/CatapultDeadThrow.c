/*-- Bäume --*/

#appendto CATA
#strict 2

local pCorps;

public func LoadCorps(pObj)
{
	if(GetAction() == "Ready" && !pCorps)
	{
		pCorps = pObj;
		pCorps->Enter(this);
		pCorps->ObjectSetAction(pCorps, "Tumble", 0, 0, 1);
		SetGraphics(0, this, 0, 1, GFXOV_MODE_Object, 0, 0, pCorps);
		AddEffect("IntCatapultCorps", pCorps, 1, 1, this);
		UpdateGraphics();
	}
}

func UpdateGraphics()
{
	var iPhase = GetPhase();
	if(GetAction() == "Fire")
		iPhase = 7-iPhase;
	var iDir = -1;
	if(GetDir()) iDir = 1;
	if(iPhase == 6)
		SetObjDrawTransform(1000, 0,-5000*iDir, 0, 1000, -1000, this, 1);
	if(iPhase == 5)
		SetObjDrawTransform(1000, 0,-4000*iDir, 0, 1000, -4000, this, 1);
	if(iPhase == 4)
		SetObjDrawTransform(1000, 0,-3000*iDir, 0, 1000, -6000, this, 1);
	if(iPhase == 3)
		SetObjDrawTransform(1000, 0, 1000*iDir, 0, 1000, -8000, this, 1);
	if(iPhase == 2)
		SetObjDrawTransform(1000, 0, 3000*iDir, 0, 1000, -9000, this, 1);
	if(iPhase == 1)
		SetObjDrawTransform(1000, 0, 5000*iDir, 0, 1000,-10000, this, 1);
	if(iPhase == 0)
		SetObjDrawTransform(1000, 0, 9000*iDir, 0, 1000,-10000, this, 1);
}
/*

func ContentsCount()
{
	var iCount = _inherited(...);
	if(pCorps) iCount++;
	return iCount;
}*/

private func Projectile()
{
  var pProjectile=Contents();
	if(pCorps) pProjectile = pCorps;
  var iX = (GetDir()*2-1)*12;
  var iY = -10;
  var iR = Random(360);
  var iXDir =  +8 * (GetDir() * 2 - 1) * iPhase / 6;
  var iYDir = -12 * iPhase / 6;
	if(pCorps)
	{
		iXDir = iXDir*2/3;
		iYDir = iYDir*2/3;
		SetGraphics(0, this, 0, 1, 0, 0, 0, 0);
		pCorps = 0;
	}
  var iRDir = 30;
  Exit(pProjectile,iX,iY,iR,iXDir,iYDir,iRDir);
  // Zufällige Streuung
  // Bei minimaler Spannung: -30 bis +30
  // Bei voller Spannung:    -80 bis +80
  var iDeviation = RandomX(-iPhase * 10 - 20, iPhase * 10 + 20);
  SetXDir(iXDir * 100 + iDeviation, pProjectile, 100);
  SetYDir(iYDir * 100 + iDeviation, pProjectile, 100);
  // Sicht verfolgen wenn kein automatischer Schuss
  if (GetActionTarget(1))
    SetPlrView(GetOwner(GetActionTarget(1)), pProjectile);
}

func FxIntCatapultCorpsTimer(pTarget, iNumber)
{
	if(pTarget->Contained())
	{
		pTarget->SetAction("Tumble");
		pTarget->SetPhase(4);
		pTarget->SetDir(GetDir());
		UpdateGraphics();
	}
	else if(pTarget->GetAction() != "Tumble")
	{
		pTarget->SetAction("Dead");
		return -1;
	}
}