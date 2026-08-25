#strict

func Initialize() 
{
  SetGamma(RGB(10,9,1), RGB(161,157,129), RGB(255,254,210));
  var lorry = CreateObject(LORY,660,340,NO_OWNER);
  Fill(lorry);
}

func InitializePlayer(iPlr) 
{
  if (!ObjectCount(FLAG)) CreateContents(FLAG,GetCrew(iPlr,0));

  var pClnk;   
  for(var pClnk;pClnk=FindObjectOwner(0,iPlr,0,0,0,0,OCF_CrewMember(),0,0,pClnk);)
    {
    pClnk->Initialize();
    
    SetPosition(570+Random(180),340, pClnk);
    }
  }
}

public func Fill(object pOfObj)
{
  if (!pOfObj) return();
  Create(CNKT,1,pOfObj);
  Create(BARL,2,pOfObj);
  return(1);
}

private func Create(id idType, int iCount, object pInObj)
{
 var i=iCount-ContentsCount(idType,pInObj);
 while (i-- > 0) { CreateContents(idType,pInObj); }
 return(1);
}