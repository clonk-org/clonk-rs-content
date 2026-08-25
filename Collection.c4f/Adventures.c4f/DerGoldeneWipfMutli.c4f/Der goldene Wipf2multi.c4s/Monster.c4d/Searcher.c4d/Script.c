/*-- Neues Objekt --*/

#strict
local target;

func Initialize()
{
SetAction("Fly");
}

func Check()
{
Local(0)++;

if(Local(0)==120)Explode(25);


target=FindObject(PLDN,0,0,-1,-1,OCF_Alive());
SetCommand(this(),"MoveTo",target);

if(FindObject(0,-10,-10,20,20,OCF_HitSpeed3()))Explode(25);

if(ObjectDistance(target,this()) < 10)Explode(25);

if(GBackSolid(0,0))Explode(25);

CastParticles("FSpark",5,5,0,0,50,100,RGB(100,0,250),RGB(100,0,250));
}

