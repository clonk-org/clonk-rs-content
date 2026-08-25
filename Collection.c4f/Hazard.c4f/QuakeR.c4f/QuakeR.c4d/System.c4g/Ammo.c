/* Ammosystem */

#strict

global func DoAmmo(id ammoid, int change, object target)
{ 
	var ret = inherited(ammoid, change, target);
	if(!target)
		target = this();
	
	if(target)
		target->~OnAmmoChange();
	return(ret);
}

global func NoAmmo()
{
	if(GetEffect("RapidFireBonus",this()) || GetEffect("RapidFireBonus",this()->~GetUser()))
		return(true);
	
	return(_inherited());
}
