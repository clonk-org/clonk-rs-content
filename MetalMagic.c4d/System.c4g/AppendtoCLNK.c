/*-- AppendtoCLNK --*/

#strict 2
#appendto CLNK

/* Einsammeln */

public func RejectCollect(id idObject, object pObject)
{
  // Objekt kann gepackt werden
  // automatisches Packen aber nur wenn die Paktteile nicht extra gezählt werden
  if(!IsSpecialItem(pObject)) if(pObject->~JoinPack(this)) return 1;
    
  // Objektaufnahme mit Limit verhindern, wenn bereits genug getragen
  if(!IsSpecialItem(pObject)) if(pObject->~CarryLimit() && ContentsCount(idObject) >= pObject->~CarryLimit() ) return 1;
    
  // Spezialitem?
  var i, iCount;
  if(i = IsSpecialItem(pObject))
  {
    // Noch genug Platz für das ganze Packet?
    if(GetSpecialCount(GetMaxSpecialCount(i-1))+Max(pObject->~PackCount(),1)<=GetMaxSpecialCount(i-1, 1)) return 0;
    iCount = GetMaxSpecialCount(i-1, 1)-GetSpecialCount(GetMaxSpecialCount(i-1));
    // Ansonten so viel wie geht rein
    if(pObject->~SplitPack(pObject->~PackCount()-iCount)) return 0;
    else return 1;
  }
  
  return GetNonSpecialCount()>=MaxContentsCount();
}

protected func Initialize()
{
  // Lebenseffekt anhängen, für spätere Untotenbelebung
  AddEffect("IntRevive", this, 1, 0, this);
  return _inherited();
}

// im ritterappendto kopiert..
protected func RejectFight(object pEnemy)
{
  // Wenn beide Reiten nicht runterwerfen
  if(IsRiding() && pEnemy->~IsRiding())
    return 1;
    
  // Auch nicht, wenn ein entsprechender Effekt auf dem Clonk liegt
  if(GetEffect("*RejectingFights*", this)) return true;
  
  return _inherited(pEnemy,...);
}

protected func ControlDigDouble()
{
  // Normale Behandlung hat Vorrang
  if(_inherited()) return 1;
  // Activieren auf dem Pferd
  if (IsRiding()) if(Contents()) return Contents()->~Activate(this);
  // Keine überladene Steuerung
  return 0;
}

protected func ControlThrow()
{
  // Reiten und Fallenlassen
  if (IsRiding())
    if (Contents(0) && GetPlrDownDouble(GetOwner()))
    {
      Exit(Contents(0), 0, 10);
      return 1;
    }
  // Keine überladene Steuerung
  return _inherited();
}

private func Control2Effect(string szControl)
  {
  // Von Effektzahl abwärts zählen
  var i = GetEffectCount(0, this()), iEffect;
  var res;
  while (i--)
    {
    // Effekte mit Control im Namen benachrichtigen	  
    iEffect = GetEffect("*Control*", this(), i);
    //  Message("%s", this(), GetEffect(0, this(), iEffect, 1));
    if ( GetEffect(0, this(), iEffect, 1) )
      res += EffectCall(this(), iEffect, szControl, ...);
    }
  return(res);
  }

public func ContextCallGriffon()
{
  [$CtxGriffonCall$|Image=GRFN|Condition=HasGriffon]
  return CallFlyAnimal(GRFN);
}

public func ContextCallSaurian()
{
  [$CtxSaurianCall$|Image=SARN|Condition=HasSaurian]
  return CallFlyAnimal(SARN);
}

private func HasGriffon() { return FindFlyAnimal(GRFN); }
private func HasSaurian() { return FindFlyAnimal(SARN); }

private func FindFlyAnimal(idID)
{
  // Tier suchen
  for(var pObj in FindObjects(Find_ID(idID), Find_Allied(GetOwner())))
  {
    // Reitet hier schon jemand?
    if(pObj->~GetRider())
      continue;
    return pObj;
  }
}

// Flugtier rufen
// aus Drachenruf adaptiert
private func CallFlyAnimal(idID)
{
  var pAnimal = FindFlyAnimal(idID);
  if(!pAnimal) return;
  // Kontrolle übernehmen
  pAnimal->~ResetControl();
  // Das Tier rufen
  if (!pAnimal->SetCommand(pAnimal, "Follow", this))
  {
    // Nicht möglich: Dann hat das Tier hier wohl kein Platz.
    Message("$MsgNoPlace$", this, GetName(0, idID));
    Sound("Error");
    return false;
  }
  // Hörbare Reaktion vom Tier
  pAnimal->Sound("DragonGrowl");
  // Effekte
  CallEffects(0, 0, pAnimal->GetX()-GetX()+pAnimal->GetVertex(0,0), pAnimal->GetY()-GetY()+pAnimal->GetVertex(0,1)+20);
  // Erfolg
  return true;
}

private func CallEffects(x1, y1, x2, y2, color1, color2) 
{
  CastParticles("PxSpark", 10, 30, x2, y2, 5*2, 5*10, RGB(196, 196, 196), RGB(255, 255, 255));
  for (var i = 0; i < 5; i++)
    DrawParticleLine("PSpark", x1+Random(20)-10,y1+Random(20)-10, x2+Random(20)-10,y2+Random(20)-10, 10+Random(20),50, RGBa(255,50+Random(100),50+Random(100),50), RGBa(50+Random(100),255,50+Random(100),100), -Random(3));
  return 1;
}

func ControlDown()
{

  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlDown")) return(1);
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlDown(this()));
  
  if(!this->~IsKnight())
  if(GetProcedure() == "FIGHT")
  {
  	if(GetEffect("*ForcesToFightWith*", GetActionTarget(0)))
  	{
  		return true;
  	}
  }
  // Keine überladene Steuerung
  return(0);
}
