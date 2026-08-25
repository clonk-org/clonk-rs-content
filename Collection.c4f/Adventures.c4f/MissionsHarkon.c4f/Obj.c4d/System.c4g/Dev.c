/*-- Entwicklungsfunktionen --*/

#strict 2

global func ForceDir(iDir)
{
	SetObjDrawTransform(1000*(1-2*iDir), 0, 0, 0, 1000);
}

global func SetLight(iLight, iLayer)
{
	SetClrModulation(RGB(iLight, iLight, iLight), 0, iLayer);
}