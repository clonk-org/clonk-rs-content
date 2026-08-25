/* Mysterious overlay bug */

#strict 2

#appendto ARWP

func OnRestore()
{
	SetGraphics(0,0,0,1,0);
	SetGraphics(0,0,0,2,0);
	UpdatePicture();
}