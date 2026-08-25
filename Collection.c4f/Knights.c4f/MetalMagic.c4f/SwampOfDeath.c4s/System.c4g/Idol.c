/*-- IDOL --*/

#strict 2
#appendto IDOL

public func CheckEmenyIntervall() { return 5; }

public func GetMaxEnergy() { return 100; }

public func UpgradeTo() { if(GetID()==IDOL) return GIDL; }

protected func FxCheckEnemyTimer(pTarget,iEffectNumber,iEffectTime)
{
  var aEnemies = FindObjects(Find_OCF(OCF_CrewMember),Find_Owner(-1),Find_Distance(40));
  for(var pClonk in aEnemies)
  {
    var iEff;
    if(iEff = GetEffect("Slow", pClonk)) EffectVar(0, pClonk, iEff) = 7;
    else AddEffect("Slow", pClonk, 1, 5, this);
  }
  return 1;
}

func FxSlowStart(pTarget, iNumber, fTmp)
{
  SetPhysical("Walk", GetPhysical("Walk",0,0, GetID(pTarget))*3/4, 2, pTarget);
  if(!fTmp)
    EffectVar(0, pTarget, iNumber) = 7;
}

func FxSlowTimer(pTarget, iNumber)
{
  if(!GetEffect("StrongSlow", pTarget)) pTarget->CastParticles("FSpark", 5, 10, 0, 0, 40, 50, RGB(150,150,150), RGB(150,150,150), pTarget);
  if(!EffectVar(0, pTarget, iNumber)--) return -1;
}

func FxSlowStop(pTarget, iNumber, iReason, fTmp)
{
  ResetPhysical(pTarget, "Walk");
}

func Incinerate()
{
    // Bestandsmaterial schleudern
    var component_id = GetComponent(0, 0, 0, GetID());
    var component_num = GetComponent(component_id, 0, 0, GetID());
    CastObjects(component_id, component_num, 35);
    // Splitter erstellen
    var sliver_id = SliverID();
    ObjectCall(CreateContents(sliver_id), "Set", 0);
    ObjectCall(CreateContents(sliver_id), "Set", 1);
    ObjectCall(CreateContents(sliver_id), "Set", 1);
    ObjectCall(CreateContents(sliver_id), "Set", 2);
    ObjectCall(CreateContents(sliver_id), "Set", 2);
    // und raus schleudern
    for (var content_num = 0; content_num < ContentsCount(); content_num++) { 
      Exit(Contents(content_num), 0, 0, Random(360), RandomX(-3, +3), RandomX(-7, +0), RandomX(-30, +30));
    }
    // Zerstören
    RemoveObject();
}
