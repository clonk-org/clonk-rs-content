/*-- Königsrettung --*/

#strict
#include GOAL

// Anfangszahl Spieler
local StartPlayerCount;

protected func Initialize()
{
  // Alle anderen Königsmord-Ziele löschen
  RemoveAll(GetID());
  // Überladene Funktion aufrufen
  return(_inherited());
}

public func IsFulfilled()
{
  // Anfangszahl Spieler bestimmen
  if(!StartPlayerCount)
  {
    if((StartPlayerCount=GetPlayerCount()) < 2)
    {
      Log("$MsgTooFewPlayers$");
      return(RemoveObject());
    }
  }

  if(pKing)
  {
    // König ist tot?
    if(!GetAlive(pKing))
    {
      // Angreifer eliminieren
      for(var i=0; i<GetPlayerCount(); i++)
        if(Global(GetPlayerByIndex(i)) != Position_Castle)
          EliminatePlayer(GetPlayerByIndex(i));
      // Nachricht
      Log("$MsgKingKilled$");
      // Spielziel erfüllt
      return(1);
    }
    else
      if(GetX(pKing)<20 || GetX(pKing)>LandscapeWidth()-20)
      {
        // Verteidiger eliminieren
        for(var i=0; i<GetPlayerCount(); i++)
          if(Global(GetPlayerByIndex(i)) == Position_Castle)
            EliminatePlayer(GetPlayerByIndex(i));
        // König nicht mehr spielbar
        SetCrewEnabled(false, pKing);
        SetVisibility(VIS_None, pKing);
        Enter(this(), pKing);
        // Nachricht
        Log("$MsgKingSaved$");
        // Spielziel erfüllt
        return(1);
      }
  }

  // Noch nicht erfüllt
  return(0);  
}

