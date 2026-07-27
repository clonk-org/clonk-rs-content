/*-- Muskete --*/

#strict 2

/* Aktivieren (zum Zielen per Maussteuerung) */
public func Activate(pClonk)
{
  [$CtrlActivateDesc$]
  // Der Clonk kann dieses Objekt nicht verwenden
  if (!pClonk->~CanUse(GetID())) return 0;
  // Träger nicht bereit zum neuen Zielen
  if (!pClonk->~ReadyToFire()) return;
  
  // Träger: neue Zielaktion setzen 
  if(Contents())
   if(pClonk ->~ AimMusket())
   {
      SetPhase(6,pClonk);
      return 1;
   }
  
  if(!pClonk->~LoadMusket(1))
  {
    if (!pClonk->~AimMusket()) 
      return;
    SetPhase(6, pClonk);
  }
  // Erfolg
  return 1;
}

/* Spezialsteuerung durch Träger */
public func ControlThrow(pClonk)
{
  // Träger hat gezielt
  if (IsAiming())
    { Fire(pClonk); return(1); }
  // Muskete einsetzen
  return Activate(pClonk);
}

protected func CalcValue()
{
 
 if(!Contents())return GetValue(0,GetID());
 return GetValue(0,GetID())+GetValue(Contents());
}

protected func ControlConf(int conf)
{
  var rVal;
  var pClonk = Contained();
  if(!pClonk) return 0;

  if(conf < 0)
  {
    if(pClonk->~MusketControlUp(this, rVal))
      return rVal;
    else if(IsAiming())
      return pClonk->AimStdConf(conf);
    else
      return 0;
  }
  else
  {
    if(pClonk->~MusketControlDown(this, rVal))
      return rVal;
    else if(IsAiming())
      return pClonk->AimStdConf(conf);
    else
      return 0;
  }
}

public func ControlUp(pClonk)
{
  return AimUp(this, 5, "ControlConf");
}

public func ControlDown(pClonk)
{
  return AimDown(this, 5, "ControlConf");
}

public func ControlUpdate(pClonk, int comdir)
{
  AimUpdate(pClonk, comdir, 5, "ControlConf");
}

public func ControlLeft(pClonk)
{
  // Träger Zielaktion
  if (IsAiming())
  {
    SetDir(DIR_Left, pClonk);
    return 1;
  }
  // Keine Spezialsteuerung
  return 0;
}

public func ControlRight(pClonk)
{
  // Träger Zielaktion
  if (IsAiming())
  {
    SetDir(DIR_Right, pClonk);
    return 1;
  }
  // Keine Spezialsteuerung
  return 0;               
}

public func ControlRightDouble(pClonk)
{
  // Zielaktion abbrechen
  if (IsAiming() || IsLoading())
  {
    if(IsAiming())pClonk->StopAiming();
    else if(IsLoading())pClonk->StopLoading();
    SetComDir(COMD_Right, pClonk);
  }
  // Keine Spezialsteuerung
  return 0;
}

public func ControlLeftDouble(pClonk)
{
  // Zielaktion abbrechen
  if (IsAiming() || IsLoading())
  {
    if(IsAiming())pClonk->StopAiming();
    else if(IsLoading())pClonk->StopLoading();
    SetComDir(COMD_Left, pClonk);
  }
  // Keine Spezialsteuerung
  return 0;
}

/* Wird gerade vom Träger gezielt? */
public func IsAiming() 
{ 
  if(!Contained()) return false;
    return (Contained()->~IsAiming()); 
}

/* Wird gerade vom Träger gelahten? */
public func IsLoading() 
{ 
  if(!Contained()) return false;
    return (Contained()->~IsLoading()); 
}

/* Hardgecodet, für alte Ritter *///Aber weil paladin und kein Ritter: Auskommentieren
/*private func GetMusketOutOld(pClonk, &iOutX, &iOutY, &iOutR, &iOutXDir, &iOutYDir, &iOutRDir)
{
  var iOutDir; if (GetDir(pClonk) == DIR_Left) iOutDir=-1; else iOutDir=1;
  var iOutPhase = GetPhase(pClonk);
  // X-Austritt  
  iOutX = iOutDir*iOutPhase*2;
  // Y-Austritt
  iOutY = iOutPhase+3; // + Random(9)-4;
  // R-Austritt
  iOutR = iOutX * 13;
  // XDir
  iOutXDir = (iOutDir*iOutPhase*3+2)*2;
  // YDir
  if (iOutPhase == 5) iOutYDir=20; else iOutYDir = ((iOutPhase-3)*3)*2;
  // Größerer Abstand
  iOutX += iOutXDir*2;
  iOutY += iOutYDir*2;
  // RDir
  iOutRDir = iOutDir*3;
}
public func GetMusketOutAngle(pClonk, iAngle, fDeviate, &iOutX, &iOutY, &iOutR, &iOutXDir, &iOutYDir, &iOutRDir) 
{
  // Zufallsabweichung
  if(fDeviate) { iAngle += Random(11) - 5; }
  // Austrittsgeschwindigkeit ermitteln 
  iOutXDir = Sin(iAngle, 200); 
  iOutYDir = -Cos(iAngle, 200); 
  // Austrittsposition... 
  iOutX = Sin(iAngle, 25); 
  iOutY = -Cos(iAngle, 25); 
  // Pfeildrehung 
  iOutR = iAngle; 
  if(iAngle > 0) iOutRDir = 3; else iOutRDir = -3;
  // Fertig 
  return 1;
} */

/* Kugl hupf! */
public func Fire()
{
  var pClonk = Contained();
  // Kein Träger?
  if(!pClonk) return 0;
  // Im Gebäude?
  if (pClonk->Contained()) { pClonk->~StopAiming(); return(0); }
  // Unvollständig?
  if (~GetOCF() & OCF_Fullcon) return 0;
  // Noch beim Nachladen
  if (GetAction() == "Reload") return 0;
  // Kugel muss in der Muskete sein
  var pArrow = Contents();
  if (!pArrow) return 0;
  // Beim Träger ggf. Bogen wieder auswählen
  ShiftContents(pClonk, 0, MUSK);
  // Austrittsgeschwindigkeit ermitteln
  var iAngle = pClonk->~GetMusketAimAngle();
  
  //if(GetEffect("SniperPotion",pClonk))
  GetSpecialAngle(iAngle,pClonk);
  
  var iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir;
  pClonk->~GetMusketOut(this, iAngle, true, iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir);
   // GetMusketOutOld(pClonk, iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir);
  // Besitzer des Projektils setzen
  pArrow->SetOwner(Contained()->GetOwner());
  SetController(Contained()->GetOwner(),pArrow);
  if(pArrow->~IsBullet())pArrow->LocalN("pLauncher")=pClonk;
    // Pfeil abfeuern
  Exit(pArrow, iOutX,iOutY,iOutR, 0,0,0 );
  pArrow->SetXDir(iOutXDir); pArrow->SetYDir(iOutYDir);
  pArrow->~Launch();
  // Sicht verfolgen
  //if (GetPlrView(GetController(pClonk)) != 2)
   // SetPlrView(GetController(pClonk), pArrow);
  // Sound
  Sound("Musket.wav");
  // Nachladen
  SetAction("Reload");
  pClonk->~LoadMusket();
  // Fertig!
  return 1;
}

/* Kugl abfeuern */
public func FireAngle(iAngle)
{
  var pClonk = Contained();
  // Kein Träger?
  if(!pClonk) return 0;
  // Im Gebäude?
  if (pClonk->Contained()) { pClonk->~StopAiming(); return(0); }
  // Unvollständig?
  if (~GetOCF() & OCF_Fullcon) return 0;
  // Noch beim Nachladen
  if (GetAction() == "Reload") return 0;
  // Pfeil vom Träger holen
  var pArrow = Contents();
  if (!pArrow) return;
  // Beim Träger ggf. Bogen wieder auswählen
  ShiftContents(Contained(), 0, MUSK);
  // Austrittsgeschwindigkeit ermitteln
  var iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir;
  pClonk->~GetMusketOut(this, iAngle, true, iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir);
   // GetMusketOutAngle(Contained(), iAngle, true, iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir);
  // Besitzer des Projektils setzen
  pArrow->SetOwner(Contained()->GetOwner());
    // Pfeil abfeuern
  Exit(pArrow, iOutX,iOutY,iOutR, 0,0,iOutRDir );
  pArrow->SetXDir(iOutXDir); pArrow->SetYDir(iOutYDir);
  pArrow->~Launch();
    // Sound
  Sound("Musket");  
  // Nachladen
  SetAction("Reload");
  Contained()->~LoadMusket();
  return 1;
}

protected func GetSpecialAngle(&iAngle,pClonk)
{
 
  var iOutX = Sin(iAngle, 25 * GetCon(pClonk) / 100);
  var iOutY = -Cos(iAngle, 25 * GetCon(pClonk) / 100); 
 
 var dir=0;
 if(GetDir(pClonk) == DIR_Left)dir=360;
 var actualTargets=[];
 for(var clnk in FindObjects(Find_OCF(OCF_Alive),Sort_Distance(iOutX,iOutY)))
 {
  
  //Log("%d %d %d %s",iAngle,Angle(GetX(pClonk),GetY(pClonk),GetX(clnk),GetY(clnk)),Angle(GetX(pClonk),GetY(pClonk),GetX(clnk),GetY(clnk))-360,GetName(clnk));
  
  //Log("%d <- %d (%s)",iAngle,Angle(GetX(pClonk)+iOutX,GetY(pClonk)+iOutY,GetX(clnk),GetY(clnk))-dir,GetName(clnk));
  if(!Inside(Angle(GetX(pClonk)+iOutX,GetY(pClonk)+iOutY,GetX(clnk),GetY(clnk))-dir,iAngle-15,iAngle+15))continue;

  
  if(!PathFree(GetX(pClonk)+iOutX,GetY(pClonk)+iOutY,GetX(clnk),GetY(clnk)))continue;
  //if(ObjectDistance(pClonk,clnk)>800)continue;
  if(clnk == pClonk)continue;
//  Log("---------");
  //Log("%d %d %d %s",iAngle,Angle(GetX(pClonk),GetY(pClonk),GetX(clnk),GetY(clnk)),Angle(GetX(pClonk),GetY(pClonk),GetX(clnk),GetY(clnk))-360,GetName(clnk));
  actualTargets[GetLength(actualTargets)]=clnk;
 }
// Log("%d",GetLength(actualTargets));
 //Vll lieber erstmal Feinde, wenn geht.
 var enemyTargets=[];
 for(var enemy in actualTargets)
 {
  if(!Hostile(GetOwner(pClonk),GetOwner(enemy)))continue;
  enemyTargets[GetLength(enemyTargets)]=enemy;
 }
 
 //Dann neutrale?
 var neutralTargets=[];
 for(var neutral in actualTargets)
 {
  if(GetOwner(neutral) != -1)continue;
  neutralTargets[GetLength(neutralTargets)]=neutral;
 }
 
 //Der Rest ist froint :(
 var friendlyTargets=[];
 for(var friend in actualTargets)
 {
  if(GetOwner(friend) == -1)continue;
  if(Hostile(GetOwner(pClonk),GetOwner(friend)))continue;
  friendlyTargets[GetLength(friendlyTargets)]=friend;
 }
 
 var finalTarget;
 
 var temp;
 if(GetLength(enemyTargets))
 for(var enem in enemyTargets)
 {
  if(!temp){temp=enem;continue;}
  if(ObjectDistance(enem,pClonk) < ObjectDistance(temp,pClonk))temp=enem;
 }
 finalTarget=temp;
 temp=0;
 
 if(!finalTarget)
 {
 if(GetLength(neutralTargets))
 for(var neut in neutralTargets)
 {
  if(!temp){temp=neut;continue;}
  if(ObjectDistance(neut,pClonk) < ObjectDistance(temp,pClonk))temp=neut;

 }
 finalTarget=temp;
 temp=0;
 }
 
 if(!finalTarget)
 {
 if(GetLength(friendlyTargets))
 for(var friend in friendlyTargets)
 {
  if(!temp){temp=friend;continue;}
  if(ObjectDistance(friend,pClonk) < ObjectDistance(temp,pClonk))temp=friend;
 }
 finalTarget=temp;
 temp=0;
 }
 
 if(!finalTarget)return 0;

 var dis=Distance(GetX(pClonk)+iOutX,GetY(pClonk)+iOutY,GetX(finalTarget)+GetXDir(finalTarget),GetY(finalTarget)+GetYDir(finalTarget));
 var x1, y1, x2, y2;

 x1=GetX(finalTarget);
 y1=GetY(finalTarget);
 x2=GetXDir(finalTarget)*BoundBy((dis/200)-1,0,100);
 y2=GetYDir(finalTarget)*BoundBy((dis/200)-1,0,100);
 iAngle=Angle(GetX(pClonk)+iOutX,GetY(pClonk)+iOutY,
 x1+x2,y1+y2);
 //GetX(finalTarget)+(GetXDir(finalTarget)/Max(1,dis/200)),
 //GetY(finalTarget)+(GetYDir(finalTarget)/Max(1,dis/200)));
// Log("tg:%s angle:%d distance:%d",GetName(finalTarget),Angle(GetX(pClonk),GetY(pClonk),GetX(finalTarget),GetY(finalTarget)),ObjectDistance(pClonk,finalTarget));
 return 1;
}

/* Kann geschmiedet werden */
public func IsAnvilProduct() { return 1; }
public func CarryLimit(){return 1;}
public func IsMusket(){return 1;}
