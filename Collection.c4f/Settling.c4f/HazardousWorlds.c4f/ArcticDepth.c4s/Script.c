/*-- Arktische Tiefen --*/

#strict

protected func Initialize() 
{// Starteinstellung
  SetGamma(RGB(15,15,15),RGB(118,118,118),RGB(215,215,215));
  SetMatAdjust(RGB(200,200,240));
  SetSkyAdjust(RGB(200,200,200));
  SetSkyParallax(0,15,17,0,0,0,0);    
  // Ziel
  FindObject(CROB)->AddType(SPDM,1);
  return(1);
}

protected func InitializePlayer(int iPlr)
{// Spielervorgaben
  SetFoW(1,iPlr);
  var x = GetX(FindObject(IGLO));
  var y = GetY(FindObject(IGLO));
  SetPosition(x,y,GetCrew(iPlr));
  var a = [SFLN,CNKT,BARL,FLNT,LNKT,GUNP,LORY,BRDG,SUB1,
           ELEV,WRKS,FNDR,POWR,CHEM,ANVL,RSRC,IGLO,SPDM];
  for(var i=0;i<GetLength(a);i++)
    SetPlrKnowledge(iPlr,a[i]);
}

///// /////
//    //
///// ////
   // //
///// /////
