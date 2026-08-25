#strict

local iAlpha;

func Initialize()
{
  iAlpha=40+Random(40);
  DoCon(-70-Random(10));
  SetAction(Format("%d",1+Random(15)));
  SetXDir(RandomX(-15,15));
  Sound("Fuse");
  return(1);
}

func Tmr()
{
  var obj;
  SetYDir(-Random(10));
  SetClrModulation(RGBa(255,255,255,iAlpha));
  iAlpha++;
  if(iAlpha>240) iAlpha=240;
  if(GetActTime()>180) RemoveObject();
  if(GetCon()<90)DoCon(1);
  if(!Random(10)) SetXDir(RandomX(-15,15));
  while(obj=FindObject(0,-10,-10,20,20,OCF_Living,0,0,NoContainer(),obj))
    {
    if(GetID(obj)!=ZWFB) if(!Random(10))DoEnergy(-2,obj);
    }
}
