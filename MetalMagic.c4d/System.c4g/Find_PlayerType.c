/* Find_PlayerType(int iType) findet nur Spieler von dem entsprechenden Typ */

#strict 2

global func Find_PlayerType(int iType)
{
  var i = GetPlayerCount(iType);
  var aFind = [C4FO_Or];
  while(i--)
    aFind[GetLength(aFind)] = Find_Owner(GetPlayerByIndex(i, iType));
  return aFind;
}
