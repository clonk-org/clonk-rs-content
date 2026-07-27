/* Assassine aufdecken */

#strict 2

local aHidden, iN;

func Activate(object pCaster, object pRealcaster)
{
	aHidden = FindObjects(Find_ID(ASAS), Find_Func("IsHidden")); // alle verdeckten Assas speichern
	// Assas entdeckt?
  if (!GetLength(aHidden))
  {
  	Message("$NoHiddenAssas$", pCaster);
  	RemoveObject();
  	return 0;
  }
	AddEffect("Scouting", this, 100, 10, this);
	Sound("Magic1");
  return 1;
}


private func FxScoutingTimer()
{
	var assa = aHidden[iN++];
	if (assa)
	{
		if (AddEffect("ScoutingNSpell", assa, 250, 1,, GetID()))
			Sound("Scout",, assa); // wird u.a. vom Immunitätstrank geblockt
		return 1;
	}
	RemoveObject();
}

public func FxScoutingNSpellTimer(object pTarget, int iNr, int iTime)
{
	var x = AbsX(GetX(pTarget)), y = AbsY(GetY(pTarget));
	CreateParticle("TSpark", x, y, RandomX(-5,5), RandomX(-5,5), 5*(20*10-iTime*10), RGBa(iTime*10+Random(50),iTime*10+Random(50),iTime*10+Random(50), 100-iTime*2),, 1);
	var a = Random(360);
	CreateParticle("MSpark", x+Sin(a, 80-iTime*4), y-Cos(a, 80-iTime*4), -Sin(a, 10), Cos(a, 10), 50, RGB(200,200,255));
	// Assa aufdecken
	if (iTime > 20)
	{
		pTarget->UnHide();
		return -1;
	}
}

public func GetSpellClass(object pMage) { return(WATR); }
public func GetSpellCombo(pMage) { return ("555"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
