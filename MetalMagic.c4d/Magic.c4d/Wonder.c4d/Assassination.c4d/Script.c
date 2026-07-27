/*-- Fire mah Lazar --*/

#strict

protected func Activate(object pCaster, object pClonk)
{
    
    // Effekt erzeugen
    if(!pClonk) pClonk = pCaster;
  
   if(!pClonk->~CanUse(MUSK)) return;
   
	// Effekte (mehr?)
  Sound("Magic*");
  
  // has to stand on the ground
  //if(!GetContact(pClonk,-1,CNAT_Bottom))
  if(!WildcardMatch(GetAction(pClonk), "*Walk*") && !WildcardMatch(GetAction(pClonk), "*Musket*"))
  {
   PlayerMessage(pClonk->GetOwner(), "$OnTheGround$", pClonk);
   Sound("Error", 0, 0, 0, pClonk->GetOwner());
   RemoveObject();
   return 0;
  }
  
  var musk=FindObject2(Find_Container(pClonk), Find_ID(MUSK));
	if(!musk)return 0;
	
	// no free shot anymore. plutos fault. everything
 //for(var obj in FindObjects(Find_Container(musk))) obj->Enter(pClonk);
 
  AddEffect("HasFreeMusketShot", pClonk, 1, 35*15);
  if(WildcardMatch(GetAction(pClonk), "*LoadMusket"))
  	pClonk->SetPhase(4);
  else pClonk->~LoadMusket();//SetAction("LoadMusket");
  	
	
	
	AddEffect("ControlAssassination", pClonk, 1, 2, 0, GetID());
	
	RemoveObject();
	return 1;

}

func FxControlAssassinationTimer(pTarget,iEffectNumber, iEffectTime)
{
	if(iEffectTime > 35*15)return -1;
	var musk=FindContents(MUSK, pTarget);//FindObject2(Find_Container(pRealCaster), Find_ID(MUSK));
	if(!musk)return 0;
	if(WildcardMatch(GetAction(pTarget), "*Musket*"))return;
	//if(Contents(0, musk))RemoveObject(Contents(0, musk));
}

func FxControlAssassinationShootProjectile(pTarget, iEffectNumber, pWhat)
{
	if(!pWhat->~IsBullet())return 0;
	
	var angle=Angle(0,0,GetXDir(pWhat), GetYDir(pWhat));
	pWhat->RemoveObject();
	var x=Sin(angle, 10);
	var y=-Cos(angle, 10);
	var laser = CreateObject(ZLAS,AbsX(GetX(pTarget))+x,AbsY(GetY(pTarget))+y,GetController(pTarget));
  laser->SetClrModulation(pTarget->GetColorDw());
  laser->Set(angle,2,1000,10,MASS,pTarget,5);
 // Set(int iAngle, int iWidth, int iDist, int iT, object pDmg, object pAtt, int iReflect)
	RemoveEffect(0, pTarget, iEffectNumber);
}

func LaserStrike(pWho, c, iFrom)
{
	var lHP=GetPhysical("Energy", 0, pWho)/1000-GetEnergy(pWho);
	lHP*=1000;
	
	var p=50;
	var dmg=(lHP*p)/100;
	dmg=BoundBy(dmg, 20, 30);
	DoEnergy(-dmg, pWho, 0, 0, iFrom);
	return true;
}

/* Zaubercombo */
//public func NeedChurch() { return scBloodchurch; }
func HasItemRequirements(){return MUSK;}
public func IsWonder(){return true;}

public func GetSpellClass(object pMage) { return(MISC); }
public func GetSpellCombo(pMage) { return ("465"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
