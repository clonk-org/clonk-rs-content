#strict 2
#include QJP1 

func Set(int x, int y)
{
	_inherited(x,y);
	var a = Angle(0,0,x,y); 
	SetR(a);
}
