/*-- Flyvat --*/

#strict 2

local pFuelSymbol;
local pWings;

static const TINDER=0;
static const OIL=1;
static const MAGIC=2;

protected func Incineration()
{
  if(pWings) RemoveObject(pWings);
  if(pFuelSymbol[OIL])    RemoveObject(pFuelSymbol[OIL]);
  if(pFuelSymbol[TINDER]) RemoveObject(pFuelSymbol[TINDER]);
  if(pFuelSymbol[MAGIC])  RemoveObject(pFuelSymbol[MAGIC]);
}
