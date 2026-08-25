#strict 2

local pObj,iDist;

func Set(pTarget,pAttach,iRange)
{
  pObj=pTarget;
  SetOwner(GetOwner(pTarget));
  SetAction("Attach",pAttach);
  iDist=iRange;
  CheckDist();
}

func CheckDist()
{
  if(ObjectDistance(pObj)>iDist-30)
    return RemoveObject();
  SetPlrViewRange(BoundBy(iDist*2-ObjectDistance(pObj)*2,0,iDist));
}

protected func AttachTargetLost() {
  RemoveObject();
}