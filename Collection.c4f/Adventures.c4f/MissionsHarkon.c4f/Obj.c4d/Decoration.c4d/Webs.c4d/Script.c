/*-- Weben --*/

#strict 2

protected func Initialize() {
  SetAction("Be");
  if(Random(3))
  SetCategory(C4D_Foreground);
  SetPhase (Random(4), 0); 
  SetClrModulation(RGBa(255,255,255,Random(40)));
	SetR(Random(360));
}

public func Set(phase) {
  SetPhase(phase);
  var width = GetDefCoreVal("Width", "DefCore", GetID());
  var height = GetDefCoreVal("Height", "DefCore", GetID());
}

