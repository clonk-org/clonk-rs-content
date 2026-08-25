/*-- Neues Script --*/

#strict 2

#appendto KNIG

public func ContextUseKey(pCaller)
{
  [$CtxKeyDesc$|Image=CPGR|Condition=FindNearDoor]
  var pDoor=FindNearDoor();
  Sound("Click");
  pDoor->Open();
  return 1;
}

protected func FindNearDoor()
{
 // Damit die frechen Assassinen nicht einfach den Schluessel benutzen
 if(GetID(this) != KNIG)return false;
 
 //Nur im Stehen
 if(GetAction() != "Walk")return false;
 
 var pDoor;
 pDoor=FindObject2(Find_Distance(15),Find_Or(Find_ID(CPGR),Find_ID(CPGL)));
 return pDoor;
}
