/*-- Heiligtum --*/

#strict 2

protected func Initialize()
{
  // Steuert das tolle Teamanzeigeobjekt
  AddEffect("TeamEffect",CreateObject(SNHP,0,-50,-1),20,4,this,0);
}

protected func Check()
{
  // Wenn nicht gerade im Wechsel
  if(!GetEffect("ChangeOwner",this))
  {
    // Nach Eroberern suchen
    CheckConquerors();

    // Heilen und Angreifen
    if(!ObjectCount(RSST))
    {
      CheckEnemy();
      CheckHeal();
    }

    // Gold umwandeln
    //ConvertGold();  
 }
}

protected func CheckEnemy()
{
  // Nicht für neutrale Heiligtümer
  if(GetOwner() == -1) return;
  // Opfer für Blitz suchen
  var crews=FindObjects(Find_Distance(LightningDis(),0,0), Find_OCF(OCF_CrewMember),Find_Layer(GetObjectLayer()),Find_Hostile(GetOwner()));
  for(var clnk in crews)
  {
    // Muss leben, nicht schon getroffen werden und erreichbar sein
    if(!GetAlive(clnk)) continue;
    if(GetEffect("StrikeEnemy",clnk)) continue;
    if(!PathFree(GetX(),GetY()-50,GetX(clnk),GetY(clnk))) continue;
    // Helfernachricht
    MMShowHelpMessage("OnEnemySainthood",GetOwner(clnk));
    // Dann los
    AddEffect("StrikeEnemy",clnk,20,33*5,this);
  }
}

protected func ConvertGold()
{
  // Nicht für neutrale Heiligtümer
  if(GetOwner() == -1) return;
 
  // Gold suchen!
  var nGold=3;
  var goldchunks=FindObjects(Find_ID(GOLD),Find_Layer(GetObjectLayer()),Find_Distance(MaxGoldDis(),0,0));
  // überhaupt zu wenige da und so?
  if(GetLength(goldchunks)<nGold)return;
 
  // Gegner mögen wir nicht.
  for( var enem in FindObjects(Find_OCF(OCF_CrewMember),Find_Layer(GetObjectLayer())))
    if(Hostile(GetOwner(),GetOwner(enem)))
      if(ObjectDistance(enem,this)<MaxConquerorDis())
        return;
  
  // Gold mit Partikel entfernen
  for(var cnt=0;cnt<3;cnt++)
  {
    var gold=goldchunks[cnt];
    var x=GetX(gold)-GetX();
    var y=GetY(gold)-GetY();
    CastParticles("FSpark",4,10,x,y,50,100,RGB(200,200,20),RGB(255,255,200));
    CreateParticle("PSpark",x,y,0,0,60,RGB(255,255,50));
    Sound("Puff");
    RemoveObject(gold);
  } 
  
  // Partikel und neues Objekt erzeugen
  CastParticles("FSpark",4,3,0,13,50,100,RGB(200,200,20),RGB(255,255,200));
  CreateParticle("PSpark",0,13,0,0,60,RGB(255,255,50));
  var ID = GetConvertIDs()[Random(GetLength(GetConvertIDs()))];
  Schedule(Format("CreateObject(%i,0,16,GetOwner())",ID),5,0,this);
}

protected func GetConvertIDs()
{
  // ID's, die aus Gold erzeugt werden können
  var aID=[BHLG,BHLG,LFAM,ALC_,BOOK,GBLT,MUSH,FLOU,ARMR,FARP];
  return aID;
}

protected func CheckConquerors()
{
  // Bewacht noch jemand das Heiligtum?
  if(GetOwner()!=-1)
  for(var clnk in FindObjects(Find_Distance(MaxConquerorDis(),0,0), Find_OCF(OCF_CrewMember),Find_Layer(GetObjectLayer()),Find_Allied(GetOwner()),Find_NoContainer()))
  {
    // Auch in Reichweite?
    if(PathFree(GetX(),GetY(),GetX(clnk),GetY(clnk)) 
    || PathFree(GetX(),GetY()-5,GetX(clnk),GetY(clnk)) 
    || PathFree(GetX()-15,GetY()-12,GetX(clnk),GetY(clnk)) 
    || PathFree(GetX()+15,GetY()-12,GetX(clnk),GetY(clnk)))
      return;
  }

  // Sind Feinde da?
  var Hostile = Find_Hostile(GetOwner());
  if(GetOwner()==-1) Hostile = Find_Allied(GetOwner());
  for(var clnk in FindObjects(Find_Distance(MaxConquerorDis(),0,0), Find_OCF(OCF_CrewMember),Find_Layer(GetObjectLayer()),Hostile,Find_NoContainer()))
  {
    // Erreichbar?
    if(!PathFree(GetX(),GetY(),GetX(clnk),GetY(clnk)) 
    && !PathFree(GetX(),GetY()-5,GetX(clnk),GetY(clnk)) 
    && !PathFree(GetX()-15,GetY()-12,GetX(clnk),GetY(clnk)) 
    && !PathFree(GetX()+15,GetY()-12,GetX(clnk),GetY(clnk))) continue;  
 
    // Dann Besitzer wechseln
    AddEffect("ChangeOwner",this,20,2,this,0,FindObjectOwner(0,GetOwner(clnk),0,0,-1,-1,OCF_CrewMember));
    return 1;
  }

  return 0;
}

protected func CheckHeal()
{
  // Nicht für neutrale Heiligtümer
  if(GetOwner() == -1) return;

  // Verbündete Clonks in Reichweite
  for(var clnk in FindObjects(Find_Distance(MaxHealDis()), Find_OCF(OCF_CrewMember),Find_Layer(GetObjectLayer()),Find_Allied(GetOwner())))
  {
    // Wird noch nicht geheilt und braucht Energie
    if(GetEffect("SainthoodHealing",clnk)) continue;
    if(GetEnergy(clnk) == GetPhysical("Energy",0,clnk)/1000) continue;
    // Helfernachricht
    MMShowHelpMessage("OnFriendlySainthood",GetOwner(clnk));
    // Heilen!
    AddEffect("SainthoodHealing",clnk,20,3,this,0,this);
  }
}

// Konstanten
public func LightningDis()    { return MaxConquerorDis(); }
public func MaxGoldDis()      { return 40; }
public func MaxHealDis()      { return 100; }
public func MaxConquerorDis() { return 50; }
public func ConquerorTime()
{
  var i;
  if(i=GameCall("SainthoodConquerTime")) return i;
  return 1000;
}

// Effekte!

// Gegner aua machen!
protected func FxStrikeEnemyTimer(object pTarget, int iEffectNumber,int iEffectTime)
{
  // Nicht weglaufen!
  if(ObjectDistance(pTarget,this)>LightningDis()) return -1;
  // Nicht weg sein!
  if(!pTarget || !GetAlive(pTarget)) return -1;
  // Nicht verstecken!
  if(Contained(pTarget)) return -1;
  // Nicht froindlich sein!
  if(!Hostile(GetOwner(this),GetOwner(pTarget))) return -1;
 
  // Aua machen!
  for(var cnt=1+Random(3);cnt;cnt--)
    DrawLightning(GetX(),GetY()-50,GetX(pTarget)+RandomX(-4,4),GetY(pTarget)-4+RandomX(-4,4));
  Punch(pTarget,3+Random(3));
  CreateParticle("PSpark",0,-50,0,0,250,RGBa(230,230,255,100));

  return 1;
}

// Teamanzeigen!
protected func FxTeamEffectTimer(object pTarget, int iEffectNumber,int iEffectTime)
{
  // Tests
  if(!pTarget || !this) return -1;
  if(GetOwner(this) == -1) return 0;
  // Besitzer wechseln?
  if(GetOwner(this) != GetOwner(pTarget))
    SetOwner(GetOwner(this),pTarget);
 
  // Partikeleffekt
  for(var cnt=0;cnt<360;cnt+=90)
  {
    CreateParticle("PSpark",Cos(cnt+iEffectTime%360,10),-50+Sin(cnt+iEffectTime%360,3),0,0,25,RGBa(255,255,50,100),pTarget);
    CreateParticle("PSpark",Cos(cnt+90+iEffectTime%360,3),-50+Sin(cnt+90+iEffectTime%360,10),0,0,25,RGBa(255,255,50,100),pTarget);
  }

  return 1;
}

protected func FxTeamEffectStop(object pTarget, int iEffectNumber, int iReason, bool fTmp)
{
  if(fTmp) return;
  // und am Ende Entfernen
  if(pTarget)
    RemoveObject(pTarget);
}

// Heilen!
protected func FxSainthoodHealingStart(object pTarget, int iEffectNumber, int iTemp,  var1)
{
  if(iTemp) return;
  // Objekt für Partikel-Attach speichern
  EffectVar(0, pTarget, iEffectNumber) = var1;
}

protected func FxSainthoodHealingTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  // Nicht weglaufen!
  if(ObjectDistance(pTarget,this)>MaxHealDis()) return -1;
  // Nicht weg sein!
  if(!pTarget || !GetAlive(pTarget)) return -1;
  // Nicht verstecken!
  if(Contained(pTarget)) return -1;
  // Nicht kämpfen!
  if(GetProcedure(pTarget)=="FIGHT") return -1;
  // Nicht heil sein!
  if(GetEnergy(pTarget) == GetPhysical("Energy",0,pTarget)/1000) return -1;
  // Nicht feindlich sein!
  if(Hostile(GetOwner(this),GetOwner(pTarget))) return -1;

  // Heilen
  if(iEffectTime%21==0 && Random(10)) DoEnergy(1,pTarget);
 
  // PArticlez!
  for(var cnt=5;cnt;cnt--)
   CreateParticle("PSpark",GetX(pTarget)-GetX()+RandomX(-6,6),GetY(pTarget)-GetY()+RandomX(-5,5),
     0,-1-Random(2),10,RGB(255,255,50),pTarget,Random(2));
  CreateParticle("PSpark",GetX(pTarget)-GetX(),GetY(pTarget)-GetY(),
    0,0,300,RGBa(255,255,50,150),EffectVar(0, pTarget, iEffectNumber));
 
  // Fertig
  return 1;
}

// Einnehmen
protected func FxChangeOwnerStart(object pTarget, int iEffectNumber, int iTemp,  var1)
{
  if(iTemp) return;
  // Eroberer speichern
  EffectVar(0, pTarget, iEffectNumber)=var1;
}

protected func FxChangeOwnerTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  // Bei zu grosser Entfernung einen kleinen Buffer gewähren
  var tooFar = false;
  if(ObjectDistance(EffectVar(0, pTarget, iEffectNumber),this)>MaxConquerorDis())
  {
    // Zeit speichern
    if(!EffectVar(2, pTarget, iEffectNumber))
      EffectVar(2, pTarget, iEffectNumber) = iEffectTime;
    else
    {
      // Maximalzeit herausfinden
      var iMaxTime=GameCall("SainthoodBufferTime");
      if(!iMaxTime)
        iMaxTime=35*5;
    
      // Überschritten?
      if(iEffectTime - EffectVar(2, pTarget, iEffectNumber) > iMaxTime)
        tooFar=true;
    }
  }
  // sonst Zeit zurücksetzen
  else
    if(EffectVar(2, pTarget, iEffectNumber))
      EffectVar(2, pTarget, iEffectNumber)=0;
  
  // Der Clonk darf nicht weglaufen
  if(!GetAlive(EffectVar(0, pTarget, iEffectNumber)) || Contained(EffectVar(0, pTarget, iEffectNumber)) || tooFar)
  {
    // und wenn, dann hat er sicher Froinde!
    var found=0;
    var clnk = FindObject2(Find_Allied(GetOwner(EffectVar(0, pTarget, iEffectNumber))),Find_Layer(GetObjectLayer()), Find_NoContainer(), Find_OCF(OCF_CrewMember), Find_Distance(MaxConquerorDis()));
    EffectVar(0, pTarget, iEffectNumber) = clnk;
    // Nicht weitermachen, keine Froinde :C
    if(!clnk)
      return -1;
  }
 
  // Effekt!
  for(var cnt=Random(iEffectTime/(ConquerorTime()/20));cnt;cnt--)
    CreateParticle("PSpark",RandomX(-25,25),RandomX(-5,10),0,-1-Random(2),10,RGB(255,255,50),this);
 
  // Fertig?
  if(iEffectTime>ConquerorTime())
  {
    // Besitzer wechseln und Effekt!
    SetOwner(GetOwner(EffectVar(0, pTarget, iEffectNumber)),this);
    GameCall("OnSainthoodOwnerChange",this);
    for(var cnt=Random(iEffectTime/20);cnt;cnt--)
      CreateParticle("PSpark",RandomX(-25,25),RandomX(-5,10),0,-1-Random(3),18,RGB(255,255,50),this);
    return -1;
  }
 
  // alles toll!
  return 1;
}
