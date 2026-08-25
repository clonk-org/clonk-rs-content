/*-- Volcan Isle --*/

#strict

static pBasement;
static bflag;

protected func Initialize() 
{
  // Ziel
  FindObject(CROB)->AddType(BLMP,1);
  // Gamma Effekt
  SetGamma(RGB(5,0,0), RGB(110,100,100), RGB(225,220,220));
  SetSkyAdjust(RGB(200,200,200));
  SetMatAdjust(RGB(200,150,150));   
  // Wind
  SetWind(0);
  // Basement & Startsachen
  var iX, iY, pBlimp, pLory;
  bflag=0;
  BasementY(iX, iY);
  pBasement=CreateConstruction(BAS2,iX,iY,-1,100,1);
  pBlimp=CreateObject(BLMB,iX,iY-7,-1); // Dekoration
   Object2Overlay(pBasement,pBlimp); 
  pLory=CreateObject(LORY,iX,iY-5,-1);  // Lore, gefüllt mit:
    CreateContents(CNKT,pLory,1);       // 1x Bausatz
    CreateContents(WOOD,pLory,5);       // 5x Holz
    CreateContents(GOLD,pLory,4);       // 4x Gold
    CreateContents(LOAM,pLory,2);       // 2x Lehm
  // Scriptcounter
  //ScriptGo(1);
  return(1);
}

func BasementY(&iX, &iY)
{
  iX = Random(LandscapeWidth());
  //while(Inside(iX, LandscapeWidth()/2, LandscapeWidth()/2)) 
  //iX = 30 + Random(LandscapeWidth() - 80);
  iY=0;
  while(!GBackSolid(iX, iY))
  {
    ++iY;
    // Sicherheitscheck   
    if(iY>LandscapeHeight())
      {iX = Random(LandscapeWidth()); iY=0;}
  }
  return(1);
}


protected func InitializePlayer(iPlr)
{
  // Sicht beschränken
  SetFoW(1,iPlr);
  var i;
  while(GetCrew(iPlr,i))
  {
    // Clonks zum Startpunkt setzen
    SetPosition(GetX(pBasement)+RandomX(-4,4),GetY(pBasement)-10,GetCrew(iPlr,i));
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
