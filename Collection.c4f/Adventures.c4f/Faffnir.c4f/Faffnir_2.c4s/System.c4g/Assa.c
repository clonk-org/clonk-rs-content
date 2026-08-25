/*--appendto --*/

#strict

#appendto ASAS

public func ContextArtefakt()
{
  
  [Artefakt|Image=MSKU|Condition=NoArtefakt]
  SetPhysical("Breath", 90000,2);
  SetPhysical("Walk", 99000,2);
  SetPhysical("Jump", 62000,2);
  SetPhysical("Scale", 70000,2);
  SetPhysical("Hangle", 70000,2);
  SetPhysical("Swim", 90000,2);
  SetPhysical("Dig", 50000,2);
  Schedule("Partikel()", 7, 200); 
  Schedule("Entfern()", 1400); 
  AddEffect("Artefakt", this(), 24, 2000);
  return(1);
}

public func Partikel() {
 CastParticles("PSpark", 2, 12, 0, 0, 40, 60,RGB(255,0,0),RGB(255,0,0));
 return(1); }
 
 
public func Entfern() {
 ResetPhysical();
 return(1); }
 
public func NoArtefakt() {
 if(!GetEffect("Artefakt", this())) return(1); }

