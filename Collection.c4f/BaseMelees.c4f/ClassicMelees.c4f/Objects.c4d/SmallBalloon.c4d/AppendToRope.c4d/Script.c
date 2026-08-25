/*-- Appendto Seil --*/

#strict 2

#appendto CK5P

func MakeThisAngleUseful(iAngle, iM, iP)
{
  while(iAngle > iP) iAngle -= 360;
  while(iAngle < iM) iAngle += 360;
  return iAngle;
}

func GetMassActionTarget(pObj)
{
if (Contained(pObj)) return(0);
return(GetMass(pObj));
}

private func PullObject(iToX, iToY, iLength, pObj, pObj2, iVtx) // pObj wird versetzt
{
  if(!pObj || !pObj2) return 0;
  // Außnahmen:
  // gefesselte Clonks lassen sich abführen
  if(GetEffect("IntTied", pObj2))
    return 0;
  // Gebäude (oder Inhaltsobjekte) sowie Hintergrundobjekte gar nicht ziehen
  if(GetCategory(pObj) & C4D_Structure || GetCategory(pObj) & C4D_StaticBack)
    return 0;
  // Objekt aus dem eigenen Grafikbereich heraus nicht ziehen, führt nur zu komischen Effekten
  for(var pObj3 in FindObjects(Find_AtPoint(GetX(pObj2) + GetVertex(pObj2->~GetVertexToConnect(), 0, pObj2), GetY(pObj2) + GetVertex(pObj2->~GetVertexToConnect(), 1, pObj2))))
    if(pObj3 == pObj)
      return 0;
  // Anker ziehen keine Schiffe nach unten
  if(pObj->~IsBoat() && GetActionTarget() && GetActionTarget(1))
  {
    if((GetActionTarget() == pObj && GetActionTarget(1)->~IsAnchor() && !Contained(GetActionTarget(1)))
    || (GetActionTarget(1) == pObj && GetActionTarget()->~IsAnchor() && !Contained(GetActionTarget())))
      return 0;
  }
  // Floateinfluss: z.B. Luftschiffe können heben APPENDTO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  if(GetProcedure(pObj) == "FLOAT")
  {
  var pID=GetID(pObj);
  var pMass=GetMassActionTarget(pObj2);
	if(Inside(pMass, 1, 20))   iLength = Max(0, iLength - (GetPhysical("Float", 0, pObj) / 20));
	else
	{
		if(pID==BLMP && pMass<200) iLength = Max(0, iLength - (GetPhysical("Float", 0, pObj) / 20));// Waggon Masse 150
		if(pID==_HEL && pMass<150) iLength = Max(0, iLength - (GetPhysical("Float", 0, pObj) / 20));// Lore Masse 75
		if(pID==BALN && pMass<100) iLength = Max(0, iLength - (GetPhysical("Float", 0, pObj) / 20));// Clonk Masse 50
		if(pID==_BLN && pMass<50)  iLength = Max(0, iLength - (GetPhysical("Float", 0, pObj) / 20));// Bausatz Masse 15
	}
  }

  var fFound;
  for(var x = 1; x <= 5; x++)
    if(GBackSolid(iToX + x - GetX(), iToY - GetY()))
    {
      iToX -= 7 - x;
      fFound = 7 - x;
      break;
    }
  if(!fFound)
  for(var x = 1; x <= 5; x++)
    if(GBackSolid(iToX - x - GetX(), iToY - GetY()))
    {
      iToX += 7 - x;
      fFound = 7 - x;
	    break;
    }
  var iLen = Min(iLength, Distance(iToX, iToY, GetX(pObj) + GetVertex(iVtx, 0, pObj), GetY(pObj) + GetVertex(iVtx, 1, pObj)));
  var iAngle = MakeThisAngleUseful(Angle(GetX(pObj) + GetVertex(iVtx, 0, pObj), GetY(pObj) + GetVertex(iVtx, 1, pObj), iToX, iToY), -180, +180);
  var iAngle2 = MakeThisAngleUseful(Angle(0, 0, GetVertex(iVtx, 0, pObj), GetVertex(iVtx, 1, pObj)), -180, +180);
  var x = Sin(iAngle, iLen * 20);
  var y = Cos(iAngle, iLen * 20);
  if(Stuck(pObj)) return 0;
  SetXDir(BoundBy(GetXDir(pObj, 100) + x, -1000, 1000), pObj, 100);
  SetYDir(BoundBy(GetYDir(pObj, 100) - y, -1000, 1000), pObj, 100);
  if(!InLiquid(pObj))
    SetRDir(GetRDir(pObj, 100) + (iAngle - iAngle2) / 10, pObj, 100);
  if(iLength > 30)
  SetPosition(GetX(pObj) + BoundBy(x, -1, 1), GetY(pObj) - BoundBy(y, -1, 1), pObj);
  //Falls Objekt dann feststecken würde, nicht wirklich versetzen
  if(Stuck(pObj))
    SetPosition(GetX(pObj) - BoundBy(x, -1, 1), GetY(pObj) + BoundBy(y, -1, 1), pObj);
}