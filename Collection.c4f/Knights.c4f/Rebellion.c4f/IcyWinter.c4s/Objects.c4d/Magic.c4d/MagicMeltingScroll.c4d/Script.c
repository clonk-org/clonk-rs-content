/*-- Magisches Auftauen --*/

#strict

protected func Activate(object pByClonk)
{
  if(GetEffect("IntCastDelay", this()))
    return(Message("$MsgCastDelay$", this()), Sound("Error"));
  // Effekt erzeugen
  AddEffect("MagicMeltingSpell", pByClonk, 160, 4, this());
  // Abklingzeit
  AddEffect("IntCastDelay", this(), 1, 380, this());
  return(1);
}

protected func FxMagicMeltingSpellStart(object pTarget, int iEffectNumber, int iTemp, var1, var2, var3, var4)
{
  // Wer den Eisblock auftaut, darf Thanathos spielen
  DesignatedThanathosPlayer = GetController(pTarget);
  // Eisblock schmelzen lassen
  var iRand = RandomX(12*60/3, 12*80/3), pIceBlock;
  while(pIceBlock = FindObject(ICBL, -iRand/2, -iRand/2, iRand, iRand,,,,, pIceBlock))
    if(PathFree(GetX(pTarget),GetY(pTarget),GetX(pIceBlock),GetY(pIceBlock)))
      pIceBlock->Destruct();
  return(1);
}

protected func FxMagicMeltingSpellTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var iRand = RandomX(iEffectTime*60,iEffectTime*80);
  CreateParticle("PxSpark",0,0,0,0,iRand,RGB(255,40+Random(40),0));
  if(iEffectTime>13) return(-1);
}

protected func FxIntCastDelayStart(object pTarget, int iEffectNumber, int iTemp, var1, var2, var3, var4)
{
  SetClrModulation(RGB(128,128,128));
}

protected func FxIntCastDelayStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  SetClrModulation(RGB(255,255,255));
}

