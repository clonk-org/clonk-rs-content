/*-- Rakete angeschraubt --*/

#strict

local FillLevel;
local iMaxFillLevel;
local pOwner;
local pTarget;
local pSelection;
local clonksize;

protected func ConnectID() { return(_RTA); }
protected func GetAlternativeType() { return(ConnectID()); }

/* Erstellen */

public func Initialize() { iMaxFillLevel=100; }

public func ControlDig(pClonk)
{
  pOwner = GetOwner(pClonk);
  // Clonk anhalten
  SetComDir(COMD_Stop(), pClonk);
  // Menu Aufruf und Überwachung starten
  clonksize=GetDefCoreVal("Offset",0,GetID(pClonk),1);
  DoMenu(pClonk);
  var smec=CreateObject(_MEC);
  smec->Init(pClonk,this(), 1);
  // fertig 
  return(1);
}

protected func DoMenu(pClonk,pSelection)
{
   pTarget=pClonk;
   /* Erzeugt das Menü mit dem Bild vom Ölfass.Wenn das Menü keine Einträge enthält, steht als Überschrift: "Tankfüllung".Der dritte Parameter nach der Überschrift (die 1) bedeutet das das Menü nach einer Wahl geöffnet bleibt. */
   CreateMenu(1RKT, pClonk, this(), C4MN_Extra_None, "$TxtFillLevel$", 0, C4MN_Style_Context, false);
   /* Erzeugt einen Menübeitrag für den Spieler. Der erste Parameter ist der Name vom menüeintrag (wie der Eintrag heißen soll), der zweite Parameter ist die Funktion, der dritte Parameter ist das Bildchen und der fünfte Parameter setzt fest, für wen der Menüeintrag hinzugefügt werden soll. */
   AddMenuItem("$TxtMoreFuel$", "ChangeFillLevel(10,pTarget)", MORE, pClonk,0,0,"",128,0);
   AddMenuItem("$TxtLessFuel$", "ChangeFillLevel(-10,pTarget)", LESS, pClonk,0,0,"",128,0);
   AddMenuItem("$TxtSetFuel$", "", BILD, pClonk,iMaxFillLevel,0,"",128,0);
   SelectMenuItem(pSelection,pTarget); 
}

private func ChangeFillLevel(iVal,pClonk)
{
	pSelection=0;
	if(iVal<0) pSelection=1;
	iMaxFillLevel += iVal;
	if(iMaxFillLevel<10) iMaxFillLevel=10;
	if(iMaxFillLevel>200) iMaxFillLevel=200;
	DoMenu(pClonk,pSelection);
}

/* Wird vom Turm als Kanone erkannt */

public func IsCannon() { return(1); }

/* An Turm anschrauben */

public func Connect(object pTower)
{
  pTower->ConnectCannonDef(this(), ConnectID() );
}

/* Aufschlaggeräusch */

protected func Hit()
{
  Sound("ClonkHit*");
}

/* Nur Schießpulver als Ladung nehmen

protected func RejectCollect(id idObj,object pObj)
{
if (idObj == GUNP)
	if(ContentsCount() < 3)
		return(0, Sound("Click"));
  return(1);
}
*/

/* Forschung */  

public func GetResearchBase() { return(CTW0); }