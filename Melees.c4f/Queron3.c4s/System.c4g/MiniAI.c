/*-- Mini KI v1.0 --*/

// by Miniwipf

#strict 2

#appendto MCLK

// für Magie

local AI_pSpellTarget, AI_idNextSpell;

global func AddMiniAI(int dType, int iL, int iR, object pThrone, int iXDBoard, int iJumpDir, object pAnvil, array aSpellIDs)
{
	var nr = AddEffect("AIBase", this, 100, 35,, MCLK);
	if (!nr) return 0;
	EffectVar(0, this, nr) = dType; // Bit|Value: 1|1: WalkSlowly, 2|2: Throne, 3|4: GoSwimming, 4|8: Anvil, 5|16: CastSpell, 6|32: Hide
	EffectVar(1, this, nr) = [iL, iR, pThrone, iXDBoard, iJumpDir, pAnvil, aSpellIDs];
	EffectVar(2, this, nr) = [0,0,0,0,0,0]; // start prio
	EffectVar(3, this, nr) = [160,100,100, 300, 100, 10]; // ranges
	EffectVar(4, this, nr) = 0; // bisherige aktion
	EffectVar(5, this, nr) = [0, 0, 0, 0, 0]; // data für jede Aktion
	//EffectVar(5, this, nr) = 0; // nächste aktion
}

public func FxAIBaseTimer(object pTarget, int iNr, int iTime)
{
	var type = EffectVar(0, pTarget, iNr);
	var l = EffectVar(1, pTarget, iNr)[0];
	var r = EffectVar(1, pTarget, iNr)[1];
	var throne = EffectVar(1, pTarget, iNr)[2];
	var board =	EffectVar(1, pTarget, iNr)[3];
	var dir = EffectVar(1, pTarget, iNr)[4];
	var anvil = EffectVar(1, pTarget, iNr)[5];
	var spells = EffectVar(1, pTarget, iNr)[6];
	
	var prev = EffectVar(4, pTarget, iNr);
	var next = -1, bits = 0;
	// welche Aktion hätte die Höchste prio?
	for (var i=0; i < GetLength(EffectVar(2, pTarget, iNr)); i++)
	{
		if (type & SetBit(bits, i, 1))
			if (next<0 || EffectVar(2, pTarget, iNr)[i]>EffectVar(2, pTarget, iNr)[next])
				next = i;
	}
	// gegen vorherige abmessen:
	if (EffectVar(2, pTarget, iNr)[next] <= (EffectVar(2, pTarget, iNr)[prev]+EffectVar(3, pTarget, iNr)[prev]))
		next = prev; // alte aktion immer noch besser
	
	/////////////////////////////////// Walk Slowly //////////////////////////////
	if (next == 0)
	{
		if (!Random(6))
		{
			SetCommand(pTarget, "MoveTo",, RandomX(l, r), pTarget->GetY());
			EffectVar(2, pTarget, iNr)[0] -= 5+Random(10);
		}
		else if (!Random(5))
		{
			pTarget->SetDir(!GetDir(pTarget));
		}
		else if (!Random(20))
		{
			// motivationsspruz
			pTarget->SetAction("Jump");
			pTarget->SetYDir(-25);
			pTarget->SetXDir(0);
			EffectVar(2, pTarget, iNr)[0] += 50+Random(100);
			if (!Random(2))
				Message("<c %x>%s</c>", pTarget, GetColorDw(pTarget), ["$MsgJump0$", "$MsgJump1$", "$MsgJump2$", "$MsgJump3$"][Random(4)]);
		}
		if (!Random(20))
				Message("<c %x>%s</c>", pTarget, GetColorDw(pTarget), ["$MsgWalk0$", "$MsgWalk1$", "$MsgWalk2$", "$MsgWalk3$"][Random(4)]);
	}
	else EffectVar(2, pTarget, iNr)[0]+=10;
	
	/////////////////////////////////// Throne ///////////////////////////////////
	if (next == 1)
	{
		if (!Random(3))
		{
			SetCommand(pTarget, "Enter", throne);
			EffectVar(2, pTarget, iNr)[1] -= 20+Random(20);
		}
	}
	else EffectVar(2, pTarget, iNr)[1]+=10;
	
	///////////////////////////////// GoSwimming /////////////////////////////////
	if (next == 2)
	{
		if (Inside(pTarget->GetX(), board-2, board+2))
		{
			// phu, vorbereiten zum sprung
			if (pTarget->GetDir() != dir)
			{
				pTarget->SetDir(dir);
			}
			// und SPRUNG!
			else
			{
				if (!Random(2))
				{
					SetCommand(pTarget, "Jump");
					EffectVar(2, pTarget, iNr)[2] -= 30+Random(60);
				}
				else
				{
					Message("<c %x>%s</c>", pTarget, GetColorDw(pTarget), ["$MsgNoJump0$", "$MsgNoJump1$", "$MsgNoJump2$", "$MsgNoJump3$"][Random(4)]);
					pTarget->SetDir(!GetDir(pTarget));
				}
			}
		}
		// raufschwimmen
		else
		{
			SetCommand(pTarget, "MoveTo",, board, pTarget->GetY());
		}
	}
	else EffectVar(2, pTarget, iNr)[2]+=10;
	
	////////////////////////////////// Anvil /////////////////////////////////////
	if (next == 3)
	{
		// gerade am arbeiten
		if (Contained(pTarget))
		{
			EffectVar(2, pTarget, iNr)[3] -= Random(60);
			if (!Random(10))
				Message("<c %x>%s</c>", pTarget, GetColorDw(pTarget), ["$MsgAnvil0$", "$MsgAnvil1$", "$MsgAnvil2$", "$MsgAnvil3$"][Random(4)]);
		}
		else
		{
			if (GetAction(pTarget) != "Push" || GetActionTarget(0, pTarget)!=anvil)
			{
				SetCommand(pTarget, "Grab", anvil);
			}
			else
			{
				// Was herstellen?
				var productID = SPRK; // nix, nur ein fünkchen
				if (!Random(120)) productID = ARMR;  // Rüstung
				else if (!Random(100)) productID = FARP; // Feuerpfeile
				else if (!Random(90)) productID = SHIE; // Ein Schild
				else if (!Random(80)) productID = SPER; // Pfeile
				else if (!Random(70)) productID = ARWP; // Pfeile
				// Kosten sponsern
				var j=0, compID;
				while (compID = GetComponent(0,j++,0, productID))
  			{
  				CreateContents(compID, anvil, GetComponent(compID,,, productID));
				}
				AddCommand(pTarget, "Call", anvil, productID, 0, 0, 0, "ProductionStart", 0, 3);
			}
		}
	}
	else EffectVar(2, pTarget, iNr)[3]+=10;
	
	//////////////////////////////// CastSpell ///////////////////////////////////
	if (next == 4)
	{
		if (!LocalN("AI_idNextSpell", pTarget))
			LocalN("AI_pSpellTarget", pTarget) = 0;
		// Falls nicht gerade am Zaubern
		if (!WildcardMatch(GetAction(pTarget), "*Magic"))
		{
			// Als nächstes Zaubern wir folgendes
			if (!LocalN("AI_idNextSpell", pTarget))
			{
				LocalN("AI_idNextSpell", pTarget) = spells[Random(GetLength(spells))];
			}
			else
			{
				// Vorbereiten, genug Zauberenergie?
				if (pTarget->GetMagicEnergy() <= GetValue(, LocalN("AI_idNextSpell", pTarget)))
				{
					if (pTarget->GetMagicEnergy() < 1) pTarget->DoMagicEnergy(1); // sonst kann er nichtmal meditieren
					// meditieren zuerst
					pTarget->DoMagic(MMED);
				}
				else
				{
					// opfer suchen
					if (!LocalN("AI_pSpellTarget", pTarget))
					{
						var t = FindObject2(Find_OCF(OCF_CrewMember), Find_Distance(100, AbsX(GetX(pTarget)), AbsY(GetY(pTarget))));
						if (t != pTarget)
						{
							// anstarren
							pTarget->SetComDir(COMD_Stop);
							pTarget->SetDir(GetX(t) > GetX(pTarget));
						}
						LocalN("AI_pSpellTarget", pTarget) = t;
					}
					else // endlich zaubern
					{
						//pTarget->DoMagicEnergy(GetValue(, spellID),, 1);
						pTarget->DoMagic(LocalN("AI_idNextSpell", pTarget));
						LocalN("AI_idNextSpell", pTarget) = 0;
					}
				}
			}
		}
		else
		{
			EffectVar(2, pTarget, iNr)[4] -= Random(10);
		}
	}
	else EffectVar(2, pTarget, iNr)[4]+=10;
	
	//////////////////////////////// Hide ///////////////////////////////////
	if (next == 5)
		{
		if (pTarget->~IsNotHidden())
			{
			if (!Random(10)) Message("<c %x>%s</c>", pTarget, GetColorDw(pTarget), ["$MsgHide0$", "$MsgHide1$"][Random(2)]);
			pTarget->Hide();
			EffectVar(2, pTarget, iNr)[5]-=200; // Motivationsdrop
			}
		else
			{
			if (!Random(10)) Message("<c %x>%s</c>", pTarget, GetColorDw(pTarget), ["$MsgUnHide0$", "$MsgUnHide1$"][Random(2)]);
			pTarget->~UnHide();
			EffectVar(2, pTarget, iNr)[5]-=200; // Motivationsdrop
			}
		}
	else EffectVar(2, pTarget, iNr)[5]+=10;
	
	// prev = next
	EffectVar(4, pTarget, iNr) = next;
	return 0;
}



// APPEND TO

public func DoSpellAim(object pSpell)
  {
  // Winkelziel-Callback
  // Normaler Spieler
  if (!GetEffect("*AI*", this)) return(_inherited(pSpell, ...));
  // KI: Zielt immer aufs gegenwärtige Ziel
  var x, y;
  if (AI_pSpellTarget != this)
  {
  	x = GetX(AI_pSpellTarget);
  	y = GetY(AI_pSpellTarget);
  }
  else
  {
  	x = Random(LandscapeWidth());
  	y = Random(LandscapeHeight());
  }
  var iAngle;
	SetDir(x > GetX());
	var iAngle = Angle(GetX(), GetY(), x, y);
	if (iAngle > 180) iAngle -= 360;
	if (GetEffect("*Recall*Aiming*", this))
	{
	  // Zauber abbrechen!
	  Call("OnAimerAbort");
	  if (pAimer) RemoveObject(pAimer);
	  return true;
	}
	pSpell->~AimingAngle(this, iAngle);
	pSpell->ActivateAngle(this, iAngle);
  return true;
  }

public func DoSpellSelect(object pSpell, int iRange)
  {
  // Zielauswahl-Callback
  // Normaler Spieler
  if (!GetEffect("*AI*", this)) return(_inherited(pSpell, iRange, ...));
  // KI: Zielt bevorzugt auf die Befehlsziele; ansonsten auf sich, wenn möglich
  var t;
  if (!TrySelectorTarget(pSpell, t=AI_pSpellTarget, iRange))
    if (!TrySelectorTarget(pSpell, t=this, iRange))
        return 0;
  pSpell->ActivateTarget(this, t);
  return true;
  }
  
private func TrySelectorTarget(object pSpell, object pTarget, int iRange)
  {
  if (!pTarget) return;
  if (ObjectDistance(pTarget) > iRange) return;
  return (pSpell->~SelectorTarget(pTarget));
  }
