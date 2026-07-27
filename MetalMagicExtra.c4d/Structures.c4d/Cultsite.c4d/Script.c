/*-- Kultstätte --*/

#strict 2

protected func Completion()
{
  // Nach dem Bauen Clonks nach oben versetzen? 
  for(var clnk in FindObjects(Find_Distance(75,0,0),Find_Or(Find_OCF(OCF_Alive),Find_Category(16))))
  {
    var cnt=0;
    while(cnt < 50 && Stuck(clnk)){SetPosition(GetX(clnk),GetY(clnk)-1,clnk);cnt++;}
    // Nicht, dass es nachher in irgendeine Decke versetzt..
    if(Stuck(clnk))SetPosition(GetX(clnk),GetY(clnk)+cnt,clnk);
  }
}

public func ControlDig(pClonk)
{
  if(GetOwner() != -1)return;
  SetOwner(GetOwner(pClonk),this);
  return 1;
}

/* Kontext */
public func ContextRitual(object caller)
{
  [$TxtSummon$|Image=CLST]
  ControlUp(caller);
}

public func ControlUp(pClonk)
{
  if(GetAction(pClonk) != "Grab")
    if(GetActionTarget(0, pClonk) != this)
    {
      SetCommand(pClonk, "Call", this, 0, 0, 0, "ControlUp");
      AddCommand(pClonk, "Grab", this);
      return;
    }
  CreateMenu(GetID(),pClonk,this, C4MN_Extra_Components, "$TxtNoRituals$");
  
 // Helfernachricht
 MMShowHelpMessage("OnCultsiteMenu",GetOwner(pClonk));
  
  var idRitual, i;
  var iChurch = MMGetChurch(GetOwner(pClonk));
  var iCount;
  // Alle Baupläne durchsuchen
  while(idRitual = GetPlrKnowledge(GetOwner(pClonk), 0, i++, C4D_All))
  {
    // Rituale rausfiltern
    if(idRitual->~IsRitual())
    {
      // Nur bei passender Kirche aufnehmen
      if(idRitual->~NeedChurch())
      {
        if(iChurch == idRitual->~NeedChurch()) AddMenuItem("$TxtSummon$ %s","Summon",idRitual,pClonk,0,pClonk);
      }
      else AddMenuItem("$TxtSummon$: %s","Summon",idRitual,pClonk,0,pClonk);
      iCount++;
    }
  }
  // Wenn keine als Baupläne da sind, dann werden alle möglichen genommen
  // - deaktiviert -
  if(!iCount && 0)
  {
    // Alle Definitionen durchsuchen
    while(idRitual = GetDefinition(i++, C4D_All))
    {
      // Rituale rausfiltern
      if(idRitual->~IsRitual())
      {
        // Nur bei passender Kirche aufnehmen
        if(idRitual->~NeedChurch())
        {
          if(iChurch == idRitual->~NeedChurch()) AddMenuItem("$TxtSummon$ %s","Summon",idRitual,pClonk,0,pClonk);
        }
        else AddMenuItem("$TxtSummon$ %s","Summon",idRitual,pClonk,0,pClonk);
        iCount++;
      }
    }
  }
  return 1;
}

public func Summon(idID, pClonk, fNoCheck)
{
  // Verfügbarkeit überprüfen
	if(!fNoCheck)
	{
		var pTemp;
		if(!ObjectCount(ALCO)) pTemp = CreateObject(ALCO,10,10,-1);
		var fCheck = CheckAlchem(idID, pClonk);
		if(!fCheck) {
			if(pTemp) RemoveObject(pTemp);
			Message("$MsgNotEnoughIngredients$",pClonk,GetNeededAlcStr(idID,pClonk));
			return 0;
		}
		// Zutaten rausfischen
		ReduceAlchem(idID,pClonk);
		if(pTemp) RemoveObject(pTemp);
	}
  // Und Starten
  AddEffect("Summon", this, 1, 1, this, 0, idID, pClonk);
}

public func DoSummon(idID, pClonk)
{
  if(idID)
    CreateObject(idID, 0,9,GetOwner(pClonk))->~Summoned(pClonk);
}

func FxSummonStart(pTarget, iNumber, fTmp, idID, pClonk)
{
  if(fTmp) return;
  EffectVar(0, pTarget, iNumber) = idID;
  EffectVar(1, pTarget, iNumber) = pClonk; 
}

func FxSummonTimer(pTarget, iNumber, iTime)
{
  if(iTime>=MaxSummonTime()) return -1;
  if(iTime % 2 == 0) return;
  
  var aX=[-23,-8,22];
  var aY=[-31+5,-31+5,-34+5];
  
  var y=-45+iTime/18;
  
  for(var i=0;i<GetLength(aX);i++)
  {
   var lgh=DrawLightning(GetX()+aX[i],GetY()+aY[i],GetX()-5,GetY()+y);
   SetClrModulation(HSL(120*i,200,200),lgh);
  }
  
  if(Sin(iTime*4,10-y/2) < 5)
   CreateParticle("PSpark",Cos(iTime*4,10-y/2),Sin(iTime*4,10-y/2),0,0,40+iTime/8,HSL(Abs(Cos(iTime*4,255)),200,200));
  
  for(var i=0;i<5;i++)
  {
   var rand=RandomX(-20,5);
   CreateParticle("PSpark",RandomX(-y/2,y/2),rand,0,-1,30+iTime/20,HSL(Abs(rand)*12,200,200));
  }
  
  /*var i = 0, iAngle = iTime+iTime*4*iTime/MaxSummonTime(), pObj1, pObj2;
  var iHigh = 30*iTime/MaxSummonTime();
  while(i++ < 10)
  {
    pObj1 = 0; if((iAngle/180)%2) pObj1 = pTarget;
    pObj2 = 0; if(!((iAngle/180)%2)) pObj2 = pTarget;
    CreateParticle("MSpark",-Cos(iAngle, 40), 10-35*i/10, 0, -10, 50-30*i/10, RGB(100,0,255), pObj1, (iAngle/180)%2);
    CreateParticle("MSpark",-Cos(iAngle+180, 40), 10-35*i/10, 0, -10, 50-30*i/10, RGB(0,100,255), pObj2, !((iAngle/180)%2));
    iAngle+=18;
  }
  var iHeight = 80-80*iTime/MaxSummonTime();//60+Sin(iTime*2, 20);
  CreateParticle("NoGravSpark",-Cos(iTime*4, 40), 15, 0, 0, 50, RGB(0,0,255), pTarget, (iTime*4/180)%2);
  CreateParticle("NoGravSpark", Sin(-15, 25), -30,-Sin(-15, 5), -iHeight/8, 50, RGB(80,80,200), pTarget, 1);
  CreateParticle("NoGravSpark", Sin(120, 25), -30,-Sin(120, 5), -iHeight/8, 50, RGB(100,0,200));
  CreateParticle("NoGravSpark", Sin(240, 25), -30,-Sin(240, 5), -iHeight/8, 50, RGB(0,100,200));
  
  var iRand = iTime*10;
  var iDist = 30-iTime*30/MaxSummonTime();
  var iSize = 80*iTime/MaxSummonTime();
  CreateParticle("NoGravSpark", Sin(iRand, iDist), Cos(iRand, iDist)-iHeight,0, 0, iSize, RGB(80,80,255));
  CreateParticle("NoGravSpark", 0,-iHeight,-Sin(iRand, iDist/2),-20,80-iSize, RGBa(80,200,255,128));*/
  //AddEffect("Summon", this, 1, 1, this, 0, PLDN, FindObject(PLDN));
  
  /*var x=Cos(iTime*(3),40);
  var y=Sin(iTime*(4),30)-13;
  
  if(Inside(x,-5,5) && Inside(y,-13-5,-13+5))
   CreateParticle("Fire",0,-5,0,0,iTime/2,RGB(25,255,25));
  else
  CreateParticle("Fire2",0,-5,0,0,iTime/2,RGB(255,255,255));
  
  if(iTime % 3 == 0)
  for(var i=-38+Random(3);i<38;i+=7)
  {
   CreateParticle("PSpark",i+RandomX(-3,3),8,0,-Random(4),60,RGBa(200+Random(55),10,10,50),this,0);
   CreateParticle("Fire",i+RandomX(-3,3),8,0,-Random(4),60,RGBa(200+Random(50),200+Random(50),200+Random(50),50));
   CreateParticle("Fire2",i+RandomX(-3,3),8,0,-Random(4),60,RGBa(200+Random(50),200+Random(50),200+Random(50),50));
  }

  
  if(iTime < MaxSummonTime()-70)
  {
   CreateParticle("PSpark",x,y,0,0,60,RGB(255,10,10),this,(y>9));
   CreateParticle("Fire2",x,y,0,0,60,RGB(255,255,255),this,(y>9));
   
   CreateParticle("PSpark",-x,y,0,0,60,RGB(10,255,10),this,(y>9));
   CreateParticle("Fire",-x,y,0,0,60,RGB(30,255,30),this,(y>9));
  }*/
}

func MaxSummonTime() { return 36*10*2; }

func FxSummonStop(pTarget, iNumber, iReason, fTmp)
{
  if(fTmp) return;
  pTarget->~DoSummon(EffectVar(0, pTarget, iNumber), EffectVar(1, pTarget, iNumber));
  
  for(var cnt=180;cnt<360;cnt+=7)
  {
   CreateParticle("PSpark",0,0,Cos(cnt,10),Sin(cnt,10),50,RGB(200,200,255));
   CreateParticle("PSpark",0,0,Cos(cnt,8),Sin(cnt,8),50,RGB(160,160,255));
  }
}
