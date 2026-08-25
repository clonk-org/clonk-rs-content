/*--- BausatzAppendto ---*/

#strict
#appendto CNKT

protected func CreateConstructionSite(idType)
{
  // Nur wenn der Clonk steht und sich im Freien befindet
  if (GetAction(Contained()) ne "Walk") return(0);
  if (Contained(Contained())) return(0);
  
  if (idType == BEKP)
  		{
  		if(!FindObject(LC0S, -40, -10, 80, 20)) return(Message("<c ff0000>Hier ist kein Fahrstuhlkorb!", this()));
  		var pElev=FindObject(LC0S, -40, -10, 80, 20);
  		CreateConstruction(BEKP, AbsX(GetX(pElev)), AbsY(GetY(pElev)+11), GetOwner(Contained()), 1, 1, 0);
  		return(RemoveObject(this()));
  		}
  	
 	//Falls kein Fahrstuhl: Normale Funktion aufrufen!
  return(_inherited(idType));
}
