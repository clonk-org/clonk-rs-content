#strict

#appendto LGFL

public func Init()
{
  if(!GetEffect("CheckPWR",this))
    AddEffect("CheckPWR",this,10,15,this);
  _inherited(...);
}

public func FxCheckPWRTimer()
{
  var pViews=FindObjects(Find_ID(PRVW),Find_ActionTarget(this));
  var pView,bDoBreak;
  for(var pClonk in FindObjects(Find_OCF(OCF_Alive),Find_OCF(OCF_CrewMember),Find_Distance(iCon-30)))
  {
    bDoBreak=false;
    for(pView in pViews)
      if(LocalN("pObj",pView)==pClonk)
      {
        bDoBreak=true;
        break;
      }
    if(bDoBreak)  continue;
    CreateObject(PRVW,0,0,-1)->Set(pClonk,this,iCon);
  }
}
