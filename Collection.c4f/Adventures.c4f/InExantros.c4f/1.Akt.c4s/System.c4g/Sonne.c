#strict
#appendto TIME

Finster:
  SetSkyAdjust(RGBa(0,0,0,100), RGB(0,0,0));
  SetGamma(0, RGB(98, 98, 128), 16777215, 3);
  SetLocal(2,100);
  return(1);

Timing:
  if(gnacht==1)  return(Finster());
  // Zeitfortschritt
  Local(1)=(Local(1)+10)%TimeResolution();
  // Helligkeit nach Tageszeit berechnen
  Var() = GetLightIntensity(Local(1));
  // Helligkeit und Himmelsfarbe anpassen
  if (Var()>Local(2)) RestoreSkyColors(++Local(2));
  if (Var()<Local(2)) RestoreSkyColors(--Local(2));
  // Fertig
  return(1);