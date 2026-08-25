/* Luftblase */

#strict

protected func Shiver()
{
  // Bewegung
  SetXDir(Random(21) - 10);
  
  if(GetActTime() > 75 + Random(75))
  	DoCon(-20);
  
  return(1);
}

protected func Initialize()
{
	SetObjectBlitMode(1);
	SetClrModulation(RGBa(255,255,255,50+Random(50)));
  // Aktion setzen
  SetAction("Bubble");
  SetPhase(Random(4));
  SetComDir(COMD_Up());
  // Nicht in Flüssigkeit gestartet: gleich weg
  if (!GBackLiquid(0,0) || Stuck()) 
    Remove();
    
  Shiver();
  return(1);
}

protected func LiquidCheck()
{
	if(!GBackLiquid(0,GetYDir()*5))
		DoCon(-20);
	if(!InLiquid())
		Remove();
}
protected func Remove()
  {
  RemoveObject();
  return(1);
  }
