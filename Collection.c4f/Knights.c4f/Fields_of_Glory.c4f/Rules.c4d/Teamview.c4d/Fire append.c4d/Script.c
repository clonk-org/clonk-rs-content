/*--Feuerbrennen --*/

#strict
#appendto *
func Incineration() {
 if(ObjectCount(RTVW)) {
	var pObj=this();
	if(!FindObject(VIEW,0,0,0,0,0,"Shine",pObj)) {
		var lgt=CreateObject(VIEW,0,0,-1); 
		lgt->SetPlrViewRange(100);
		lgt->AttachTo(pObj);
		//SetActionTargets(pObj,,lgt);
		AddEffect("CheckF1re",lgt,10,10,0,GetID(this()));
		}
	}
 _inherited();
 }

func FxCheckF1reTimer(object pTarget, int iEffectNumber, int iEffectTime) {
 //Log("%v: Effekt %d; fire: %v; target0:%v, t2: %v",pTarget, iEffectTime, OnFire(GetActionTarget(0,pTarget)), GetActionTarget(0,pTarget), GetActionTarget(1,pTarget));
 if(!OnFire(GetActionTarget(0,pTarget)) || !GetActionTarget(0,pTarget)) {
  //Schedule("CheckLights()",1);
  //Log("Remove %s von %s",GetName(pTarget),GetActionTarget(0,pTarget))&&
  RemoveObject(pTarget);
  //return(-1);  //wenn das objekt entfernt wird, wird auch der effekt entfernt..?
  }
 return(1);
 }