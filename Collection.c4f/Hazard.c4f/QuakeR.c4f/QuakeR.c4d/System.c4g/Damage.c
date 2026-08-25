/* Schadenssystem */

#strict

//Schadens-Konstanten
static const DMG_Melee      = 1;
static const DMG_Projectile = 2;
static const DMG_Fire       = 3;
static const DMG_Explosion  = 4;
static const DMG_Energy     = 5;
static const DMG_Bio        = 6;

//static debugMe;

//global func DebugMe() { debugMe = 1; _inherited(); }

global func DoDmg(int iDmg, int iType, object pTarget, int iPrecision, int dmgplayer) {
  if(!pTarget)
    if(!(pTarget = this()))
      return(0);

  if(!iPrecision)
    iPrecision = 10;

  var dmg;
  var dmgdealer = dmgplayer-1;
  if(dmgdealer < 0)
  	dmgdealer = GetController(this());
  if(!this())
	dmgdealer = -1;

  var red = pTarget->~OnDmg(iDmg, iType); //reduction
  // reduction
  dmg = iDmg*(100-red)*iPrecision;

  //Schaden machen
  var pFrom;
  //if(pTarget != this())
    pFrom = this();
  pTarget->~LastDamageType(iType);
  pTarget->~OnHit(dmg/1000, iType, pFrom);

  // Killer setzen wenns nicht die umgebung war
  if(this() && this() != pTarget && GetOCF(pTarget) & OCF_CrewMember || dmgplayer)
  {
    //DebugLog("DoDmg sets killer: %d, owner of %s", "damage", this()->GetOwner(), this()->GetName());
	  //Log("%d -> %d, %s -> %s                 (%d)", dmgdealer, GetController(pTarget), GetPlayerName(dmgdealer), GetPlayerName(GetController(pTarget)));
    SetKiller(dmgdealer, pTarget);
	SetLastDmgDealer(dmgdealer,pTarget);
  }

  if(GetCategory(pTarget) & C4D_Living)
  {
	return(DoEnergy(-dmg,pTarget, true,0,dmgdealer+1));
  }
  return(DoDamage(dmg/1000,pTarget));
}

global func GetKiller(object target)
{
	if(target == 0)
		target = this();
	var effect = GetEffect("QLastDamage", target);
	if(effect != 0)
	{
		//Log("Killer from Effect: %d", EffectVar(0, target, effect));
		return(EffectVar(0, target, effect));
	}
	
	return inherited(target);
}

global func SetLastDmgDealer(int plr, object target)
{
	var effect = GetEffect("QLastDamage", target);
	if(!effect)
		AddEffect("QLastDamage", target, 1, 0, target, 0, plr);
	else
		EffectVar(0,target,effect) = plr;

	// Backup.
	SetLocal(999,plr,target);
//	if(debugMe)
//		Message("@%d",target,plr);

	return effect;
}

//KillStats-Effekt, Tötungsstatistiken!
global func FxQLastDamageStart(object pTarget, int iEffectNumber, int iTemp, int dmgdealer) {
  if(iTemp)
    return(FX_OK);
  //Effectvars:
  // 0 - Spieler der letzten Damage gemacht hat
  EffectVar(0, pTarget, iEffectNumber) = dmgdealer;
}

global func FxQLastDamageStop(object pTarget, int iEffectNumber, int reason, int iTemp) {
	SetKiller(EffectVar(0, pTarget, iEffectNumber),pTarget);
	// effekt wird niemals gelöscht
	return(-1);
}

//Dmg++
global func FxQLastDamageEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  if(szNewEffectName S= "QLastDamage")
    return(-2);
}

global func FxQLastDamageAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer, int dmgdealer) {
  EffectVar(0, pTarget, iEffectNumber) = dmgdealer;
}