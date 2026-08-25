/*-- Ascheberg --*/

#strict

static pBasement;
static bflag;

protected func Initialize() 
{// Spieleinstellung
  SetGamma(RGB(0,5,0), RGB(100,110,100), RGB(220,225,220));
  SetSkyAdjust(RGB(200,200,200));
  SetMatAdjust(RGB(150,200,150));    
  SetSkyParallax(0,15,17,0,0,0,0);     
  // Basement & Startsachen
  var x, y, pElev, pLory;
  bflag=0;
  for(var y=LandscapeHeight()/4*3;y>0;y++){
  for(var x=LandscapeWidth ()/3  ;x<
            LandscapeWidth ()/3*2;    x++)
    if(GetMaterial( x, y)==Material("Earth")||
       GetMaterial( x, y)==Material("Ashes"))
    if(GetMaterial(x,y-9)!=Material("Earth")||
       GetMaterial(x,y-9)!=Material("Ashes"))
    {
      pBasement=CreateConstruction(BAS2,x,y,-1,100,1);
      FreeRect(x-20,y-80,40,LandscapeHeight()-y);
      break;
    }
    if(FindObject(BAS2)) break;
  }
  pElev=CreateObject(ELVB,x,y-8,);      // Dekoration
  SetClrModulation(RGB(30,40,40),pElev);// Einfärben
  SetObjectBlitMode(2,pElev);           // Einfärben 2
  Object2Overlay(pBasement,pElev);      // Festketten
  pLory=CreateObject(LORY,x,y-5,-1);    // Lore, gefüllt mit:
    CreateContents(CNKT,pLory,1);       // 1x Bausatz
    CreateContents(WOOD,pLory,5);       // 5x Holz
    CreateContents(GOLD,pLory,4);       // 4x Gold
    CreateContents(LOAM,pLory,2);       // 2x Lehm  
    CreateContents(ROCK,pLory,2);       // 2x Stein  
  // Scriptcounter starten
  ScriptGo(1); goto(0); 
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

protected func Script5000()
{// Böse Tiere erzeugen
  if(ObjectCount(BBRD)>3) 
    return(goto(Random(2500)));
  else
    CreateObject(BBRD,Random(LandscapeWidth()),0,-1);
  return(goto(Random(2500)));
}

///// /////
//    //
///// ////
   // //
///// /////
