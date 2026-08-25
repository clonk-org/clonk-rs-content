#strict

#appendto _BAD

public func Researched(iOwner) // int iOwner;
{
	SetPlrKnowledge(iOwner,GetID()); // Metallurgie
	SetPlrKnowledge(iOwner,_F1A); // Eisenhütte
	SetPlrKnowledge(iOwner,_K1A); // Schmiede
	SetPlrKnowledge(iOwner,LORY); // Lore
	SetPlrKnowledge(iOwner,BARL); // Faß
	SetPlrKnowledge(iOwner,_Y3B); // Feuerlöscher
	SetPlrKnowledge(iOwner,_N3A); // Metropole
	return true;
}
