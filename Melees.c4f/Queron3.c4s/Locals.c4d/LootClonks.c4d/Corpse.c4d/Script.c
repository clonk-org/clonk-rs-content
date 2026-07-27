/* Toter Clonk */

#strict 2

public func SetDeathAction(id idClonk)
{
	// Grafik des sterbenden Clonks laden
	SetGraphics(,, idClonk);
	// Gewisse Clonktypen haben ein anderes Animationsfacet
	if (idClonk == KAND) SetAction("DeadKendarian");
	else if (idClonk == MYST) SetAction("DeadMystic");
	else SetAction("Dead");
	return 1;
}

// Steuerung

protected func ControlDig(object pClonk)
	{
	if (!pClonk) return 0; // kommt vom kontrollierenden Spieler
	// Herausnehmen per Graben: Holen-Menü öffnen
	pClonk->SetCommand(0, "Get", this(), 0,0, 0, 1);
	}

protected func Ejection(object pObj)
	{
	if (GetPhase() == 5) CheckContents();
	}

// EndCall von "Dead"
protected func CheckContents()
	{
	if (!Contents()) AddEffect("IntFadeOut", this, 1, 5, this);
	//AddEffect("IntFadeOut", this, 1, 5, ,CRPS);
	}

/* IntFadeOut */

private func FxIntFadeOutTimer(object pTarget, int iNr, int iTime)
{
	var t1 = 100;
	if (iTime > t1) return RemoveObject(pTarget);
	
	/* quadratische abnahme */
	
	var minAlpha = 0;
	var a = Max((255-minAlpha)*(iTime**2)/(t1**2)+minAlpha, 0); // ansteigend
	
	var dwClrMod = GetClrModulation(pTarget);
  if (!dwClrMod) dwClrMod = 16777215 | a<<24;
  else dwClrMod = dwClrMod & 16777215 | a<<24;
  SetClrModulation(dwClrMod, pTarget);
  return 1;
}
