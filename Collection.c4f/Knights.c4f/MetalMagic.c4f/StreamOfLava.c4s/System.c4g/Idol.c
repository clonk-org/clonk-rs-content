/*-- Satue --*/

#strict 2

#appendto IDOL

public func Construction()
{
  SetClrModulation(RGB(200,40));
}

public func Initialize()
{
  AddEffect("SpawnPoint", this, 1, 10, this);
}

public func GetRandomItem()
{
  var aItems = [STM1, FLNT, SFLN, EFLN, FBMP, SBLP];
  return aItems[Random(GetLength(aItems))];
}

func FxSpawnPointTimer(pTarget, iNumber, iTime)
{
  var pClonk, iAngle;
  if(!EffectVar(0, pTarget, iNumber))
  {
    if(!EffectVar(1, pTarget, iNumber))
    {
      EffectVar(1, pTarget, iNumber) = GetRandomItem();
      SetGraphics(0, this, EffectVar(1, pTarget, iNumber), 1, 1);
      SetObjDrawTransform(1000, 0, 0, 0, 1000,-30000, this, 1);
    }
    for(var i = 0; i<10; i++)
    {
      iAngle = Random(360);
      CreateParticle("FSpark", 0, -30, Sin(iAngle, 10), Cos(iAngle, 10), RandomX(50, 100), RGBa(RandomX(255,188),128,155,0), this, 1);
    }
    if(pClonk = FindObject2(Find_ID(STTK), Find_Distance(30), Find_Func("GetPilote")))
    {
      //  gleich dem Clonk geben
      pClonk = pClonk->~GetPilote();
      CreateContents(EffectVar(1, pTarget, iNumber),pClonk);
      EffectVar(1, pTarget, iNumber) = 0;
      EffectVar(0, pTarget, iNumber) = 40;
      SetGraphics(0, this, 0, 1);
    }
    if(pClonk = FindObject2(Find_OCF(OCF_CrewMember), Find_Distance(10)))
    {
      CreateContents(EffectVar(1, pTarget, iNumber),pClonk);
      EffectVar(1, pTarget, iNumber) = 0;
      EffectVar(0, pTarget, iNumber) = 40;
      SetGraphics(0, this, 0, 1);
    }
  }
  else EffectVar(0, pTarget, iNumber)--;
}
