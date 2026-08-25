/*-- Zombie-Kampf-KI --*/
// Angelehnt an die "Einfache Kampf-KI" aus der 3. Kampangenmission

#strict 2
#appendto CLNK

func AddWCAI()
{
  AddEffect("WalkingCorpseAI", this, 1, RandomX(4,6), this);
  ResetPhysical();
  SetPhysical("CanScale",true,PHYS_Temporary);
  if(GetAction()=="Dead")
  {
    SetAlive(true);
    DoEnergy(100);
    SetAction("FlatUp",0,0,true);
  }
  if(GetID()!=ZOMB)
  {
    // Hautfarbe färben :)
    SetObjectBlitMode(4);
    SetClrModulation(RGB(200,255,228));
    // Physikals machen
    SetPhysical("Fight",100000,PHYS_Temporary);
  }
  SetCon(RandomX(80,120));
  SetPhysical("Walk",40000*100/GetCon(),PHYS_Temporary);
  SetPhysical("Jump",30000*100/GetCon(),PHYS_Temporary);
  SetPhysical("Energy",30000*GetCon()/100,PHYS_Temporary);
  
  Local(1)=GetX();
  Local(2)=GetY();
  return(1);
}

protected func FxWalkingCorpseAITimer(pTar, iEffNum, iEffTime)
{
  var temp;
  // Nächsten Gegner angreifen
  var ai_pTarget = EffectVar(1,pTar,iEffNum);
  if(ai_pTarget)
    if(ObjectDistance(ai_pTarget)>250||!Random(50)||!GetAlive(ai_pTarget))
      ai_pTarget = 0;
  if(!ai_pTarget)
    ai_pTarget=FindObject2(Sort_Random(),Find_Hostile(GetOwner()),Find_OCF(OCF_Alive),Find_NoContainer(),Find_Or(Find_Distance(75),Find_InRect(0,-50,-200+400*GetDir(),100)));

  // Gegner folgen
  if(ai_pTarget)
  {
    if(!GetCommand())
      if(!Random(10))
      {
        if(!Contained(ai_pTarget))
          SetCommand(this,"Attack",ai_pTarget);//SetCommand(this,"MoveTo",0,GetX(ai_pTarget),GetX(ai_pTarget));//
        else
          SetCommand(this,"Enter",Contained(ai_pTarget));
      }
  }
  // Kein Gegner?
  else
  // Leichen beleben
    if(!Random(10))
    {
      var clr;
      if(temp=FindObject2(Find_Or(Find_And(Find_Action("Dead"),Find_Not(Find_OCF(OCF_Alive))),Find_ID(CORP)),Find_Distance(20)))
      {
        clr=GetColorDw(temp);
        SetOwner(GetOwner(),temp);
        temp->SetColorDw(clr);
        if(GetID(temp)==CORP)
        {
          temp->DoEnergy(100);
          temp->~Resuscitate();
          temp->DoEnergy(100);
          SetPosition(GetX(temp),GetY(temp)-10,temp);
          if(GetID(temp)==CLN2)
            temp->Redefine(CLNK);
        }
        temp->~AddWCAI();
      }
    }
  // Kein Befehl?
  if(!GetCommand())
  {
    if((ai_pTarget&&!Random(10))||!Random(30))
    {
      if(Random(2))
        SetComDir(COMD_Left);
      else
        SetComDir(COMD_Right);
    }
    else if(!Random(30))
      SetComDir(COMD_None);
  }
  // Kämpft? Gegner wegschleudern...
  if(GetProcedure()=="FIGHT")
    if(!Random(7))
    {
      // Gegner neu setzen
      ai_pTarget=GetActionTarget();
      Fling(ai_pTarget,RandomX(-5,5),-Random(5));
      DoEnergy(-Random(GetCon()/15),ai_pTarget);
      SetAction("Walk");
    }
  if(!GetCommand())
    if(!ai_pTarget)
      if(!Random(30))
        SetComDir(COMD_None);
  
  // Manchmal schnellen Objekten ausweichen
  if(Random(3))
  {
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
  }
  // Sonne? Verbrennen...
  if(GBackSky()&&IsDay())
  {
    DoEnergy(-2);
    if(!Random(50))
      Incinerate();
    // Aus der Sonne gehen (bzw. eher irre werden)
    if(!Random(10))
    {
      if(GetComDir()==COMD_Right)
        SetComDir(COMD_Left);
      else
        SetComDir(COMD_Right);
      if(!Random(3))
        AddCommand(this,"Jump");
    }
  }
  // Auftauchen, wenn der Atem ausgeht
  if(InLiquid() && GetBreath() <= 25 && GetComDir() != COMD_Up)
  {
    SetComDir(COMD_Up);
    SetCommand(this,"None");
    DoBreath(1);
  }
  // Steckt fest?
  if(Stuck())
  {
    SetAction("Dig");
    if(Random(2))
      SetComDir(COMD_Left);
    else
      SetComDir(COMD_Right);
  }
  else
    if(GetAction()=="Dig")
      SetAction("Walk");
  // Fürs nächste mal speichern
  EffectVar(1,pTar,iEffNum)=ai_pTarget;
  return 0;
}
 
global func Hits(iX,iY)
{
  return Abs(GetXDir()*2/3)>Abs(GetX()-iX)&&Abs(GetYDir()*2/3)>Abs(GetY()-iY);
}
