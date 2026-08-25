#strict

func Initialize() 
{
  SetGamma(RGB(0,0,0),RGB(50,90,70),RGB(100,150,130));
  CastObjects(WOOD,4,0,1040,670);
  CreateObject(SLBT,1110,660,NO_OWNER);
}

func InitializePlayer(iPlr) 
{
  if (!ObjectCount(FLAG)) CreateContents(FLAG,GetCrew(iPlr,0));
  if (!ObjectCount(CNKT)) CreateContents(CNKT,GetCrew(iPlr,1));

  var pClnk;   
  for(var pClnk;pClnk=FindObjectOwner(0,iPlr,0,0,0,0,OCF_CrewMember(),0,0,pClnk);)
    {
    pClnk->Initialize();
    
    SetPosition(900+Random(200),660, pClnk);
    }
  }
}