#strict
#include SHT1

local Col, ishit;

func SetClr(clr)
{
	Col = clr;
}

// Extern für Überladung
private func CreateTrail(int iSize, int iTrail) {

}

private func HitObject(object pObject) {
  if(BulletStrike(pObject)) {
    Remove();
  }
}

public func BulletStrike(object pObj) {
	if(pObj && !ishit)
	{
		Sound("Thump*");
		var x = Sin(GetR(), 5);
		var y = -5;
		Fling(pObj,x,y);
		ishit = true;
	}
	return(1);
}

private func Color(int iATime) {
	var f = Sin(iATime/10, 150);
	var r,g,b;
	r = BoundBy(GetRGBValue(Col, 1)-f,0,255);
	g = BoundBy(GetRGBValue(Col, 2)-f,0,255);
	b = BoundBy(GetRGBValue(Col, 3)-f,0,255);
  return(RGBa(r,g,b));
}

public func TrailColor(int iATime) {

}

public func GlowColor(int iATime) {
  return(RGBa(0,0,255,200));
}

// Dummy-func
func HitExclude()
{

}
