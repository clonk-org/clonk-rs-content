/*-- Blut --*/

#strict

#appendto CLNK

protected func Activate(int plr)
{
  if (GetType(plr) != C4V_Int())	return(_inherited());
  // Beschreibung ausgeben
  return MessageWindow(GetDesc(),plr);
}

protected func Death(int iKiller, a,b,c,d,e)
{
 if(FindObject(CKBT))
  var pID = GetID();
  if (pID==SKLT || pID==_SKG || pID==_SKN || pID==_SSR)
  {
   CastObjects(_BOP,1,4);
   CastObjects(_BOP,1,4);
   CastObjects(_BOP,1,4);
   CastObjects(_BOP,1,10);
   CastObjects(_BOP,1,8);
   CastObjects(_BOP,1,9);
  return(_inherited(iKiller, a,b,c,d,e));
  }
  //Blutenlassen ;)
   CastObjects(BLOD,8,4);
   CastObjects(BLOD,1,4);
   CastObjects(BLOD,6,4);
   CastObjects(BLOD,1,10);
   CastObjects(BLOD,12,8);
   CastObjects(BLOD,13,9);

  // Basisfunktion aufrufen
  return(_inherited(iKiller, a,b,c,d,e) );
}

private func Punching()
{
   
  if (!Random(3)) Sound("Kime*");
  if (!Random(5)) Sound("Punch*");
  if (!Random(2)) return(1);
  if(FindObject(CKBT))
  var pID = GetID();
  if (pID==SKLT || pID==_SKG || pID==_SKN || pID==_SSR)
  {
  CastObjects(_BOP,1,12);
  CastObjects(_BOP,1,3);
  return(1);
  }
  CastObjects(BLOD,16,12);
  CastObjects(BLOD,6,3);

  Punch(GetActionTarget());
  return(1);
}

protected func Hurt()
{
  Sound("Hurt*");
   if(FindObject(CKBT))
  var pID = GetID();
  if (pID==SKLT || pID==_SKG || pID==_SKN || pID==_SSR)
  {
  CastObjects(_BOP,1,3);
  return(1);
  }
  CastObjects(BLOD,4,3);
  return(1);
}

protected func CatchBlow()
{
  if (GetAction() eq "Dead") return(0);
  if (!Random(5)) Hurt();
  if(FindObject(CKBT))
  var pID = GetID();
  if (pID==SKLT || pID==_SKG || pID==_SKN || pID==_SSR)
  {
  CastObjects(_BOP,1,6);
  return(1);
  }
  CastObjects(BLOD,8,6);
  return(1);
}

