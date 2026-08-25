/*-- Alkohol --*/

#strict

/* Globale Funktionen */

global func DoAlcohol(object pClonk, int Strength)  // trinkender Clonk, St‰rke des Getr‰nks
{
  // Alkoholeffekt hinzuf¸gen
  AddEffect("Alcohol", pClonk, 101, 1, 0, ALCE, Strength);
}

/* Effekt */

public func FxAlcoholStart(target, no, temp, alcohol) // alcohol = St‰rke des Getr‰nks
{
  if(!temp)
    {
    EffectVar(0, target, no) = alcohol;
    EffectVar(1, target, no) = 0;
    }
}

public func FxAlcoholTimer25(target, no)
{
  // zu viel Alkohol zieht Lebensenergie ab
  if(EffectVar(0, target, no) >= 1400)
    {
    DoEnergy((1400-EffectVar(0, target, no))/10, target);
    EffectVar(0, target, no) = 1399;
    }
  // Verwirrungseffekte
  // wenig Alkohol -> Zielen verruckelt manchmal, Springt verschiedenen weit
  if(GetAction(target) eq "Jump")
    SetXDir(GetXDir(target)+RandomX(-3,3), target);
  // mehr Alkohol -> Manchmal dreht der Clonk um, stolpert er, f‰llt vom Pferd
  if(EffectVar(0, target, no) >= 100 && !Random(15))
    { SetDir(!GetDir(target), target); SetComDir(3+(GetDir(target)*4), target);
    }    
  if(EffectVar(0, target, no) >= 300 && !Random(20) && GetContact(target, 2, 8))
    Fling(target, GetDir(target)*2-1, -1);
  if(EffectVar(0, target, no) >= 500 && !Random(15) && WildcardMatch(GetAction(target),"*Ride*"))
    Fling(target, RandomX(-5,5),-2);
  // viel Alkohol -> er kann nicht mehr richtig springen, manchmal l‰sst er Dinge fallen
  if(EffectVar(0, target, no) >= 700 && !Random(5) && WildcardMatch(GetAction(target),"*Jump*"))
    Fling(target, GetXDir(target)/10);
  if(EffectVar(0, target, no) >= 900 && !Random(25) && Contents(0, target))
    Exit(Contents(0), AbsX(GetX(target)), AbsY(GetY(target))+10, RandomX(-10,10), RandomX(-1,1), 0, RandomX(-10,10));
  // Alkoholeinfluﬂ ist sichtbar
  if(!Contained(target))  CreateParticle("Smoke",GetX(target), GetY(target)-9, RandomX(-3,3), -7,120,RGBa(120,120,120,64));
  // CreateParticle("AlcoholBubble", GetX(target), GetY(target)-9, RandomX(-3,3), -7, 30, RGBa(50,100,155,0));
}

public func FxAlcoholTimer(target, no, time)
{
  if(EffectVar(0, target, no) <= 0) return(-1);
  // Alkoholgehalt nimmt ab
  EffectVar(0, target, no) -= time - EffectVar(1, target, no);
  EffectVar(1, target, no) = time;

  if(!(time%25)) EffectCall(target, no, "Timer25", target, no);
  // Sichtschwankungen
  if(GetCursor(GetOwner(target)) == target)
    {
    SetViewOffset(GetOwner(target), Sin(time*10, EffectVar(0, target, no)/100));
    }
  else
    {
    SetViewOffset(GetOwner(target));
    }
}

public func FxAlcoholEffect(name)
{
  if(WildcardMatch(name, "*Alcohol*")) return(-2);
}

public func FxAlcoholAdd(target, no, name, timer, alcohol)   // Alkoholeffekte ¸berladen sich
{
  EffectVar(0, target, no) += alcohol;
}
