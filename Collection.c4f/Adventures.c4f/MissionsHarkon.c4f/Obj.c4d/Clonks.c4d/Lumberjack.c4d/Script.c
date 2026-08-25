/*-- Aquaclonk --*/

#strict 2

#include CLNK

local fHasLog;

func StartWalk()
{
	var szAction = "Walk";
	if(fHasLog) szAction = "WalkWoodLog";
	if(GetAction() != szAction) SetAction(szAction);
}

func StartJump()
{
	var szAction = "Jump";
	if(fHasLog) szAction = "JumpWoodLog";
	if(GetAction() != szAction) SetAction(szAction);
}

func PickUpLog()
{
	SetAction("TakeUpLog");
	SetXDir(0);
	SetComDir(COMD_Stop);
	fHasLog = 1;
}

func PutLogDown()
{
	fHasLog = 0;
}

private func Picking()
{
  if (!GetActTime()) return; // Erster Schlag kein Sound. Clonk holt noch aus.
  Sound("MetalHit*");
  CastParticles("MaterialSpark",Random(3)+1,6,-10+20*GetDir(),6,50,62, RGB(100,100), RGB(100,100));
  return 1;
}