/*--- Stab von Cla-Kra-Zoth  ---*/

#strict 2

local r,g,b,r2,g2,b2;

public func Initialize() {
  r=128;
  g=128;
  b=128;
  return 1;
}

protected func ChooseElement(pCaller)
{
 CreateMenu(GetID(),pCaller,this);
 AddMenuItem("$Fire$", "Become(SMFR)",FIRE,pCaller);
 AddMenuItem("$Earth$","Become(SMER)",EART,pCaller);
 AddMenuItem("$Air$",  "Become(SMAR)",AIR1,pCaller);
 AddMenuItem("$Water$","Become(SMWT)",WATR,pCaller);
}

public func Become(idElement)
{
  // Definition ändern
  ChangeDef(idElement);
  // Raus und rein, damit der Magus den Stab richtig behandelt
  var pClonk = Contained();
  Exit(0,0,10);  Collect(this, pClonk);
}

public func Activate(caller)
{
  [$TxtUse$]
  // nur für Kampfzauberer
  if(!(caller->~CanUse(GetID()))) return PlayerMessage(GetOwner(caller),"$TxtError$",caller,GetName(caller))&Sound("Error");
  
  ChooseElement(caller);
  return 0;
}

// Fuer die Hilfsnachricht
protected func Entrance(pObj)
{
 if(GetOCF(pObj) & OCF_CrewMember && GetPlayerName(GetOwner(pObj)))MMShowHelpMessage("OnStaffCollect",GetOwner(pObj));
}
  
public func Departure(object pCaller)
{
  if(GetAction(pCaller) != "Throw" && GetAction(pCaller) != "RideThrow" )return;
  var angle=-5;
  if(GetDir(pCaller)==DIR_Left)angle=-175;
  if(pCaller->~BattleSpells())
  if(GetMagicEnergy(pCaller)>7)
  {
    DoMagicEnergy(-7,pCaller);
    AddEffect("StaffFlight",this,20,2,this,0,pCaller,angle);
  }
}

protected func Coloring()
{
  if(Contained())
    if(this != Contained()->FindObject(GetID(), 0, 0, 0, 0, 0, 0, 0, Contained()))
      return 1;
      
  r2+=RandomX(-2,2);
  g2+=RandomX(-2,2);
  b2+=RandomX(-2,2);
 
  r+=r2;
  g+=g2;
  b+=b2;
 
  r=BoundBy(r,20,250);
  g=BoundBy(g,20,250);
  b=BoundBy(b,20,250);
  if(Contained())
  {
    var i, pObj;
    while(pObj = Contents(i++, Contained())) if(GetID(pObj)==GetID())
      SetColorDw(RGB(r,g,b), pObj);
  }
  else SetColorDw(RGB(r,g,b));
}
 
protected func Hit()
{
  if(GetEffect("StaffFlight",this))
  {
  SetXDir(GetXDir()/-2);SetYDir(GetYDir()/-2);SetRDir(Random(180)+1);
  EffectVar(3,this,GetEffect("StaffFlight",this))=1;}
}
  
protected func FxStaffFlightStart(object pTarget, int iEffectNumber, int iTemp, par1,angle)
{
  EffectVar(0,pTarget,iEffectNumber)=par1;
  EffectVar(1,pTarget,iEffectNumber)=angle;
  EffectVar(3,pTarget,iEffectNumber)=0;
  pTarget->SetXDir(Cos(angle,50));
  pTarget->SetYDir(Sin(angle,50));
  pTarget->SetRDir(90);
}
  
protected func FxStaffFlightTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(Contained(pTarget))
  {
  	if(Contained(pTarget) == EffectVar(0,pTarget,iEffectNumber))pTarget->~OnMagicStaffRecollection();
  	return -1;
  }
  if(!GetRDir(pTarget))SetRDir(Random(180),pTarget);
  if(Abs(GetRDir(pTarget))<90)SetRDir(GetRDir(pTarget)+5,pTarget);
   
  if(!EffectVar(0,pTarget,iEffectNumber) || !GetAlive(EffectVar(0,pTarget,iEffectNumber)))return -1;
  
  if(ObjectDistance(pTarget,EffectVar(0,pTarget,iEffectNumber))<GetDefCoreVal("Height","DefCore",GetID(EffectVar(0,pTarget,iEffectNumber)))/2)
  {
    pTarget->Collect(pTarget,EffectVar(0,pTarget,iEffectNumber));
    if(Contained(pTarget) == EffectVar(0,pTarget,iEffectNumber))pTarget->~OnMagicStaffRecollection();
    return -1;
  }
   
  var angle=Angle(pTarget->GetX(),pTarget->GetY(),GetX(EffectVar(0,pTarget,iEffectNumber)),GetY(EffectVar(0,pTarget,iEffectNumber)));
   
  if(iEffectTime<20)
  {
    pTarget->SetXDir(Cos(EffectVar(1,pTarget,iEffectNumber),40));
    pTarget->SetYDir(Sin(EffectVar(1,pTarget,iEffectNumber),40));
    return 0;
  }  
    
  pTarget->SetXDir(BoundBy(pTarget->GetX(EffectVar(0,pTarget,iEffectNumber))-pTarget->GetX(),pTarget->GetXDir()-10,pTarget->GetXDir()+10));
  if(iEffectTime>35 || EffectVar(3,pTarget,iEffectNumber))pTarget->SetYDir(BoundBy(pTarget->GetY(EffectVar(0,pTarget,iEffectNumber))-pTarget->GetY(),pTarget->GetYDir()-10,pTarget->GetYDir()+10));
  else pTarget->SetYDir(-25);
  
  if(Abs(pTarget->GetXDir())<3)pTarget->SetXDir(Cos(angle,20));
  if(Abs(pTarget->GetYDir())<3)pTarget->SetYDir(Sin(angle,20));
   
  pTarget->SetXDir(BoundBy(pTarget->GetXDir(),-50,50));
  pTarget->SetYDir(BoundBy(pTarget->GetYDir(),-50,50));
   
  return 1;
}

protected func FxStaffFlightStop(pTarget, iEffectNumber, iReason, iTemp)
{
	if(iTemp)return;
	if(pTarget)
		pTarget->~OnFlightStop();
}

// Routineabfragen für Herstellung
public func IsWizardTowerProduct() { return 1; }
