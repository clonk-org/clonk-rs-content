/*-- Einfache Kampf-KI --*/

#strict
#appendto CLNK

func AddFightAI()
{
  AddEffect("SimpleFightAI", this, 1, 5, this());
  if(!Contents())
  {
    if(Random(2))
      CreateContents(FLNT);
    else
      CreateContents(ROCK);
  }
  SetPhysical("Energy",GetPhysical("Energy")*2,PHYS_Temporary);
  SetPhysical("Fight",GetPhysical("Fight")*2,PHYS_Temporary);
  SetPhysical("CanScale",true,PHYS_Temporary);
  DoEnergy(100);
  Local(1)=GetX();
  Local(2)=GetY();
  return(1);
}

protected func FxSimpleFightAITimer(pTar, iEffNum, iEffTime)
{
  // Nächsten Gegner angreifen
  var ai_pTarget = EffectVar(1,pTar,iEffNum);
  if(ai_pTarget)
    if(ObjectDistance(ai_pTarget)>250||!Random(50)||!GetAlive(ai_pTarget))
      ai_pTarget = 0;
  if(!ai_pTarget)
    ai_pTarget=FindObject2(Sort_Random(),Find_Hostile(GetOwner()),Find_OCF(OCF_Alive),Find_NoContainer(),Find_Or(Find_Distance(75),Find_InRect(0,-50,-200+400*GetDir(),100)));
  if(!Random(3))
    if(Contents())
      if(FindObject2(Find_InRect(0,-20,50*(GetDir()*2-1),40),Find_OCF(OCF_Alive),Find_Hostile(GetOwner()),Find_Func("Visible",GetOwner()),Find_NoContainer()))
        if(!(this->~ControlThrow()))
          AddCommand(this,"Throw");
  if(!Random(30))
    if(!Contents())
    {
      if(Random(2))
        CreateContents(FLNT);
      else
        CreateContents(ROCK);
    }
  if(ai_pTarget)
    if(!GetCommand())
      if(!Random(10))
      {
        if(!Contained(ai_pTarget))
          if(GetOCF(ai_pTarget)&OCF_FightReady)
            SetCommand(this,"Attack",ai_pTarget);
          else
            SetCommand(this,"Follow",ai_pTarget);
        else
          SetCommand(this,"Enter",Contained(ai_pTarget));
      }
  if(!Random(30))
    if(GetCommand())
      SetCommand(this,"None");
  // Bewegung wenn nichts besonderes vor
  //if(!Random(3))
    if(!GetCommand())
    {
      var pObj;
      // Projektile aufsammeln
      if(!Contents()&&(pObj=FindObject2(Find_Category(C4D_Object),Find_OCF(OCF_Available),Find_Not(Find_OCF(OCF_HitSpeed1)),Find_OCF(OCF_Collectible),Find_Distance(100),Sort_Multiple(Sort_Random(),Sort_Value()))))
          SetCommand(this,"Get",pObj);
      else if(ai_pTarget&&!Random(10))
      {
        if(Random(2))
          SetComDir(COMD_Left);
        else
          SetComDir(COMD_Right);
      }
      else if(!Random(30))
        SetComDir(COMD_None);
    }
  
  // Schnellen Objekten ausweichen
  var pHitObj = FindObject2(Find_Func("Hits",GetX(),GetY()),Find_Or(Find_Category(C4D_Object), Find_Func("IsDangerous4AI")), Find_OCF(OCF_HitSpeed2), Find_NoContainer());
  if(pHitObj)
  {
    if(GetX(pHitObj)>GetX())
    {
      SetComDir(COMD_Left);
      AddCommand(this,"Jump");
    }
    else
    {
      SetComDir(COMD_Right);
      AddCommand(this,"Jump");
    }
  }
  // Zuweit von zuhause weg? :/ umkehren *Heimweh*
  if(!Random(10))
    if(Distance(GetX(),GetY(),Local(1),Local(2))>RandomX(150,250))
    {
      ai_pTarget=0;
      SetCommand(this,"MoveTo",0,Local(1),Local(2));
    }
  // Auftauchen, wenn der Atem ausgeht
  if(InLiquid() && GetBreath() <= 25 && GetComDir() != COMD_Up())
  {
    SetComDir(COMD_Up());
    SetCommand(this,"None");
  }
  // Fürs nächste mal speichern
  EffectVar(1,pTar,iEffNum)=ai_pTarget;
  return 0;
}
 
global func Hits(iX,iY)
{
  return Abs(GetXDir()*2/3)>Abs(GetX()-iX)&&Abs(GetYDir()*2/3)>Abs(GetY()-iY);
}
