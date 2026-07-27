/*-- Spellstones --*/

#strict 2

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return 1;
}

global func RemoveSpellstone(int iPlr)
{
 if(! ObjectCount(SPST))return 1;
 
 var found=0;
 var stone;
 for(var obj in FindObjects(Find_ID(ALTR),Find_Allied(iPlr)))
 {
  if(!(stone=FindContents(BHLG,obj)))continue;
  CreateParticle("PxSpark",GetX(obj)-GetX(),GetY(obj)-GetY()-10,0,-10,70,RGB(255,255,255),obj);
  RemoveObject(stone);
  obj->RefreshAction();
  found=1;
  break;
 }
 
 return found;
}

