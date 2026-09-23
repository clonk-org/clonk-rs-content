#strict

#include CLNK

local ibla,ibla2,ifrage,iantwort,iSelection,imenu,ischlaf,istatus,ihaus,isound,iwork;

Initialize:
  CreateObject(ABS_,0,0,-1);
  SetAction("Teil1");
  SetDir(Random(2));
  return(1);

Teil1:
  Message("$Punzel01$",this());
  Sound("QI_01",1);
  return(1);

Teil2:
  Message("$Punzel02$",this());
  Sound("QI_02",1);
  return(1);

Teil3:
  Message("$Punzel03$",this());
  Sound("QI_03",1);
  return(1);

Teil4:
  Message("$Punzel04$",this());
  Sound("QI_04",1);
  return(1);

Teil5:
  Message("$Punzel05$",this());
  Sound("QI_05",1);
  return(1);

Teil6:
  Message("$Punzel06$",this());
  Sound("QI_06",1);
  return(1);

Teil7:
  Message("$Punzel07$",this());
  Sound("QI_07",1);
  return(1);

Teil8:
  Message("$Punzel08$",this());
  Sound("QI_08",1);
  return(1);

Teil9:
  Message("$Punzel09$",this());
  Sound("QI_09",1);
  SetLocal(8,1);
  return(1);

Teil10:
  SetAction("Walk");
  Message("<c 00d8e5> <Punzel>|Igil oh drama holtixius</c>",this());
  Sound("QI_10",1);
  SetCommand(this(),"MoveTo",0,GetX(FindObject(DKR_)),GetY(FindObject(DKR_)),0,0,5);
  return(1);

Teil11:
  Sound("QI_11",1);
  CastParticles("Flash", 30, 170, 0,0, 10, 500+Random(800), RGBa(255,255,255,50+Random(120)));
  GameCall("DasEnde");
  RemoveObject();
  return(1);

Check:
  if(!Local(8))  return(0);
  SetCommand(this(),"MoveTo",0,GetX(FindObject(DKR_)),GetY(FindObject(DKR_)),0,0,5);
  if(Distance(GetX(FindObject(DKR_)),GetY(FindObject(DKR_)),GetX(),GetY() )<170)  return(Teil11());
  return(1);