/*-- Modernes Gefecht --*/

#strict

static aRelaunches;
static aMagiTecRel;
static fStarted;

func Initialize()
{
  aRelaunches=CreateArray();
  aMagiTecRel=CreateArray();
  ScriptGo(true);
  LocalN("phase",FindObject(SMON))=0;
  FindObject(SMON)->ResetPhase();
  FindObject(TIME)->SetTime(30);
  SetSkyParallax(true);
}

func InitializePlayer(iPlr)
{
  fStarted=false;
  SetTechStage(iPlr,RdW_TL_Modern);
  aRelaunches[iPlr]=5;
  aMagiTecRel[iPlr]=1+Random(aRelaunches[iPlr]-1);
  RemoveObject(GetCrew(iPlr));
  RelaunchPlr(iPlr,true);
  fStarted=true;
  return(1);
}

func RelaunchPlayer(iPlr)
{
  if(fStarted)
    RelaunchPlr(iPlr,false);
}

func Script150()
{
  var iRand;
  var pCrate;
  for(var i; i<GetPlayerCount(); i++)
  {
    if(Random(5))
      pCrate=CreateObject(CRAT,RandomX(10,LandscapeWidth()-10),0,-1);
    else
    {
      pCrate=CreateObject(CANN,RandomX(10,LandscapeWidth()-10),0,-1);
      CreateContents(FLNT,pCrate,Random(5));
      CreateContents(GUNP,pCrate,RandomX(3,10));
      continue;
    }
    iRand=Random(6);
    if(!pCrate){Log("Crate-ERROR"); continue;}
    if(!iRand--)
      CreateContents(AMBO,pCrate,RandomX(2,3));
    else if(!iRand--)
      CreateContents(STGR,pCrate,RandomX(2,3));
    else if(!iRand--)
      CreateContents(OBRL,pCrate,2);
    else if(!iRand--)
      CreateContents(ADM3,pCrate,Random(3));
    else if(!iRand--)
      CreateContents(AIRS,pCrate,1);
    else
    { 
      ChangeDef(LORY,pCrate);
      if(Random(3))
        CreateContents(FLNT,pCrate,Random(3));
      if(Random(2))
        CreateContents(TFLN,pCrate,Random(3));
      if(!Random(3))
        CreateContents(SFLN,pCrate,Random(3));
      if(!Random(4))
        CreateContents(STFN,pCrate,Random(3));
      if(!Random(4))
        CreateContents(FBMP,pCrate,1);
      if(!Random(8))
        CreateContents(EFLN,pCrate,1);
    }
    iRand=Random(8);
    if(!iRand--)
      CreateContents(AMBO,pCrate,RandomX(1,2));
    else if(!iRand--)
      CreateContents(METL,pCrate,1);
    else if(!iRand--)
      CreateContents(CNKT,pCrate,1);
    else if(!iRand--)
      CreateContents(BRED,pCrate,1);
    else if(!iRand--)
      CreateContents(_Y7B,pCrate,1);
    else if(!iRand--)
    {
      if(Random(2))
        CreateContents(FARP,pCrate,1);
      else
        CreateContents(XARP,pCrate,1);
    }
    else if(!iRand--)
    {
      if(Random(2))
        CreateContents(_Y8B,pCrate,1);
      else
        CreateContents(SPGS,pCrate,1);
    }
    else
    { 
      ChangeDef(LORY,pCrate);
      if(Random(3))
        CreateContents(FLNT,pCrate,Random(3));
      if(Random(2))
        CreateContents(TFLN,pCrate,Random(3));
      if(!Random(3))
        CreateContents(SFLN,pCrate,Random(3));
      if(!Random(4))
        CreateContents(STFN,pCrate,Random(3));
      if(!Random(4))
        CreateContents(FBMP,pCrate,1);
      if(!Random(8))
        CreateContents(EFLN,pCrate,1);
    }
  }
  goto(50);
}

global func RelaunchPlr(iPlr,fSilent)
{
  var pSoldier,fMTC,iX;
  aRelaunches[iPlr]--;
  if(aRelaunches[iPlr]<=0)
    return Log("%s hat alle seine Tickets verbraucht.",GetTaggedPlayerName(iPlr));
  if(aRelaunches[iPlr]==aMagiTecRel[iPlr])
    fMTC=true;
  iX=BoundBy((GetPlayerTeam(iPlr)-1)*LandscapeWidth()+RandomX(-150,150),0,LandscapeWidth());
  pSoldier=CreateObject(CLN2,iX,GetHorizonHeight(iX),iPlr);
  MakeCrewMember(pSoldier,iPlr);
  pSoldier->~MakeSoldier();
  SetCursor(iPlr,pSoldier);
  // MagiTec-Ausrüstung
  if(fMTC)
  {
    var pMagi;
    pMagi=CreateObject(_MTC,iX,GetHorizonHeight(iX),iPlr);
    pSoldier->Enter(pMagi);
    CreateContents(SFLN,pSoldier,2);
    CreateContents(FLNT,pSoldier,5);
    CreateContents(ROCK,pSoldier,3);
    CreateContents(STGR,pSoldier,1);
    if(!fSilent)
      Log("%s rückt mit einem MagiTek an und hat noch <c ffff00>%d</c> Tickets.",GetTaggedPlayerName(iPlr),aRelaunches[iPlr]-1);
  }
  // Soldaten-Ausrüstung
  else
  {
    CreateContents(GUN_,pSoldier);
    CreateContents(AMBO,pSoldier);
    CreateContents(SFLN,pSoldier);
    CreateContents(STGR,pSoldier);
    if(!fSilent)
      Log("%s schickt einen neuen Soldaten und hat noch <c ffff00>%d</c> Tickets.",GetTaggedPlayerName(iPlr),aRelaunches[iPlr]-1);
  }
}

global func GetHorizonHeight(int iX)
{
  var iY=0;
  while(iY<LandscapeHeight()&& !GBackSemiSolid(iX,iY))
    iY+=10;
  return(iY);
}
