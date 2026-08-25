/*-- Fass Appendto--*/

#strict
#appendto BARL

// Füllstand
local iFillLevel;

// Füllmaterial+1
// Benutzt nur vom Lavafass, um zwischen Lava und DuroLava zu unterscheiden
// Kann für Fässer aus alten Spielständen 0 sein!
local iFillMaterial;

// Gegenwärtiges Material, mit dem nicht aufgefüllt werden konnte
local iBackMaterial;

/* TimerCall */

private func FillCheck()
{
  // Nicht füllen wenn das Fass enthalten ist.
  if (Contained())
	if(GetID(Contained())!=LORY) return(1);
  // Wenn das normale Fass per ChangeDef zurueckgeaendert wurde
  // koennte die Fuellvariable noch voll sein
  if(BarrelMaterial() == -1)
  {
  	if(iFillLevel)
  		iFillLevel=0;
  }
  
  // Material an aktueller Position überprüfen
  var iMaterial;
  if ( ((iMaterial = GetMaterial()) == -1) || (iMaterial == iBackMaterial) ) return(0);
  // Fass für dieses Material suchen
  var idBarrel;
  if (!(idBarrel = GetBarrelType(iMaterial))) return(iBackMaterial = iMaterial);
  // Fass füllen wenn entweder leer oder mit dem gleichen Material aufgefüllt werden kann
  if(BarrelMaterial() != -1 && BarrelMaterial() != iMaterial) return(0);
  // Wenn nicht bereits voll
  if(iFillLevel >= BarrelMaxFill()) return(0);
  // Fass füllen
  iFillLevel += ExtractMaterialAmount(0, 0, iMaterial, BarrelMaxFill() - iFillLevel);
  // Zum vollen Fass werden, wenn wir es nicht bereits sind
  if(BarrelMaterial() == -1)
  {
    iFillMaterial = iMaterial+1;
    ChangeDef(idBarrel);
  }
  return(1);
}