/*-- Eis --*/

#strict
local except;

func Initialize() {
  SetAction("Fly");
  return(1);
}

func Except(p)
{
	except=p;
}

func Flying()
{
	var size=GetActTime()*4;
	if(size > 35*2)return RemoveObject();
	if(!Random(2))
		CreateParticle("SnowA",0,0,GetXDir()/7,RandomX(-3,3),size*4,RGBa(255,255,255, 120), this);
		//CreateParticle("PSpark",0,0,GetXDir()/15,0,size*4,RGB(200,200,255));
	else CreateParticle("Snow2",0,0,GetXDir()/7,RandomX(-3,3),size*4,RGBa(120,120,255, 100), this, 1);
	
	var f=0;
	for(var obj in FindObjects(Find_Distance(size/2), Find_Exclude(except), Find_OCF(OCF_Alive)))
	{
		Freeze(obj);
		obj->SetKiller(GetController());
		f=1;
	}
	
	if(f)
	{
		//CastParticles("Snow",10,10,0,0,30,40,RGB(100,100,200),RGB(200,200,255));
		//return RemoveObject();
	}
}
