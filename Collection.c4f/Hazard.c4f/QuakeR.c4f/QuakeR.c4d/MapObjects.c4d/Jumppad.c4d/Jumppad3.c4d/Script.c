#strict 2
#include QJP1 

func Set(int x, int y)
{
	_inherited(x,y);
	if(x < 0)
		SetDir(DIR_Left);
	else if(x > 0)
		SetDir(DIR_Right);
}
