/*-- Waffenpläne --*/

#strict
#appendto WPPL

public func Activate(pByObject)
{               
  _inherited(pByObject);
  RemoveObject();
  return(1);
}

