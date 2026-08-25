/*Schriftrolle*/

#strict

func Initialize()
{
  ScheduleCall(this(),"LaterInit",4);
  return(1);
}

func LaterInit()
{
  var scroll= CreateObject(SCRL,0,0,GetOwner());
  scroll->~ SetSpell(RUND);
  if(Contained()) Enter(Contained(),scroll);
  RemoveObject();
  return(1);
}