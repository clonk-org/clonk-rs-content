/*-- Arrays --*/

// script v1.6 by miniwipf
#strict 2

global func DefPosInArray(int iPos, int iLength, bool fAdd)
{
	if (iPos < 0) iPos = iLength+fAdd+iPos;
	if (iPos < 0) return -1;
	return iPos;
}

// HINWEIS: Um einen Eintrag einfach am Ende des Arrays hinzuzufügen empfiehlt
//es sich, array[GetLength(array)] = val zu benutzen.
global func AddArrayEntry(C4Value, & aArray, int iPos)
{
	var length = GetLength(aArray);
	if ((iPos = DefPosInArray(iPos, length, 1)) < 0) return;
	// Einträge verschieben
	var temp, otemp = aArray[iPos];
	for (var i=iPos; i < length; i++) {
		temp = aArray[i+1];
		aArray[i+1] = otemp;
		otemp = temp;
	}
	// Wert(C4Value) einfügen
	aArray[iPos] = C4Value;
	return 1;
}

global func RemoveArrayEntry(int iPos, & aArray)
{
	var length = GetLength(aArray);
	if ((iPos = DefPosInArray(iPos, length)) < 0) return;
	// Array verschieben
	for (var i=iPos; i < length-1; i++)
		aArray[i] = aArray[i+1];
	// Array kürzen
	SetLength(aArray, length-1);
	return 1;
}

// ACHTUNG: Rückgabewert für fail ist -1, da 0 in Indizen die erste Stelle ist.
global func FindArrayEntry(C4Value, array aArray, int iInit)
{
	var length = GetLength(aArray);
	// Array durchsuchen
	for (var i=iInit; i < length; i++)
		if (aArray[i] == C4Value) return i;
	// Nix gefunden :(
	return -1;
}

// Stellt sicher, dass der Array bei der Abfrage des Eintrags nicht verlängert wird
global func ArrayEntry(int iPos, & aArray)
{
	var length = GetLength(aArray);
	if ((iPos = DefPosInArray(iPos, length)) < 0) return;
	// Wert zurückgeben
	if (iPos > length-1) return 0;
	return aArray[iPos];
}

global func MatchArrays(& aArray1, array aArray2)
{
	var length = GetLength(aArray1), i = 0;
	while (i < length) {
	 	// Eintrag in Array1 entfernen, wenn er nicht in Array2 vorkommt
		if (FindArrayEntry(aArray2, aArray1[i]) < 0)
			RemoveArrayEntry(aArray1, i);
		else i++;
	}
	return 1;
}
