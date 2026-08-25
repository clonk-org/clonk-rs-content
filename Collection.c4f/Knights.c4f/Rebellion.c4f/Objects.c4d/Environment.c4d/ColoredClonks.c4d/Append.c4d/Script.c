/*-- Anhang für den Clonk --*/

#strict

#appendto CLNK

// Farbe speichern
local Color_006C;

// Auch während des Spiels erzeugte Clonks sollen gefärbt werden
protected func Initialize(a,b,c,d,e,f,g,h,i,j)
{
  if(FindObject(CC6C))
  {
    CLFS->Colorize(this());
    Color_006C = GetColorDw();
  }
  // Überladene Funktion (mit etwaigen Parametern) aufrufen
  return(_inherited(a,b,c,d,e,f,g,h,i,j));
}

// Spielerfarbe nicht beachten
protected func Recruitment(a,b,c,d,e,f,g,h,i,j)
{
  if(FindObject(CC6C))
    SetColorDw(Color_006C);
  // Überladene Funktion (mit etwaigen Parametern) aufrufen
  return(_inherited(a,b,c,d,e,f,g,h,i,j));
}
            
