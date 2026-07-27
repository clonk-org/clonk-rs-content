/*-- Misteriöser Kopf --*/

// Script v2.0 by Miniwipf
#strict 2

// Damit er Objekt ausspuckt müssen diese sich im Inhalt befinden

// Local
local aItems, iPower, iRndOff;
local iTotalItems;

public func SetItems(array aItemList) // aItems muss richtig übergeben werden: Form [[_ID1, #anzahl1], [_ID2, #anzahl2], ...]
{
	aItems = aItemList;
	// Total items updaten
	iTotalItems = 0;
	for (var i=0; i < GetLength(aItems); i++)
		iTotalItems += aItems[i][1];
	return 1;
}

public func SetPower(int iPower2, int iRndOff2)
{
	iPower = iPower2;
	iRndOff = iRndOff2;
	return 1;
}

public func SwitchOn()
{
  SetAction("On");
}

public func SwitchOff()
{
  SetAction("Off");
}

/* ENGINE */

protected func Initialize()
{
  // Normalerweise ausgeschaltet
  SwitchOff();
  // SolidMask setzen
  UpdateTransferZone();
  return 1;
}

protected func Throw()
{
	// Gravitation nicht normal -> meh!
	if (GetGravity() != 100) return 0;
	
	// auswählprozedere
	var n = Random(iTotalItems), ID;
	for (var i=0; i < GetLength(aItems); i++)
	{
		if ((n-=aItems[i][1]) < 0)
		{
			ID = aItems[i][0];
			break;
		}
	}
	if (!ID) return;
	
	var r = iPower+RandomX(-iRndOff, +iRndOff), a = GetR()-90, d = GetDir();
	var item = CreateContents(ID);
	Exit(item, GetVertex(d, 0), GetVertex(d, 1),, Sin(a + 180*d, r), -Cos(a + 180*d, r));
	// * Spuck *
	Sound("Throw");
	// Partikeleffekt
	var nr = AddEffect("FlintEffect", item, 120, 1, this);
	EffectVar(0, item, nr) = r*9; // Effektzeit bis Zerstörung
	return 1;
}

/*protected func CheckEntrance()
{
	// Doch nur in einem gewissen Bereich aufnehmen
	var dir = GetDir();
	for (var obj in FindObjects(Find_Distance(11, GetVertex(dir, 0), GetVertex(dir, 1)), Find_Category(C4D_Object), Find_Not(Find_ID(MRCK)), Find_Not(Find_ID(CITB)))) // Getarnte Magier (MRCK) sind nicht so lecker
		if (!GetEffect("FlintEffect", obj))
			Enter(this, obj);
	return 1;
}*/

// überladen

public func SetDir()
{
	var r = inherited(Par(0), Par(1), Par(2), Par(3), Par(4), Par(5), Par(6), Par(7), Par(8), Par(9));
	UpdateTransferZone();
	return r;
}

protected func UpdateTransferZone()
{
  return SetSolidMask(GetDir()*20, 52, 20, 52);
}

/* EFFEKTE */

public func FxFlintEffectStart(object pTarget, int iNr, int iTemp, int iMaxLifeTime)
{
	if (iTemp) return;
	EffectVar(0, pTarget, iNr) = iMaxLifeTime;
	return 1;
}

public func FxFlintEffectTimer(object pTarget, int iNr, int iTime)
{
	// Partikeleffekt
	var s;
	for (var i=Random(5); i; i--) {
		s = 20+Random(41);
		CreateParticle("PxSpark", AbsX(GetX(pTarget))+RandomX(-3, 3), AbsY(GetY(pTarget))+RandomX(-3, 3),,, s, RGBa(255, Random(100), s*4, 128));
	}
	CreateParticle("PxSpark", AbsX(GetX(pTarget)), AbsY(GetY(pTarget)),,, 5*5, RGBa(255, 155, 0, 128));
  
	// Jemand hat den Flint gefangen?
	if (Contained(pTarget)) return -1;

	// Zeit abgelaufen -> Flint zerstören * PUFF *
	if (EffectVar(0, pTarget, iNr)-iTime <= 0) 
	{
		RemoveObject(pTarget);
		return -1;
	}
}
