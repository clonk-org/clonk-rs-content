/*-- Gigant --*/

#strict 2

#include CLNK

public func Purchase(int iByPlr, object BuyObject)
{
	if(!GetName(0, CLST)) return;
	var pCultSite = FindObject2(Find_ID(CLST), Find_Allied(iByPlr));
	if(pCultSite)
	{
		pCultSite->~Summon(GetID(), BuyObject, 1);
		DrawParticleLine("MSpark", GetX(pCultSite)-GetX(), GetY(pCultSite)-GetY(), GetX(BuyObject)-GetX(), GetY(BuyObject)-GetY(), 20, 100, RGBa(255,255,255,50), RGBa(200,255,255,100), -5);
		Message("", BuyObject, GetName());
		RemoveObject(this);
	}
}

protected func Initialize()
{
  AddEffect("IntBlockFIRE", this, 210, 0, this);
  return _inherited();
}

protected func Departure()
{
  SetPosition(GetX(), GetY()+10);
}

protected func Recruitment()
{
  SetPortrait("random");
  return _inherited();
}

protected func QueryCatchBlow(pObj) 
{ 
  // Eine Leich wehrt nicht mehr ab
  if(!GetAlive()) return false;
  // Pfeile nicht extra behandeln
  if(pObj->~IsArrow()) return;
  // Schutzfaktor: lässt Objekte zurückprallen 
  var iPower = BoundBy(GetMass(pObj), 0, 50) * Distance(GetXDir(pObj), GetYDir(pObj)) * Distance(GetXDir(pObj), GetYDir(pObj));   if(Random(50000) + iPower / 3 < IsShielded() * 1000) 
  {           
    // Projektil abprallen lassen 
    if(GetXDir(pObj) < 0) if(GetX(pObj) < GetX() + 6) SetPosition(GetX() + 9, GetY(pObj) - GetYDir(pObj) / 3, pObj); 
    if(GetXDir(pObj) > 0) if(GetX(pObj) > GetX() - 6) SetPosition(GetX() - 9, GetY(pObj) - GetYDir(pObj) / 3, pObj); 
    SetXDir(BoundBy(-GetXDir(pObj) / 3, -10, 10), pObj); SetYDir(0, pObj); 
    // Sounds dazu 
    if(GetMass(pObj) >= 10) Sound("ClonkHit*"); 
    if(GetMass(pObj) < 10) Sound("ArrowRicochet*"); 
    ProtectedCall(pObj,"Hit");       
    return 1; 
  }            
  return _inherited(pObj);
} 

public func IsShielded() { return Random(20); }

/* Itemlimit */
public func MaxContentsCount() { return 1; }

private func FxIntBlockFIREEffect(string szNewEffectName, object pTarget) {
  // nicht anbrennen lassen
  if(WildcardMatch(szNewEffectName,"*Fire*")) {
    AddEffect("IntHot", this, 1,15, this);
    return -1;
  }
}

private func FxIntHotStart(object pTarget, int iNumber, bool fTmp)
{
  if(fTmp) return;
  EffectVar(0,pTarget,iNumber) = 5;
}

private func FxIntHotTimer(object pTarget, int iNumber, int iTime)
{
  if(EffectVar(0,pTarget,iNumber)--<=0) return -1;
  Smoke(0, 0, Random(5)+10);
  DoEnergy(-1,this);
}

private func FxIntHotEffect(string szNewEffectName, object pTarget) {
  // gleichen Effekt zusammenfassen
  if(szNewEffectName == "IntHot") return -2;
}

private func FxIntHotAdd(object pTarget, int iNumber, string szNewEffectName, int iNewEffectTimer, int iTime) {
  // Effekt übernehmen
  EffectVar(0,pTarget,iNumber) = 5;
}

protected func StartClubStrike()
{
  AddEffect("IntStrikeWait", this, 1, 30);
  if(WildcardMatch(GetAction(),"HeavyClubStrike*")) Sound("ClubWhoosh");
 //Sound("OrkGrowl2");
}

protected func PhaseClubStrike()
{
 if(GetPhase() == 9 && WildcardMatch(GetAction(),"HeavyClubStrike*")) return HeavyStrike();
 if(GetPhase() == 7 && WildcardMatch(GetAction(),"LightClubStrike*")) return LightStrike();
}

protected func LightStrike()
{
  var x = 10;
  if(GetDir() == DIR_Left) x *= -1;
  var y = 0;
  var radius = 15;
 
 
  var iColor, iMat;
  iMat = GetMaterial(x,y+14);
  if(!GBackSolid(x,y+14)) iMat = -1;
  else Sound("Discharge");
  if(iMat != -1) //Bei GetMaterialColor steht als Anmerkung, dass es nicht synchrosicher ist.
    iColor=RGB(GetMaterialVal("Color","Material",iMat,0),
               GetMaterialVal("Color","Material",iMat,1),
               GetMaterialVal("Color","Material",iMat,2));
 
  if(iMat != -1)
  for(var cnt=0;cnt<40;cnt++)
    CreateParticle("MaterialSpark",x,y+6,RandomX(-15,15),RandomX(-15,15),30+Random(30),iColor);
 
  ShakeFree(GetX()+x,GetY()+y,radius);
}

protected func HeavyStrike()
{
  var x = 12;
  if(GetDir() == DIR_Left) x *= -1;
  var y = 14;
  var isJumping=false;
  if(WildcardMatch(GetAction(),"*Jump*"))isJumping=true;
  
  if(!isJumping)
   for(var cnt=-10;cnt<20;cnt++)
    if(GBackSolid(x,cnt) && !GBackSolid(x,cnt-2)) { y=cnt;break; }
 
 
  var radius = 15;
    
  for(var clnk in FindObjects(Find_Distance(radius,x,y),Find_Exclude(this),Find_Layer(GetObjectLayer()),Find_OCF(OCF_Alive)))
  {  
    Punch(clnk,20+Random(10));
    SetXDir(GetXDir(clnk)/2,clnk);
    SetYDir(Abs(GetXDir(clnk))*-2,clnk);
  }
 
  for(var obj in FindObjects(Find_Distance((radius/4)*3,x,y),Find_Exclude(this),Find_Layer(GetObjectLayer()),Find_Category(C4D_Object)))
  {
    var tX=1;
    if(GetX(obj) < GetX()+x)tX=-1;
    SetXDir(GetXDir(obj)+Random(20)*tX,obj);
    SetYDir(GetYDir(obj)-Random(15),obj);
  }
 
 
  var iColor, iMat;
  iMat=GetMaterial(x,y+1);
  if(!GBackSolid(x,y+1)) iMat = -1;
  else Sound("RockHit*");
  ShakeFree(GetX()+x,GetY()+y,(radius/4)*2);
 
  if(iMat != -1)// Bei GetMaterialColor steht als Anmerkung, dass es nicht synchrosicher ist.
    iColor=RGB(GetMaterialVal("Color","Material",iMat,0),
               GetMaterialVal("Color","Material",iMat,1),
               GetMaterialVal("Color","Material",iMat,2));
 
  if(iMat != -1)
    for(var cnt=0;cnt<40;cnt++)
      CreateParticle("MaterialSpark",x,y-2,RandomX(-4,4),RandomX(-25,-10),30+Random(30),iColor);
}

protected func Fighting()
{
  if(!Random(4)) SetAction("AxeStrike");
}

protected func Striking()
{
  Punch(GetActionTarget(),10+Random(10));
}

protected func ControlDigDouble()
{
  return ControlDig();
}

protected func ControlDigSingle()
{
  return ControlDig();
} 

protected func ControlDig()
{
  if(GetAction() == "Walk" && !GetEffect("IntStrikeWait", this))
    {SetAction("HeavyClubStrike",this);return 1;}
  if(GetAction() == "Jump" && !GetEffect("IntStrikeWait", this))
    {SetAction("HeavyClubStrikeJump",this);return 1;}
  return 1;
}

protected func ControlSpecial()
{
  if(GetAction() == "Walk" && !GetEffect("IntStrikeWait", this))
    SetAction("LightClubStrike",this);
  if(GetAction() == "Jump" && !GetEffect("IntStrikeWait", this))
    SetAction("LightClubStrikeJump",this);
  return true;
}


protected func Death()
{
  // den Beutel fallenlassen
  if(GetAlchemBag()) GetAlchemBag()->~Loose();

  Sound("OrkGrowl1");
  DeathAnnounce();
  // Letztes Mannschaftsmitglied tot: Script benachrichtigen
  if (!GetCrew(GetOwner()))
    GameCall("RelaunchPlayer",GetOwner());
  return 1;
}

protected func Hurt()
{
  Sound("OrkGrowl2");
  return 1;
}

public func Never() { return false; }

public func ContextChop(pCaller) { [Condition=Never] }

public func ContextConstruction(pCaller) { [Condition=Never] }

public func Summoned(pClonk)
{
  // Bei der Beschwörung zur Crew hinzufügen
  MakeCrewMember(this, GetOwner(pClonk));
}

// Kann mit einem Ritual beschworen werden
public func IsRitual()   { return 1; }

// Nur für die Lichtkirche
public func NeedChurch() { return scLightchurch; }
