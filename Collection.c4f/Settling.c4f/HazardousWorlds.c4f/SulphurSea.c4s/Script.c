/*-- Sulphur Lake --*/

#strict

static phut;
static bflag;

protected func Initialize() 
{
  // Ziel
  if(!FindObject(GLDM))
    CreateObject(GLDM);
  // Gamma Effekt
  SetGamma(RGB(0,5,0), RGB(100,110,100), RGB(220,225,220));
  SetSkyAdjust(RGB(200,200,200));
  SetMatAdjust(RGB(150,200,150));    
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

///// /////
//    //
///// ////
   // //
///// /////
