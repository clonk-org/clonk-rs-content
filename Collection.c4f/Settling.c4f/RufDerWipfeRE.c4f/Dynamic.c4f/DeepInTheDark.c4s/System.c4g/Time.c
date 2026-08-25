/*-- Zeit im Dunkeln^^ --*/

#strict
#appendto TIME

private func Timing()
{
  // Helligkeit nach 0 setzen
  var gamma;
  gamma=0;
  if(flash>0)
   flash=BoundBy(flash-15,0,200);
  // Helligkeit und Himmelsfarbe anpassen
  if (gamma>dayhelligkeit) ++dayhelligkeit;
  if (gamma<dayhelligkeit) --dayhelligkeit;
  if (gamma!=dayhelligkeit||flash) RestoreColors(dayhelligkeit+flash);
  // Fertig
  return(1);
}

public func RestoreColors(gamma)
{
  var blue=50-gamma/2;
  RestoreSkyColors(gamma,0,0,blue);
  RestoreMatColors(gamma,0,0,blue);
  // Umwelt druff
  var green=ProzentVon(globalnature,globalpollution)/3;
  var red  =0;
  gamma = (gamma-50)*2;
  SetGamma(RGB(BoundBy(0+ red/2,0,255),BoundBy(0+ green,0,255),0),RGB(BoundBy(128+gamma+red,0,255),BoundBy(128+green+gamma,0,255),BoundBy(128+gamma,0,255)),RGB(BoundBy(245+gamma/4+red/4,0,255),BoundBy(245+gamma/4,0,255),BoundBy(245+gamma/4,0,255)),1);
  for(var obj in FindObjects(Find_Func("IsLight")))
    obj->~CheckLight();
  return(1);
}

public func RestoreSkyColors(ipercent,exred,exgreen,exblue)
{ 
  return(1);
}  

public func RestoreMatColors(ipercent,exred,exgreen,exblue)
{
  exred+=GetTemperature()*2;
  exblue-=GetTemperature()*2;
  var red,green,blue;
  var gamma=100-(100-ipercent)*5/6;
  red=green=blue=255*gamma/100;
  /* Extrafärbung drauf */
  red+=exred;
  green+=exgreen;
  blue+=exblue;
  /* Limits */
  if(red>255) red=255;  if(green>255) green=255;  if(blue>255) blue=255;
  if(red<000) red=000;  if(green<000) green=000;  if(blue<000) blue=000;
  /* Umfärben */
  SetMatAdjust(RGB((red+255+255)/3,(green+255+255)/3,(blue+255+255)/3));
  return(1);  
}

private func CalcDayRed()
{
  return 0;
}

global func IsDay()    {return false;}
global func IsNight()  {return true;}
global func IsVeryDay(){return false;}
