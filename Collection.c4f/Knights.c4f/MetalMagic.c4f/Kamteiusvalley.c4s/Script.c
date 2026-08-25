/*-- Kamteius' Valley --*/

#strict

static main_king,first_left,first_right;


func Initialize() {

  //Truhen füllen
  for(var chest in FindObjects(Find_ID(CHST)))
  {
  while(Contents(0,chest))RemoveObject(Contents(0,chest));
  FillChest(chest);
  }
  
  var chest=FindObject(CHST,684,607,-1,-1);
  if(!FindContents(SCKZ,chest))CreateContents(SCKZ,chest);
  if(!FindContents(LFAM,chest))CreateContents(LFAM,chest);
  
  if(!ObjectCount(SPST))CreateObject(SPST,10,10,-1);
  if(!ObjectCount(ATCC))CreateObject(ATCC,10,10,-1);
  if(!ObjectCount(NOUD))CreateObject(NOUD,10,10,-1);
  if(!ObjectCount(MELE))CreateObject(MELE,10,10,-1);
  ScriptGo(1);
}
protected func Script0()
{
 var elev=FindObject(ELEV);
 var x=12;
 DrawMaterialQuad("Tunnel",GetX(elev)-x,GetY(elev),GetX(elev)+x-1,GetY(elev),GetX(elev)+x-1,GetY(elev)+90,GetX(elev)-x,GetY(elev)+90,1);
 elev->DoCon(10);
 if(!GetEffect("MageAI",FindObject(SCLK)))
 AddEffect("MageAI",FindObject(SCLK),20,5);
 //Sie haben Kenny getötet!
 SetName("Kenny",FindObject(SCLK));
}

protected func Script15()
{
 if(!main_king || !GetAlive(main_king))
 {
  for(var cnt=0;cnt<GetPlayerCount();cnt++)
  {
   var plr=GetPlayerByIndex(cnt);
   if(Global(plr)==1)EliminatePlayer(plr);
  }
 }
 return(goto(5));
}

protected func FillChest(object pChest)
{
 var count=4+Random(7);
 while(count--)
 CreateContents(RandChestID(),pChest);
}

protected func RandChestID()
{
 var rand=Random(15);
 if(!rand)return(BRED);
 if(!--rand)return(SPER);
 if(!--rand)return(MUSK);
 if(!--rand)return(BLTP);
 if(!--rand)return(BOW1);
 if(!--rand)return(FARP);
 if(!--rand)return(XARP);
 if(!--rand)return(STFN);
 if(!--rand)return(EFLN);
 if(!--rand)return(AXE1);
 if(!--rand)return(SWOR);
 if(!--rand)return(METL);
 if(!--rand)return(TENP);
 if(!--rand)return(BHLG);
 if(!--rand)return(PBLP);
 return(COKI);
}

protected func InitializePlayer(int player, int tx, int ty, object pBase, int iTeam)
{
 if(iTeam==1)
 {
  if(!main_king)
  {main_king=CreateObject(KING,0,0,player); SetName("Kamteius",main_king);
   AddEffect("KingEnergy",main_king,20,3,0,0);
   MakeCrewMember(main_king,player);
  }
 }
 
 if(iTeam==2)
 {
  var altr=FindObject(ALTR);if(GetOwner(altr)== -1)SetOwner(player,altr);
  var elev=FindObject(ELEV,34,350,-1,-1); if(GetOwner(elev)== -1)SetOwner(player,elev);
  var elec=FindObject(ELEC,34,350,-1,-1); if(GetOwner(elec)== -1)SetOwner(player,elec);
 }
 
 
 var x=1396;
 var y=249;
 if(iTeam==2)
 {
  x=62;
  y=367;
 }
 
 for(var clnk in FindObjects(Find_OCF(OCF_Alive()),Find_Owner(player)))
 {
//  MakeCrewMember(clnk,player);
  SetPosition(x,y,clnk);
  DoEnergy(100,clnk);
  if(GetID(clnk)==KING) while(DoMagicEnergy(1,clnk));
 }
 Global(player)=iTeam;
 CreateContents(FLAG,GetCrew(player));
 //SelectCrew(player,GetCrew(player),1);
 return(1);
}

protected func CopyPhysical(szPhysical, pSource, pTarget)
{
  SetPhysical(szPhysical, 2, GetPhysical(szPhysical, 0, pSource), pTarget);
}

global func FxKingEnergyStart(object pTarget, int iEffectNumber, int iTemp)
{
 EffectVar(0,pTarget,iEffectNumber)=0;
}

global func FxKingEnergyTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
 if(!GetAlive(pTarget) || !pTarget)return(-1);
 
 if(GetAction(pTarget) eq "Walk" && !GetXDir(pTarget))
 if(!Contained(pTarget) || GetID(Contained(pTarget))==THRN)
  EffectVar(0,pTarget,iEffectNumber)++;
 else
  EffectVar(0,pTarget,iEffectNumber)=0;
  
 if(EffectVar(0,pTarget,iEffectNumber)>20)
 {
  EffectVar(0,pTarget,iEffectNumber)=0;
  DoMagicEnergy(1,pTarget);
 }
 
 return(1);
}

global func FxMageAIStart(object pTarget, int iEffectNumber, int iTemp)
{
 SetPhysical("CanScale",1,0,pTarget);
}

global func FxMageAITimer(object pTarget, int iEffectNumber, int iEffectTime)
{
 if(!GetAlive(pTarget) || !pTarget)return(-1);
 
 var nTarget;
 for(var clnk;clnk=FindObject(0,GetX(pTarget),GetY(pTarget),-1,-1,OCF_CrewMember(),0,0,NoContainer(),clnk);)
 {
  if(clnk == pTarget)continue;
  if(!PathFree(GetX(pTarget),GetY(pTarget),GetX(clnk),GetY(clnk)))continue;
  if(ObjectDistance(clnk,pTarget)>200)continue;
  if(InLiquid(clnk))continue;
  nTarget=clnk;
  break;
 }
 
 if(InLiquid(pTarget))
 if(!GetXDir(pTarget) && !GetYDir(pTarget))
 SetComDir(COMD_Up(),pTarget);
 
 if(GetCommand(pTarget) eq "MoveTo")
 if(!PathFree(GetX(pTarget),GetY(pTarget),GetCommand(pTarget,2),GetCommand(pTarget,3)))
 FinishCommand(pTarget,0,1);
 
 if(GetCommand(pTarget) eq "Attack")
 if(!PathFree(GetX(pTarget),GetY(pTarget),GetX(GetCommand(pTarget,1)),GetY(GetCommand(pTarget,1))) || InLiquid(GetCommand(pTarget,1)))
 FinishCommand(pTarget,0,1);
 
 if(!GetCommand(pTarget))
 if(nTarget)
 {
  if(Random(3))SetCommand(pTarget,"Wait",0,0,0,0,33*3);
  else SetCommand(pTarget,"Attack",nTarget);
  
  if(GetCommand(pTarget) eq "Wait")
  if(Random(2))Message(Format("<c ffaaaa>%s",RandomString("$MageGo1$","$MageGo2$","$MageGo3$")),pTarget);
 }
 
 if(GetCommand(pTarget) eq "Attack")
 if(Contents(0,pTarget))
 if(GetID(Contents(0,pTarget)) != ROCK)
 SetCommand(pTarget,"Throw",0,GetX(GetCommand(pTarget,1)),GetY(GetCommand(pTarget,1)));
 
 return(1);
}

global func FxMageAIDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
 if(iCause<0)
 if(iCause != 35 && iCause != 37)
 CastPXS("Water",iDmgEngy/-2000,10,GetX(pTarget),GetY(pTarget));
 
 if(OnFire(pTarget))Extinguish(pTarget);
 
 if(Random(4) && (iCause == 33 || iCause ==34))
 Message(Format("<c ffaaaa>%s",RandomString("$MageDmg1$","$MageDmg2$","$MageDmg3$")),pTarget);
 
 return(iDmgEngy/2);
}

global func RandomString()
{
 var amount;
 for(var cnt=0;Par(cnt);cnt++)
 amount++;
 
 return(Par(Random(amount)));
}
