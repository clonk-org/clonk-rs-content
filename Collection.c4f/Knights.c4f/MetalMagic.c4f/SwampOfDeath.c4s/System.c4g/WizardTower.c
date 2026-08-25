/*-- Zauberturm --*/

#strict 2

#appendto WTWR

public func NeedLevel() { return (GetID()==WZKP)*2; }

public func GetUpgradeValue() { if(GetID()==WTWR) return 10; }
public func UpgradeTo() { if(GetID()==WTWR) return WZKP; }

public func GetMaxEnergy() { return 600+500*(GetID()==WZKP); }
public func CheckEmenyIntervall() { return 20-10*(GetID()==WZKP); }

public func GetDistance() { if(GetID()==WZKP) return(100); return(65); } 

public func Upgraded(iLive)
{
  SetObjDrawTransform(800, 0, 5000, 0, 1000);
  SetPosition(GetX()-2, GetY()-5);
  SetAction("Ready");
}

protected func FxCheckEnemyTimer(pTarget,iEffectNumber,iEffectTime)
{
  var aEnemies = FindObjects(Find_OCF(OCF_CrewMember),Find_Owner(-1),Find_Distance(GetDistance()));
  for(var pClonk in aEnemies)
  {
    if(!GetAlive(pClonk)) continue;
    if(GetEffect("StrikeEnemy",pClonk)) continue;
    if(GetID(pClonk)==BTRL) continue;
    AddEffect("StrikeEnemy",pClonk,20,1,this);
    break;
  }
  return 1;
}

protected func FxStrikeEnemyTimer(object pTarget, int iEffectNumber,int iEffectTime)
{
  // Warten
  if(EffectVar(0, pTarget, iEffectNumber))
  {
    if(EffectVar(0, pTarget, iEffectNumber)++>=6) return -1;
    return 0;
  }
  // Nicht weglaufen!1
  if(ObjectDistance(pTarget,this)>GetDistance()) return -1;
  // Nicht weg sein!
  if(!pTarget || !GetAlive(pTarget)) return -1;
  // Nicht verstecken!
  if(Contained(pTarget)) return -1;
 
  //aua machen!
  for(var cnt=1+Random(3);cnt;cnt--)
  DrawLightning(GetX(),GetY()-50,GetX(pTarget)+RandomX(-4,4),GetY(pTarget)-4+RandomX(-4,4));
  DoEnergy(-8-Random(3), pTarget);
  //CastParticles("PxSpark",10,5,GetX(pTarget)-GetX(),GetY(pTarget)-GetY()-4,20,50,RGB(200,200,100),RGB(255,255,255));
  CreateParticle("PSpark",0,-50,0,0,250,RGBa(230,230,255,100));
  if(!pTarget) return;
  EffectVar(0, pTarget, iEffectNumber) = 1;

  return 1;
}
