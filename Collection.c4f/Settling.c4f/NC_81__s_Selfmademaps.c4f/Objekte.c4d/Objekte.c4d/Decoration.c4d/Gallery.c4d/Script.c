/*-- Bergwerksstütze --*/

#strict

protected func Initialize()
{
  // Zufällige Grafik wählen
  if(Random(4))
    SetGraphics(Format("%d", Random(3)+1));
  // Aktion setzen
  SetAction("Stand");
  return(1);
}

