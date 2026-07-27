/*-- Verbrannter Burgfahrstuhl --*/

#strict 2
#include PPN2

local pCase;

func Incineration() { 
	if(pCase) RemoveObject(pCase); 
	return _inherited();
}
func IncinerationEx() { 
	if(pCase) RemoveObject(pCase); 
	return _inherited();
}
func Destruction() { 
	if(pCase) RemoveObject(pCase); 
	return _inherited();
}
