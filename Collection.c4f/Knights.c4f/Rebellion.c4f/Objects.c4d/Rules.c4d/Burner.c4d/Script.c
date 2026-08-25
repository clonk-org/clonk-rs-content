#strict

protected func Check()
{
 var king, burnobject;
 while (king = FindObject(KING, 0,0,0,0, 0, 0,0, NoContainer(), king))
 {
  burnobject = FindObject(_BOB, 0,0,0,0, 0, "Exist",king);
/*  if (Contained(king))
   continue;*/
  if (!burnobject)
   ObjectSetAction(CreateObject(_BOB, 0,0, GetOwner(king)), "Exist", king);
 }
}

protected func Activate()
{
 MessageWindow(GetDesc(),Par());
}
