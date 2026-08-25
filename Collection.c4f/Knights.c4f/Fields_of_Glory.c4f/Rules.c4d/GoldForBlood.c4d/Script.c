/*-- Gold für Blut --*/

#strict

#appendto CLNK

protected func Activate(int plr)
{
  if (GetType(plr) != C4V_Int())	return(_inherited());
  // Beschreibung ausgeben
  return MessageWindow(GetDesc(),plr);
}

protected func Death(int iKiller, a,b,c,d,e)
{
  if(FindObject(GOBT))	{ 
			if( Hostile( iKiller, GetOwner(this()) ) )	SetWealth(iKiller, GetWealth(iKiller)+Value(GetID(this())));
			}
  // Basisfunktion aufrufen
  return(_inherited(iKiller, a,b,c,d,e) );
}