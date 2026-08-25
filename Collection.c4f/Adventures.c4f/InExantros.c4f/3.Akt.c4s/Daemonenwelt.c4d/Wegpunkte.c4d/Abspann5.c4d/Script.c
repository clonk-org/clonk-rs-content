#strict

Initialize:
  SetComDir(COMD_Down());
  SetYDir(1);
  SetAction("Speak");
  return(1);

Text:
  if(GreaterThan(GetActTime(),5000))  RemoveObject();
  SetComDir(COMD_Down());
  Message("@Ich danke  all meinen Betatestern:|Sven2(Sven Eberhardt),|Dominik1(Dominik Oman),|BlackTempler(Mike Komaromi),|Knufflwipf(Florian Graier),|Goldständer(Lukas Klier),|LoneS(Johannes Henriksson),|JimiRaynor(Kevin Porebski),|Killerwipf(Oliver Sebert)",this());
  return(1);