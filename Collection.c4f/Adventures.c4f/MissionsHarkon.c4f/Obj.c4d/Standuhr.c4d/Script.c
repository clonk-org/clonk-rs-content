/*-- Groﬂe Uhr --*/

#strict 2

protected func Initialize()
{
  SetAction("Tick");
  SoundLevel("Clock",0,this());
  SoundLevel("Clock",100,this());
  CreatePendulum();
  return 1;
}

/* Pendel erzeugen */

private func CreatePendulum()
{
  ObjectSetAction(CreateObject(_PND,0,0,GetOwner()),"Swing",this());
}