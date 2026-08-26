/*-- Neues Szenario --*/

#strict


func Initialize() {
  ScriptGo(1);
  return(1);
}

func Script1()
{	
	SetMaxPlayer (1);
	var iPlayers = (GetPlayerCount());  
        if (iPlayers == 0) return();
        var iDistance = LandscapeWidth ()/ (iPlayers+1);

        var aPositions = CreateArray(iPlayers);        
        for(var i = 0; i < iPlayers; i++) {
          aPositions[i] = i+1;
        }

        ShuffleArray(aPositions);

        if (iPlayers > 2)
	{
		SetWealth(GetArrayItemPosition(1,aPositions),GetWealth(GetArrayItemPosition(1,aPositions))-50);
        	SetWealth(GetArrayItemPosition(iPlayers,aPositions),GetWealth(GetArrayItemPosition(iPlayers,aPositions))-50);
	}
        for(var i = 0;i < iPlayers;i++)
          MovePlayerHere(i,aPositions[i],iDistance);
        return(1);
}

func MovePlayerHere(iPlayer, iPosition, iDistance) {
  CreateObject(E58Z,(iPosition)*iDistance,DetectHeightOverGround((iPosition)*iDistance)-10,iPlayer);
  for(var pItem in FindObjects(Find_Owner(iPlayer)))
    SetPosition((iPosition)*iDistance,DetectHeightOverGround((iPosition)*iDistance)-10,pItem);
}

protected func DetectHeightOverGround(iX) {
  var Y;
  for(var X = 0; (GBackSolid(iX,X) == false); X++) {
    if (X == LandscapeHeight()) return(0);
    Y = X;
  }
  return(Y);
}