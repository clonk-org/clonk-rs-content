/*-- Knochensturm --*/

#strict

func Activate(pCaller)
{
  // Nicht im Gebäude zaubern
  if(Contained(pCaller))
  {
    Sound("Error");
    PlayerMessage(GetOwner(pClonk), "$must_be_in_free$", pCaller);
    RemoveObject();
    return(1);
  }

  // Effekt prüfen
  var iResult;
  if (iResult = CheckEffect("BigRockNSpell", 0, 125)) return(iResult!=-1 && RemoveObject());
  // Zauber
  Sound("Magic*");

  id = _BNP;
  
  // Zielen wenn möglich
  if (pCaller->~DoSpellAim(this())) return(1);
  // Clonk kann nicht zielen:
  ActivateAngle(pCaller, GetDir(pCaller)*180-90);
//  RemoveObject();
  return(1);
}

local pClonk;
local iAimAngle;
local id;
// Aktivierung nach Zielen (iAngle: -90=links; 0=oben; +90=rechts)
public func ActivateAngle(object pCaller, int iAngle)
  {
  SetPosition(GetX(pCaller), GetY(pCaller));
  pClonk = pCaller;
  iAimAngle = iAngle;
  var i = RandomX(10, 15);
  while(i--)
  { ScheduleCall(this(), "CastBone", i, 0, pClonk, iAimAngle+RandomX(10, -10)); 
  }
  // Objekt löschen
  return(Schedule("RemoveObject()", 100, 0, this()));
  }

func CastBone(pCaller, iAngle)
{
   var iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir;
 
  var pBone = CreateContents(id, pCaller);
  if(id!=_BNP) AddEffect("IntArrowVanish",pBone,1,3,0,MGBW);

  // Austrittsgeschwindigkeit ermitteln 
  iOutXDir = Sin(iAngle, 100);
  iOutYDir = -Cos(iAngle, 100);
  // Austrittsposition... 
  iOutX = Sin(iAngle, 25 * GetCon() / 100);
  iOutY = -Cos(iAngle, 25 * GetCon() / 100); 
  // Drehung 
  iOutR = iAngle; 
  if(iAngle > 0) iOutRDir = 3; else iOutRDir = -3;
  
  // Besitzer des Projektils setzen
  pBone->SetOwner(pCaller->GetOwner());
  // Stein abfeuern
  Exit(pBone, iOutX,iOutY,iOutR, 0,0,iOutRDir );
  pBone->SetXDir(iOutXDir); pBone->SetYDir(iOutYDir);
  if(id->~IsArrow()) pBone->~Launch();
}

public func IsAimerSpell() { return(1); }

public func GetSpellStaff(object pMage) { return(SMER); }
public func GetSpellStaffCombo(pMage) { return ("43"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)

public func GetSpellClass(object pMage) { return(EART); }
public func GetSpellCombo(pMage) { return ("432"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
