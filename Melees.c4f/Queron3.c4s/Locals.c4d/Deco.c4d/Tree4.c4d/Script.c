/*-- Busch --*/

#strict 2

func Initialize()
{
	SetAction("Shrub");
	Rand();
}

func Rand() { SetDir(Random(2)); }
