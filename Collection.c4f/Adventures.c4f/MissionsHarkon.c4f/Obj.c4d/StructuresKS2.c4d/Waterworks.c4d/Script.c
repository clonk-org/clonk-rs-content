/*-- Wasserwerk --*/

#strict 2
#include DOOR
#include B_89

local Wheel;

protected func Initialize()
{
  // Rad (Zusatzobjekt)
  Wheel=CreateObject(WHEL,-20,21);
  return 1;
}

public func CheckClonk()
{
  // Schauen ob ein Clonk da ist, um das Rad zu bedienen
  if(FindObject2(Find_Container(this), Find_OCF(OCF_CrewMember), Find_OCF(OCF_Alive)))
    Wheel->StartWheel();
  else Wheel->StopWheel();
}

public func Collection2()
{
  // Nach Clonk prüfen
  CheckClonk();
}

public func Ejection()
{
  // Nach Clonk prüfen
  CheckClonk();
}