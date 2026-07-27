/*-- Neues Objekt --*/

#strict 2
#appendto FXL1
#include FXL1

protected func Initialize()
{
 //SetClrModulation(RGBa(255,255,255,255));
 return _inherited();
}
private func Advance()
{
  // Einschlag
  var iVtx = GetVertexNum()-1, iVtxX = GetVertex(iVtx, 0), iVtxY = GetVertex(iVtx, 1);
  if (GBackSemiSolid(iVtxX-GetX(), iVtxY-GetY() ))
    return Remove();

  // Objektschaden
  if (iVtx>5) BlastObjects(iVtxX, iVtxY, 5);

  // Verzweigung
  if (!Random(35))
    LaunchLightning(iVtxX, iVtxY, iAdvX, iVarX, iAdvY, iVarY);

  // Neuen Eckpunkt
  if (!AddVertex( iVtxX+iAdvX+Random(iVarX), iVtxY+iAdvY+Random(iVarY)))
    return Remove();
    else
    DrawLightning(GetX()+GetVertex(GetVertexNum()-2,0)-45,GetY()+GetVertex(GetVertexNum()-2,1)-45,GetX()+GetVertex(GetVertexNum()-1,0)-45,GetY()+GetVertex(GetVertexNum()-1,1)-45);

  // Objektanziehung
  iVtx = GetVertexNum()-1; iVtxX = GetVertex(iVtx, 0); iVtxY = GetVertex(iVtx, 1);
  var obj;
  if (iVtx>7)
    if (obj = FindObject( 0, iVtxX-GetX()-50, iVtxY-GetY()-50, 100, 100, OCF_AttractLightning, 0,0, NoContainer()  ) )
      Attraction(obj);

  LightningEffect(4);

  // Weiter 
  return ExecLgt();
}

global func DrawLightning(x0, y0, x1, y1)
{
  var dis=Distance(x0,y0,x1,y1)/2;
  var lgh = CreateObject(SLGH,0,0,-1);
  SetCon(dis, lgh);
  
 

  
  SetR(Angle(x0,y0,x1,y1), lgh);
  SetPosition((x0+x1)/2, (y0+y1)/2, lgh);
  

  return lgh;
}
