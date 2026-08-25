/*-- Glas schmelzen --*/

#strict
#appendto FNDR

/* Produktion */

private func Burning()
{
  // Rauch
  Smoke(+23,Random(3)-24,Random(5)+8);
  // Noch nicht fertig
  if (GetActTime()<150) return(1);
  // Fertig
  SetAction("Idle");
  // Material suchen
  var pOre;
  var pSand; 

  if(pOre=FindContents(ORE1))
  {// Metall machen
  RemoveObject(pOre);
  Exit(CreateContents(METL),-27,+13,0,-1);
  return(Sound("Pshshsh"));
  }
  
  if(pSand=FindContents(SAND))
  {// Glass machen
  RemoveObject(pSand);
  Exit(CreateContents(GLAS), -27,+13,0, -1);
  return(Sound("Pshshsh"));
  }
  // Fehlschlag?
  return(false);
}

/* Inhaltsüberprüfung */  
  
private func ContentsCheck()
{
  // Warten...
  if (!ActIdle()) 
    return(1);
  // Loren rausschicken
  var obj;
  for(var i = 0; obj = Contents(i); i++)
    if(obj->~IsLorry())
      {
      SetCommand(obj,"Exit");
      }
  // Erstes Inhaltsobjekt überprüfen
  obj = Contents(0); var id = GetID(obj);
  // Alles außer Mannschaftsmitgliedern, Rohmaterial und Loren sofort auswerfen
  if (obj)
    if ( !(    GetOCF(obj) & OCF_CrewMember()
            || obj->~IsLorry()
            || id==COAL
            || id==OBRL
            || id==WOOD
            || id==SAND
            || id==ORE1 ) )
      if(GetDefFragile(id) || GetCategory(obj) & C4D_Vehicle)
        SetCommand(obj, "Exit");
      else
        Exit(obj,-27,+13,0,-1);
  // Material prüfen
  if (!FindContents(ORE1))
  if (!FindContents(SAND))
    return(1);
  // Holz verbrennen
  if (ContentsCount(WOOD)>1) return(BurnWood());
  // Öl verbrennen
  if (ContentsCount(OBRL)>0) return(BurnOil());
  // Kohle verbrennen
  if (!(obj=FindContents(COAL))) return(1);
  RemoveObject(obj);
  SetAction("Burning");
  return(1);
}
