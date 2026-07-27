/* Frostwelle */

#strict 2

func Activate(pCaster,pRealcaster) {
  var pClonk, pComboObj, fBlast;
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

  // Zielen wenn möglich
  if (pCaster->~DoSpellAim(this)) return 1;
  
  var iNum = AddEffect("FrostbreezeNSpell",0,130,1,0,GetID(),0,GetX(pCaster),GetY(pCaster),pCaster);
  if (iNum <= 0) { RemoveObject(); return iNum<0; }
  Sound("MgWind*");
  RemoveObject();
  return 1;
}

// Aktivierung nach Zielen (iAngle: -90=links; 0=oben; +90=rechts)
public func ActivateAngle(object pCaller, int iAngle)
  {
  // its magic!
  var iNum = AddEffect("FrostbreezeNSpell",0,130,1,0,GetID(),iAngle-90,GetX(pCaller),GetY(pCaller),pCaller);
  if (iNum <= 0) { RemoveObject(); return iNum<0; }
  Sound("MgWind*");

  // Objekt löschen
  RemoveObject();
  return 1;
  }

func FxFrostbreezeNSpellStart(object pTarget, int iNumber, int iTemp, int iAngle, int iX, int iY, object pCaller) {
  if(iTemp) return;

  // Radius anpassen
  EffectVar(1,pTarget,iNumber) = 20;
  
  // Winkel speichern
  EffectVar(4,pTarget,iNumber) = iAngle;
  
  // Position
  EffectVar(2,pTarget,iNumber) = iX;
  EffectVar(3,pTarget,iNumber) = iY;
  
  // Caster speichern
  EffectVar(5,pTarget,iNumber) = pCaller;
}

func FxFrostbreezeNSpellTimer(object pTarget, int iNumber, int iTime) {

  var iRaduis = EffectVar(1,pTarget,iNumber);
  var iAngle = EffectVar(4,pTarget,iNumber);
  
  if (iAngle < 0)
   iAngle+=360;
   
  var pCaller = EffectVar(5,pTarget,iNumber);
  // löschen falls schon zu weit
  if(iRaduis >= EffectCall(pTarget,iNumber,"MaxRange")) return -1;

  // Position
  var iX = EffectVar(2,pTarget,iNumber);
  var iY = EffectVar(3,pTarget,iNumber);
  
  // Winkel
  var xDir = Cos(iAngle,iRaduis);
  var yDir = Sin(iAngle,iRaduis);
  
  // Effekt!
  CreateParticle("FrostBreeze",iX,iY, xDir+RandomX(-2,2) ,yDir+RandomX(-2,2), 400+10*iRaduis,RGBa(255,255,255,50));
var obj;

  var angle;
  // Alle Gegner wegschleudern
  while(obj = FindObject(0, -iRaduis-5+iX,-iRaduis-5+iY,iRaduis*2+10,iRaduis*2+10, OCF_Collectible|OCF_Alive, 0,0, NoContainer(), obj))
  {
   
    if(obj == Contained()) continue;
        if(GetOCF() & OCF_InSolid) continue;
    
    var oAng=Angle(iX,iY,GetX(obj),GetY(obj))-90;
    if(!Inside(oAng,iAngle-40,iAngle+40)) continue;    
    
    if(Inside(Distance(GetX(obj), GetY(obj), iX, iY), iRaduis-5, iRaduis+5))
    {   
    angle=Angle(iX, iY, GetX(obj), GetY(obj));      
    if(GetAlive(obj)) 
    { 
        Fling(obj, Sin(angle, 80)/12, -Cos(angle, 80)/19);
        if(!Frozen(obj)) Freeze(obj, pCaller); 
    }
    else { SetXDir(Sin(angle, 20), obj); SetYDir(-Cos(angle, 20), obj);}
    }
  } 
    
  EffectVar(1,pTarget,iNumber)+=8;
}

func FxFrostbreezeNSpellMaxRange() { return 150; }

/* Zaubercombo */

public func GetSpellStaff(object pMage) { return SMWT; }
public func GetSpellStaffCombo(pMage) { return ("33"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
