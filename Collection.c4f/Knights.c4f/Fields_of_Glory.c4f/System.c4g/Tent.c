/*-- Zelt Inhalt gehört dem Killer --*/

#strict
#appendto TENT

protected func Damage(int iChange, int iByPlayer)
{
for (var a in FindObjects( Find_Container(this()),Find_Not( Find_OCF(OCF_CrewMember) ) ) ) SetOwner(iByPlayer,a);
return(_inherited(...));
}