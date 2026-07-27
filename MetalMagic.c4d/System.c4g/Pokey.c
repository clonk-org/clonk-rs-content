/*-- Pokey --*/

#strict 2
#appendto BLMP
#appendto BALN


/* Trefferkontrolle */

protected func RejectCollect(id idObject)
{
    if ( DefinitionCall(idObject,"IsDangerousToAirTravel") )
    return Poke();
   return _inherited(idObject, ...);
}
