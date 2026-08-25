#strict

func Initialize() {
 //SetOwner(-1);
 return(1);
 }

func AttachTo(object pObject) {
 SetAction("Shine",pObject);  
 if(GetID(GetActionTarget()) == IDOL) SetPlrViewRange(200*ObjectCount(RIVW)); 
 if(GetID(GetActionTarget()) == GIDL) SetPlrViewRange(300*ObjectCount(RIVW)); 
 if(GetID(GetActionTarget()) == _BLN) SetPlrViewRange(400*ObjectCount(RIVW)); 
 if(GetID(GetActionTarget()) == BK5P) SetPlrViewRange(150*ObjectCount(RIVW)); 
 if(GetID(GetActionTarget()) == _MOF) SetPlrViewRange(25*ObjectCount(RIVW));
 return(1);
 }
 
public func AttachTargetLost()
{
  RemoveObject();
}

func Check() {
 if(GetOwner()==-1) return(RemoveObject());
 }

func Entrance (object pContainer) 
{
 RemoveObject();
 return(_inherited());
}