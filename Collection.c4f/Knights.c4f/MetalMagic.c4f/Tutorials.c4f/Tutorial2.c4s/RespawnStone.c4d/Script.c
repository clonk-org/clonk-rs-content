/*-- Großer Stein --*/

#strict 2

local idStaff;
local fActivated;

protected func Initialize()
{
  SetAction("Wait");
}

public func Activate(pClonk)
{
  if(GetAction()=="Wait") SetAction("Up");
  else if(GetAction()=="Down")
  {
    var iPhase = GetPhase();
    SetAction("Up");
    SetPhase(5-iPhase);
  }
  else Up();
}

public func Up()
{
  // Effekte!
  CastParticles("MSpark", 20, 40, 0,-14, 30, 45, GetColorDw(), GetColorDw());
}

public func End()
{

}

public func Check()
{
  for(var pClonk in FindObjects(Find_ID(MAGE), Find_Distance(50)))
  {
    if(!GetEffect("IntDoMana", pClonk)) AddEffect("IntDoMana", pClonk, 1, 2, this);
    // Nicht markiert?
    if(!GetEffect(Format("HasStaff%i", idStaff), pClonk) && idStaff)
    {
      // Markieren
      AddEffect(Format("HasStaff%i", idStaff), pClonk, 1);
      // Alten Stab entfernen
      if(pClonk->HasStaff())
      {
        RemoveObject(pClonk->HasStaff());
        if(LocalN("pStaffPict", pClonk)) RemoveObject(LocalN("pStaffPict", pClonk));
      }
      CreateContents(idStaff, pClonk);
      // Hier relaunchen lassen
      AddEffect("RelaunchHere", pClonk, 500, 0, this);
      Activate(pClonk);
      // Wenn es das erste Mal ist SzenarioScript benachrichtigen
      if(!fActivated)
      {
        fActivated = 1;
        GameCall(Format("Staff%iActivated", idStaff));
      }
    }
  }
}

func FxRelaunchHereStop(pClonk, iNumber, iReason, fTmp)
{
  if(fTmp) return;
  if(iReason==4)
  {
    SetPosition(GetX(), GetY(), pClonk);
    DoEnergy(100, pClonk);
    SetAlive(1, pClonk);
    ObjectSetAction(pClonk, "Walk", 0, 0, 1);
    return -1;
  }
}

func FxIntDoManaTimer(pClonk)
{
  if(DoMagicEnergy(1, pClonk))
  {
    CreateParticle("MSpark",GetX(pClonk)+RandomX(-8, 8)-GetX(),GetY(pClonk)+RandomX(-10, 10)-GetY(),0,-20,RandomX(18,38),RGB(21,20,200));
    CreateParticle("PSpark",GetX(pClonk)-GetX(),GetY(pClonk)-GetY(),0,0,180,RGB(0,8,16));
  }
  if(!FindObject2(Find_ID(_RSS), Find_Distance(50, GetX(pClonk)-GetX(),GetY(pClonk)-GetY()))) return -1;
}
