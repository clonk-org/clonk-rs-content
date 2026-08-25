/*-- Sirenenbau --*/

#strict
#include GOAL

public IsFulfilled:
  // Sirenen gebaut?
  if(GibFreiSpitzen()) return(0);
  // eingeschalten?
  if(!ObjectCall(FindObject(_V1A),"IsOn")) return(0);
  // fertig 
  return(1);

private GibFreiSpitzen:
  SetPosition(1,1);
  var siren;
  if(!FindObject(_V1A,750,750,160,160)) siren++; 
  if(!FindObject(_V1A,1200,350,180,180)) siren++; 
  if(!FindObject(_V1A,1700,450,250,200)) siren++; 
  if(!FindObject(_V1A,3300,0,250,200)) siren++; 
  return(siren);

protected Activate: // int iPlayer
  if (IsFulfilled()) return(MessageWindow("Die Sirenen arbeiten",Par()));
  var spitzen=GibFreiSpitzen();
  if( spitzen) MessageWindow(Format("%d Schneespitzen haben noch keine Sirene",spitzen),Par(0));
  if(!spitzen) MessageWindow("Die Sirenen sind noch nicht alle aktiviert",Par(0));
  return(1);