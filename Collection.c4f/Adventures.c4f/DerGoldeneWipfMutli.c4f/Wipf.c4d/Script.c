#strict
protected Initialize:
  SetPortrait("1",this(),WIPF);
  SetGraphics(0,this(),WIPF);
  ControlDigDouble();
  SetComDir(COMD_Left());
  if (Random(2)) SetComDir(COMD_Right());
  return(1); 

protected Activity:
  if (!Random(15)) Sound("Snuff*");
  return(1);

ControlLeft:
  if (GetAction()S="Sit") SetAction("Walk");
  Sound("Snuff*");
  return(0);

ControlRight:
  if (GetAction()S="Sit") SetAction("Walk");
  Sound("Snuff*");
  return(0);

ControlDownDouble:
  if (GetAction()S="Walk") SetAction("Sit");
  return(0);

protected CatchBlow:
  if (GetAction() eq "Dead") return(0);
  if (!Random(2)) Sound("WipfHurt");
  return(1);

protected Death:
  if(!GetCrew(GetOwner())) GameCall("RelaunchPlayer",GetOwner());
  Sound("WipfDead");
  SetDir(0);
  ChangeDef(DWPF);
  SetAction("Dead");
  return(1);

protected RejectCollect:
  return(1);

protected func ControlCommand(szCommand, pTarget, iTx, iTy)
{
  if (szCommand eq "MoveTo" || szCommand eq "Get" || szCommand eq "Dig")
    return(SetCommand(this(),szCommand, pTarget, iTx, iTy));
  return(0);
}

// --------------

private func MachClonk(id id)
{
  var clonkid=CreateObject(id, 0, 5, GetOwner());
  MakeCrewMember(clonkid,GetOwner());
  SetXDir(GetXDir(),clonkid);
  SetYDir(GetYDir(),clonkid);
  SetCursor(GetOwner(),clonkid);                           
  DoEnergy(+10000,clonkid);
  RemoveObject();
  return(1);
}

Damage:
  DoEnergy(GetPhysical("Energy",0,0)/1000-50,0);
  return(1);

ControlDigDouble:
  CreateMenu(WIPF,this(),this());
  AddMenuItem("%s","MachClonk",PLDN);
  AddMenuItem("%s","MachClonk",ASAS);
  AddMenuItem("%s","MachClonk",MAGE);
return(1);
           