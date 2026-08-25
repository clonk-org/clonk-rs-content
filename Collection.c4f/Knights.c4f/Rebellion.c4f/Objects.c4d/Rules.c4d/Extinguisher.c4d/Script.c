/*-- Löschobjekt --*/

#strict

func Check()
{
  if(GetAction() ne "Idle") return(0);
  for(var pClonk in FindObjects(Find_OCF(OCF_OnFire | OCF_Alive)))
    if(!FindObject(GetID(),0,0,0,0,0,"IsOnFire",pClonk))
     CreateObject(GetID(),0,0,-1)->SetAction("IsOnFire", pClonk);
  return(1);
}

func DoExtinguish()
{
  if(!GetAlive(GetActionTarget())) return(RemoveObject());
  if(GetActTime()>200 && !Random(4))
  {
    Extinguish(GetActionTarget());
    return(RemoveObject());
  }
  return(1);
}

func Activate(object pByObj)
{
  MessageWindow(GetDesc(), pByObj);
  return(1);
}
