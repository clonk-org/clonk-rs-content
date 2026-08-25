/*-- Spawnpoint --*/

#strict

local initialized, angle, spawntimer, nextobj, timer;

static BonusList;

global func PlaceBonusSpawnpoint(int iX, int iY, int timer)
{
  var spwn = CreateObject(BSPN, iX, iY, -1);
  if(timer)
  {
    spwn->LocalN("spawntimer") = timer;
    spwn->LocalN("timer") = timer;
  }
  return(spwn);
}


/* Sonstiges */

protected func Initialize()
{
  // Standardtimer = 2500 Frames
  spawntimer = 2500;
  timer = 2500;
  SetClrModulation(RGBa(0,0,0,255));
  
  // keine Bonusliste gesetzt?
  if(!BonusList)
  {
  	// generieren.
  	BonusList = CreateArray();
  	var def;
  	for(var i = 0; def = GetDefinition(i); i++)
  		if(def->~IsBonus())
  			if(WildcardMatch(Format("%i",def),"QB??"))
  				BonusList[GetLength(BonusList)] = def; 
  }
}

protected func Timer()
{
  // Tolle Effekt starten
  angle += 10;
  if(angle >= 360) angle = 0;
  SetObjDrawTransform(1000, 0,0,0, 1000, Sin(angle, 8)*1000 - 2000,0, 1);
  if(Random(2) && Local()) CreateParticle("NoGravSpark", RandomX(-5,5), RandomX(5,10), 0, -5, 25, nextobj->~Color());
  // Timer runterzählen
  if(timer > 0) DecreaseTimer();
}

/* Timer runterzählen */

private func DecreaseTimer()
{
  // Timer runterzählen
  timer -= 5;
  if(timer <= 0)
  {
    timer = 0;
    // gibt es bereits einen aktiven Bonusspawnpunkt?
  	if(ObjectCount2(Find_ID(GetID()), Find_Func("IsReady")))
  		return();
  		
  	var bonuspoint = FindObject2(Find_ID(GetID()), Sort_Random());
  	bonuspoint->SetActive();
  	Sound("Extra",0,bonuspoint);
  }
}

public func RejectEntrance(object pClonk)
{
  // Objekt ist ein Clonk?
  if(!(GetOCF(pClonk) & OCF_CrewMember)) return(1);
  // Darf einsammeln
  if(Local())
  {
  	// hat der schon einen Bonus? :O
  	if(GetEffect("*Bonus",pClonk))
  		return(1);
    if(Contents()->~Activate(pClonk)) {
      //Sound("Grab", 0, pClonk, 0, GetOwner(pClonk)+1);
      for(var obj in FindObjects(Find_ID(GetID())))
  			obj->Collected(GetOwner(pClonk));
    }
  }
  return(1);
}

private func Collected(int iPlr) // Regelt, dass ein Spieler das Objekt einsammelt
{
  // Timer hochsetzen
  timer = spawntimer;
  // Nicht mehr einsammelbar
  Local() = false;
  // Nicht mehr sichtbar
  SetVisibility(VIS_God());
}

private func GetRandomBonus()
{
	if(!GetLength(BonusList))
		return(timer = 100);

  // nextobj belegen
  nextobj = BonusList[Random(GetLength(BonusList))];
  CreateContents(nextobj);
  SetGraphics(0, 0, 0, 1, 5,0,0,Contents());
}

func IsReady() { return(Local()); }

func SetActive()
{
	Local() = true;
	SetVisibility(VIS_All());
	GetRandomBonus();
}
