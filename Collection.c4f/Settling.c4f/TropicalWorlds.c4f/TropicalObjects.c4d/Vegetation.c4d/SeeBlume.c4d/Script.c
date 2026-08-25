/* -- Seeblume -- */

#strict

protected func Check() {
  Activity();
  if(!Random(10)) Bubble();
  if (!FindObject(0,-100,-100,200,200,OCF_CrewMember()))
   if (GBackLiquid())
    if (GetAction()S="Dry")
       return(SetNewAction("Wiggle"));
  
  if (!FindObject(0,-100,-100,200,200,OCF_CrewMember()))
   if (!GBackLiquid())
    if (GetAction()S="Wiggle")
       return(SetNewAction("Dry"));
       
  return(1);
}

public func SetNewAction(string szAction) {
  if(szAction S= "Wiggle"){
   SetAction("Wiggle");
   SetPhase(Random(8));
  }
  if(szAction S= "Dry"){
   SetAction("Dry");
  }
  return(1);
}
protected func Completion() {
  SetNewAction("Dry");
  return(1);
 }
 
public func Activity() {
  if (GBackLiquid())
    if (SEqual(GetAction(),"Wiggle"))
      if (FindObject(0,-100,-100,200,200,OCF_CrewMember()))
        SetAction("Wiggle2");
  if (GBackLiquid())
    if (SEqual(GetAction(),"Dry"))
      if (!FindObject(0,-100,-100,200,200,OCF_CrewMember()))
        SetAction("Wiggle1"); 
  return(1);
 }
 
