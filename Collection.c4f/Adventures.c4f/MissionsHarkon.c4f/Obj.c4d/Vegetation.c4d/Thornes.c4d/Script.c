/*-- Dornen --*/

#strict 2

protected func Initialize() {
  SetAction("Be");
  SetPhase (Random(3), 0); 
}

func Timer()
{
  for(var pObj in FindObjects(Find_InRect(-13,-22,25,45), Find_OCF(OCF_Alive), Find_Layer(GetObjectLayer())))
    if( (GetXDir(pObj) <= 0 && GetDir() == 0) || (GetXDir(pObj) >= 0 && GetDir() == 1))
    {
      Fling(pObj, 2-4*GetDir(), -2);
      DoEnergy(-Random(5), pObj);
    }
}

public func AttachLeft()
  {
  var obj = FindObject(0, 13,0,0,0);
  if (!obj) return;
  // Vertex ankleben
  SetVertex(0, 0, obj->GetX()-GetX()+GetVertex(0, 0, obj));
  SetVertex(0, 1, obj->GetY()-GetY()+GetVertex(0, 1, obj));
  SetAction("Attach", obj);
  SetCategory(4);
  SetDir(1);
  return true;
  }
  
public func AttachRight()
  {
  var obj = FindObject(0,-13,0,0,0);
  if (!obj) return;
  // Vertex ankleben
  SetVertex(0, 0, obj->GetX()-GetX()+GetVertex(0, 0, obj));
  SetVertex(0, 1, obj->GetY()-GetY()+GetVertex(0, 1, obj));
  SetAction("Attach", obj);
  SetCategory(4);
  SetDir();
  return true;
  }
