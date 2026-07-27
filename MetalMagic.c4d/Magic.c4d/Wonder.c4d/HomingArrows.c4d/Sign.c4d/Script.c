/*-- Neues Objekt --*/

#strict

local used;

func Initialize() {
   
  return(1);
}

func AttachTargetLost()
{
	return RemoveObject();
}

func Update(times)
{
	var o=this;
	if(!times)o=0;
	for(var i=0;i<5;i++)
		CreateParticle("PxSpark", RandomX(-6,6),RandomX(-3,3),RandomX(-2,2),RandomX(-10,-3), 60,RGB(255,255,10), o, 1);
	
	for(var i=0;i<used;i++)
	{
		SetGraphics(0,this,0,i+1);
	}
	
	used=times;
	for(var i=0;i<times;i++)
	{
		SetGraphics(0, this, ARRW, i+1, GFXOV_MODE_IngamePicture, 0, GFX_BLIT_Mod2, 0);
		SetClrModulation(RGB(255,255,10), this, i+1);
		SetObjDrawTransform(800,0,(-30+9*(i+1))*1000,0,800,0,this, i+1);
	}
}
