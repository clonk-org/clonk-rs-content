/*-- Magic Skies --*/

#strict

static pBasement;
static bflag;


protected func Initialize() 
{
  // Spielziel
  //FindObject(CROB)->AddType(ANVL,1);
  FindObject(CROB)->AddType(WZKP,1);
  FindObject(CROB)->AddType(CST3,1);
  FindObject(CROB)->AddType(WTWR,3);
  //Nebel platzieren
  for(var i;i<260;++i) 
    CreateParticle("Fog",Random(LandscapeWidth()),Random(LandscapeHeight()),RandomX(3,9),0,RandomX(1000,1500));   
  // Basement & Startsachen
  var iX, iY, pLory;
  bflag=0;
  BasementY(iX, iY);
  pBasement=CreateConstruction(BAS2,iX,iY,-1,100,1);
  pLory=CreateObject(LORY,iX,iY-5,-1);  // Lore, gefüllt mit:
    CreateContents(CNKT,pLory,2);       // 2x Bausatz
    CreateContents(WOOD,pLory,6);       // 6x Holz
    CreateContents(METL,pLory,2);       // 2x Metall
    CreateContents(ROCK,pLory,5);       // 5x Stein  
    CreateContents(FLNT,pLory,3);       // 3x Feuerstein
  ScriptGo(1);
  return(1);
}

protected func Script0()
{
  CreateParticle("Fog",0,Random(LandscapeHeight()),RandomX(3,9),0,RandomX(900,1400)); 
  CreateParticle("Fog",0,Random(LandscapeHeight()),RandomX(3,9),0,RandomX(900,1400)); 
}

protected func Script1()
{
  return(goto(0));
}

protected func BasementY(&iX, &iY)
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
  // Magieeffekt!
  CastParticles("PSpark", 50, 40, x, y, 50, 70, RGB(255,255,255), RGB(235,235,235));
  return(1);
}

///// /////
//    //
///// ////
   // //
///// /////
