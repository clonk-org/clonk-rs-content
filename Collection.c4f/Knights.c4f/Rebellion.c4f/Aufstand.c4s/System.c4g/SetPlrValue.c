/* Implementiert die Funktion SetPlrValue, die in
   der Engine anscheinend nicht vorhanden ist,
   obwohl sie in der Doku vorkommt...
*/

#strict

/*
global func DoPlrValue(int iValue, int iPlr)
{
  var iEffect = GetEffect("SetPlrValue_fx6C", 0, 0, 0);
  if(!iEffect)
    iEffect = AddEffect("SetPlrValue_fx6C", 0, 1);
  if(!iEffect) return(Log("Error in SetPlrValue: Effect creation failed!"));

  var obj = EffectVar(iPlr, 0, iEffect);
  // Objekt vorhanden?
  if(!obj)
    obj = CreateObject(TIM1,10,10,iPlr);
  // Objekt gehört falschem Spieler?
  if(GetOwner(obj) != iPlr)
  
  // ...
    
}
*/
