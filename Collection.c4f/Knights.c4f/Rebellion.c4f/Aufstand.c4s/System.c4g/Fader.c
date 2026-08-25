/*-- Fader --*/

#strict

// Setzt die Helligkeit. 0=ganz dunkel; 128=normal; 255=ganz hell
global func SetFadeLightness(int iLightness, int iGammaRamp)
{
  iLightness = BoundBy(iLightness, 0, 255);
  var iDark = Min(Max(iLightness - 128, 0) * 2, 255);  // Normalwert: 0
  var iAverage = Min(iLightness, 255);                 // Normalwert: 128
  var iLight = Min(iLightness * 2, 255);               // Normalwert: 255
  SetGamma(RGB(iDark, iDark, iDark), 
           RGB(iAverage, iAverage, iAverage), 
           RGB(iLight, iLight, iLight), iGammaRamp);
}

// Setzt den Fader auf iActualValue und fadet bis iFadeTo auf iGammaRamp.
// iTimerInterval und iFadeSteps bestimmen die Fadegeschwindigkeit und sind optional.
global func Fade(int iActualValue, int iFadeTo, int iGammaRamp, int iTimerInterval, int iFadeSteps)
{
  // Prüfen, ob Werte gleich sind
  if(iActualValue == iFadeTo)
    return(0);
  // Timerintervall zwischen 1 und 100; Standardwert 5
  if(!iTimerInterval) iTimerInterval = 5;
  else iTimerInterval = BoundBy(iTimerInterval, 1, 100);
  // Fadesteps zwischen 3 und (iFadeTo - iActualValue); Standardwert (iFadeTo - iActualValue)
  if(!iFadeSteps) iFadeSteps = Abs(iFadeTo - iActualValue);
  else iFadeSteps = BoundBy(iFadeSteps, 3, Abs(iFadeTo - iActualValue));
  // Effekt starten
  if(!AddEffect("IntFader_fx6C", 0, 1, iTimerInterval, 0,0, iActualValue, iFadeTo, iGammaRamp, iFadeSteps))
    return(0);
  // Fadeeffekt erfolgreich erzeugt
  return(1);
}

global func FxIntFader_fx6CStart(object pTarget, int iEffectNumber, int iTemp, var1, var2, var3, var4)
{
  // iStartValue
  EffectVar(0, pTarget, iEffectNumber) = var1;
  // iFadeTo
  EffectVar(1, pTarget, iEffectNumber) = var2;
  // iGammaRamp
  EffectVar(2, pTarget, iEffectNumber) = var3;
  // iFadeSteps
  EffectVar(3, pTarget, iEffectNumber) = var4;
  // Aktueller Wert (wird jeden Timeraufruf aktualisiert)
  EffectVar(4, pTarget, iEffectNumber) = var1;
  // Helligkeit setzen
  SetFadeLightness(var1, var3);
}

global func FxIntFader_fx6CTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var iStartValue  = EffectVar(0, pTarget, iEffectNumber);
  var iFadeTo      = EffectVar(1, pTarget, iEffectNumber);  
  var iGammaRamp   = EffectVar(2, pTarget, iEffectNumber);  
  var iFadeSteps   = EffectVar(3, pTarget, iEffectNumber);
  var iActualValue = EffectVar(4, pTarget, iEffectNumber);
  // Fadewert ausrechnen
  var iFadeValue = (iFadeTo - iStartValue) / iFadeSteps;
  if(!iFadeValue) iFadeValue = Abs(iFadeTo - iStartValue) / (iFadeTo - iStartValue);
  // Wert erhöhen
  iActualValue += iFadeValue;
  // Helligkeit setzen
  SetFadeLightness(iActualValue, iGammaRamp);
  // Prüfen, ob Endwert erreicht
  if((iActualValue == iFadeTo) ||
     !Inside(iActualValue, Min(iStartValue, iFadeTo), Max(iStartValue, iFadeTo)))
    return(-1); // Effekt löschen
  // Endwert noch nicht erreicht
  EffectVar(4, pTarget, iEffectNumber) = iActualValue;
  // Effekt weiter ausführen
  return(0);
}









