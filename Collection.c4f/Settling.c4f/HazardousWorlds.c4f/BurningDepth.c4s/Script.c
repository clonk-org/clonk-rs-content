/*-- Brennende Tiefe --*/

#strict

static pBasement;

protected func Initialize() 
{// Spielvorgaben
  // Gamma Effekt
  SetGamma(RGB(10,0,0), RGB(110,80,80), RGB(225,190,190));
  SetMatAdjust(RGB(250,190,190));
  // Platz suchen
  for(var y=220;y<LandscapeHeight()    ;y++){
  for(var x=300;x<LandscapeWidth ()-300;x++)
    if(GetMaterial(x,y)==Material("Tunnel"));
    {// Fundament erstellen & Freibuddeln
      var obj;
      while(obj=FindObject2(Find_InRect(x-30,0,60,y)))
        RemoveObject(obj);
      pBasement=CreateConstruction(BAS4,x,y,-1,100,1);
      FreeRect(x-30, 0,60, y); 
      break;
    }
    if(FindObject(BAS4)) break;
  }
  // Fahrstuhl erzeugen
  var pLift=CreateObject(ELCB,x-15,y-9,-1);
  pLift->SetClrModulation(RGB(100,90,90));
  pLift->SetObjectBlitMode(2);
  Object2Overlay(pBasement,pLift);
  
  // Statue setzen
  CreateObject(_STA,x+10,y-9,-1);
  PlaceObjPart(_PA1,1);
  PlaceObjPart(_PA2,2);
  PlaceObjPart(_PA3,3);
  PlaceObjPart(_PB4,4);
  PlaceObjPart(_PA5,5);
  PlaceObjPart(_PA6,6); 
}

protected func InitializePlayer(int iPlr)
{// Spielereinstellungen
  var x = GetX(pBasement);
  var y = GetY(pBasement)-13;
  SetPosition(x,y,GetCrew(iPlr));
}

global func PlaceObjPart(id ID, int iY)
{
  var x = Random (LandscapeWidth());
  var y = RandomX(LandscapeHeight()/6*iY,
                  LandscapeHeight()/7*iY);
  
  while(GetMaterial(x,y)!=Material("Earth"))
  {
     x = Random (LandscapeWidth());
     y = RandomX(LandscapeHeight()/6*iY,
                  LandscapeHeight()/7*iY);
  }
  return(CreateObject(ID,x,y));
}

global func OnPartGain(int iProcess)
{// Boni für die Teile
  SetWealth(0,GetWealth()+iProcess*25);
  Sound("Cash");  
  if(iProcess==1)
  {// Lore mit Belohnungen
    var pLorry = CreateObject(LORY);
    pLorry->SetPosition(GetX(pBasement),10);
    pLorry->SetOwner(0);
    CreateContents(CNKT,pLorry,2);
    CreateContents(WOOD,pLorry,5);
    CreateContents(METL,pLorry,3);
    CreateContents(FLNT,pLorry,4);
    CreateContents(FLAG,pLorry,1);
  }

  if(iProcess==4)
  {// Lore mit Belohnungen
    var pLorry = CreateObject(LORY);
    pLorry->SetPosition(GetX(pBasement),10);
    CreateContents(CNKT,pLorry,1);
    CreateContents(METL,pLorry,7);
  }
}

///// /////
//    //
///// ////
   // //
///// /////