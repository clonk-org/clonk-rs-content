/* Magischer Funken */

#strict 2

protected func Initialize()
{
	SetVisibility(VIS_Owner);
	SetAction("Exist");	
}



protected func Destroy()
{
	RemoveObject();
}
