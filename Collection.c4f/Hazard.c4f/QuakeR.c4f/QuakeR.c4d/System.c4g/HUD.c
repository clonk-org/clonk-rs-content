/*-- HUD --*/

#strict 2
#appendto 1HUD

private func InitAmmoBagHUD() { ScheduleCall(this,"InitAmmoBagHUD2",1); }

private func InitAmmoBagHUD2() {
	// remove ammobag display
	allAmmo = CreateArray();
	
	// hardcode. :I
	var ammos = [QSQS,QMGS,QSGS,QGLS,QPGS,QRGS,QRLS];
	// cycle through every ammo and see if it's available in this map
	for(var a in ammos)
		if(FindObject( C4Id(Format("Q_%c%c", GetChar(Format("%i",a),1), GetChar(Format("%i",a),2) )) ))
			allAmmo[GetLength(allAmmo)] = a;
	
	ammoobjs = CreateArray();
	
	// place all ammo objects
	for(var i=0; i < GetLength(allAmmo); ++i) {
	
		// ...now
		ammoobjs[i] = CreateObject(2HUD,0,0,GetOwner());
		ammoobjs[i]->SetPosition(70,-85-i*28);
		ammoobjs[i]->SetGraphics(0, 0, allAmmo[i],1,4);
		ammoobjs[i]->SetObjDrawTransform(750,0,-50000,0,750,-62000,0,1);
		ammoobjs[i]->SetVisibility(VIS_None);
	}

}
