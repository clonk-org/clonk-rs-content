/*-- Blitzlicht --*/

/*
   Während dieser Effekt aktiviert ist, ist der Bildschirm
   ganz dunkel (schwarz), und wird ab und zu von Blitzen
   aufgehellt. Gute Horroratmosphäre, sollte in der Nacht
   eingesetzt werden.
*/

#strict

global func CreateFlashlight(int iGammaRamp)
{
  AddEffect("Flashlight_fx6C",, 110, 10,,, iGammaRamp);
}

global func RemoveFlashlight()
{
  RemoveEffect("Flashlight_fx6C");
}

global func FxFlashlight_fx6CStart(object pTarget, int iEffectNumber, bool fTemp, var1, var2, var3, var4)
{
//  Log("flashlight-start: temp=%d var1=%d", fTemp, var1);
  if(fTemp) return();
  // Ganz dunkel
  SetFadeLightness(0, var1);
  // Gamma-Rampe speichern
  EffectVar(0, pTarget, iEffectNumber) = var1;
}

global func FxFlashlight_fx6CTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  // Gamma zurücksetzen (es tritt ein Fehler mit Gammarampe 0 auf?)
//  ResetGamma(0);
  // Zufallssteuerung
  if((iEffectTime-EffectVar(1, pTarget, iEffectNumber)) < 30)
    if(Random(5)) 
      return();
  // Blitzgeräusch
  Sound("Thunder*", true);
  // Aufhellen
//  AddEffect("IntFlash", pTarget, 1, 1, 0, 0);
  Fade(200+Random(51), 0, EffectVar(0, pTarget, iEffectNumber), 1, 10+Random(5));
  // Letzter Blitz: Zeit speichern
  EffectVar(1, pTarget, iEffectNumber) = iEffectTime;
}

global func FxFlashlight_fx6CStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
//  Log("flashlight-stop: temp=%d", fTemp);
  if(fTemp) return();
  // Normale Helligkeit
  Fade(0, 128, EffectVar(0, pTarget, iEffectNumber), 1, 7+Random(7));
  ResetGamma(EffectVar(0, pTarget, iEffectNumber));
  // "Nachdonnern"
  Sound("Thunder*", true, 0, 100);
  for(var i=0; i<7; i++)
    Schedule(Format("Sound(\"Thunder*\", true, 0, %d);", 100-i*10), i*10+Random(20));
//  ResetGamma(0);
}

global func FxFlashlight_fx6CEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber, var1, var2, var3, var4)
{
  // Neue Flashlights ablehnen
  if(szNewEffectName eq "Flashlight_fx6C") return(-1);
}

/*
global func FxIntFlashStart(object pTarget, int iEffectNumber, int fTemp, var1, var2, var3, var4)
{
  // Gamma-Rampe speichern
  EffectVar(0, pTarget, iEffectNumber) = var1;
  // Helligkeit auch speichern
  EffectVar(1, pTarget, iEffectNumber) = 200+Random(51);
  // Es wird hell
  SetFadeLightness(2, var1);
}

global func FxIntFlashTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  // Helligkeit nimmt ab
  EffectVar(1, pTarget, iEffectNumber) -= (15+Random(7));
  // Helligkeit setzen
  SetFadeLightness(Max(0,EffectVar(1, pTarget, iEffectNumber)), 2);
  // Effekt eventuell entfernen
  if(EffectVar(1, pTarget, iEffectNumber) <= 0) return(-1);
}
*/
