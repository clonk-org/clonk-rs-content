#strict

func Initialize() 
{
  var goal = CreateObject(CROB, 50,50,-1);
  if (goal)
  {
    goal->AddType(BLMP, 1);
  }

  SetSkyParallax (0,17,19,0,0,0,0); 
  SetGamma(RGB(15,15,15),RGB(118,118,118),RGB(215,215,215));

  var lorry = CreateObject(LORY,900,875,NO_OWNER);
  Fill(lorry);

  for(var i=0;i<180;++i) 
    CreateParticle("Fog",Random(LandscapeWidth()),Random(LandscapeHeight()),0,0,RandomX(900,1700));
}

func InitializePlayer(iPlr) 
{
  if (!ObjectCount(FLAG)) CreateContents(FLAG,GetCrew(iPlr,0));
  SetFoW(1,iPlr);

  var pClnk;   
  for(var pClnk;pClnk=FindObjectOwner(0,iPlr,0,0,0,0,OCF_CrewMember(),0,0,pClnk);)
    {
    pClnk->Initialize();
    
    SetPosition(900+Random(15),875, pClnk);
    }
  }
}

public func Fill(object pOfObj)
{
  if (!pOfObj) return();
  Create(LOAM,5,pOfObj);
  Create(WOOD,5,pOfObj);
  Create(ROCK,5,pOfObj);
  Create(METL,3,pOfObj);
  Create(CNKT,1,pOfObj);
  return(1);
}

private func Create(id idType, int iCount, object pInObj)
{
 var i=iCount-ContentsCount(idType,pInObj);
 while (i-- > 0) { CreateContents(idType,pInObj); }
 return(1);
}