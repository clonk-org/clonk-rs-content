/*-- Schadenanzeige --*/

// script v1.6 by miniwipf
#strict 2

local iOverlayCnt, dwColor;

global func ViewDamage(int iDmg, int iByPlr)
{
	if (!this) return 0;
	var obj = CreateContents(DVWR);
	obj->ResizeTo(30);
	// rausspringen
	obj->SetAction("Jump");
	obj->Exit(,,,,, -5);
	var nr = AddEffect("IntDmgStack", this, 2, 3, obj,, iDmg, iByPlr);
	// Effekt übernommen? -> Befehlsziel ist nicht mehr obj -> überflüssig!
	if (GetEffect(, this, nr, 4) != obj) RemoveObject(obj);
	return 1;
}

public func ResizeTo(int iSize)
{
	SetCon(iSize);
}

public func DisplayNumber(int iNumber)
	{
	var o=0, n = Digits(iNumber);
	SetGraphics(,, [SGNP, SGNM][iNumber<0], ++o, GFXOV_MODE_IngamePicture); // wegen einfacher Skalierung mit SetCon()
	for (var i=0; i < n; i++)
		SetGraphics(,, C4Id(Format("DNR%d", GetDigit(iNumber, i))), ++o, GFXOV_MODE_IngamePicture);
	// restliche Overlays löschen
	while (i++ < iOverlayCnt)
		SetGraphics(,,, ++o);
	iOverlayCnt = 1+n;
	// Ausrichten
	AdjustOverlays();
	// Farbe updaten (damit evtl. neue 10er-Potenzen auch in derselben Farbe dargestellt werden)
	Colorize(dwColor);
	return 1;
	}

public func Colorize(int dwClr)
{
	dwColor = dwClr;
	for (var i=0; i < iOverlayCnt; i++)
		SetClrModulation(dwColor,, i+1);
	// Farbmodulation in FxIntFlash updaten
	var nr = GetEffect("IntFlash", this, 0);
	if (nr)
		{
		for (var i=0; i < iOverlayCnt; i++)
			EffectVar(0, this, nr)[i] = dwClr;
		}
}

public func Flash(int iSize)
{
	if (!iSize)
		{
		var nr = GetEffect("IntFlash", this);
		if (nr) iSize = EffectVar(1, this, nr)*6/4;
		else iSize = GetCon()*5/4;
		}
	return AddEffect("IntFlash", this, 2, 1, this,, iSize);
}

private func AdjustOverlays()
	{
	var w = 40;
	// Overlay Offset für erste Ziffer berechnen
	var off = -w*iOverlayCnt/2+w/2;// = 0;//-w*iOverlayCnt/2-w/2;
	for (var i=0; i < iOverlayCnt; i++) // dann ist jede Ziffer um + breite verschoben
		{
		SetObjDrawTransform(1000,0, off*1000,0, 1000,0,, i+1);
		off += w;
		}
	}

private func GetDigit(int iNumber, int iIndex)
	{
	return GetChar(Format("%d", Abs(iNumber)), iIndex)-48; // 0 startet bei 48 Ascii
	}

private func Digits(int iNumber)
	{
	return GetLength(Format("%d", Abs(iNumber)));
	}

/* IntFlash */

protected func FxIntFlashEffect(string szNewName, object pTarget, int iNr, int iNewEffectNumber)
{
	//Log("IntFlashEffect: szNewName=%s, pTarget=%v, iNr=%d, iNewNr=%d", szNewName, pTarget, iNr, iNewEffectNumber);
	if (szNewName == GetEffect(, pTarget, iNr, 1))
	{ // diesen Effekt entfernen
		return RemoveEffect(, pTarget, iNr);
		}
}

protected func FxIntFlashStart(object pTarget, int iNr, int iTemp, int iSize)
{
	//Log("IntFlashStart:pTarget=%v, iNr=%d, iTemp=%d", pTarget, iNr, iTemp);
	if (iTemp) return 0;
	// Farbmodulation speichern und weiss flashen
	EffectVar(0, pTarget, iNr) = [];
	for (var i=0; i < iOverlayCnt; i++)
		{
		EffectVar(0, pTarget, iNr)[i] = GetClrModulation(, i+1);
		SetClrModulation(RGB(255,255,255),, i+1);
		}
	// aktuelle Grösse speichern und vergrössern
	EffectVar(1, pTarget, iNr) = GetCon();
	EffectVar(2, pTarget, iNr) = iSize;
	SetCon(iSize);
	return 1;
}

protected func FxIntFlashTimer(object pTarget, int iNr, int iTime)
{
	var t1 = 8;
	if (iTime > t1) return -1;
	
	/* quadratische abnahme*/
	
	SetCon(-(EffectVar(2, pTarget, iNr)-EffectVar(1, pTarget, iNr))*(iTime**2)/(t1**2)+EffectVar(2, pTarget, iNr));
	
	var r,g,b,a, maxRGB=300;
	for (var i=0; i < iOverlayCnt; i++)
		{
		SplitRGBaValue(EffectVar(0, pTarget, iNr)[i], r,g,b,a);
		// nur Farbwerte setzen
		r = Min(-(maxRGB-r)*(iTime**2)/(t1**2)+maxRGB, 255);
		g = Min(-(maxRGB-g)*(iTime**2)/(t1**2)+maxRGB, 255);
		b = Min(-(maxRGB-b)*(iTime**2)/(t1**2)+maxRGB, 255);
		a = GetClrModulation(, i+1)>>24;
		SetClrModulation(RGBa(r,g,b,a),, i+1);//Log("FlashClr=%x",RGBa(r,g,b,a));
		}
	return 1;
}

protected func FxIntFlashStop(object pTarget, int iNr, int iReason, bool fTemp)
{
	//Log("IntFlashStop:pTarget=%v, iNr=%d, iReason=%d, fTemp=%d", pTarget, iNr, iReason, fTemp);
	if (fTemp) return 0;
	// Normalgrösse und Farbe
	var dwClrMod;
	for (var i=0; i < iOverlayCnt; i++)
		SetClrModulation(EffectVar(0, pTarget, iNr)[i] | (GetClrModulation(, i+1) & 255<<24),, i+1);
	SetCon(EffectVar(1, pTarget, iNr));
	return 1;
}

/* IntDmgStack */

// 0: int iDmg
// 1: int iPlr
// 2: int aInitialAlpha
// 3: int t1 (Peak-Zeit)
public func FxIntDmgStackEffect(string szNewEffectName, object pTarget, int iNr, int iNewNr)
	{
	// Gleichen Effekt übernehmen
	if (szNewEffectName == GetEffect(, pTarget, iNr, 1))
		if (GetEffect(, pTarget, iNr, 6) < EffectVar(3, pTarget, iNr)) // nur falls Effektzeit < t1
			return -2;
	}

public func FxIntDmgStackAdd(object pTarget, int iNr, string szNewEffectName, int iNewTimer, int iDmg, int iByPlr)
	{
	if (!iNewTimer) return 1; // CheckEffect
	// Schaden aufaddieren
	EffectVar(0, pTarget, iNr) += iDmg;
	/*EffectVar(3, pTarget, iNr) += 8;
	SetYDir(GetYDir()-15);*/
	// Schaden & Farbe updaten
	DisplayNumber(EffectVar(0, pTarget, iNr));
	if (GetPlayerName(iByPlr)) Colorize(GetPlrColorDw(iByPlr));
	else Colorize(GetPlrColorDw(GetOwner(pTarget)));
	Flash();
	return 1;
	}

public func FxIntDmgStackStart(object pTarget, int iNr, int iTemp, int iDmg, int iByPlr)
	{
	if (iTemp) return 0;
	// Schaden speichern
	EffectVar(0, pTarget, iNr) = iDmg;	
	// alpha speichern
	EffectVar(2, pTarget, iNr) = [];
	for (var i=0; i < iOverlayCnt; i++)
		EffectVar(2, pTarget, iNr)[i] = GetClrModulation(, i+1)>>24;
	// t1 -Zeit bis Fade out
	EffectVar(3, pTarget, iNr) = 20;
	// Schaden anzeigen
	DisplayNumber(iDmg);
	if (GetPlayerName(iByPlr)) Colorize(GetPlrColorDw(iByPlr));
	else Colorize(GetPlrColorDw(GetOwner(pTarget)));
	Flash();
	// Schaden in Promille anzeigen
	/*var pmDmg = 1000*EffectVar(0, pTarget, iNr)/GetPhysical("Energy", 0, pTarget);
	var sign = "";
	if (pmDmg > 0) sign = "+";
	else if (pmDmg < 0) sign = "-";
	return Message("@<c %x>%s%d.%d</c>", this, rgba, sign, Abs(pmDmg)/10, Abs(pmDmg)%10);*/
	return 1;
	}

public func FxIntDmgStackTimer(object pTarget, int iNr, int iTime)
	{
	var t1 = EffectVar(3, pTarget, iNr), t2 = t1+20;
	if (iTime > t2) return -1;
	// Wenn über Peak-Zeit ausfaden
	if (iTime > t1)
		{
		/* lineare abnahme */
		
		var a, dwClrMod;
		for (var i=0; i < iOverlayCnt; i++)
			{
			a = (255-EffectVar(2, pTarget, iNr)[i])*(iTime-t1)/(t2-t1); // ansteigend
			
			// nur Alpha setzen
			dwClrMod = GetClrModulation(, i+1);
  		if (!dwClrMod) dwClrMod = a<<24;
 		 	else dwClrMod = dwClrMod & 16777215 | a<<24; // untere 24 Bits (=16777215) für RGB
			SetClrModulation(dwClrMod,, i+1);//Log("DmgStackClr=%x",dwClrMod);
			}
		}
	return 1;
	}
	
public func FxIntDmgStackStop(object pTarget, int iNr, int iReason, bool fTemp)
	{
	if (fTemp) return;
	if (iReason == 4) return -1; // Clonk Stirbt -> Effekt noch ausfaden lassen
	if (iReason == 0) return RemoveObject(); // Fertig gefadet -> Befehlsziel löschen
	}
