/*-- Zielscheibe --*/

#strict 2

/* Initialisierung */

local aClonks;

local aPositions;

local iState;
local iShootCounter;
local iAimPhase;
local pBow;
local pArrow;
local iYOffset;
local iFinishTime;

protected func Initialize()
{
  SetAction("Stand");
	aClonks = [];
	aPositions = [];
	iState = 0;
  return 1;
}

func GetClonkCount() { return GetLength(aClonks); }
func FreePlace() { return GetLength(aClonks)<GetLength(aPositions); }

func AddPosition(pClonk)
{
	aPositions[GetLength(aPositions)] = [GetX(pClonk), GetY(pClonk), GetDir(pClonk)];
}

func ExecActivity(pClonk, iTime)
{
	if(!aClonks) aClonks = [];
	if(GetIndexOf(pClonk, aClonks) >= 0) return;
	if(!FreePlace()) return;
	aClonks[GetLength(aClonks)] = pClonk;
	iFinishTime = iTime;
}

func CancelActivity(pClonk)
{
	var index = GetIndexOf(pClonk, aClonks);
	if(index < 0) return; // nicht mehr in der Liste
	pClonk->MessageEx("Ich muss weg Leute.", pClonk);
	if(index > 0)
		return DeleteArrayItem(index, aClonks);
	DeleteArrayItem(index, aClonks);
	if(pBow) pBow->RemoveObject();
	if(pArrow) for(var obj in pArrow) if(obj) obj->RemoveObject();
	pArrow = 0;
	iState = 0;
}

func Timer()
{
	if(GetEffect("IntWait", this)) return 1;
	if(!GetLength(aClonks)) return;
	if(iState == 0)
	{
		var fail = 0;
		for(var i = 0; i < GetLength(aClonks); i++)
			if(!aClonks[i]->ExecMoveTo(aPositions[i][0], aPositions[i][1], aPositions[i][2]))
				fail += 1;
		if(fail) return;
		iState = 1;
		iShootCounter = 0;
		return;
	}
	if(iState == 1)
	{
		if(!aClonks[0]->GetArrow()) aClonks[0]->CreateContents(ARRW);
		if (!aClonks[0]->IsAiming())
    {
			var pActBow = aClonks[0]->FindContents(BOW1);
			if(!pActBow) { pBow = aClonks[0]->CreateContents(BOW1); pActBow = pBow; }			
			aClonks[0]->LoadBow(pActBow, 1);
			return;
    }
		iState = 2;
		return;
	}
	if(iState == 2)
	{
		iYOffset = RandomX(-9,20);
		aClonks[0]->FireBowAtX(GetX(), GetY()-iYOffset, 0, ARRW);
		iYOffset -= 6;
		AddEffect("IntWait", this, 1, 50);
		iShootCounter += 1;
		if(iShootCounter < 3)
		{
			if(!aClonks[0]->GetArrow()) aClonks[0]->CreateContents(ARRW);
			aClonks[0]->LoadBow(aClonks[0]->FindContents(BOW1), 1);
		}
		else
			iState = 3;
		return;
	}
	if(iState == 3)
	{
		if(pBow) pBow->RemoveObject();
		pBow = 0;
		var pClonk = aClonks[0];
		if(GetLength(aClonks) > 1)
		{
			var pTalker = aClonks[Random(GetLength(aClonks)-1)+1];
			CommentShot(pClonk, pTalker);
		}
		
		DeleteArrayItem(0, aClonks);
		if(pClonk->GetLifeState() == 1) // Wenn er noch Freizeit hat, noch ne Runde
			PushBack(pClonk, aClonks);
		else
		{
			if(pClonk->GetLifeState() == 2)
				pClonk->MessageEx("Ich muss jetzt ins Bett.", pClonk);
			else
				pClonk->MessageEx("Ich muss jetzt an die Arbeit.", pClonk);
			pClonk->FinishActivity();
		}
		if(iFinishTime <= GetTime())
		{
			for(var i = GetLength(aClonks)-1; i >= 0; i--)
			{
				pClonk = aClonks[i];
				DeleteArrayItem(i, aClonks);
				pClonk->FinishActivity();
			}
		}
		iState = 0;
		if(pArrow) for(var obj in pArrow) if(obj) obj->RemoveObject();
		pArrow = FindObjects(Find_ID(ARRW), Find_Action("Stick"), Find_AtRect(-13,-20,26,40));
		return;
	}
}

func CommentShot(pClonk, pTalker)
{
	if(Abs(iYOffset) < 2)
	{
		MessageEx(Format("Hey voll ins schwarze %s!", GetName(pClonk)), pTalker);
		pTalker->JumpUp();
	}
	else if(Abs(iYOffset) < 5) MessageEx(Format("Naja %s fast hättest du getroffen!", GetName(pClonk)), pTalker);
	else if(Abs(iYOffset) < 9) MessageEx(Format("Vielleicht wird es das nächste mal besser %s", GetName(pClonk)), pTalker);
	else MessageEx(Format("Das war ja wohl mal voll danneben %s!", GetName(pClonk)), pTalker);
}

/* Kann von Pfeilen getroffen werden */

public func IsArrowTarget() { return GetCon() >= 100; } 