/*--- Fackel ---*/

#strict

public Activate:
  var gbaeude, obj;
  //Nich im Himmel (außer an Gebäuden)
  if(GetMaterial()!=Material("Tunnel") && !(gbaeude=FindObject(0,1,0,0,0,OCF_Exclusive())))
      return(Sound("CommandFailure1.wav"),Message("$MsgCannotAttach$",this()));
  //Nich im Entrancebereich von Gebäuden
  if(GetOCF(gbaeude)&OCF_Entrance()) if(InEntrance(gbaeude))
      return(Sound("CommandFailure1.wav"),Message("$MsgCannotAttach$",this()));
  //Nich in Flüssigkeiten
  if(InLiquid(Par()))
      return(Sound("CommandFailure1.wav"),Message("$MsgCannotAttach$",this()));
  Sound("Connect");
  ObjectSetAction(obj=CreateObject(TORC,0,12,GetOwner()),"Burning",gbaeude);
  SetDir(Random(2)); if(gbaeude) SetDir(GetX(obj)>GetX(gbaeude),obj);
  RemoveObject();
  return(1);

public IsAnvilProduct: return(1);

private func InEntrance(gbaeude) {
  if(Inside(GetX(),
    GetX(gbaeude)+GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),0),
    GetX(gbaeude)+GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),2)+GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),0) ))
      if(Inside(GetY(),
        GetY(gbaeude)+GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),1),
        GetY(gbaeude)+GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),3)+GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),1) ))
          return(1);}