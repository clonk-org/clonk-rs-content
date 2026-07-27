/* Objectschleuder */

#strict 2

func Activate(pCaster,pRealcaster) {
  var pClonk,fExplo;
  if(pRealcaster) pClonk=pRealcaster;
  else pClonk=pCaster;
 
  // Nicht im Gebäude zaubern
  if(Contained(pClonk))
  {
    Sound("Error");
    PlayerMessage(GetOwner(pClonk), "$must_be_in_free$", pClonk);
    RemoveObject();
    return 0;
  }

  // Ohne Contents? Versagt
  if(!Contents(0,pCaster))
  {
    PlayerMessage(GetOwner(pClonk), "$NoObject$", pClonk);
    RemoveObject();
    return 0;
  }
  // Nur unter bestimmten Gewicht
  if( GetMass(Contents(0,pCaster))>=17 || GetID(Contents(0,pCaster)) == EFLN)
  {
   PlayerMessage(GetOwner(pClonk), "$Mass$",pCaster);
   RemoveObject();
   return 0;
  }
  
  // Zielen wenn möglich
  if (pClonk->~DoSpellAim(this)) return 1;
    
  if(GetDir(pClonk)==DIR_Left)
  ActivateAngle(pClonk,-90);
  else ActivateAngle(pClonk,+90);
  
  Sound("MgWind*");
  RemoveObject();
  return 1;
}

// Aktivierung nach Zielen (iAngle: -90=links; 0=oben; +90=rechts)
public func ActivateAngle(object pCaller, int iAngle)
{
  if (GetID(Contents(0, pCaller)) == FLNT)
  {
    ChangeDef(METO,Contents(0,pCaller));
    Contents(0,pCaller)->SetAction("Evaporate");
  }
  
  var speed=BoundBy(13-GetMass(Contents(0,pCaller))/2,5,15);
  
  var yDir=Sin(iAngle-90,speed);
  var xDir=Cos(iAngle-90,speed);
  
  var yDir2=Sin(iAngle-90,10);
  var xDir2=Cos(iAngle-90,10);
  
  var arw;
  var mass;
  var pObj;
  // Pfeilpaketkombo abgeschaltet. Zu stark
  /*if(GetID(Contents(0, pCaller)) == ARWP)
  {
   var owner = GetOwner(Contents(0,pCaller));
   var iCount = Contents(0, pCaller)->~PackCount();
   RemoveObject(Contents(0,pCaller));
   for(var cnt=0;cnt<iCount;cnt++)
   {
    arw=CreateObject(ARRW,AbsX(GetX(pCaller))+xDir2/2,AbsY(GetY(pCaller))+yDir2/3,owner);
    arw->NoArrowHit(pCaller,20);
    arw->Launch(pCaller);
    var rand=Random(50);
    SetXDir(Cos(iAngle-90,10+rand)+RandomX(-10,10),arw);
    SetYDir(Sin(iAngle-90,10+rand)+RandomX(-10,10),arw);

    SetR(Angle(GetX(arw),GetY(arw),GetX(arw)+GetXDir(arw),GetY(arw)+GetYDir(arw)),arw);
   }
   CreateParticle("Blast",AbsX(GetX(pCaller))+xDir/2,AbsY(GetY(pCaller))+yDir/3,0,0,50,RGB(255,255,255));
  }
  else*/
  
    mass=GetMass(Contents(0,pCaller));
    pObj = Contents(0,pCaller);
    pObj->~Launch(pCaller);
    if(pObj->~IsArrow())
    {
      xDir/=2;
      yDir/=2;
      pObj->~NoArrowHit(pCaller,20);
    }
    Exit(pObj,AbsX(GetX(pCaller))+xDir2/2,AbsY(GetY(pCaller))+yDir2/2,900/xDir+90,xDir,yDir);
  
  
  //Für leichte Objekte etwas Mana zurück?
  if(mass<=5) DoMagicEnergy(16-mass,pCaller);
  // Objekt löschen
  RemoveObject();
  return 1;
  }

/* Zaubercombo */

public func GetSpellStaff(object pMage) { return SMER; }
public func GetSpellStaffCombo(pMage) { return ("313"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
