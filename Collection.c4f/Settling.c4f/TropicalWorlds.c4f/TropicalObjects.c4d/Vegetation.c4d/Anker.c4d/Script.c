/*-- Neues Objekt --*/

#strict

func Initialize() {
   if(!Random(2)) SetGraphics("2");
  return(1);
}
func Damage() {
  if(GetDamage() > 40){ 
   CastObjects(METL,3,RandomX(20,40));
   CastObjects(MUEL,5,RandomX(30,50));
   RemoveObject(this());
   }
  return(1);
}
