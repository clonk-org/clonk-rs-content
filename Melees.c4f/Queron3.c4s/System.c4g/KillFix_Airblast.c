/* Windstoss: Killverfolgung */

#strict

#appendto ABLA

func FxAirblastNSpellTimer(object pTarget, int iNumber, int iTime) {
  var iPos, pObj, iX, iY, iSpeed, iRandom, iAngle, iSize, iStep;

  var iPos = EffectVar(1,pTarget,iNumber);
  // löschen falls schon zu weit alles weggeblasen
  if(iPos >= EffectCall(pTarget,iNumber,"MaxRange")) return(-1);

  iStep = -iPos/18+EffectCall(pTarget,iNumber,"MaxRange")/17;

  iX = EffectVar(2,pTarget,iNumber);
  iY = EffectVar(3,pTarget,iNumber);

  iAngle = EffectVar(0,pTarget,iNumber);
  iSize = iStep+iPos/5;
  
  iX += Sin(iAngle, iPos);
  iY -= Cos(iAngle, iPos);

  //Log("%d,%d",iSize,iStep);


  //  Log("%d,%d,%d,%d",iX+Sin(iAngle,iPos)-iSize,iY-Cos(iAngle,iPos)-iSize,iSize*2,iSize*2);

  // Objekte wegblasen
  while(pObj=FindObject(0,iX-iSize,iY-iSize,iSize*2,iSize*2,OCF_Collectible()|OCF_Alive(),0,0,NoContainer(),pObj))
    {
    if(Stuck(pObj)) continue;
    if (GetProcedure(pObj) eq "ATTACH") continue; // Keine getragenen Schilde, Alchemiebeutel, etc.
    iSpeed=iStep*6;
    if(iSpeed<0) iSpeed=0;
   
    // Sound fürs Wegblasen
    Sound("MgWndB", false, pObj);

    //Log("%s: %d; %d,%d",GetName(pObj),iSpeed,Sin(iAngle,iSpeed+20),-iSpeed);

    if(GetAlive(pObj))
      {
      iRandom=Random(3);
      //DoEnergy(-1-iRandom,pObj);
      //pObj->~CatchBlow(-1-iRandom,this());
      pObj->SetKiller(EffectVar(6,pTarget,iNumber));
      Fling(pObj,Sin(iAngle,iSpeed+20)/9,-iSpeed/2/9);
      }
    else
      {
       SetSpeed(Sin(iAngle,iSpeed+20),-iSpeed/2,pObj);
       
       // Richtigen Controller setzen
       SetController(EffectVar(6,pTarget,iNumber),pObj);
      }
    }
    
  // Explosionen bei Kombo
  var iMat;
  if (EffectVar(4, pTarget, iNumber))
    if (GBackSolid(iX, iY))
      {
      iMat = GetMaterial(iX,iY);
      if (iMat >= 0 && iMat != Material("Vehicle"))
        {
        Explode(17, CreateObject(ROCK, iX,iY,EffectVar(5, pTarget, iNumber)));
        return(-1);
        }
      }

  var clr = iPos*255/EffectCall(pTarget,iNumber,"MaxRange");
  CreateParticle("PSpark",iX,iY,0,0,iSize*15,RGBa(200,200,255,clr*127/255));

  // alle 20 Pixel wird nach Objekten gesucht
  EffectVar(1,pTarget,iNumber)+=iStep;
  while(pObj=FindObject(0,iX-iSize,iY-iSize,iSize*2,iSize*2,OCF_Alive(),0,0,NoContainer(),pObj)) {
  	if(GetKiller(pObj)!=EffectVar(6,pTarget,iNumber)) {
  		DoEnergy(-1,pObj, 1,,EffectVar(6,pTarget,iNumber)+1);
  		DoEnergy(+1,pObj, 1,,EffectVar(6,pTarget,iNumber));
  	}
	}
}
