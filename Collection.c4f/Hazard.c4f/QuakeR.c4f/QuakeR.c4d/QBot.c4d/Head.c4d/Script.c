/*-- Body Part --*/

#strict

func NoWarp() { return(true); }

func Initialize()
{
	SetAction("Flesh");
	SetDir(Random(2));
}

func Set(string Graphics, int dir, int type)
{
	// richtige Grafik setzen
	SetGraphics(Graphics, this(), QBOT, 0, GFXOV_MODE_Base);
	// und Richtung
	SetDir(dir);
	// und wegfliegen!
	SetXDir(RandomX(10,20)*(2*dir-1));
	SetYDir(RandomX(10,20)*-1);
	
	if(type == 0)
	{
		SetAction("Flesh");
		SetDir(dir);
	}
	else if(type == 1)
	{
		SetAction("Beheaded");
		SetXDir(GetXDir()*2);
	}
	else if(type == 1)
		SetAction("Armless");
	else if(type == 2)
		SetAction("Legless");
}

func Adjust()
{
	if(GetXDir() && GetYDir())
		SetR(Angle(0,0,GetXDir(),GetYDir()));
	if(!Random(10) && GetAction() ne "Flesh")
		CastBlood(1);
	
	if(GetAction() S= "Flesh" && GetActTime() > 50)
		DoCon(-1);
	
	if(GetActTime() > 150)
		DoCon(-Random(5));
}
