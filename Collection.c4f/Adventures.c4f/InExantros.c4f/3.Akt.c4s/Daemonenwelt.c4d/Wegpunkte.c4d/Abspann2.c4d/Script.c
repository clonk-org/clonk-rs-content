#strict

Initialize:
  SetComDir(COMD_Down());
  SetYDir(1);
  SetAction("Speak");
  return(1);

Text:
  if(GreaterThan(GetActTime(),5000))  RemoveObject();
  SetComDir(COMD_Down());
  Message("@Durch die Zerstörung des Kristalls floh das Böse aus dem Land und es kehrte wieder Frieden in Exantros ein.",this());
  return(1);