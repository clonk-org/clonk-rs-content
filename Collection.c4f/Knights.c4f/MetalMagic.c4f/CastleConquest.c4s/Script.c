/*-- Castle conquest --*/

#strict

static attackers_base;
static attackers_base2;
static deffers_base;
static first_camp_left,first_camp_right;
static first_castle;

func Initialize() {
  attackers_base=FindObject(FLGB,357,281,-1,-1);
  attackers_base2=FindObject(FLGB,1419,280,-1,-1);
  deffers_base=FindObject(FLGB,888,66,-1,-1); 
  
  RemoveAll(FLGP);RemoveAll(POS1);
  CreateObject(FLGP,0,0,-1)->Enter(attackers_base);
  CreateObject(FLGP,0,0,-1)->Enter(attackers_base2);
  
  RemoveAll(CLK_);CreateObject(CLK_,0,0,-1);
  
  DigCastlesFree();
  
  //fahrstühle der lager frei buddeln
  var aElevator = FindObjects(Find_ID(ELEV),Find_Or(Find_InRect(LandscapeWidth()-300,0,300,LandscapeHeight()),
                                                    Find_InRect(0,0,200,LandscapeHeight())));
  for(var pElev in aElevator)
  {
    var x = GetX(pElev)-GetDefCoreVal("Width", "DefCore", GetID(pElev))/2;
    var y = GetY(pElev)-GetDefCoreVal("Height", "DefCore", GetID(pElev))/2;
    DigFreeRect(x,y,GetDefCoreVal("Width", "DefCore", GetID(pElev)),LandscapeHeight()-100);
  }
  
  for(var cnt=0;cnt<30;cnt++)PlaceInMaterial(GOLD,Material("Earth"),0);
  for(var cnt=0;cnt<30;cnt++)PlaceInMaterial(FLNT,Material("Earth"),0);
  for(var cnt=0;cnt<20;cnt++)PlaceInMaterial(ROCK,Material("Earth"),0);
  for(var cnt=0;cnt<20;cnt++)PlaceInMaterial(BONE,Material("Earth"),0);
  for(var cnt=0;cnt<30;cnt++)PlaceInMaterial(LOAM,Material("Earth"),0);
  
  if(!ObjectCount(MELE))CreateObject(MELE,10,10,-1);
  if(!FindObject(FSTF)) CreateObject(FSTF,10,10,-1); // Stäbe für Magi
  ScriptGo(1);
}

protected func DigCastlesFree()
{
 for(var obj;obj=FindObject(0,0,0,0,0,0,0,0,NoContainer(),obj);)
 {
  if(!obj->~CastlePartWidth())continue;
  var x=GetX(obj)+GetDefCoreVal("Offset","DefCore",GetID(obj),0);
  var y=GetY(obj)+GetDefCoreVal("Offset","DefCore",GetID(obj),1);
  var w=GetDefCoreVal("Width","DefCore",GetID(obj),0);
  var h=GetDefCoreVal("Height","DefCore",GetID(obj),0);
  DigFreeRect(x,y,w,h);
 }
}

protected func Reinforcement()
{
  var x=825;
  var y=36;
 
  var owners=[];
  for(var cnt=0;cnt<GetPlayerCount();cnt++)
  {
    var plr=GetPlayerByIndex(cnt);
    if(Global(plr) == 1)PushBack(plr,owners);
  }
  if(!GetLength(owners))return();
 
  // Verstärkung kommt!
  for(var cnt=0;cnt<100;cnt++)
  CreateParticle("Smoke",x+Random(100),y+Random(10),0,0,300,RGB(255,255,255));
 
  // Erstes Flugfass
  var flvt=CreateObject(FLVT,x,0,owners[0]);
  flvt->SetAction("Fly");
  flvt->SetComDir(COMD_Down());
 
  for(var iPlr in owners)
  {
    var pldn=CreateObject(PLDN,x,y,iPlr);
    MakeCrewMember(pldn,iPlr);
    Enter(flvt,pldn);
    GivePalaStuff(pldn);
    pldn->DoEnergy(100);
  }
  CreateContents(CATA,flvt);
 
  for(var cnt=0;cnt<20;cnt++)
    CreateContents(RandomID(),flvt);
 
  // Zweites Flugfass
  var flvt=CreateObject(FLVT,x+110,0,owners[0]);
  flvt->SetAction("Fly");
  flvt->SetComDir(COMD_Down());
 
  for(var iPlr in owners)
  {
    for(var cnt=0;cnt<2;cnt++)
    {
      var knig=CreateObject(PLDN,x+110,y,iPlr);
      MakeCrewMember(knig,iPlr);
      Enter(flvt,knig);
      CreateContents(EFLN,knig);
      CreateContents(XARP,knig);
      CreateContents(BOW1,knig);
      CreateContents(WBRL,knig);
      knig->DoEnergy(100);
    }
  }

  for(var cnt=0;cnt<10;cnt++)
    CreateContents(RandomID(),flvt);

  var lorry=CreateContents(LORY,flvt);
  for(var cnt=0;cnt<30;cnt++)
    CreateContents(RandomID(),lorry);
}

protected func GivePalaStuff(pObj)
{
  for(var cnt=0;cnt<3;cnt++)
  {
    var id = RandomPalaID();
    CreateContents(id,pObj);
    if(id==MUSK)
      CreateContents(RandomBullet(),pObj);
    if(id==BOW1)
      for(var cnt2=0;cnt2<3;cnt2++)
        CreateContents(RandomArrow(),pObj);
  }
}

protected func RandomPalaID()
{
 return(GetRandomItem([BRED, MUSK, BOW1, SWOR, AXE1, SPER, 0]));
}

protected func RandomBullet()
{
  return(GetRandomItem([BLTP, SBLP, PBLP]));
}

protected func RandomArrow()
{
  return(GetRandomItem([ARWP, FARP, XARP, 0]));
}

protected func RandomID()
{
  return(GetRandomItem([STFN, BRED, SFLN, GUNP, WBRL, SHIE, BOW1, FARP, XARP, ARWP, SWOR, AXE1, SPER, EFLN, 0]));
}

protected func GetRandomItem(aArray) { return(aArray[Random(GetLength(aArray))]); }

protected func InitializePlayer(iPlr,foox,fooy,base,iTeam)
{
 Global(iPlr)=iTeam;
 
 var x=900;
 var y=395;
 if(iTeam == 1)
 {
  if(!first_castle)
  {
   SetCastleOwner(iPlr,0);
   first_castle=1;
  }
 }
 
 if(iTeam == 2)
 {
  var cntr=0;
  for(var cnt=0;cnt<GetPlayerCount();cnt++)
   if(GetPlayerByIndex(cnt) != iPlr)if(Global(GetPlayerByIndex(cnt)) == 2)cntr++;
  
  if(cntr%2==0)
  {x=142;y=302;}
  else
  {x=1645;y=302;}
  if(x > 500)SetCastleOwner(iPlr,2);
  else SetCastleOwner(iPlr,1);
 }
 
// CreateContents(FLAG,GetCrew(iPlr,1));
 
 for(var cnt=0;GetCrew(iPlr,cnt);cnt++)
  SetPosition(x,y,GetCrew(iPlr,cnt));
 
 return(1);
}

protected func SetCastleOwner(int iPlr,int iPart)
{
  var x=LandscapeWidth()/3;
  if(Global(iPlr) == 2 && iPart==1) x=1;
  if(Global(iPlr) == 2 && iPart==2) x=(LandscapeWidth()/3)*2;
 
  for(var obj=0;obj=FindObject(0,x,0,LandscapeWidth()/3,LandscapeHeight(),0,0,0,NoContainer(),obj);)
  {
    if((GetID(obj)==CPOF || GetID(obj)==BRCK) && !FindContents(FLAG) && !FindObject2(Find_ID(FLAG), Find_ActionTarget(obj)))
      CreateContents(FLAG, obj)->SetOwner(iPlr);
    if(~GetCategory(obj) & C4D_Structure() && !obj->~CastlePartWidth() && GetID(obj) != BANR) continue;
    if(GetOwner(obj) != -1) continue;
    SetOwner(iPlr,obj);
  }
}
