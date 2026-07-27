/*-- Das Kombomenü zur Auswahl von Zaubern --*/
//
// Callbacks in den Zauberklassen:
// IsSpellClass(pMage): (bool) Rückgabewert true, wenn die Klasse von pMage angewählt werden kann
// GetElementColor: (dword) DWORD-Wert (ohne Alpha) der Elementfarbe
//
// Callbacks in den Zaubern:
// GetSpellClass: (id) ID der Zauberklasse (FIRE, WATR, EART, AIR1, MISC)
// GetSpellCombo: (string) Tastenkombination (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
//
// Aufruf des Menüs:
// global func CreateSpellMenu(object pCommandObject, object pMenuObj, id idSpellClass)
//
// Callback aus dem Menü (an pCommandObject):
// public func OnComboMenuEnter(id idSpell)  - Spruch ausgewählt
// public func OnComboMenuAbort()            - Menü abgebrochen

#strict

#include CBMU

static const CBM2_ShowElementalSymbols = false; // Wenn wahr, werden anstatt der Zauber auf höchster Menüebene Klassensymbole angezeigt


/* Initialisierung */

public func Create(object pCommandObject, object pMenuObj, id idSpellClass, bool faFixedDir)
  {
  // Felder kopieren
  pCallbackObject = pCommandObject;
  pTargetObject = pMenuObj;
  idSelectedElement = idSpellClass;
  fFixedDir = faFixedDir;
  // Aktivität setzen
  SetAction("Open", pMenuObj);
  // Eine volle Energieleiste sieht doch viel schöner aus :)
  DoEnergy(100);
  // Als Cursorobjekt auswählen, damit Tastaturkommandos gesendet werden und die Sicht stimmt
  SetCursor(GetOwner(), this(), 1, 1);
  SelectCrew(GetOwner(), pMenuObj, 0, 1);
  SetViewCursor(GetOwner(), pMenuObj); // Magier als ViewCursor
  // Letztes Kommando löschen, um ControlSpecial auch beim schnellen ControlSpecialDouble abzufangen
  ClearLastPlrCom(GetController(pMenuObj));
  // Überwachungseffekt fürs Schließen an den Clonk
  AddEffect("IntCBMUCheck", pMenuObj, 1, 0, this());
  
  szCastKeys="3";
  iCastControlCount=1;
  
  // Symbole erstellen
  return(CheckSpells());
  }


global func CreatePaladinSpellMenu(object pCommandObject, object pMenuObj, id idSpellClass, bool faFixedDir)
  {
  // Menüposition beenden
  var x = pMenuObj->GetX(), y = pMenuObj->GetY();
  if(CBMU_OverheadMenu) y -= 120;
  // Erzeugen
  var pMenu = CreateObject(CBM2,AbsX(x),AbsY(y),GetOwner(pMenuObj));
  if (pMenu) pMenu->Create(pCommandObject, pMenuObj, idSpellClass, faFixedDir);
  return (pMenu);
  }


/* Zaubersymbole */

// Zaubersymbol-Overlay erzeugen
private func CreateSpellSymbol(idSpell, szAction, iPosX, iPosY,fSmall,&riMinX, &riMaxX, &riMinY, &riMaxY)
  {
  // Der Menü-Basispunkt liegt leicht unterhalb des Objektes
  if(CBMU_OverheadMenu) iPosY += 40;
  // Symbol erzeugen
  var pSymbol = CreateObject(MGSM, 0,0, GetOwner());
  pSymbol->SetSymbolGfx(szAction, idSpell, this(), pCallbackObject);
  if(!CBMU_OverheadMenu && fSmall) pSymbol->SetCon(pSymbol->GetCon() / 2);
  SetVertex(0, 0, -iPosX, pSymbol); SetVertex(0, 1, -iPosY, pSymbol);
  // Grenzwerte anpassen
  if (iPosX < riMinX) riMinX = iPosX; else if (iPosX > riMaxX) riMaxX = iPosX;
  if (iPosY < riMinY) riMinY = iPosY; else if (iPosY > riMaxY) riMaxY = iPosY;
  // Sichtbarkeit nur für den Besitzer oder neutrale Viewports
  pSymbol->SetVisibility(VIS_Owner() | VIS_God());
  // Fertig; Symbol zurückgeben
  return(pSymbol);
  }

private func GetSpellCombo(id idSpell, id idClass, int i)
 {
  // Standard-Spellcombo
  //var szCombo = idSpell->GetSpellCombo();
  
  var szCombo="";
  i-=1;
  var combo=["4", "6", "2", "5", "1", "3"];
  var l=GetLength(combo);
  
  //for(var cnt=0;cnt<3;++cnt)
  {
  	szCombo=Format("%s%s", szCombo, combo[i%l]);
  	szCombo=Format("%s%s", szCombo, combo[(i/l)%l]);
  	szCombo=Format("%s%s", szCombo, combo[((i/l)/l)%l]);
  	//i%=l;
  }
  
  // Kein ausgewähltes Element: Elementkombo voranstellen
  if (!idSelectedElement && szCombo)
    if (idClass == MISC)
      szCombo = Format("3%s", szCombo);
    else
      szCombo = Format("%s%s", idClass->GetClassCombo(), szCombo);
  // Combo zurückgeben
  return(szCombo);
  }
  
private func CheckSpells()
  {
  var i,idSpell,iCount,szKey,szCombo,idClass, iKey, idLastSpell;
  // Alle bekannten Sprüche prüfen
  while (idSpell = pCallbackObject->GetCastableWonder(i++))
  //while (idSpell = GetDefinition(i++, 1<<17))
    if (idClass=idSpell->~GetSpellClass(this()))
      if (idClass == idSelectedElement || idClass == MISC || !idSelectedElement)
      	if(!idSpell->~HasItemRequirements() || (FindContents(idSpell->~HasItemRequirements(), pCallbackObject)))
        {
        // Tastenkombination für den Spruch abfragen
        szCombo = GetSpellCombo(idSpell, idClass, i);
        // Spruch gefunden? (Keine weiteren Tasten erforderlich) -> Ausführen!
        if (szCombo eq szCastKeys) return(DoSpell(idSpell));
        // Bisherige Tastenkombination nicht OK?
        var j=iCastControlCount, fWrong=0;
        while (j--) if (GetChar(szCombo, j) != GetChar(szCastKeys, j)) fWrong=1;
        if (fWrong) {continue;}
        // Spruch sichern
        idLastSpell = Var(12+iCount*2) = idSpell;
        // Zugehörige Taste ermitteln, zählen und sichern
        ++Var(Var(13+iCount++*2) = GetComCharIndex(GetChar(szCombo, iCastControlCount)));
        }
  // Nur ein Spruch gefunden?
  if (iCount == 1)
    // Und nicht Klassenwahl?
    if (iCastControlCount>1 || idSelectedElement || CBM2_ShowElementalSymbols)
      // Dann weitere Kombo überspringen und diesen zaubern
      {return(DoSpell(idLastSpell));}
  // Kein Spruch passt? Menü abbrechen
  if (!iCount) return (Abort());
  // Bild aufbauen
  var iXMin, iXMax, iYMin, iYMax, iSymCount, iMaxSymCount, x,y;
  ClearSpellSymbols();
  i=iCount;
  while (i--)
    {
    // Zauber und Taste ermitteln
    idSpell=Var(12+i*2); iKey=Var(13+i*2); iSymCount=Var(iKey);
    if(CBMU_OverheadMenu)
      {
      // Tastenposition ermitteln
      var iKeyX = iKey%3-1,iKeyY=-iKey/3;
      if(!fFixedDir) if (!iKeyY && !GetDir(pTargetObject)) iKeyX=-iKeyX;
      // Basisposition ermitteln
      var iBaseX = iKeyX * 50, iBaseY = iKeyY * 50;
      }
    else
      {
      // Winkel ermitteln
      var iAngle = 0;
      if(iKey == 0) if (GetDir(pTargetObject) || fFixedDir) iAngle = 150; else iAngle = 30;
      if(iKey == 1) iAngle = 90;
      if(iKey == 2) if (GetDir(pTargetObject) || fFixedDir) iAngle = 30; else iAngle = 150;
      if(iKey == 3) iAngle = -150;
      if(iKey == 4) iAngle = -90;
      if(iKey == 5) iAngle = -30;
      // Und daraus Basisposition
      var iBaseX = Cos(iAngle, 35), iBaseY = Sin(iAngle, 35);
      }
    // Erster Zauber auf dieser Taste?
    iSymCount = Var(iKey+6)++; iMaxSymCount = Var(iKey);
    if (!iSymCount)
      // Tastensymbol erzeugen
      CreateSpellSymbol(0, GetComName(iKey), iBaseX, iBaseY, true, iXMin, iXMax, iYMin, iYMax);
    // Bei Elementwahl nur das Elementsymbol anzeigen
    if (!iCastControlCount && !idSelectedElement && CBM2_ShowElementalSymbols)
      if (!iSymCount)
        {
        idSpell = idSpell->GetSpellClass(this());
        iMaxSymCount = 1;
        }
      else
        // Elementsymbol schon erzeugt: Okay; Zauber überspringen
        {continue;}
    // Position für das Zaubersymbol ermitteln
    if(CBMU_OverheadMenu)
      {
      var iYMove; if (iKey==1) iYMove=1; else if (iKey==4) iYMove=-1; else iYMove=0;
      if (iYMove)
        {
        // Oben oder unten: Spaltenzahl (max. 4)
        var iColNum = BoundBy(Sqrt(iMaxSymCount), 1, 4);
        var iSpellsInThisRow;
        if (iSymCount/iColNum >= (iMaxSymCount-1)/iColNum)
          // Letzte Reihe: Weniger Spalten
          iSpellsInThisRow = (iMaxSymCount-1)%iColNum +1;
        else
          // Ansonsten maximale Spaltenzahl
          iSpellsInThisRow=iColNum;
        // Symbolposition
        x = iBaseX + (iSymCount % iColNum) * 35 - (iSpellsInThisRow-1) * 35/2;
        y = iBaseY + (iSymCount/iColNum+1) * 35 * iYMove;
        }
      else
        {
        // Links und rechts: Zeilenzahl (max. 2);
        var iRowNum = BoundBy(iMaxSymCount/4+1, 1, -iKeyY+1);
        var iSpellsInThisCol;
        if (iSymCount/iRowNum >= (iMaxSymCount-1)/iRowNum)
          iSpellsInThisCol = iMaxSymCount%iRowNum+1;
        else
          iSpellsInThisCol = iRowNum;
        x = iBaseX +iKeyX * ((iSymCount/iRowNum) * 35 + 50);
        y = iBaseY +iKeyY*20 + (iSymCount%iRowNum)*35 - (iSpellsInThisCol-1)*35/2;
        }
      }
    else
      {
      // Reihe und Spalte ausrechnen
      var iRow = 0, iCol = iSymCount, iRemain = iMaxSymCount;
      while(iCol > iRow) { iRow++; iCol -= iRow; iRemain -= iRow; }
      // Radius, Winkelabstand zwischen Symbolen
      var iRadius = 70 + iRow * 40,
          iSymAngleDiff = iRadius / 5;
      // Anzahl Symbole in dieser Reihe
      var iCols = Min(iRow + 1, iRemain);
      // Winkel berechnen
      iAngle = iAngle - (iSymAngleDiff * (iCols - 1)) / 2 + iSymAngleDiff * iCol;
      x = Cos(iAngle, iRadius);
      y = Sin(iAngle, iRadius);
      }
    // Zaubersymbol erzeugen
    var pSymbol = CreateSpellSymbol(idSpell, 0, x,y, false, iXMin, iXMax, iYMin, iYMax);
    }
  // Prüfen, ob das Menü aus der Landschaft verschwindet (2do: Nötigenfalls stauchen?))
  var iMoveX, iMoveY;
  if (iXMin+GetX() < 17) iMoveX = 17-iXMin-GetX(); else if (iXMax+GetX()>LandscapeWidth ()-17) iMoveX = -17-iXMax-GetX()+LandscapeWidth ();
  if (iYMin+GetY() < 17) iMoveY = 17-iYMin-GetY(); else if (iYMax+GetY()>LandscapeHeight()-17) iMoveY = -17-iYMax-GetY()+LandscapeHeight();
  if (iMoveX || iMoveY) {MoveSpellSymbols(iMoveX, iMoveY);}
  // Fertig
  return(1);
  }
