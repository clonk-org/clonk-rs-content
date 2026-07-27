/*-- Menüverwaltung --*/

// script v1.5 by miniwipf
#strict 2
#appendto *

// -Gelöschte entries in array (wie sbrd)
// -desc nicht in array
// -zusatz fx's: überschreiben oder ersetzen?
// -wenn menü geschlossen, untermenüs auch schliessen
// -icon und desc setter zusammennehmen
// -Open/Close Menü überarbeiten
// -SetMenuEntryExtra
// -MenuSize
// -MenuDecoration
// Mehr pars für addmenu und addmenuentry

/* ENGINE */

protected func MenuQueryCancel(int iSel, object pMenuObj)
{
	// Effektfunktion aufrufen
	if (!MenuSelection(, pMenuObj, -2,, iSel))
		return _inherited(Par(0), Par(1), Par(2), Par(3), Par(4), Par(5), Par(6), Par(7), Par(8), Par(9));
	return 1; // Menü bleibt geöffnet
}

protected func OnMenuSelection(int iSel, object pMenuObj)
{
	// Effektfunktion aufrufen
	if (!MenuSelection(, pMenuObj, -1,, iSel))
		return _inherited(Par(0), Par(1), Par(2), Par(3), Par(4), Par(5), Par(6), Par(7), Par(8), Par(9));
	return 1; // Menü bleibt geöffnet
}

/* GLOBAL */

// Commits

global func GetMenuFxNr(id idMenu)
{
	var obj = this;
	// Effektliste nach einem Menüeffekt mit id 'idMenu' durchsuchen
	for (var i=0, nr; i < GetEffectCount("IntMenu", obj); i++) {
		nr = GetEffect("IntMenu", obj, i);
		if (EffectVar(0, obj, nr)[0] == idMenu)
			return nr;
	}
	// fail
	return;
}

global func CallMenuFx(id idMenu, string szFoo)
{
	var target = this;
	var nr = target->GetMenuFxNr(idMenu);
	if (nr) return EffectCall(target, nr, szFoo, Par(2), Par(3), Par(4), Par(5), Par(6), Par(7), Par(8));
}

// Menüfunktionen

global func AddMenu(id idMenu, object pCmdObj, id idIcon, string szTitleEmpty)
{
	var target = this;
	return AddEffect("IntMenu", target, 2,, target,, idMenu, [pCmdObj, idIcon, szTitleEmpty]);
}

global func RemoveMenu(id idMenu)
{
	var target = this;
	// Effekt löschen
	var nr = target->GetMenuFxNr(idMenu);
	if (nr) return RemoveEffect(, target, nr);
}

//--

global func OpenMenu(id idMenu) { return CallMenuFx(idMenu, "Open"); }
global func CloseMenu2(id idMenu) { return CallMenuFx(idMenu, "Close"); }

//--

global func GetMenuSelection2(id idMenu) { return CallMenuFx(idMenu, "GetSelection"); }
global func SetMenuParent(id idMenu, id idParent) { return CallMenuFx(idMenu, "SetParent", idParent); }
global func SetMenuStyle(id idMenu, int iStyle) { return CallMenuFx(idMenu, "SetStyle", iStyle); }
global func SetMenuExtra(id idMenu, int iExtra, int iExtraData, bool fOw) { return CallMenuFx(idMenu, "SetExtra", iExtra, iExtraData, fOw); }
global func SetMenuSize2(id idMenu, int iCols, int iRows, bool fOw) { return CallMenuFx(idMenu, "SetSize", iCols, iRows, fOw); }
global func SetMenuDecoration2(id idMenu, id idDeco) { return CallMenuFx(idMenu, "SetDecoration", idDeco); }


// Eintragsfunktionen

global func AddMenuEntry(id idMenu, id idEntry, int iPos, string szCmd, id idIcon, string szTitle, string szText)
{
	return CallMenuFx(idMenu, "AddEntry", idEntry, iPos, [szCmd, idIcon, szTitle, szText]);
}

global func RemoveMenuEntry(id idMenu, int iPos)
{
	return CallMenuFx(idMenu, "RemoveEntry", iPos);
}

global func M_EntryID(id idMenu, int iPos) { return CallMenuFx(idMenu, "EntryID", iPos); }

//--

global func SetMenuEntryBasic(id idMenu, int iPos, id idIcon, string szTitle, string szText, bool fOw) { return CallMenuFx(idMenu, "SetEntryBasic", iPos, idIcon, szTitle, szText, fOw); }
global func SetMenuEntryCount(id idMenu, int iPos, int iCount) { return CallMenuFx(idMenu, "SetEntryCount", iPos, iCount); }
global func SetMenuEntryExtra(id idMenu, int iPos, int iExtra, iXPar1, iXPar2, bool fOw) { return CallMenuFx(idMenu, "SetEntryExtra", iPos, iExtra, iXPar1, iXPar2, fOw); }


// Effekt

/*global func FxIntMenuUpdate(object pTarget, int iNr)
{
	if (!EffectVar(0, pTarget, iNr)[1]) return 1;
	// Menü schliessen
	var ID = GetMenu(pTarget), nr;
	if (ID) {
		nr = pTarget->GetMenuFxNr(ID);
		if (nr) EffectCall(pTarget, nr, "Close", 1);
		else CloseMenu(pTarget); // Keine Effektmenü
	}
	// BUG: Dialogmenüs werden übergross -> Säuberungsaufruf
	if (EffectVar(0, pTarget, iNr)[10] & 3) {
		CreateMenu(, pTarget); CloseMenu(pTarget);
	}

	var m = EffectVar(0, pTarget, iNr);
	// Menü erstellen
	var idSymbol = m[8];
	var pCmdObject = m[7];
	var iExtra = m[11];
	var szCaption =  m[9];
	var iExtraData = m[12];
	var iStyle = m[10];
	var idMenuID = m[0];
	CreateMenu(idSymbol, pTarget, pCmdObject, iExtra, szCaption, iExtraData, iStyle, 1, idMenuID);

	// Einträge hinzufügen
	var e;
	var szCaption, szCommand, idItem, iCount, szInfoCaption, iExtra, iXPar1, iXPar2;
	for (var entry in EffectVar(0, pTarget, iNr)[2]) {
		e = EffectVar(entry, pTarget, iNr);
		// Eintrag erstellen
		szCaption = e[3];
		szCommand = 0; // Bei szCommand = 0 kann man den Eintrag nicht anwählen
		if (e[1]) szCommand = "MenuSelection";
		idItem = e[2];
		iCount = e[5];
		szInfoCaption = e[4];
		iExtra = e[6];
		iXPar1= e[7];
		iXPar2= e[8];
		AddMenuItem(szCaption, szCommand, idItem, pTarget, iCount, pTarget, szInfoCaption, iExtra, iXPar1, iXPar2);
	}
	// Grösse setzen wenn gegeben
	if (m[13] || m[14]) SetMenuSize(m[13], m[14], pTarget);
	// Dekoration
	if (m[15]) SetMenuDecoration(m[15], pTarget);
	// Eintrag anwählen
	EffectVar(0, pTarget, iNr)[5] = 1; // Interne Wahl
	SelectMenuItem(m[4], pTarget);
}*/

// EVar0:[0: ID, 1: fShow, 2: aEntries, 3: aFree, 4: iSelected, 5: IntSel, 6: idParent, 7: pCmdObj, 8: idIcon, 9: szTitleEmpty, 10: iStyle, 11: iExtra, 12: iExtraData, 13: iCols, 14: iRows, 15: idDeco]
// EVarX[0: ID, 1: szCmd, 2: idIcon, 3: szTitle, 4: szText, 5: iCount, 6: iExtra, 7: iXPar1, 8: iXPar2]
global func FxIntMenuUpdate(object pTarget, int iNr)
{
	if (!EffectVar(0, pTarget, iNr)[1]) return 1; // Wird nicht angezeigt
	// Menü schliessen
	var ID = GetMenu(pTarget), nr;
	if (ID) {
		nr = pTarget->GetMenuFxNr(ID);
		if (nr) EffectCall(pTarget, nr, "Close", 1);
		else CloseMenu(pTarget); // Keine Effektmenü
	}
	// BUG: Dialogmenüs werden übergross -> Säuberungsaufruf
	if (EffectVar(0, pTarget, iNr)[10] & 3) { CreateMenu(, pTarget); CloseMenu(pTarget); }
	// Menü erstellen
	var m = EffectVar(0, pTarget, iNr);
	CreateMenu(m[8], pTarget, m[7], m[11], m[9], m[12], m[10], 1, m[0]);
	// Einträge hinzufügen
	var e, cmd;
	for (var entry in EffectVar(0, pTarget, iNr)[2]) {
		e = EffectVar(entry, pTarget, iNr);
		cmd = 0; // Bei cmd = 0 kann man den Eintrag nicht anwählen
		if (e[1]) cmd = "MenuSelection";
		AddMenuItem(e[3], cmd, e[2], pTarget, e[5], pTarget, e[4], e[6], e[7], e[8]);
	}
	// Grösse & Deko
	SetMenuSize(m[13], m[14], pTarget);
	SetMenuDecoration(m[15], pTarget);
	// Eintrag anwählen
	EffectVar(0, pTarget, iNr)[5] = 1; // Interne Wahl; wird nicht weitergeleitet
	SelectMenuItem(m[4], pTarget);
	return 1;
}

// Effektspezifisch

global func FxIntMenuEffect(string szNewName, object pTarget, int iNr, int iNewNr, idMenuNew)
{
	if (iNewNr) return 1; // CheckEffect()-Aufrufe ignorieren
	// Menüeffekt mit gleicher ID wird nicht geduldet
	if (szNewName == GetEffect(, pTarget, iNr, 1))
		if (idMenuNew == EffectVar(, pTarget, iNr)[0]) return -1;
	return 1;
}

global func FxIntMenuStart(object pTarget, int iNr, int iTemp, id idMenu, array aData)
{
	if (iTemp) return;
	if (!idMenu) return -1; // Ohne ID undefiniert!
	// Initialisieren
	EffectVar(0, pTarget, iNr) = [idMenu, 0, [], [1], -1, 0, 0];
	// Parameter übertragen
	var n = 7;
	for (var data in aData)
		EffectVar(, pTarget, iNr)[n++] = data;
	// Menü updaten
	return EffectCall(pTarget, iNr, "Update");
}

global func FxIntMenuStop(object pTarget, int iNr, int iReason, bool fTemp)
{
	if (fTemp) return;
	// Kindermenüs ebenfalls löschen
	for (var i=0, nr; i < GetEffectCount("IntMenu", pTarget); i++) {
		nr = GetEffect("IntMenu", pTarget, i);
		if (EffectVar(0, pTarget, nr)[6] == EffectVar(0, pTarget, iNr)[0])
			RemoveEffect(, pTarget, nr);
	}
	// Menü löschen
	return EffectCall(pTarget, iNr, "Close");
}

//--

global func FxIntMenuOpen(object pTarget, int iNr)
{
	var ID = GetMenu(pTarget), nr;
	if (ID == EffectVar(0, pTarget, iNr)[0])
		if (EffectVar(0, pTarget, iNr)[1]) return; // Unser Menü wird schon angezeigt
	if (ID) {
		nr = pTarget->GetMenuFxNr(ID);
		if (nr) EffectCall(pTarget, nr, "Close", 1);
		else CloseMenu(pTarget); // Keine Effektmenü
	}
	// Anzeigestatus speichern
	if (!EffectVar(0, pTarget, iNr)[1])
		EffectVar(0, pTarget, iNr)[1] = 1;
	// Menü öffnen
	return EffectCall(pTarget, iNr, "Update");
}

global func FxIntMenuClose(object pTarget, int iNr, bool fTemp)
{
	if (!EffectVar(0, pTarget, iNr)[1]) return; // Nicht unser Menü
	// Menü schliessen
	var ID = GetMenu(pTarget);
	if (ID == EffectVar(0, pTarget, iNr)[0]) {
		EffectVar(0, pTarget, iNr)[4] = GetMenuSelection(pTarget); // Menüauswahl speichern
		CloseMenu(pTarget);
	}
	if (fTemp) return 1;
	// Anzeigestatus speichern
	EffectVar(0, pTarget, iNr)[1] = 0;
	// Parentmenü
	var parentID = EffectVar(0, pTarget, iNr)[6], nr;
	if (parentID) {
		nr = pTarget->GetMenuFxNr(parentID);
		if (nr) EffectCall(pTarget, nr, "Update");
	}
	return 1;
}

//--

global func FxIntMenuGetSelection(object pTarget, int iNr)
{
	// Wenn das Menü geschlossen ist geben wir die gespeicherte Auswahl zurück
	if (!EffectVar(0, pTarget, iNr)[1]) return EffectVar(0, pTarget, iNr)[4];
	var ID = GetMenu(pTarget);
	if (ID != EffectVar(0, pTarget, iNr)[0]) return EffectVar(0, pTarget, iNr)[4];
	// Sonst die aktuelle Auswahl
	return GetMenuSelection(pTarget);
}

global func FxIntMenuSetParent(object pTarget, int iNr, id idParent)
{
	EffectVar(0, pTarget, iNr)[6] = idParent;
	return 1;
}

global func FxIntMenuSetStyle(object pTarget, int iNr, int iStyle)
{
	EffectVar(0, pTarget, iNr)[10] = iStyle;
	return EffectCall(pTarget, iNr, "Update");
}

global func FxIntMenuSetExtra(object pTarget, int iNr, int iExtra, int iExtraData, bool fOw)
{
	if (iExtra || fOw) EffectVar(0, pTarget, iNr)[11] = iExtra;
	if (iExtraData || fOw) EffectVar(0, pTarget, iNr)[12] = iExtraData;
	return EffectCall(pTarget, iNr, "Update");
}

global func FxIntMenuSetSize(object pTarget, int iNr, int iCols, int iRows, bool fOw)
{
	if (iCols || fOw) EffectVar(0, pTarget, iNr)[13] = iCols;
	if (iRows || fOw) EffectVar(0, pTarget, iNr)[14] = iRows;
	return EffectCall(pTarget, iNr, "Update");
}

global func FxIntMenuSetDecoration(object pTarget, int iNr, id idDeco)
{
	EffectVar(0, pTarget, iNr)[15] = idDeco;
	return EffectCall(pTarget, iNr, "Update");
}

//--

global func FxIntMenuAddEntry(object pTarget, int iNr, id idEntry, int iPos, array aData)
{
	if (!idEntry) return; // Ohne ID undefiniert!
	// Freien Speicherplatz beziehen
	var last = GetLength(EffectVar(0, pTarget, iNr)[3])-1;
	var entry = EffectVar(0, pTarget, iNr)[3][last];
	if (!last) ++EffectVar(0, pTarget, iNr)[3][0];
	else SetLength(EffectVar(0, pTarget, iNr)[3], last);
	// Absolute Position bestimmen
	if ((iPos = DefPosInArray(iPos, GetLength(EffectVar(0, pTarget, iNr)[2]), 1)) < 0) return;
	// In aEntries einfügen
	if (!AddArrayEntry(entry, EffectVar(0, pTarget, iNr)[2], iPos)) return;
	// Initialisieren
	EffectVar(entry, pTarget, iNr) = [idEntry, 0];
	// Parameter übertragen
	var n = 1;
	for (var data in aData)
		EffectVar(entry, pTarget, iNr)[n++] = data;
	EffectCall(pTarget, iNr, "Update");
	// Auswahl anpassen
	var curr = EffectVar(0, pTarget, iNr)[4];
	if (iPos <= curr) // Vor oder bei Auswahl eingefügt: Auswahl um eins nach hinten
			++EffectVar(0, pTarget, iNr)[4];
	else if (curr < 0) { // Bei noch undefinierter Auswahl (-1) und anwählbarem Eintrag, Auswahl auf neuen Eintrag setzen
		if (EffectVar(entry, pTarget, iNr)[1])
			EffectVar(0, pTarget, iNr)[4] = iPos;
	}
	return iPos;
}

global func FxIntMenuRemoveEntry(object pTarget, int iNr, int iPos)
{
	// Absolute Position bestimmen
	if ((iPos = DefPosInArray(iPos, GetLength(EffectVar(0, pTarget, iNr)[2]))) < 0) return;
	// Speicherplatz freigeben
	var entry = EffectVar(0, pTarget, iNr)[2][iPos];
	var i = 0;
	for (var free in EffectVar(0, pTarget, iNr)[3]) {
		if (entry > free) break;
		++i;
	}
	if (i == 1) --EffectVar(0, pTarget, iNr)[3][0];
	else AddArrayEntry(entry, EffectVar(0, pTarget, iNr)[3], i);
	// Aus aEntries entfernen
	if (!RemoveArrayEntry(iPos, EffectVar(0, pTarget, iNr)[2])) return;
	// Selektion anpassen
	var curr = EffectVar(0, pTarget, iNr)[4];
	if (curr >= iPos)
		--EffectVar(0, pTarget, iNr)[4];
	EffectCall(pTarget, iNr, "Update");
	return iPos;
}

//--

global func FxIntMenuSetEntryBasic(object pTarget, int iNr, int iPos, id idIcon, string szTitle, string szText, bool fOw)
{
	var entry = ArrayEntry(iPos, EffectVar(0, pTarget, iNr)[2]);
	if (!entry) return;
	// Parameter setzen
	if (idIcon || fOw) EffectVar(entry, pTarget, iNr)[2] = idIcon;
	if (szTitle || fOw) EffectVar(entry, pTarget, iNr)[3] = szTitle;
	if (szText || fOw) EffectVar(entry, pTarget, iNr)[4] = szText;
	return EffectCall(pTarget, iNr, "Update");
}

global func FxIntMenuSetEntryCount(object pTarget, int iNr, int iPos, int iCount)
{
	var entry = ArrayEntry(iPos, EffectVar(0, pTarget, iNr)[2]);
	if (!entry) return;
	// Parameter setzen
	EffectVar(entry, pTarget, iNr)[5] = iCount;
	return EffectCall(pTarget, iNr, "Update");
}

global func FxIntMenuSetEntryExtra(object pTarget, int iNr, int iPos, int iExtra, iXPar1, iXPar2, bool fOw)
{
	var entry = ArrayEntry(iPos, EffectVar(0, pTarget, iNr)[2]);
	if (!entry) return;
	// Parameter setzen
	if (iExtra || fOw) EffectVar(entry, pTarget, iNr)[6] = iExtra;
	if (iXPar1 || fOw) EffectVar(entry, pTarget, iNr)[7] = iXPar1;
	if (iXPar2 || fOw) EffectVar(entry, pTarget, iNr)[8] = iXPar2;
	return EffectCall(pTarget, iNr, "Update");
}

global func FxIntMenuEntryID(object pTarget, int iNr, int iPos)
{
	var entry = ArrayEntry(iPos, EffectVar(0, pTarget, iNr)[2]);
	if (!entry) return;
	return EffectVar(entry, pTarget, iNr)[0];
}

//--

// iMode: -2: fCancel, -1: Selection, 0: Normal, 1: bRight
global func MenuSelection(id idItem, object pMenuObj, int iMode, int iValue, int iPos)
{
	// Effekt bestimmen
	var ID = GetMenu(pMenuObj);
	var nr = pMenuObj -> GetMenuFxNr(ID);
	if (!nr) return; // Falscher oder nicht existenter Effekt
	// Interne Wahlen werden nicht weitergeleitet
	if (EffectVar(0, pMenuObj, nr)[5]) {
		EffectVar(0, pMenuObj, nr)[5] = 0;
		return;
	}
	// iMode: -1 Abbruch, 0: Markierung, 1: Auswahl, 2: Auswahl mit Spec2
	++iMode;
	iPos = GetMenuSelection(pMenuObj); // Normalerweise wird die Auswahl nicht übergeben
	var entry, cmd;
	if (iPos < 0) { // -1 = keine Einträge im Menü
		cmd = Format("On_%i_MenuChoice", ID); // default
	}
	else {
		entry = EffectVar(0, pMenuObj, nr)[2][iPos];
		cmd = EffectVar(entry, pMenuObj, nr)[1];
	}
	var pre = ""; // Foo(ID, mode);
	if (this) pre = Format("Object(%d)->", ObjectNumber(this)); // obj -> Foo(ID, mode);
	// Funktion aufrufen
	eval(Format("%s%s(%d, %d, Object(%d))", pre, cmd, iPos, iMode, ObjectNumber(pMenuObj)));
	return 1; // Menü bleibt immer geöffnet
}
