/*-- Baumsteuerung append --*/

#strict
#appendto TREE

/* Überladbare Konstanten */

private func ReproductionAreaSize() { return(800); } // Größe des Bereichs, in dem neue Bäume dieser Art entstehen können
private func ReproductionRate()     { return(250); } // Die Chance, dass in einem Timerintervall eine Vermehrung stattfindet
private func MaxTreeCount()         { return(80); } // Maximale Baumanzahl im Umkreis