//Spielregel

#strict

func InitializePlayer() {
 ScheduleCall(0,"AddView",1);
 return(_inherited());
 } 

func OnClonkCreation() {
 ScheduleCall(0,"AddView",1);
 return(_inherited()); 
 }
 
func AddView() {
 // Alle Spieler überprüfen
 for(var iPlr=0;iPlr<GetPlayerCount();iPlr++) {
  if(!GetCrew(iPlr)) continue;
  var pObjs=FindObjects(Find_OCF(OCF_CrewMember,OCF_Alive),Find_Allied(iPlr),Find_Not(Find_Owner(iPlr)));
  for(var j=0;j<GetLength(pObjs);j++) {
   //Log("%d %d",j,GetLength(pObjs));
   if(!FindObject(VIEW,0,0,0,0,0,"Shine",pObjs[j])) {
    //Log("%s to Clonk from %s",GetPlayerName(iPlr),GetPlayerName(GetOwner(pObjs[j])));
    var lgt=CreateObject(VIEW,0,0,iPlr); 
    lgt->AttachTo(pObjs[j]);
    }
   }
  }
 return(1);
 }
 

func Execute() {
 for(var obj in FindObjects(Find_Owner(NO_OWNER),Find_ID(VIEW))) {
    if(GetAlive(GetActionTarget(0,obj)) && GetOCF(GetActionTarget(0,obj)) & OCF_CrewMember()
    && !(GetOCF(GetActionTarget(0,obj)) & OCF_OnFire()))
     RemoveObject(obj);
    }
 return(1);
 } 

protected func Activate(int plr)
{
  //if (GetType(plr) != C4V_Int())    return(_inherited());
  // Beschreibung ausgeben
  return MessageWindow(GetDesc(),plr);
}

func EliminatePlayer(int iPlr) {
 //Log("Log: EliminatePlayer");
 while(FindObjectOwner(VIEW,iPlr)) RemoveObject(FindObjectOwner(VIEW,iPlr));
 return(_inherited(iPlr));
 }