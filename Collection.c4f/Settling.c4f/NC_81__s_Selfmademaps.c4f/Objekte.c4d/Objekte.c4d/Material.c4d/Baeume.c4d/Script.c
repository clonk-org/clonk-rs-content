#strict

#appendto TREE

// Abwärtskompatibiliät mit GWE4

func C4D_Foreground() { if(_inherited()) return(_inherited()); }

func Construction() {
  if(GetID()==BM9Z) // Für Bäume die Anfangs platziert werden
  {
  var pTree;
  while(pTree = FindObject(0,0,0,0,0,OCF_Chop(),0,0,0,pTree))
    if(!Random(2))
      SetForeground(pTree);
  }
  _inherited();
  if(!ObjectCount(BM9Z)) return();
  if(!Random(2)) 
    SetForeground();
}

func SetForeground(pObj)
{
  // CE
  if(C4D_Foreground()) SetCategory(C4D_Foreground()|GetCategory(pObj),pObj);
  // GWE4
  else if(!GetDefIncompleteActivity(GetID(pObj))) // es ist davon auszugehen, dass der Baum keine ActMap hat
    SetCategory(C4D_Object()|GetCategory(pObj),pObj);
}
