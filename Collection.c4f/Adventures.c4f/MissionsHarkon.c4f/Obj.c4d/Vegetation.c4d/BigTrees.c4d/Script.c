/*-- BigTrees --*/

#strict 2

protected func Initialize()
  {
  var v;
  if (!Random(3))
    {
    if (!Random(3))   SetBole();
    else SetBranch();
    }
  else
    {
    SetLeaf();
    }
  }

private func SetBole(iIndex)
{
  if(!iIndex) iIndex = Random(3)+1;
  var szName = "BoleNormal";
  if(iIndex==1) szName = "BoleBig";
  if(iIndex==1) szName = "BoleTall";
  SetAction(szName);
  SetGraphics(szName);
  SetSolidMask(GetActMapVal("Facet", GetAction(), 0, 0),GetActMapVal("Facet", GetAction(), 0, 1),GetActMapVal("Facet", GetAction(), 0, 2),GetActMapVal("Facet", GetAction(), 0, 3));
  if (!GetR()) SetR(Random(360));
  SetClrModulation();
}

private func SetBranch(iIndex)
{
  if(!iIndex) iIndex = Random(6)+1;
  SetAction(Format("Branch%d", iIndex));
  SetSolidMask(GetActMapVal("Facet", GetAction(), 0, 0)+GetActMapVal("Facet", GetAction(), 0, 2),GetActMapVal("Facet", GetAction(), 0, 1),GetActMapVal("Facet", GetAction(), 0, 2),GetActMapVal("Facet", GetAction(), 0, 3));
  if (!GetR()) SetR(Random(360));
  SetClrModulation();
  SetGraphics(Format("Branch%d", iIndex));
}

private func SetLeaf(iIndex)
{
  var v;
  if(!iIndex) iIndex = Random(6)+1;
  SetAction(Format("Leaf%d", iIndex));
  SetR(Random(360));
  if (!Random(4)) SetCategory(GetCategory() | C4D_Foreground);
  else SetClrModulation(RGB(v=Random(128)+100,v+100,v+100));
  SetGraphics(Format("Leaf%d", iIndex));
}

private func SetVertexXY(int x, int y, int i)
  {
  SetVertex(i, 0, x, this, 1);
  SetVertex(i, 1, y, this, 1);
  }
  
private func UpdateVertices()
  {
  var i = GetAction();
       if (i == "Wood1") { SetVertexXY(-30,-6); SetVertexXY(45, 22, 1); SetVertexXY(0,0,2); SetVertexXY(0,0,3); }
  else if (i == "Wood2") { SetVertexXY(15, 26); SetVertexXY(-27, 22, 1); SetVertexXY(14, -19, 2); SetVertexXY(0,0,3); }
  else if (i == "Wood3") { SetVertexXY(-18, 27); SetVertexXY(-16, -24, 1); SetVertexXY(17, -16, 2); SetVertexXY(0,0,3); }
  else if (i == "Wood4") { SetVertexXY(-54, -8); SetVertexXY(57, -7, 1); SetVertexXY(0,0,2); SetVertexXY(0,0,3); }
  else if (i == "Wood5") { SetVertexXY(5, 51); SetVertexXY(2, -54, 1); SetVertexXY(0,0,2); SetVertexXY(0,0,3); }
  else if (i == "Wood5") { SetVertexXY(-14, -22); SetVertexXY(-10, 17, 1); SetVertexXY(19, -17, 2); SetVertexXY(18, 22, 3); }
  else if (i == "Leaf10") { SetVertexXY(-80, -80); SetVertexXY(-80, 80, 1); SetVertexXY(80, 80, 2); SetVertexXY(80, -80, 3); } // Groﬂe Bl‰tter
  else { SetVertexXY(-20, -20); SetVertexXY(-20, 20, 1); SetVertexXY(20, 20, 2); SetVertexXY(20, -20, 3); } // Kleine Bl‰tter
  SetVertex(0, 3, 100, this, 2);
  }

public func ad()
  {
  var i = Random(200);
  SetClrModulation(RGB(i,i,i));
  }
  
protected func Damage()
  {
  if (OnFire() && 40-Random(Random(40)) < GetDamage())
    {
    // Automagisch alles umliegende per Script anz¸nden, damit nicht zuf‰llig ein halber Baum stehen bleibt
    var o; while (o = FindObject(0, -70,-70,140,140, 0, 0,0, NoContainer(), o))
      if (!OnFire(o)) if (o->~IsBigTreeElement()) { o->Incinerate(); break; }
    // Zu Kohle verbrennen
    if (GetContact(0, -1, 8))
      if (!Stuck())
        CastPXS("Coal", 10, 20, Random(9)-4,0);
    }
  if (GetDamage() > 45)
    if (GetCategory() & C4D_StaticBack)
      {
      UpdateVertices();
      SetCategory(C4D_Vehicle);
      SetRDir(Random(31)-15);
      Sound("TreeDown*");
      SetSolidMask();
      }
  }

protected func LightningStrike()
  {
  return Incinerate();
  }

public func IsBigTreeElement() { return true; }
