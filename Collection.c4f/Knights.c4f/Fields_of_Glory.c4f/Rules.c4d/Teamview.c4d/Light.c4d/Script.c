#strict

func Initialize() {
 //SetOwner(-1);  
 SetPlrViewRange(100*ObjectCount(RTVW));
 return(1);

 }

func AttachTo(object pObject) {
 SetAction("Shine",pObject);
 return(1);
 }
 
public func AttachTargetLost()
{
  RemoveObject();
}


func Check() {
 if(GetAction(GetActionTarget()) eq "Dead") return(RemoveObject()); 
 } 
 