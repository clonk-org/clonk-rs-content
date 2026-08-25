/*-- Hochofen --*/


#strict

#include BAS5
#include DOOR
//#include FNDR

local lastCoralID;


private func Burning()
{
  // Rauch
  Smoke(+23,Random(3)-24,Random(5)+8);
  CreateParticle("Blast", 9, -22, 0, RandomX(-8,-10), RandomX(90,110), RGBa(000,255,200,210));
  CreateParticle("Fire3", 9, -26, 0, RandomX(-10,-8), RandomX(90,110), RGBa(000,255,200,100));
  CreateParticle("Fire3", 9, -26, 0, RandomX(-14,-12), RandomX(90,110), RGBa(000,255,200,100));  
  // Noch nicht fertig
  if (GetActTime()<80) return(1);
  // Fertig
  SetAction("Idle");
  // Erz suchen
  var pOre;
  if (!(pOre=FindContents(lastCoralID))) return(1);
  // Metall auswerfen
  CreateContents(pOre->GetChangeID());
  // Erz verbrauchen
  RemoveObject(pOre);
  // Sound
  Sound("Pshshsh");
  // Fertig
  return(1);
}

/* Inhaltsüberprüfung */  
  
private func ContentsCheck()
{
  // Warten...
  if (!ActIdle()) 
    return(1);
  // Loren rausschicken
  var obj;
  for(var i = 0; obj = Contents(i); i++)
    if(obj->~IsLorry())
      {
      SetCommand(obj,"Exit");
      }
  // Erstes Inhaltsobjekt überprüfen
  obj = Contents(0); var id = GetID(obj);
  // Alles außer Mannschaftsmitgliedern, Rohmaterial und Loren sofort auswerfen
  if (obj)
    if ( !(    GetOCF(obj) /*& OCF_CrewMember()*/
            || obj->~IsLorry()
            || id==GCOR
            || id==BCOR
            || id==RCOR ) )
      if(GetDefFragile(id))
        SetCommand(obj, "Exit");
      else
        SetCommand(obj, "Exit");
  // Erz prüfen
  if (FindContents(GCOR) || FindContents(BCOR) || FindContents(RCOR) ){ 
   
   if(FindContents(RCOR)){ lastCoralID = RCOR;
    }
    else {
       if(FindContents(BCOR)){ lastCoralID = BCOR;
        }
        else {
       if(FindContents(GCOR)){ lastCoralID = GCOR;
          }        
         }
        }
       
   SetAction("Stampfing");
  }
  return(1);
}
public func Stampf(){
 if(!Random(4)){
 var r,g,b;
 if(lastCoralID == RCOR){ r = 200; g = 40; b = 40; }
 if(lastCoralID == BCOR){ r = 40; g = 40; b = 200; }
 if(lastCoralID == GCOR){ r = 40; g = 200; b = 40; }
 CreateParticle("Smoke",-14,0,RandomX(-1,1),-3,40,RGB(r,g,b,100));
 CreateParticle("Smoke", -7,0,RandomX(-1,1),-3,40,RGB(r,g,b,100));
 }
 if (GetActTime()<150) return(1);
SetAction("Burning");
  return(1);
}
