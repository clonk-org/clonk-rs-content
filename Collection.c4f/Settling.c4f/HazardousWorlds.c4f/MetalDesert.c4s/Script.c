/*-- Metal Desert --*/

#strict

static phut;
static bflag;

protected func Initialize() 
{
  // Ziel
  FindObject(CROB)->AddType(RSRC,1);  // Forschungslabor
  FindObject(CROB)->AddType(WRKS,1);  // Werkstatt
  // Gamma Effekt
  SetGamma(RGB(0,0,0), RGB(100,100,100), RGB(240,240,240));
  SetSkyAdjust(RGB(240,240,240));
  SetMatAdjust(RGB(240,240,240));    
  // Basement & Startsachen
  bflag=0;  
  return(1);
}


protected func InitializePlayer(iPlr)
{
  // Sicht beschränken
  SetFoW(1,iPlr);
  var i;
  phut = FindObject(HUT2);
  while(GetCrew(iPlr,i))
  {
    // Clonks zum Startpunkt setzen
    SetPosition(GetX(phut)+RandomX(-8,8),GetY(phut)+6,GetCrew(iPlr,i));
    ++i;
  }
  // Flagge geben, wenn noch keine vorhanden ist.
  if(bflag==0){CreateContents(FLAG, GetCrew(iPlr, 0)); bflag++;}
  return(1);    
}


public func VolcanoMaterial() 
{ 
  return(Material("DuroLava")); 
}

///// /////
//    //
///// ////
   // //
///// /////
