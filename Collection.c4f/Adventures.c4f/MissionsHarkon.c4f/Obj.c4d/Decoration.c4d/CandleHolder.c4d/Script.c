/* Kerzenhalter */

#strict

local aOMGCheck;
local fNoCutDown;

protected func Initialize()
  {
  SetAction("On");
  return(true);
  }

public func SetOMG()
  {
  RemoveEffect("IntOMG", this());
  AddEffect("IntOMG", this(), 1, 51, this());
  aOMGCheck = Find_And(Find_InRect(-35,100,70,70), Find_OCF(OCF_Alive), Find_Not(Find_Owner(NO_OWNER)));
  }

protected func FxIntOMGTimer()
  {
  // Runterfallcheck
  if (FindObject2(aOMGCheck))
    {
    Sound("TreeDown2");
    //SetAction("Off");
    SetCategory(2);
    return(-1);
    }
  }

func CanBeCutDown() { return !fNoCutDown; }
  
func CutDown()
{
	Sound("TreeDown2");
	SetAction("Fall");
	var pReminder = CreateObject(GetID(), 0, GetDefBottom()-GetY());
	pReminder->SetAction("Reminder");
	pReminder->LocalN("fNoCutDown") = 1;
  SetCategory(2);
	fNoCutDown = 1;
}
  
  
protected func Hit()
  {
  // Rumms!
  var pBrick;
  if (pBrick = FindObject2(Find_ID(_BRK), Find_InRect(-35,0,70,50)))
    {
    Split2Components(pBrick);
    }
  Sound("Crystal1");
  Sound("Crystal2");
  Sound("Crystal3");
	for(var obj in FindObjects(Find_OCF(OCF_CrewMember), Find_AtRect(-40,-10,80,20)))
		obj->~DoEnergy(-50, obj);
	for(var i = 0; i < 8; i++)
	{
		Explode(20, CreateObject(CNDL,RandomX(-40,+40),RandomX(5,10),-1));
//		CreateObject(FLAM, RandomX(-20,20), 0, -1);
	}
	SetAction("Broken");
//  Split2Components();
  }
