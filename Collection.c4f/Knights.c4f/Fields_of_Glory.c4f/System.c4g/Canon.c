/*-- Kanonen Setup --*/

#strict
#appendto CCAN

protected func DoAutoReload() { return(0); }		// Automatisch Nachladen? (Zeit in Frames)
protected func LoadsMax() { return(2); }		// Wie viele Ladungen zulassen?
protected func CanonballsOnly() { return(TRUE); }	// Nur Kanonenkugeln laden?
protected func GetAutoAmmo()				// Kanonen-, Chemie-, Kristal-, Holz- oder Granitkugel für DoAutoReload
{
pAmmos = [_CBL,_CCL,_XCB,_GRT,_CBT];
return (pAmmos[Random(GetLength(pAmmos))]);
}

protected func CheckProjectile(id idObj)		// Prüfung zulässiger Projektile für CanonballsOnly
{
pAmmos = [_CBL,_CCL,_XCB,_GRT,_CBT];			// Kanonen-, Chemie-, Kristal-, Holz- und Granitkugeln sind erlaubt
	for (var item in pAmmos)
		if(idObj==item) return(1);
return(0);
}