/*-- Efeu --*/

#strict 2

func Initialize()
{
	SetAction("Ivy");
	Rand();
}

func Rand() { SetDir(Random(2)); }
