/*-- Neues Script --*/

#strict

#appendto KNIG

func SpecialArrowTreatment(pArrow)
{
	return SpecialEffectCall("OnFireArrow", pArrow);
}

func SpecialEffectCall(string szControl)
  {
  // Von Effektzahl abwärts zählen
  var i = GetEffectCount(0, this()), iEffect;
  var res=0;
  while (i--)
    {
    // Effekte mit Control im Namen benachrichtigen	  
    iEffect = GetEffect("*", this(), i);
    //  Message("%s", this(), GetEffect(0, this(), iEffect, 1));
    if ( GetEffect(0, this(), iEffect, 1) )
      res = EffectCall(this(), iEffect, szControl,...);
    if(res)return res;
    }
  return(res);
  }
