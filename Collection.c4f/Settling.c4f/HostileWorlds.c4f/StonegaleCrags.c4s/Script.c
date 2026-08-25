#strict

func Initialize() 
{
  SetGamma(RGB(0,0,0),RGB(110,105,105),RGB(210,200,200));
  var bridge = CreateObject(BRDG,1370,790,NO_OWNER);
  bridge -> Release(); 
  bridge = CreateObject(BRDG,240,790,NO_OWNER);
  bridge -> Release(); 
  CastObjects(BONE,4,5,510,550);
  var lorry = CreateObject(LORY,1710,90,NO_OWNER);
  Fill(lorry);
  lorry = CreateObject(LORY,510,570,NO_OWNER);
  Fill2(lorry);
}

func InitializePlayer(iPlr) 
{
  if (!ObjectCount(FLAG)) CreateContents(FLAG,GetCrew(iPlr,0));
  SetFoW(1,iPlr);

  var pClnk;   
  for(var pClnk;pClnk=FindObjectOwner(0,iPlr,0,0,0,0,OCF_CrewMember(),0,0,pClnk);)
    {
    pClnk->Initialize();
    
    SetPosition(1710+Random(15),90, pClnk);
    }
  }
}

public func Fill(object pOfObj)
{
  if (!pOfObj) return();
  Create(LOAM,7,pOfObj);
  Create(METL,7,pOfObj);
  Create(WOOD,7,pOfObj);
  Create(ROCK,7,pOfObj);
  Create(_TRC,3,pOfObj);
  Create(CNKT,3,pOfObj);
  return(1);
}

public func Fill2(object pOfObj)
{
  if (!pOfObj) return();
  Create(METL,3,pOfObj);
  Create(FLNT,3,pOfObj);
  Create(TFLN,3,pOfObj);
  Create(_TRC,1,pOfObj);
  return(1);
}

private func Create(id idType, int iCount, object pInObj)
{
 var i=iCount-ContentsCount(idType,pInObj);
 while (i-- > 0) { CreateContents(idType,pInObj); }
 return(1);
}

public func VolcanoMaterial() { return(Material("DuroLava")); }