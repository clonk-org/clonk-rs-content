/*-- Lichtschein --*/

#strict 2

local iSize;
local iColor;
local iActive;

func Initialize()
{
 iActive=true;
}

public func AttachTo(pTarget)
{
 SetAction("Attach",pTarget);
}

public func SetSize(iNewSize)
{
 iSize=iNewSize;
 if(iSize <= 0)iSize=1;
 //SetObjDrawTransform(10*iSize,0,0,0,10*iSize,0,this);
 SetCon(iSize);
}

public func Deactivate()
{
 iActive=false;
 SetClrModulation(RGBa(255,255,255,255));
}

public func Activate()
{
 iActive=true;
 SetClrModulation(iColor);
}

public func SetLightColor(iNewColor)
{
 iColor=iNewColor;
 SetClrModulation(iColor,this);
}

public func GetSize()
{
 return iSize;
}

public func AddFlicker()
{
 AddEffect("Flickering",this,20,55+Random(20),this);
}

public func FxFlickeringStart(pTarget,iEffectNumber,iTemp)
{
 if(iTemp)return;
}

public func FxFlickeringTimer(pTarget,iEffectNumber,iEffectTime)
{
 if(!Random(25))
 if(!GetEffect("ExecFlicker",pTarget))
  AddEffect("ExecFlicker",pTarget,20,2,pTarget);
}

public func FxExecFlickerStart(pTarget,iEffectNumber)
{
 EffectVar(0,pTarget,iEffectNumber)=pTarget->~GetSize();
}

public func FxExecFlickerTimer(pTarget,iEffectNumber,iEffectTime)
{
 if(iEffectTime > 75+Random(60))
  return -1;
 
 pTarget->SetSize(Random(EffectVar(0,pTarget,iEffectNumber)/2));
 return 1;
}

public func FxExecFlickerStop(pTarget,iEffectNumber)
{
 pTarget->SetSize(EffectVar(0,pTarget,iEffectNumber));
}

