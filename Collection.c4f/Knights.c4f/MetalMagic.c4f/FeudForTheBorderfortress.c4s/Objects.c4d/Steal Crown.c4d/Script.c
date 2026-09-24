/*-- Kronenklau --*/

#strict



public func IsFulfilled()
{
    if(! FindObject(_CRN))
    {
        GameCall("Eliminate", 0);
        return(1);
    }
}

public func Activate(iByPlayer)
{
  var TimeToGo;
  TimeToGo = FindObjectOwner(_ZAE, iByPlayer) -> LocalN("time");
  var sec = TimeToGo % 60;
  var min = TimeToGo / 60;
  var string = Format("$Crown01$", min, sec);
  MessageWindow(string, iByPlayer);
  return(1);
}
