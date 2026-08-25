/*-- Amythystmuschel --*/

#strict

public func IsShell(){ return(1); }
/* - Muschelumwandler - */
public func ShellChangeAble(){ return(1); }
public func ShellChangeTime(){ return(250); }
public func ShellChangeMats(){ return(MEHY); }
public func ShellChangeAmount(){ return(2); }

func Initialize() {
  AddEffect("Blubber", this(), 1, 15, this());
  return(1);
}

protected func Hit()
{
  Sound("MetalHit*");
  return(1);
}

public func Activate(pObj) {
  [$TxtEat$]
  Eat(pObj);
  return(1);
}

public func Eat(pObj) {
  DoBreath(+50, pObj);
  RemoveObject();
}
func FxBlubberTimer(pTarget, iNum, iTime)
{
  if(!Random(5)) 
  {
    Bubble();
  }    
  else if(!Random(150)) AddEffect("Blubber2", this(), 1, 1, this());
}

func FxBlubber2Timer(pTarget, iNum, iTime)
{
  if(!Random(3)){ Bubble();
  } 
  var obj;
  while(obj=FindObject(0,-10, -50, 20, 50, OCF_Alive(), 0, 0, NoContainer(), obj))
  {
    obj->DoBreath(+1);
    //obj->SetYDir(Min(GetYDir(obj)-1, -25));
  }  
  if(!Random(305)) return(-1);
}  


