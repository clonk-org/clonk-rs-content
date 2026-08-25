/* Koralle */

#strict 2

func Hit() { Sound("CoralHit*"); }
public func GetChangeID(){ if(!Random(2)){ return(SPHR); } else { return(COAL); } }
