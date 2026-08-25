#strict

func Execute(pIdol,pOwner) {
 // Alle Spieler überprüfen
 for(var iPlr=0;iPlr<GetPlayerCount();iPlr++)
  {
   if(!Hostile(iPlr,pOwner) || pOwner==-1)
     {
     var lgt=CreateObject(BVIW,0,0,iPlr);
     lgt->AttachTo(pIdol);
     }
  }
 return(1);
}

protected func Activate(int plr)
{
  if (GetType(plr) != C4V_Int())	return(_inherited());
  // Beschreibung ausgeben
  return MessageWindow(GetDesc(),plr);
}

#appendto IDOL
#appendto GIDL
#appendto BK5P
#appendto _MOF

func Initialize() {
 var idol_view=FindObject(RIVW);
 var pOwner=GetOwner();
 var pIdol=this();
 if(idol_view) ScheduleCall(idol_view,"Execute",1,0,pIdol,pOwner);
 return( _inherited(...));
 }


func Departure (object pContainer) 
{
 var idol_view=FindObject(RIVW);
 var pOwner=GetOwner();
 var pIdol=this();
 if(idol_view) ScheduleCall(idol_view,"Execute",1,0,pIdol,pOwner);
 return(_inherited(...));
}