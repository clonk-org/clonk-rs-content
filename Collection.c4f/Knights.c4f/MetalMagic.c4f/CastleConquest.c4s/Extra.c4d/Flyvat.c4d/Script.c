/*-- Flyvat --*/

#strict 2

local aFuel;
local pFuelSymbol;
local pWings;

static const TINDER=0;
static const OIL=1;
static const MAGIC=2;

func Initialize() {
  aFuel=[];
  SetLength(aFuel,2);
  pFuelSymbol = [];
  // Setze Action
  SetAction("Fold");
  SetPhase(10);
  // Reinkommen? aber gerne!
  SetEntrance(1);

  //Flügel.
  pWings=CreateObject(AWNG,0,0,GetOwner());
  pWings->SetAction(GetAction(),this);
  //SetGraphics("Wing",this,0,3,GFXOV_MODE_ExtraGraphics);
  
  // Done.
  return 1;
}

protected func ContactBottom() {
 // Neu gelandet und nicht grade erst gestartet?
 if(!GetEffect("WarmedUp",this))
 if (GetAction() != "Fold" && GetAction() != "Open" && GetAction() != "FlySlow") SetAction("Fold");
 return 1;
}

/* Steuerung */

// Aus dem Luftschiff
/* Steuerung */

private func DoDirection(int comdir)
{
  var cur_comdir = GetComDir();
  if(GetAction() == "Fold") cur_comdir = COMD_Stop;

  cur_comdir = ComDirTransform(cur_comdir, comdir);

  SetDirection(cur_comdir);
}

private func ClearDir(bool fX)
{
  if(fX && GetXDir())
  {
    if(GetXDir() > 0) SetXDir(GetXDir() - 1);
    else SetXDir(GetXDir() + 1);
  }
  if(!fX && GetYDir())
  {
    if(GetYDir() > 0) SetYDir(GetYDir() - 1);
    else SetYDir(GetYDir() + 1);
  }
}

private func SetDirection(int comdir)
{
  // Richtungsaenderung nach oben/unten geht auch mit "Turn", aber eine
  // ComDir-Aenderung, die wieder eine Turn-Action erfordern wuerde muss
  // warten, bis die jetzige Turn-Action fertig ist.
  if(GetAction() == "Turn")
  {
    //turn_end_dir = comdir;
    if(comdir == COMD_Stop || (ComDirLike(comdir, COMD_Right) && GetDir() == DIR_Left) || (ComDirLike(comdir, COMD_Left) && GetDir() == DIR_Right))
    {
      return 0;
    }
  }
	
  // ComDir uebernehmen
  SetComDir(comdir);

  // Vorherige ClearDirs wegtun
  ClearScheduleCall(this, "ClearDir");
  // Wenn Bewegung in nur eine Richtung, dann Bewegung in die andere
  // Richtung aufgeben (macht FLOAT-Prozedur nicht automagisch)
  if(comdir == COMD_Down || comdir == COMD_Up)
    ScheduleCall(this, "ClearDir", 1, Abs(GetXDir()), true);
  if(comdir == COMD_Left || comdir == COMD_Right || comdir == COMD_Stop)
  {
    ScheduleCall(this, "ClearDir", 1, Abs(GetYDir()), false);
  }
  
  if(comdir == COMD_Down && (GetComDir() == COMD_UpRight || GetComDir() == COMD_UpLeft))
   {
    var newcomd=COMD_Left;
    if(GetComDir() == COMD_UpRight)newcomd=COMD_Right;
    SetComDir(COMD_Stop);
    SetYDir();
    SetComDir(newcomd);
   }
  
  if(ComDirLike(comdir, COMD_Right) && GetDir() == DIR_Left)
  {
    SetDir(DIR_Right);
    SetAction("Turn");
  }

  if(ComDirLike(comdir, COMD_Left) && GetDir() == DIR_Right)
  {
    SetDir(DIR_Left);
    SetAction("Turn");
	}
}

// Zusatz

protected func ContainedLeft() {
    /*// Fliegen wir?
    if (WildcardMatch(GetAction(),"Fly")) {
        if (GetDir() == DIR_Left){SetComDir(COMD_Left); return(1);}
        // Wir fliegen nach rechts? Umdrehen!
        SetAction("Turn");
        SetDir(DIR_Left);
        SetComDir(COMD_Left);
        return 1;
    }
    
    if(GetAction() == "Turn")
    if(GetDir() == DIR_Right)
    {SetComDir(COMD_Stop);SetXDir();}
    // Nichts.*/
    DoDirection(COMD_Left);
    return 1;
}


protected func ContainedRight() {
    /*// Fliegen wir?
    if (WildcardMatch(GetAction(),"Fly")) {
        if (GetDir() == DIR_Right){SetComDir(COMD_Right); return(1);}
        // Wir fliegen nach rechts? Umdrehen!
        SetAction("Turn");
        SetDir(DIR_Right);
        SetComDir(COMD_Right);
        return 1;
    }
    
    if(GetAction() == "Turn")
    if(GetDir() == DIR_Left)
    {SetComDir(COMD_Stop);SetXDir();}
    // Nichts.*/
    DoDirection(COMD_Right);
    return 1;
}

protected func ContainedUp() {
    // Fliegen wir?
    if (WildcardMatch(GetAction(),"*Fly*") || GetAction() == "Turn") {
        /*// Steigen
        if(GetComDir() == COMD_UpLeft || GetComDir() == COMD_UpRight)
        {
         SetComDir(COMD_Stop);
         SetSpeed();
         SetComDir(COMD_Up);
        }
        else 
        if(GetComDir() == COMD_DownLeft || GetComDir() == COMD_DownRight)
        {
         var val=COMD_Left;
         if(GetComDir() == COMD_DownRight)val=COMD_Right;
         SetComDir(COMD_Stop);
         SetSpeed();
         SetComDir(val);
        }
        else
        SetComDir(COMD_Up);*/
        DoDirection(COMD_Up);
    }
    // Wir sind am Boden?
    if (GetAction() == "Fold") {
        // Abheben!
        CheckFuel();
        
       // if(!GetEffect("StartFly"))
        //AddEffect("StartFly",this,20,5,this);
        if(HasFuel())
        SetAction("Open");
    }
 return 1;
}

protected func ContainedDown() { return ContainedDownSingle(); }
protected func ContainedDownSingle() {
    // Fliegen wir?
    if (WildcardMatch(GetAction(),"*Fly*") || GetAction() == "Turn") {
       /* // Runter
        if(GetComDir() == COMD_DownLeft || GetComDir() == COMD_DownRight)
        {
         SetComDir(COMD_Stop);
         SetSpeed();
         SetComDir(COMD_Down);
        }
        else 
        if(GetComDir() == COMD_UpLeft || GetComDir() == COMD_UpRight)
        {
         var val=COMD_Left;
         if(GetComDir() == COMD_UpRight)val=COMD_Right;
         SetComDir(COMD_Stop);
         SetSpeed();
         SetComDir(val);
        }
        else
        SetComDir(COMD_Down);*/
        DoDirection(COMD_Down);
       return 1;
    }
 return 0;
}
        
protected func Collection2(pObj)
{
 if(!ObjectCount(NOFL))
 CheckConvert(pObj);
}

protected func CheckConvert(pObj)
{
 
 if(GetID(pObj) == COAL)
 {
  aFuel[TINDER]+=7;
  RemoveObject(pObj);
 }
 
 if(GetID(pObj) == OBRL)
 {
  aFuel[OIL]+=15;
  ChangeDef(BARL,pObj);
 }
 
 if(GetID(pObj) == CRYS)
 {
  aFuel[MAGIC]+=GetCon(pObj)/12;
  RemoveObject(pObj);
 }
 
}

protected func CheckFuel()
{
 if(!ObjectCount(NOFL))
 {
 if(!GetEffect("CheckAircraftFuel",this))
 AddEffect("CheckAircraftFuel",this,20,2,this);
 }
 
 Sound("DragonWing*");
 
 AdjustWingAction();
}        

protected func CheckWater()
{
 if(InLiquid() && GBackLiquid(0,-15))
 SetAction("Fold");
}

protected func OpenCheckCrewContents()
{
 var clnk=FindObject(0,0,0,0,0,OCF_CrewMember,0,0,this);
 if(!clnk)
 {
  var p=10-GetPhase();
  SetAction("Fold");
  SetPhase(p);
  return 1;
 }
 
 return 0;
}

protected func FlySlowEndcall()
{
  var clnk=FindObject(0,0,0,0,0,OCF_CrewMember,0,0,this);
 if(!clnk)
 SetAction("Fold");
 
 
  if(!GetEffect("WarmedUp",this))
  AddEffect("WarmedUp",this,20,30*3,this);
  SetComDir(COMD_Up);
 return 0;
}
protected func HasFuel()
{
 if(ObjectCount(NOFL))return 1;
 return aFuel[TINDER] || aFuel[OIL] || aFuel[MAGIC];
}

protected func WasteFuel()
{
 if(ObjectCount(NOFL))return 1;
 
 if(aFuel[OIL])
  aFuel[OIL]--;
 else
 if(aFuel[TINDER])
  aFuel[TINDER]--;
 else
 if(aFuel[MAGIC])
  aFuel[MAGIC]--;
}

protected func AdjustWingAction()
{
 var wing;
 if(wing=FindObject(AWNG,0,0,0,0,0,0,this))
 wing->~Check();
}

protected func FxStartFlyStart(pTarget,iEffectNumber)
{
 if(!HasFuel())return -1;
 
 if(GetAction() == "Fly" || GetAction() == "FlySlow")return 0;
 
 EffectVar(0,pTarget,iEffectNumber)=0;
 if(GetAction() != "Open")SetAction("Open");
 return 1;
}

protected func FxStartFlyTimer(pTarget,iEffectNumber,iEffectTime)
{
 AdjustWingAction();
 if(GetAction() == "Open")return;
 if(GetAction() != "FlySlow")return -1;
 
 EffectVar(0,pTarget,iEffectNumber)+=iEffectTime/30;
 
 if(EffectVar(0,pTarget,iEffectNumber) >= 15)
 {
  EffectVar(0,pTarget,iEffectNumber)=0;
  SetPhase((GetPhase()+1)%19,this);
  AdjustWingAction();
 }
 if(iEffectTime/30 >= 15)
 {
  
  SetAction("Fly"); 
  if(!GetEffect("WarmedUp",this))
  AddEffect("WarmedUp",this,20,30*3,this);
  SetComDir(COMD_Up);
  return -1;
 }
 
 return 1;
}
/*
protected func FxWarmedUpStart()
{
 return 1;
}

protected func FxWarmedUpTimer()
{
 return -1;
}*/

protected func FxCheckAircraftFuelStart(pTarget,iNumber,bTemp)
{
  if(bTemp)return 1;
  if(!pFuelSymbol[0])
  {
    pFuelSymbol[OIL]    = CreateObject(FLVS,0,0,GetOwner());
    pFuelSymbol[OIL]->SetAction("Oil",this);
    pFuelSymbol[OIL]->SetVertex(0,0,20);

    pFuelSymbol[TINDER] = CreateObject(FLVS,0,0,GetOwner());
    pFuelSymbol[TINDER]->SetAction("Coal",this);
    pFuelSymbol[TINDER]->SetVertex(0,0,16);

    pFuelSymbol[MAGIC]  = CreateObject(FLVS,0,0,GetOwner());
    pFuelSymbol[MAGIC]->SetAction("Magic",this);
    pFuelSymbol[MAGIC]->SetVertex(0,0,12);
/*    var dings=CreateObject(FLVS,0,0,GetOwner());
//    dings->SetAction("Oil",this);
//    dings->SetVertex(0,0,20);
  
    dings=CreateObject(FLVS,0,0,GetOwner());
    dings->SetAction("Coal",this);
    dings->SetVertex(0,0,16);
  
    dings=CreateObject(FLVS,0,0,GetOwner());
    dings->SetAction("Magic",this);
    dings->SetVertex(0,0,12);*/
  }
 
  return 1;
}      
 
protected func FxCheckAircraftFuelTimer(pTarget,iEffectNumber,iEffectTime)
{
  if(!HasFuel()) return -1;

  if(GetAction() == "Fly")
  {
    if(iEffectTime % 36 == 0)
      WasteFuel();
    else
      return;
  }
 
  if(FindObject(0,0,0,0,0,OCF_CrewMember,0,0,this))
  {
    SetOwner(GetOwner(FindObject(0,0,0,0,0,OCF_CrewMember,0,0,this)));
    SetOwner(GetOwner(),pFuelSymbol[OIL]);
    SetOwner(GetOwner(),pFuelSymbol[TINDER]);
    SetOwner(GetOwner(),pFuelSymbol[MAGIC]);
  }
  else
    return -1;
//bool SetObjDrawTransform(int iWidth, int iXSkew, int iXAdjust, int iYSkew, int iHeight, int iYAdjust, object pObj, int iOverlayID);

  SetObjDrawTransform(1000,0,0,0,1000*aFuel[OIL],0,pFuelSymbol[OIL]);
  SetObjDrawTransform(1000,0,0,0,1000*aFuel[TINDER],0,pFuelSymbol[TINDER]);
  SetObjDrawTransform(1000,0,0,0,1000*aFuel[MAGIC],0,pFuelSymbol[MAGIC]);

  return 1;
}      

protected func FxCheckAircraftFuelStop(pTarget,iEffectNumber)
{
  if(GetAction() == "Fly")
    if(!FindObject(0,0,0,0,0,OCF_CrewMember,0,0,this) || !HasFuel())
      SetAction("Fold");
}
