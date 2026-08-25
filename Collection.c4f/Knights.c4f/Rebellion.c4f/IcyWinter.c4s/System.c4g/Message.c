/*-- Zeitgesteuerte Nachrichten --*/

// evtl. Todo: Bei Überschreiben von Nachrichten Löscheffekt der vorherigen Nachricht ändern oder entfernen

#strict

// Eine Nachricht zeitgesteuert ausgeben.
global func TimeMessage(string szMessage, object pObj, int WaitTime, int DisplayTime, e,f,g,h,i,j)
{
  // Nachrichteneffekt
  var iEffect = AddEffect("IntTimeMessage", pObj, 1, Max(WaitTime, 1));
  EffectVar(0, pObj, iEffect) = Format("@%s", szMessage);
  EffectVar(1, pObj, iEffect) = e;
  EffectVar(2, pObj, iEffect) = f;
  EffectVar(3, pObj, iEffect) = g;
  EffectVar(4, pObj, iEffect) = h;
  EffectVar(5, pObj, iEffect) = i;
  EffectVar(6, pObj, iEffect) = j;
  // Effekt, um Nachricht zu löschen
  iEffect = AddEffect("IntTimeMessage", pObj, 1, Max(WaitTime + DisplayTime, 1));
  EffectVar(0, pObj, iEffect) = "";
  return(1);
}

global func FxIntTimeMessageTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  Message(EffectVar(0, pTarget, iEffectNumber), pTarget,
          EffectVar(1, pTarget, iEffectNumber), EffectVar(2, pTarget, iEffectNumber),
          EffectVar(3, pTarget, iEffectNumber), EffectVar(4, pTarget, iEffectNumber),
          EffectVar(5, pTarget, iEffectNumber), EffectVar(6, pTarget, iEffectNumber));
  return(-1);
}

// Eine Nachricht zeitgesteuert ausgeben.
// Hier wird die Anzeigedauer aus der Textlänge berechnet.
global func TimeMessage2(string szMessage, object pObj, int WaitTime, e,f,g,h,i,j)
{
  var iDuration = Max(StrLen(szMessage) * 3/2, 35);
  TimeMessage(szMessage, pObj, WaitTime, iDuration, e,f,g,h,i,j);
  return(iDuration);
}

// Gibt einen formatieren Dialog-String in einer TimeMessage2 aus.
// Liefert die Dauer zurück.
global func FilmOutput(string msg, object pObj, int iWaitTime, d,e,f,g,h,i,j)
{
  if(!msg) return(0);
  msg = Format("<c %x>< %s ></c>|<c c8c8c8>%s", GetColorDw(pObj), GetName(pObj), msg);
  return(TimeMessage2(msg, pObj, iWaitTime, d,e,f,g,h,i,j));
}

// Gibt einen formatieren Dialog-String aus.
global func DialogueOutput(string msg, object pObj, c,d,e,f,g,h,i,j)
{
  if(!msg) return(0);
  msg = Format("@<c %x>< %s ></c>|<c c8c8c8>%s", GetColorDw(pObj), GetName(pObj), msg);
  return(Message(msg, pObj, c,d,e,f,g,h,i,j));
}

// Länge eines Strings herausfinden.
global func StrLen(string szText)
{
  var i = 0;
  while(GetChar(szText, i++)); // Leerlaufschleife beabsichtigt!
  return(i-1);
}

