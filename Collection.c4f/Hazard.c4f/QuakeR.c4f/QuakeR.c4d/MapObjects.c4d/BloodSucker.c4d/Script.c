/*-- Blutsauger --*/

#strict 2

func SuckBlood()
{
	if(GetBlood(true))
		return;
	var count = GetBlood()*10;
	for(var i; i < count; i++)
		if(GBackLiquid())
			ExtractLiquid();
}
