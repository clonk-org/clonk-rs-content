/*-- T_Flint Jumpsperre --*/

#strict

#appendto TFLN

private func BlowUp()
{
 // ist das Ding in einem Clonk
 if(Contained())
  {
   // Dann wird dieser bescheuerte Jump gesperrt
   //Explosion simulieren
   Sound("Blast1");
   DoEnergy(-23, Contained());
   Fling(Contained(), 1, -1);
   //weg damit
   RemoveObject();
   return(1);
   }
 Explode(ExplodeSize());
 return(1);
}
