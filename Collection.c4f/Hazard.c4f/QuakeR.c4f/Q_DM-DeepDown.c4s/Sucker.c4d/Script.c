/*-- Sauger --*/

#strict

func TurnOn()
{
	if(GetAction() ne "Turn")
		SetAction("Turn");
}

func RemoveLiquid()
{
	var tosuck = 100;
	
	tosuck -= ExtractMaterialAmount(0,10,Material("Lava"), tosuck);
	tosuck -= ExtractMaterialAmount(0,10,Material("DuroLava"), tosuck);
	tosuck -= ExtractMaterialAmount(0,10,Material("Blood"), tosuck);
	

	if(tosuck == 100)
	{
		var p = GetPhase();
		SetAction("TurnOff");
		SetPhase(p);
		GameCall("SuckOff");
		return();
	}
}

func IsOn() { return(GetAction() S= "Turn"); }
