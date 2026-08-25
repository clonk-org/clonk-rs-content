/*-- Bacsktab --*/

#strict 2
#appendto KNFP

public func Activate(pClonk)
{
  if(!Contained()->~CanUse(GetID())) return;
  if(GetAction(Contained()) != "Walk") return;
  if(pClonk->~IsNotHidden()) return;
  var aRect = Find_InRect(-30,-10,30,20); if(GetDir(pClonk)) aRect = Find_InRect(0,-10,30,20);
  //var pEnemy = FindObject2(Find_OCF(OCF_CrewMember), aRect, Find_Hostile(GetOwner(pClonk)), Find_OCF(OCF_Alive));
  for(var pEnemy in FindObjects(Find_OCF(OCF_Alive), aRect))
  {
   if(!GetEffect("BabbleAI",pEnemy) && (!Hostile(GetOwner(pEnemy),GetOwner(pClonk)) || !(GetOCF(pEnemy)&OCF_CrewMember)))continue;
   break;
  }
  if(!pEnemy) return;
  if(GetDir(pEnemy)!=GetDir(pClonk)) return;
  pClonk->~DoShow(250);
  pClonk->SetAction("Assassasinate", pEnemy);
}
