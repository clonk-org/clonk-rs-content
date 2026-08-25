#strict 2

local iDir;
local iLength;

local pUpperTarget;
local pLowerTarget;

func GetUpperTarget() { return pUpperTarget; }
func GetLowerTarget() { return pLowerTarget; }

local pTargetLadder;

func GetTargetLadder() { return pTargetLadder; }

func UpdatePositionTarget()
{
	if(pTargetLadder)
	{
		SetLadderLength(30);
		var pObj = pTargetLadder->LocalN("pLowerTarget");
		SetPosition(pTargetLadder->GetX()+pObj->GetLayerOffset(), pTargetLadder->GetY()+pObj->GetLayerOffset(1)-pTargetLadder->LocalN("iLength")+iLength);
	}
	
}

func UpdateTransferZone()
{
	SetLadderLength(iLength, iDir);
}

func SetLadderLength(iNewLength, iNewDir)
{
	iDir = iNewDir;
	iLength = iNewLength;
	SetShape(-6,+50-iLength,12, iLength);
}

func GetLadderDir() { return iDir == 1; }

func TestLowerEnd()
{
	var i = 0;
	SetPosition(GetX(), GetY()-10);
	while(!GBackSolid(0,51) && i++<50) SetPosition(GetX(), GetY()+1);
	while( GBackSolid(0,50) && i++<50) SetPosition(GetX(), GetY()-1);
	if(!GBackSolid(0,50) && GBackSolid(0,51))
		return 1;
}

func TestUpperEnd()
{
	var iTestLength = iLength+10;
	var i = 0;
	while(!GBackSolid(0,51-iTestLength+18) && i++<20 && iTestLength > 20) iTestLength--;
	while( GBackSolid(0,50-iTestLength+18) && i++<20) iTestLength++;
	if(!GBackSolid(0,50) && GBackSolid(0,51))
		SetLadderLength(iTestLength, iDir);
	return iTestLength;
}

func Test()
{
	TestLowerEnd();
	TestUpperEnd();
}