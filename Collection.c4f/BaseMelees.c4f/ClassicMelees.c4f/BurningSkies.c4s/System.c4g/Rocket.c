/*--- Rakete (angeschraubt) Append für Landschaft mit oberer Begrenzung---*/

#strict
#appendto _RTA

// Timer Call (Jedes 1 Frame)

public func CheckFlight()
{
// Flug zu Ende und nicht Explodiert?
if(GetAction() eq "RUN1")
	if (!GetXDir() || !GetXDir())	ChangeDef(CannonMobileID());
/* Ziel getroffen? */
   SetVar(0,Or(FindObject(SUB1,0,1,0,0,0, 0,0, NoContainer()),
   		FindObject2(Find_ID(_RTA),Find_AtPoint(0,1),Find_Not(Find_ActionTarget(GetActionTarget())), Find_NoContainer()),
   		 FindObject(_HEL,0,1,0,0,0, 0,0, NoContainer())));  
   if (Var(0)) return(Hit());

// Am oberen Rand Rotation horizonzal ausrichten.
if (GetY()<30)
  {
  var r = GetR();
  if (r<0 && r>-90)	SetR(r-2);
  if (r>0 && r<+90)	SetR(r+2);
  }

return(1);
}