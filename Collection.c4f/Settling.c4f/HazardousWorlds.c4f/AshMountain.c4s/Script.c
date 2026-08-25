/*-- Ascheberg --*/

#strict

static pBasement;
static bflag;

protected func Initialize() 
{// Spieleinstellung
  SetGamma(RGB(0,0,0),RGB(118,118,118),RGB(215,215,215));
  SetMatAdjust(RGB(180,180,180));
  SetSkyAdjust(RGB(220,220,220));    
  // Basement & Startsachen
  var x, y, pBlimp, pLory;
  bflag=0;
  for(var y=0;y<LandscapeHeight();y++){
  for(var x=0;x<LandscapeWidth ();x++)
    if(GetMaterial(x,y)==Material("Earth")||
       GetMaterial(x,y)==Material("FlyAshes"))
    {
      pBasement=CreateConstruction(BAS2,x,y,-1,100,1);
      break;
    }
    if(FindObject(BAS2)) break;
  }
  pBlimp=CreateObject(BLMP,x,y-7,-1); // Dekoration
  SetClrModulation(RGB(30,40,40),pBlimp);
  SetObjectBlitMode(2,pBlimp);          // Einfärben
  Object2Overlay(pBasement,pBlimp);     // Festketten
  pLory=CreateObject(LORY,x,y-5,-1);    // Lore, gefüllt mit:
    CreateContents(CNKT,pLory,1);       // 1x Bausatz
    CreateContents(WOOD,pLory,5);       // 5x Holz
    CreateContents(GOLD,pLory,4);       // 4x Gold
    CreateContents(LOAM,pLory,2);       // 2x Lehm  
  
  return(1);
}

protected func InitializePlayer(iPlr)
{// Spielereinstellungen
  // Sicht beschränken
  SetFoW(1,iPlr);
  var i;
  var x = GetX(pBasement);
  var y = GetY(pBasement);
  while(GetCrew(iPlr,i))
  {
    // Clonks zum Startpunkt setzen
    SetPosition(x+RandomX(-4,4),y-10,GetCrew(iPlr,i));
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
