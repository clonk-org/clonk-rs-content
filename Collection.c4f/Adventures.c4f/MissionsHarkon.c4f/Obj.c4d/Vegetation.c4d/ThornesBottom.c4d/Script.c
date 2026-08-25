/*-- Dornen --*/

#strict 2

protected func Initialize() {
  SetAction("Be");
  SetPhase (Random(3), 0); 
}

func Timer()
{
  for(var pObj in FindObjects(Find_InRect(-22,-13,45,25), Find_OCF(OCF_Alive), Find_Layer(GetObjectLayer())))
    if( (GetYDir(pObj) <= 0 && GetDir() == 0) || (GetYDir(pObj) >= 0 && GetDir() == 1))
    {
      if(GetDir()==0) Fling(pObj, -2+4*(GetXDir(pObj)<0), -2);
      else pObj->SetAction("Tumble");
      DoEnergy(-Random(5), pObj);
    }
}

public func AttachBottom()
  {
  var obj = FindObject(0, 0,13,0,0);
  if (!obj) return;
  // Vertex ankleben
  SetVertex(0, 0, obj->GetX()-GetX()+GetVertex(0, 0, obj));
  SetVertex(0, 1, obj->GetY()-GetY()+GetVertex(0, 1, obj));
  SetAction("Attach", obj);
  SetCategory(4);
  SetDir();
  return true;
  }
  
public func AttachTop()
  {
  var obj = FindObject(0,0,-13,0,0);
  if (!obj) return;
  // Vertex ankleben
  SetVertex(0, 0, obj->GetX()-GetX()+GetVertex(0, 0, obj));
  SetVertex(0, 1, obj->GetY()-GetY()+GetVertex(0, 1, obj));
  SetAction("Attach", obj);
  SetCategory(4);
  SetDir(1);
  return true;
  }
