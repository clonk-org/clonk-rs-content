/*-- Verbranntes Burgteil --*/

#strict 2

#include CPN2

//Gesamt kaputt
protected func Destruction()
{
RemoveCannonHelper();
return _inherited(Par(),Par(1),Par(2));
}
//anbrennen
protected func Incineration()
{
RemoveCannonHelper();
return _inherited(Par(),Par(1),Par(2));
}
//anbrennen, auch so
protected func IncinerationEx()
{
RemoveCannonHelper();
return _inherited(Par(),Par(1),Par(2));
}
//Funktion
protected func RemoveCannonHelper()
{
if(Var()=FindObject(RFCH,0,0,0,0,0,"Attach",this))Var()->LooseTarget();
}
