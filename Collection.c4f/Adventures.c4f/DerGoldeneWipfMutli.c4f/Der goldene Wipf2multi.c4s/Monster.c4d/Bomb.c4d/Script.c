/*-- Neues Objekt --*/

#strict
local dir3;

func Initialize()
{
SetAction("Fly");
/*dir3=GetDir(Local(0))+1;
if(dir3==2)dir3=-1;*/
}

func Check()
{


CreateParticle("PSpark",0,0,0,0,70,RGB(100,0,250));
if(GBackSolid())Explode(20);
if(FindObject(PLDN,-7,-7,14,14,OCF_Alive()))Explode(20);
SetXDir(30*-Local(0));
SetYDir(RandomX(-4,4));
}

