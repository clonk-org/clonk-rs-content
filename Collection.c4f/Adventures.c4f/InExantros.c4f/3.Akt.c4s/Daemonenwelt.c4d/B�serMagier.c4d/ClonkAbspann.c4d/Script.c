#strict

#include CLNK

local ibla,ibla2,ifrage,iantwort,iSelection,imenu,ischlaf,istatus,ihaus,isound,iwork;

Initialize:
  CreateObject(ABS_,0,0,-1);
  SetAction("Teil1");
  SetDir(Random(2));
  return(1);

Teil1:
  Message("<c 00d8e5> <Punzel>|Ich bin frei.| Frei aber am Ende| meiner Kräfte.</c>",this());
  Sound("QI_01",1);
  return(1);

Teil2:
  Message("<c 00d8e5> <Punzel>|Vor vielen Jahren| habe ich das Haus| meines verstorbener| Großvater geerbt.</c>",this());
  Sound("QI_02",1);
  return(1);

Teil3:
  Message("<c 00d8e5> <Punzel>|Dort hab ich ein Buch gefunden,| doch dieses Buch war von| einem Dämon besessen, welcher| die Kontrolle über mich nahm</c>",this());
  Sound("QI_03",1);
  return(1);

Teil4:
  Message("<c 00d8e5> <Punzel>|Er lies mich verfaulen,| wie eine Leiche...| was hat er nur mit mir gemacht!</c>",this());
  Sound("QI_04",1);
  return(1);

Teil5:
  Message("<c 00d8e5> <Punzel>|Aber ihr habt ihn aus| mir vertrieben. Doch ihr| konntet mich nicht retten,| seht mich doch nur mal an.</c>",this());
  Sound("QI_05",1);
  return(1);

Teil6:
  Message("<c 00d8e5> <Punzel>|Es ist an der Zeit, dass| ich ein letztes mal das| Buch zu mir nehme und| das Letzte Kapitel aufschlage.</c>",this());
  Sound("QI_06",1);
  return(1);

Teil7:
  Message("<c 00d8e5> <Punzel>|Das letzte Kapitel zum Ende.| Ich werde mit dem| Kristall untergehen.</c>",this());
  Sound("QI_07",1);
  return(1);

Teil8:
  Message("<c 00d8e5> <Punzel>|Es ist meine letzte Pflicht.| Sagt der Welt, dass ich| trotzalledem ein Held war.</c>",this());
  Sound("QI_08",1);
  return(1);

Teil9:
  Message("<c 00d8e5> <Punzel>|Ich bin bereit.</c>",this());
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