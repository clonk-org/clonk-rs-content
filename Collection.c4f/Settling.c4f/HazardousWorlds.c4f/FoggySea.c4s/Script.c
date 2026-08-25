//*-- Nebelmeere --*//

#strict 2

protected func Initialize()
{// Nebel aus Nebelklippe
  var iTimeout;
  SetGamma(RGB(15,15,15),RGB(118,118,118),RGB(215,215,215));
  SetMatAdjust(RGB(200,200,240));
  SetSkyAdjust(RGB(200,200,200));
  SetSkyParallax(0,15,17,0,0,0,0); 
  //Handgemacht... ;)
  Meeresrauschen(170);
  Meeresrauschen(1100);
  Meeresrauschen(2060);
  Meeresrauschen(2900);
  Meeresrauschen(3800);
  Meeresrauschen(4800);
  Meeresrauschen(5700);
  Moewen(900);
  Moewen(2300);
  Moewen(3900);
  Moewen(5000);
  //Nebel platzieren
  for(var i;i<260;++i) 
    CreateParticle("Fog",Random(LandscapeWidth()),Random(LandscapeHeight()*2/3),RandomX(3,9),0,RandomX(1000,1500)); 
  ScriptGo(1);

  // Ziel
  FindObject(CROB)->AddType(BLMP,1);  

  // Statue und Schiffswrack
  var pChest;
  for(var y=0;y<LandscapeHeight();y++){
  for(var x=0;x<LandscapeWidth ();x++)
    if(GetMaterial(x,y)==Material("Sand"))
    {
      pChest =
      CreateObject(CHST,x,y,-1);
      CreateObject(IDOL,x,y,-1);
      break;
    }
    if(FindObject(IDOL)) break;
  }
  
  for(y=LandscapeHeight()-100;y>  0;y--){
  for(x=LandscapeWidth ()-200;x>200;x--)
    if(GetMaterial(x,y)==Material("Water"))
    {
      CreateObject(_WRK,x,y,-1);
      break;
    }
    if(FindObject(_WRK)) break;
  }
  // Materialien verteilen
  y-=20;
  CreateContents(WOOD,pChest, 5);
  CreateContents(METL,pChest, 9);
  CreateContents(CNKT,pChest, 1);
     CastObjects(WOOD,10,25,x,y);
     CastObjects(METL, 4,25,x,y);
     CastObjects(BAS2,1,,x+15,y);
}

protected func InitializePlayer(int iPlr)
{// Einstellungen für die Spieler
  SetFoW(1,iPlr);             // Kriegsnebel
  SetPlrKnowledge(iPlr,HUT1); // Verführung
  SetPlrKnowledge(iPlr,ANVL); // Amboss
  SetPlrKnowledge(iPlr,SAWM); // Sägewerk  
  SetPlrKnowledge(iPlr,WMIL); // Windrad
  SetPlrKnowledge(iPlr,FNDR); // Hochofen  
  SetPlrKnowledge(iPlr,CHEM); // Chemiewerk
  SetPlrKnowledge(iPlr,RSRC); // Researchlab
  SetPlrKnowledge(iPlr,CNKT); // Bausatz
  SetPlrKnowledge(iPlr,LNKT); // Leitung
  SetPlrKnowledge(iPlr,FLNT); // Feuerstein
  SetPlrKnowledge(iPlr,BLMP); // Luftschiff
}

// Nebel erstellen
func Script0() 
  {CreateParticle("Fog",0,Random(LandscapeHeight()*2/3),RandomX(3,9),0,RandomX(1000,1500));}
func Script4() 
  {return(goto(0));}

// Sound-Objekte platzieren
func Meeresrauschen() 
  {ObjectSetAction(CreateObject(SL9Z,Par(),LandscapeHeight()/2,-1),  "Meer");}
func Moewen() 
  {ObjectSetAction(CreateObject(SL9Z,Par(),LandscapeHeight()/3,-1),"Moewen");}

///// /////
//    //
///// ////
   // //
///// /////
