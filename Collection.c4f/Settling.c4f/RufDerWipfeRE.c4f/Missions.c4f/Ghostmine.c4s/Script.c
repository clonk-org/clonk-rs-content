/*-- Geistermine --*/

#strict

static const AI_Player=1;

func InitializePlayer(iPlr)
{
  // Mit den "Zombies" verfeinden
  SetHostility(iPlr,AI_Player,1,true,true);
  SetHostility(AI_Player,iPlr,1,true,true);
  return(1);
}

public func Initialize()
{
  CreateObject(_Z9A,620,920,-1)->Set(1);
  CreateObject(_Z9A,660,1200,-1)->Set(2);
  CreateObject(_Z9A,1000,340,-1)->Set(3);
  for(var i; i<10; i++)
    PlaceZombie();
}

public func ChooserFinished()
{
  var i=GetDifficulty()*4;
  if(!i)
    i=8;
  for(; i>0; i--)
    PlaceZombie();
}

public func PlaceZombie()
{
  var pZombie;
  pZombie=PlaceAnimal(ZOMB);
  while(GetY(pZombie)<950)
  {
    RemoveObject(pZombie);
    pZombie=PlaceAnimal(ZOMB);
  }
  SetOwner(AI_Player,pZombie);
  pZombie->AddWCAI();
  SetColor(Random(12),pZombie);
}

func LocalPlaceName(para)
{
  if(para==1) return("Verlassene Mine"); 
  if(para==2) return("Untere Stollen");
  if(para==3) return("Felsenberg");
  return("Komisches Unbekanntes Ding");
}

public func IsCampaignMission()
{
  return true;
}

func ForcedTechLevel()
{
  return RdW_TL_Modern;
}
