/*-- Materialhelfer --*/

#strict 2

func Initialize() {
  var pObj;
  if (!Random(3))
       pObj = CreateObject(HLGN);
  else pObj = CreateObject(ROCK);
  SetXDir(GetXDir(), pObj);
  SetYDir(GetYDir(), pObj);
  RemoveObject();
  return 1;
}
