
#strict


#include WTWR
#include BAS4

local timestayed;

protected func Initialize()
{
	timestayed = [0,0];
	return(_inherited());
}

public func IsHealersHouse() { return(0); }

protected func Ejection(pObj)
{
	if((GetID(pObj) != CLNK) && (GetID(pObj) != LORY))
		Enter(this,pObj);
	return();
}

protected func ActivateEntrance(pByObj)
{
	if(GetID(pByObj) == CLNK)
		if(FindContents(CLNK) && (FindContents(CLNK) != pByObj))
			if(GetAction(FindContents(CLNK)) ne "Dead")
				return(0);
	if(GetID(pByObj) == LORY)
		return(0);
 	if (ActIdle()) 
		if(GetID(pByObj) == CLNK)
			SetAction("OpenDoor");
  	return(1);
}

protected func CheckPlayer()
{
	DoMagicEnergy(+1);
	if (FindContents(LORY))
		Exit(FindContents(LORY));
	var aliveClonks = FindObjects(Find_OCF(OCF_Alive),Find_ID(CLNK),Find_Container(this));
	if (GetLength(aliveClonks) > 1)
		for(var x in aliveClonks)
			Exit(x);
	var clonk, highnr, owner;
	clonk = FindObject2(Find_OCF(OCF_Alive),Find_ID(CLNK),Find_Container(this));
	if (clonk)
	{
		owner = GetOwner(clonk);
		timestayed[owner] += 1;
		SetScoreboardData((owner+1),2, Format("%d", timestayed[owner] ,0));
		Victory();
	}
	else
	{
		if (!Random(300))
			Teleport();
	}
}

public func Teleport()
{
	if (FindContents(CLNK))
		Exit(FindContents(CLNK));
	for (var x = 0; x < ContentsCount(); x++)
	{
		RemoveObject(Contents(0));
	}
	CastObjects(MSTB,10,25, GetX(target)-GetX(), GetY(target)-GetY());
  	CastParticles("MSpark", 200, 50, GetX(target)-GetX(),GetY(target)-GetY(), 10, 50, RGBa(255,100,100,128), RGBa(255,0,0,0));
	Sound("Plopp");

	SetPosition(Random(LandscapeWidth()-40)+20,0);
}

public func Victory()
{
	var win, winner;
	win = 0;
	winner;
	for(var x = 0; x < GetPlayerCount(); x++) 
	{
		var player, playsco;
		player = GetPlayerByIndex(x);
		playsco = timestayed[player];
		if (playsco > win) 
		{
			win = playsco;
			winner = player;
		}
	}
	if ( win > 300 )
	{
		for(var x = 0; x < GetPlayerCount(); x++) 
			if (x != winner)
				EliminatePlayer(x);
	}
}

public func ChangeScore(scoreChange,target)
{
    if (scoreChange < 0)
    {
    	if ((timestayed[target]) > (Abs(scoreChange)))
	    timestayed[target] += scoreChange;
        else
	    timestayed[target] = 0;
    }
    else
	timestayed[target] += scoreChange;
    SetScoreboardData((target+1),2, Format("%d", timestayed[target] ,0));

}

public func DoMagicCall(object caster, id spell_id, int direction)
{
  DoMagic(spell_id, caster, direction);
}
 	 
protected func DoMagic(id spell_id, object caster, int direction)
{
  if (!caster) return();

  // Zaubernden Clonk in den Turm bewegen, wenn er nicht drin ist
  if (caster->Contained() != this())
  {
    SetCommand(caster, "None");
    AddCommand(caster, "Call", this(), spell_id, direction, 0,0, "DoMagicCall");
    AddCommand(caster, "Enter", this());
    return(1);
  }
  if (GetAction() ne "Idle")
  {
    SetCommand(caster, "None");
    AddCommand(caster, "Call", this(), spell_id, direction, 0,0, "DoMagicCall");
    AddCommand(caster, "Wait", 0,0,0,0,0, 10);
    return(1);
  }
  if (direction > 0)
    AimRight(caster);
  else
    AimLeft(caster);
  
  // Nicht genug Zauberenergie
  if (!CheckMana(spell_id))
  {
    Sound("Error");
    Message("$TxtNotEnoughEnergy$", this());
    return();
  }
  
  // Zauberobjekt erschaffen und aktivieren
  if (CreateObject(spell_id, 0, 0, GetController(caster))->~Activate(this(), caster)) 
  {
    // Energie abziehen
    DoMagicEnergy(-Value(spell_id));
      
    // Aktion setzen
    SetAction("Magic");
  }
  // Zauberobjekterschaffung fehlgeschlagen
  else 
  {
    SetAction("Idle");
    Sound("Error");
    return(0);
  }
  
  // Fertig
  return(1);
}

public func SelectSpell(object caller, int direction)
{
  // Forschung abbrechen
  CancelResearch();
  // Menü erzeugen und mit den Zaubersprüchen des Turmbesitzers füllen
  // Gehört der Turm keinem gülötigen Besitzer, stattdessen Zauber des Clonks nehmen
  OpenEmptySpellMenu(MCMS, caller, this, this, "$TxtNoSpells$");
  var spell_num;
  var spell_id;
  var iPlr = GetOwner(); if (!GetPlayerName(iPlr)) iPlr = GetOwner(caller);
  while (spell_id = GetPlrMagic(iPlr, 0, spell_num++)) 
    if(!spell_id->~NoTowerMagic() )
      {
      //AddMenuItem("$TxtDoMagic$", "DoMagic", spell_id, caller, 0, caller);
      var strCommand = Format("Object(%d)->DoMagic(%i,Object(%d),%d)", ObjectNumber(this()), spell_id, ObjectNumber(caller), direction);
      AddMenuItem("$TxtDoMagic$", strCommand, spell_id, caller, 0, caller);
      }
  return(1);
}

public func AimLeft() 
{
  if (GetDir() == DIR_Left()) return(0);
  SetDir(DIR_Left());
  Sound("Lever1");
  return(1);
}

public func AimRight() 
{
  if (GetDir() == DIR_Right()) return(0);
  SetDir(DIR_Right());
  Sound("Lever1");
  return(1);
}

public func ContainedLeft(object pCaller) 
{
  [$TxtAimspellsleft$]
  CancelResearch();
  AimLeft(pCaller);
  SelectSpell(pCaller, 0);
  return(1);
}

public func ContainedRight(object pCaller) 
{
  [$TxtAimspellsright$]
  CancelResearch();
  AimRight(pCaller);
  SelectSpell(pCaller, 1);
  return(1);
}

public func ContextMagicLeft(object caller)
{
  [$TxtAimspellsleft$|Image=MCMS|Condition=CanMagic]
  SelectSpell(caller, 0);
}

public func ContextMagicRight(object caller)
{
  [$TxtAimspellsright$|Image=MCMS|Condition=CanMagic]
  SelectSpell(caller, 1);
}

public func CanMagic(object caller)
{
  var iPlr = GetOwner(); 
  if (!GetPlayerName(iPlr)) iPlr = caller->GetOwner();
  return(GetPlrMagic(caller->GetOwner()) != 0);
}