/*-- Offener Spielfeldrand --*/

// script v1.0 by miniwipf
#strict 2

protected func CheckBorders()
{
	// Prüfen ob ein Objekt gerade aus dem Spielfeld fliegt
	var mapwdt = LandscapeWidth();
	var xdir, xoff, x;
	for (var obj in FindObjects(Find_Func("GetXDir"))) {
		xdir = GetXDir(obj)/10; xoff = obj->GetDefOffset();
		if (xdir < 0) { // Linker Rand
			x = (GetX(obj)+xoff)+xdir;
			if (x <= 0)
				SetPosition(BoundBy(mapwdt+x-xoff, 0, mapwdt+xoff), GetY(obj)+GetYDir(obj)/10, obj, 1);
		}
		else { // Rechter Rand
			x = (GetX(obj)-xoff)+xdir-mapwdt;
			if (x >= 0)
				SetPosition(BoundBy(x+xoff, -xoff, mapwdt), GetY(obj)+GetYDir(obj)/10, obj, 1);
		}
	}
	return 1;
}
