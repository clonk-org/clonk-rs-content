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
  var string = Format("Das Verteidigerteam muss noch %d Minuten und %d Sekunden aushalten.", min, sec);
  MessageWindow(string, iByPlayer);
  return(1);
}
