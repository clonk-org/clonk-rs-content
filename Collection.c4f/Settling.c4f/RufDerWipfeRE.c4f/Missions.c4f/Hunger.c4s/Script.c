/*-- Hungersnot --*/

#strict

static pGoal;

public func Initialize()
{
  pGoal=FindObject2(Find_ID(CROB));
  if(!pGoal) pGoal=CreateObject(CROB,0,0,-1);
  pGoal->AddType(BRED, 100);
}

public func ChooserFinished()
{
  pGoal->ResetTypes();
  pGoal->AddType(BRED, 25+GetDifficulty()*5);
  ScriptGo(true);
}

func Script300()
{
  goto(BoundBy(RandomX(GetDifficulty()*20,GetDifficulty()*40),1,150));
  if(!Random(10))
  {
    var iRand;
    iRand=Random(4);
    if(!iRand--)
    {
      Log("Der Goldwipf meint das ihr das Gold nicht verdient habt und verwandelt alle eure Goldklumpen in Steine");
      for(var pObj in FindObjects(Find_ID(GOLD)))
        pObj->ChangeDef(ROCK);
      return 1;
    }
    if(!iRand--)
    {
      Log("Die Schattenschlange ist der Grund warum ihr Nachts nicht schlafen könnt! Sie hat einen Geist zu euch geschickt!");
      PlaceAnimal(_Z3A);
      return 1;
    }
    if(!iRand--)
    {
      Log("Das Magmamonster schickt euch neue Spielkameraden. Die Feuermonster haben jedenfalls ihren Spaß...");
      CreateObject(FMEG,Random(LandscapeWidth()),0,-1);
      if(Random(3))
        CreateObject(FMEG,Random(LandscapeWidth()),0,-1);
      return 1;
    }
    if(!iRand--)
    {
      iRand=Random(3);
      if(!iRand--)
      {
        Log("Der Goldwipf mag euch garnicht und sorgt für unstillbaren Hunger...");
        for(var pObj in FindObjects(Find_ID(CLN2)))
        {
          ScheduleCall(pObj,"CheckFood",RandomX(5,6),RandomX(100,200),-1);
          ScheduleCall(pObj,"CheckFood",RandomX(3,4),RandomX(35,45),-1);
        }
      }
      if(!iRand--)
      {
        Log("Die Schattenschlange hasst euch und lässt ein paar eurer Clonks irre werden!");
        for(var pObj in FindObjects(Find_ID(CLN2)))
          if(!Random(2))
            pObj->CheckPsyche(RandomX(-1500,-1));
      }
      if(!iRand--)
      {
        Log("Ihr habt das Magmamonster erzürnt! Vor Wut wirft er nun mit Felsbrocken um sich...");
        CreateObject(METO,Random(LandscapeWidth()),0,-1);
        return 1;
      }
    }
  }
}

public func IsCampaignMission()
{
  return true;
}

func ForcedTechLevel()
{
  return RdW_TL_Knight;
}
