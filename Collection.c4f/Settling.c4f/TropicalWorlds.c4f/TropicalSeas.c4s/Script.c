/*-- Neues Szenario --*/

#strict

func Initialize() {
  FillTheWorldWithLife();
  SetGamma(RGB(7,6,0), RGB(152,147,128), RGB(255,254,236) );
  FindObject(CROB)->AddType(SUB1, 1);
  FindObject(CROB)->AddType(CST2, 1);
  return(1);
}
global func FillTheWorldWithLife() {
  // Algen, Anker, Seegras, Muscheln, Palmen, Büsche, Haie, Fische, Knochen, Kaputte dinger
    // Seegräser
    PlaceInSea(SWD1, 10);
    PlaceInSea(SWD2, 10);
    PlaceInSea(SWD4, 10);
    PlaceInSea(SWD5, 10);
    PlaceInSea(SWD6, 10);
    PlaceInSea(SWD7, 10);
    PlaceInSea(SEBL,  5);
    // Muscheln
    /*PlaceInSea(SHEL, 5);
    PlaceInSea(GSHL, 1);
    PlaceInSea(VSHL, 2);
    PlaceInSea(RSHL, 2);
    PlaceInSea(SSHL, 2);
    PlaceInSea(MSHL, 8);
    PlaceInSea(BSHL, 12); */
    // Fische und Haie
    PlaceInSea(FISH, 12);
    PlaceInSea(SHRK, 3);    
    // Palmen und so
    PlaceOnLand(PLM2, 20,0,0,0,0,0, 1);
    PlaceOnLand(PLM1, 16,0,0,0,0,0, 1);  
    PlaceOnLand(BUSH, 14,0,0,0,0,0, 1);
    PlaceOnLand(PLM2, 20,0,0,0,0,0, 0);
    PlaceOnLand(PLM1, 16,0,0,0,0,0, 0);  
    PlaceOnLand(BUSH, 14,0,0,0,0,0, 0);
    // bla und so
    PlaceOnLand(NOVO, 1); 
    PlaceOnLand(HTB1, 1); 
    PlaceOnLand(GDLS, 3);     
    PlaceInSea(DANK, 2); 
    PlaceInSea(SUBB, 1);    
    PlaceInSea(SLBB, 1);  
  return(1);
}
global func FillTheWorldWithPlrStuff() {
     // Muscheln
    PlaceInSea(SHEL, 5);
    PlaceInSea(GSHL, 1);
    PlaceInSea(VSHL, 2);
    PlaceInSea(RSHL, 2);
    PlaceInSea(SSHL, 2);
    PlaceInSea(MSHL, 8);
    PlaceInSea(BSHL, 12);
    PlaceInSea(SHRK, 1); 
    PlaceOnLand(PLM2, 20,0,0,0,0,0, 0);
    PlaceOnLand(PLM1, 16,0,0,0,0,0, 0);  
    PlaceOnLand(BUSH, 14,0,0,0,0,0, 0);
  return(1);
}
protected func InitializePlayer(iPlr) { 
CreateContents(FLAG,GetCrew(iPlr));
CreateContents(CNKT,GetCrew(iPlr,1));
GetCrew(iPlr)->CastObjects(WOOD,3,10);
FillTheWorldWithPlrStuff();
}
