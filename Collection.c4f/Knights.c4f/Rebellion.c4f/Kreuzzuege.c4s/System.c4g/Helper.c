
#strict

global func IsFood(object pObj)
{
  var id = GetID(pObj);
  return(id == BRED || id == COKI || id == CBRD || id == CSNK || id == MUSH);
}

