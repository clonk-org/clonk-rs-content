/*-- Das Seil --*/

#strict 2

local iLength, iCalcedLength;
local aPointsX;
local aPointsY;

protected func Initialize()
{
  Local (0)=82;
  Local (1)=82;
  aPointsX = [0,0];
  aPointsY = [0,0];
  SetPosition(0,0);
  return 1;
}

private func GetPoint(int iIndex, int fGetY)
{
  if(fGetY)
  {
    return aPointsY[iIndex];
  }
  return aPointsX[iIndex];
}

private func GetPointNum()
{
  return GetLength(aPointsX);
}

private func AddPoint(int iX, int iY)
{
  var iLength = GetLength(aPointsX);
  aPointsX[iLength] = iX;
  aPointsY[iLength] = iY;
  return 1;
}

private func RemovePoint(int iIndex)
{
  if(iIndex >= GetLength(aPointsX)) return;
  DeleteArrayItem(iIndex, aPointsX);
  DeleteArrayItem(iIndex, aPointsY);
}

private func SetPoint(int iIndex, int iX, int iY)
{
  if(iIndex >= GetLength(aPointsX)) return;
  aPointsX[iIndex] = iX;
  aPointsY[iIndex] = iY;
  return 1;
}

public func CalcLength (Target1, Target2) {
  // Anzahl Paare
  var i = GetPointNum()-1;
  var iCount = 0;
  // für jedes Paar berechnen
  while (i-->0) iCount += VerticeLength (i, Target1, Target2);
  return iCount;
}

private func VerticeLength (i, Target1, Target2) {
  // erster Vertex: Geschwindigkeit von 1.Actiontarget berücksichtigen
  if (i == 0) return Distance (
    GetPoint (0) + GetXDir (Target1) / 2, 
    GetPoint (0, 1) + GetYDir (Target1) / 2,
    GetPoint (1),
    GetPoint (1, 1));                                        
  // vorletzter Vertex: Geschwindigkeit von 2.Actiontarget berücksichtigen
  if (i == (GetPointNum () - 2)) return Distance (
    GetPoint (i),
    GetPoint (i, 1),          
    GetPoint (i + 1) + GetXDir (Target2) / 2, 
    GetPoint (i + 1, 1) + GetYDir (Target2)) / 2;
  // Ansonsten nur die Positionen
  return Distance (
    GetPoint (i),
    GetPoint (i, 1),
    GetPoint (i + 1),
    GetPoint (i + 1, 1));
}

private func Timer()
{
  var i=0;
  var iSize = 10;
  var iAmount;
  // Partikel-Effekt
  while(i<GetPointNum()-1)
  {
    iAmount += DrawParticleLine2 ("Rope", GetPoint(i)-GetX(), GetPoint(i, 1)-GetY(), 
		    GetPoint(i+1)-GetX(), GetPoint(i+1,1)-GetY(),  5, 40, RGBa(100,50,0,50));
//    CreateParticle("Rope", GetPoint(i)-GetX(), GetPoint(i, 1)-GetY(), 0, 0, 50, RGB(255));
    i++;
  }
}

private func DrawParticleLine2 (szKind, x0, y0, x1, y1, prtdist, a, b0, b1, ydir)
  {
  // Parameter gültig?
  if (!prtdist) return;
  // Anzahl der benötigten Partikel berechnen
  var prtnum = Max(Distance(x0, y0, x1, y1) / prtdist, 2);
  var i=prtnum;
  var xdir = Sin(Angle(x0,y0,x1,y1),10);
  ydir = -Cos(Angle(x0,y0,x1,y1),10);
  b1 = b0;//RGB(0,0,255);
  var i1,i2,b;
  var szPart;
  // Partikel erzeugen!
  while (i>-1)
    {
    i2 = i*256/prtnum; i1 = 256-i2;

    b =   ((b0&16711935)*i1 + (b1&16711935)*i2)>>8 & 16711935
        | ((b0>>8&16711935)*i1 + (b1>>8&16711935)*i2) & -16711936;
    if (!b && (b0 | b1)) ++b;
    szPart = szKind;
    if(i==prtnum) { szPart = "Rope3"; }
    if(!i){ szPart = "Rope2"; }
    CreateParticle(szPart, x0+(x1-x0)*i/prtnum, y0+(y1-y0)*i--/prtnum, xdir,ydir, a, b, this);
    }
  // Erfolg; Anzahl erzeugter Partikel zurückgeben
  return prtnum;
  }


func MoveRope (obj1, obj2) {
  var a,b, d1, d2;
  // Hier, weil das Seil immer verschwinden soll, wenn ein angeknotetes Objekt verschwindet.
  if (!obj1) return RemoveObject ();
  if (!obj2) return RemoveObject ();
  // Wir brauchen mindesten 3 Vertices, damit die Algorithmen funktionieren
  if (GetPointNum () < 3)
    InsertPoint (1, (GetPoint (0) + GetPoint (1)) / 2, (GetPoint (0, 1) + GetPoint (1, 1)) / 2);
  SetRopeEnd (obj1, 0, Local (2), 2);
  SetRopeEnd (obj2, GetPointNum () - 1, Local (3), -2);
  var vertexdistance = 7 + 3 * GetPointNum ();
//  var vertexdistance = 4 + 2 * GetVertexNum ();
  //  var vertexdistance = 25;
  for (var i = GetPointNum () - 2; i > 0; i--) {
    // Neue Position des Vertices
    a = (GetPoint (i - 1) + GetPoint (i + 1)) / 2;
    b = (GetPoint (i - 1, 1) + GetPoint (i + 1, 1)) / 2 +1;
    // Überschüssige Vertices, die nahe genug aneinander sind entfernen
    if (GetPointNum () > 3)
    if (Distance (GetPoint (i - 1, 0), GetPoint (i - 1, 1), GetPoint (i + 1, 0), GetPoint (i + 1, 1)) < vertexdistance)
    if (PathFree (GetPoint (i - 1, 0), GetPoint (i - 1, 1), GetPoint (i + 1, 0), GetPoint (i + 1, 1))) {
      RemovePoint (i);
      continue;
    }
    // bei zu langem Abstand einen neuen Vertex einfügen
    if (Distance (GetPoint (i - 1, 0), GetPoint (i - 1, 1), GetPoint (i, 0), GetPoint (i, 1)) > (7 + vertexdistance)) {
      InsertPoint (i, (GetPoint (i - 1, 0) + GetPoint (i, 0)) / 2, (GetPoint (i - 1, 1) + GetPoint (i, 1)) / 2);
      continue;
    }
    // Wenn durch verschieben des Vertex das Seil durch solides ginge, nicht verschieben, und bei genug Platz einen zusätzlichen Vertex einfügen
    if (!PathFree (a, b, GetPoint (i - 1), GetPoint (i - 1, 1))) {
      if (Distance (GetPoint (i - 1, 0), GetPoint (i - 1, 1), GetPoint (i, 0), GetPoint (i, 1)) > vertexdistance)
        InsertPoint (i, (GetPoint (i-1, 0)+GetPoint (i, 0))/2, (GetPoint (i-1, 1)+GetPoint (i, 1))/2);
      continue;
    }
    if (!PathFree (a, b, GetPoint (i + 1), GetPoint (i + 1, 1))) {
      if (Distance (GetPoint (i+1, 0),GetPoint (i+1, 1),GetPoint (i, 0),GetPoint (i, 1)) > vertexdistance)
        InsertPoint (i+1, (GetPoint (i+1, 0) + GetPoint (i, 0))/2, (GetPoint (i+1, 1) + GetPoint (i, 1))/2);
      continue;
    }
    SetPoint (i, a, b);
  }
  Timer();
}

func SetRopeEnd (obj, ropevertex, objvertex, whichend) {
  var x, y;
  x = GetX (obj) + GetVertex (objvertex, 0, obj);
  y = GetY (obj) + GetVertex (objvertex, 1, obj);
  // Wenn durch die Bewegung des angebundenen Objekts das Seil durch solides ginge, einen Vertex an der letzten Position des Objekts einfügen
  // wenn das Objekt selbst in fester Materie steckt, hülfe das auch nichts
  if (!GBackSolid (x - GetX (), y - GetY ())) {
    if (!PathFree (x, y, GetPoint (ropevertex + whichend, 0), GetPoint (ropevertex + whichend, 1))) {
      InsertPoint (ropevertex - BoundBy (whichend, -1, 0), x, y);
    } else {
      SetPoint (ropevertex, x, y);
    }
  }
}

private func InsertPoint (index, x, y, loc) {
  if (!AddPoint ()) return;
  for (var i = GetPointNum () - 1; i > index; i-- ) {
    SetPoint (i, GetPoint (i - 1, 0), GetPoint (i - 1, 1));
  }
  SetPoint (index, x, y);
  return 1;
}

func Connecting () {
  var Target1 = GetRealContainer (GetActionTarget (0));
  var Target2 = GetRealContainer (GetActionTarget (1));
  MoveRope (GetActionTarget (0), GetActionTarget (1));
  if (Target1 == Target2) return;
  iCalcedLength = CalcLength (Target1, Target2);
  var Difference = iCalcedLength - iLength;
  iLength = Max (iLength + ObjectCall (GetActionTarget (0), "RopeAskChangeLength", Difference, this ()), 3);
  iLength = Max (iLength + ObjectCall (GetActionTarget (1), "RopeAskChangeLength", Difference, this ()), 3);
  Difference = iCalcedLength - iLength;
  if (Difference <= 0) return;
  var Masse = GetMass (Target1) + GetMass (Target2);
  Do_Stuff (Target1, Target2, Masse, Difference, 1, Local (2));
  Do_Stuff (Target2, Target1, Masse, Difference, GetPointNum () - 3, Local (3));
}

func ConnectingDouble() {
  var Target1 = GetRealContainer (GetActionTarget (0));
  var Target2 = GetRealContainer (GetActionTarget (1));
  MoveRope (GetActionTarget (0), GetActionTarget (1));
  if (Target1 == Target2) return;
  iCalcedLength = CalcLength (Target1, Target2);
  var Difference = iCalcedLength - iLength;
  iLength = Max (iLength + ObjectCall (GetActionTarget (0), "RopeAskChangeLength", Difference, this ()), 3);
  iLength = Max (iLength + ObjectCall (GetActionTarget (1), "RopeAskChangeLength", Difference, this ()), 3);
  Difference = iCalcedLength - iLength;
  if (Difference <= 0) return;
  var Masse = GetMass (Target1) + GetMass (Target2);
  Do_Stuff (Target1, Target2, Masse, Difference, 1, Local (2));
  Do_Stuff (Target2, Target1, Masse, Difference, GetPointNum () - 3, Local (3));
}

func ConnectingSingle() {
  var Target1 = GetRealContainer (GetActionTarget (0));
  var Target2 = GetRealContainer (GetActionTarget (1));
  MoveRope (GetActionTarget (0), GetActionTarget (1));
  if (Target1 == Target2) return;
  iCalcedLength = CalcLength (Target1, Target2);
  var Difference = iCalcedLength - iLength;
  iLength = Max (iLength + ObjectCall (GetActionTarget (0), "RopeAskChangeLength", Difference, this ()), 3);
  iLength = Max (iLength + ObjectCall (GetActionTarget (1), "RopeAskChangeLength", Difference, this ()), 3);
  Difference = iCalcedLength - iLength;
  if (Difference <= 0) return;
  Do_Stuff (Target1, Target2, GetMass (Target2), Difference, 1, Local (2));
}

func ConnectingB() {
  var Target1 = GetRealContainer (GetActionTarget (0));
  var Target2 = GetRealContainer (GetActionTarget (1));
  MoveRope (Target1, Target2);
  if (Target1 == Target2) return;
}

public func SetLength(nlen) { return iLength=Max(nlen,0); }

func GetRealContainer (obj) {
  var ret = obj;
  while (Contained (ret)) ret = Contained (ret);
  return ret;
}

func Do_Stuff (Target1, Target2, Masse, Difference, RopeVertex, ObjVertex) {
  var Strength = (Difference * GetMass (Target2)) / Masse;
  /* Wenn Crewmember, dann wird Physical Fight miteingerechnet: Je größer, desto weniger wird gezogen
  Masse: Je schwerer das gegenüberliegende Objekt, desto mehr wird gezogen */
  if (GetProcedure (Target1) == "WALK")
    Strength = Max (0, Strength - (GetPhysical ("Fight", 0, Target1) / 30000 ));
  if (GetProcedure (Target1) == "FLOAT")
    Strength = Max (0, Strength - (GetPhysical ("Float", 0, Target1) / 100 ));
  PullObject ( (GetPoint (RopeVertex)+GetPoint (RopeVertex+1))/2, (GetPoint (RopeVertex, 1)+GetPoint (RopeVertex+1, 1))/2, Difference, Target1, ObjVertex);  
}

private func PullObject (toX, toY, length, obj, vtx) {
  var fFound;
  for(var x = 1; x <= 5; x++)
    if(GBackSolid(toX+x-GetX(), toY-GetY()))
    {
      toX -= 7-x;
      fFound = 7-x;
      break;
    }
  if(!fFound)
    for(var x = 1; x <= 5; x++)
      if(GBackSolid(toX-x-GetX(), toY-GetY()))
      {
        toX += 7-x;
        fFound = 7-x;
	break;
      }
  var len=Min (length,Distance (toX, toY,GetX (obj)+GetVertex (vtx, 0, obj), GetY (obj)+GetVertex (vtx, 1, obj)));
  var winkel = MakeThisAngleUseful (Angle (GetX (obj)+GetVertex (vtx, 0, obj), GetY (obj)+GetVertex (vtx, 1, obj), toX, toY), -180, +180);
  var winkel2 = MakeThisAngleUseful (Angle (0, 0, GetVertex (vtx, 0, obj), GetVertex (vtx, 1, obj)), -180, +180);
  var x = Sin (winkel, len*20);
  var y = Cos (winkel, len*20);
  if (Stuck (obj)) return;
  SetXDir (BoundBy (GetXDir (obj, 100) + x, -1000, 1000), obj, 100);
  SetYDir (BoundBy (GetYDir (obj, 100) - y, -1000, 1000), obj, 100);
  SetRDir (GetRDir (obj, 100) + (winkel - winkel2) / 10, obj, 100);
  if (length > 30) 
  SetPosition (GetX (obj) + BoundBy (x,-1,1), GetY (obj) - BoundBy (y,-1,1), obj);
  //Drehung verbessern
 }

func MakeThisAngleUseful (winkel, m, p) {
  while (winkel > p)
    winkel = winkel - 360;

  while (winkel < m)
    winkel = winkel + 360;

  return winkel;
}

public func Activate (obj1, obj2, fFlag) {
  Local (2) = GetMiddlestVertex (obj1);
  Local (3) = GetMiddlestVertex (obj2);
  SetPoint (0, GetX (obj1) + GetVertex (Local (2), 0, obj1), GetY (obj1) + GetVertex (Local (2), 1, obj1));
  SetPoint (1, GetX (obj2) + GetVertex (Local (3), 0, obj2), GetY (obj2) + GetVertex (Local (3), 1, obj2));
  if(fFlag) SetAction ("ConnectB", obj1, obj2 );
  else SetAction ("Connect", obj1, obj2 );
  iLength = CalcLength ();
}

public func CheckConVertices () {
  Local (2) = GetMiddlestVertex (GetActionTarget ());
  Local (3) = GetMiddlestVertex (GetActionTarget (1));
}

private func GetMiddlestVertex (obj) {
  var i,s = 1000,t,n;
  if (n = ObjectCall (obj, "GetVertexToConnect")) return n-1;
  // alle Vertices durchgehen
  for (i = GetVertexNum (obj); i >= 0; i--)
  {// x-Differenz zur Mitte + y-Differenz (eigentlich müsste ich Distance benutzen)
    t= (Abs (GetVertex (i, 0, obj))+Abs (GetVertex (i, 1, obj)));
    //s: kürzester Abstand bis jetzt
    if (s > t)
    {//n: Vertex mit kürzestem Abstand
      s = t;
      n = i;
    }
  }
  return n;
}

/* Mit SetObject kann man die angeknüpften Objekte ändern. num=0 für Actiontarget0 und num=1 für Actiontarget1 */
public func SetObject (obj, num) {
  // keine Gebäude
  if (GetCategory (obj) & C4D_Structure) return;
  // 1.Actiontarget
  if (num == 0)
  {
    // Action und 2.Actiontarget bleiben erhalten
    Local (2 ) = GetMiddlestVertex (obj);
    SetAction ("Connect", obj, GetActionTarget (1));
  }
  // 2.Actiontarget
  if (Par (1)==1)
  {
    Local (3 ) = GetMiddlestVertex (obj);
    SetAction ("Connect", GetActionTarget (), obj);
  }
  return 1;
}

public func LineBreak()
{
  Sound ("LineBreak");
  if (Equal (Par (0),0)) BreakMessage ();
  return 1;
}

private func BreakMessage()
{
  // Meldung bei Leitungsbausatz am einen oder anderen Ende ausgeben
  var pObj = GetActionTarget(0);
  if (GetID(pObj)!=LNKT)
    pObj = GetActionTarget(1);
  Message("$TxtBreak$",pObj);
  return 1;
}

public func GetRopeLength () { return iLength; }

protected func Destruction () {
  // Benachrichtigungen
  ObjectCall (GetActionTarget (), "RopeBreak", GetID (GetActionTarget (1)));
  ObjectCall (GetActionTarget (1), "RopeBreak", GetID (GetActionTarget ()));
}
