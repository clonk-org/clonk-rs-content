/* Squeeker */

#strict

local fdig, ptarget;
local owner;


public func Launch (int angle, bool fMode)
{
  var speed;
  fdig=fMode;
  if (!fMode) speed=RandomX(60,80); else speed=RandomX(40,60); //Will ?: :/
  SetXDir(Sin(angle,speed));
  SetYDir(-Cos(angle,speed));
  Sound("sqk_deploy1");
  if (GetXDir()>0) SetDir(DIR_Right());
  else SetDir(DIR_Left());
  SetAction("Jump");
  if(!fdig) AddEffect("RemoveSqueeker",this(),1,RandomX(1500,2000),0,GetID());
  owner = GetController();
  SetController(NO_OWNER);
  SetOwner(NO_OWNER);
}

public func OnHit() { Splotsch();}

func TheCheck() { if(!GBackSolid(0,5)) SetAction("Jump"); }

//SPLOTSCH(tm)
public func Splotsch()
{
	Sound("sqk_blast");
	//insert tolle Partikel
	for(var i; i < 6; i++)
		MuzzleFlash2(RandomX(20,30),0,0,0,Random(360),RGBa(0,100,0,50));
	RemoveObject();
}

//Die KI
func ClearCommands()
{
 var i=0;
 while(GetCommand(this,0,i))
 {
  FinishCommand(0,0,i);
  i++;
 }
}

protected func Life()
{
	//Alternativer Modus?
	var dir = GetDir()*2-1;
	if (fdig)
	  {
	  	//Graben wir? Dann nichts tun.
	  	if(GetAction() eq "Grab" || GetAction() eq "Warten") return;
	  	
			if (GetAction() eq "Scale")
			if(!(GetComDir() & COMD_Down || GetComDir() & COMD_Up))
			{
			 if(Random(2))SetComDir(COMD_Up);
			 else SetComDir(COMD_Down);
			 return true;
			}
			else return true;
			
	  	//Kein Ziel? -> Graben!
	  	if (!ptarget)
	  		{
	  		if(GBackSolid(0, 4))
	  			{
	  			SetAction("Grab");
	  			SetSpeed(); //Stillstehen bitte.
	  			}
	  		return;
	  		}
	  	//Timer drauf, wenn bisher noch nicht.
	  	if (!GetEffect("RemoveSqueeker",this())) AddEffect("RemoveSqueeker",this(),1,RandomX(1500,2000),0,GetID());
		  if(!Random(30)) Sound("sqk_hunt*");
			if(!Random(40)) Jump();
			//Target angreifen?
 			var fAttack=false;
 			for(var pTarget in FindObjects(
 				Find_AtPoint(),
			 	Find_NoContainer(),
				Find_Or(
					Find_Func("IsBulletTarget",GetID(),this()),
					Find_OCF(OCF_Alive)),
 				Find_Not(Find_Func(ObjectCall(this(),"HitExclude"))),
 				Find_Not(Find_ID(QSQK)))) 
 				{ pTarget->DoDmg(27+Random(6),DMG_Bio,pTarget,0,owner+1); fAttack=true; }
  		if (fAttack) Splotsch();
  		  		
  		return;
	   }
	if (GetAction() eq "Scale")
	if(!(GetComDir() & COMD_Down || GetComDir() & COMD_Up))
	{
	 if(Random(2))SetComDir(COMD_Up);
	 else SetComDir(COMD_Down);
	 return true;
	}
	else return true;
	if (GetAction() eq "Attack" || GetAction() eq "Idle") return true;
	//if (GetAction() eq "Hangle") return true;
  //Richtung vllt ändern
	if(!Random(25) || !GetXDir())
	{
		if (GetDir()==DIR_Right)
  	{
  		SetDir(DIR_Left());
  		SetComDir(COMD_Left());
  	}
  	else
  	{
  		SetDir(DIR_Right());
  		SetComDir(COMD_Right());  
  	}
 	}
 //Target suchen
 var pTarget;
 if(pTarget=FindObject2(
 	Find_Distance(160,40*dir),
 	Find_NoContainer(),
	Find_Or(
		Find_Func("IsBulletTarget",GetID(),this()),
		Find_OCF(OCF_Alive)),
		Find_Not(Find_ID(QSQK)),
  Find_Not(Find_Func(ObjectCall(this(),"HitExclude"))),
  Sort_Distance()))
  {
   ClearCommands();
   SetCommand(this(),"Follow",pTarget);
  }
 //Target angreifen?
 var fAttack=false;
 pTarget=0;
 for(pTarget in FindObjects(
 	Find_AtPoint(),
 	Find_NoContainer(),
	Find_Or(
		Find_Func("IsBulletTarget",GetID(),this()),
		Find_OCF(OCF_Alive)),
  Find_Not(Find_Func(ObjectCall(this(),"HitExclude"))), Find_Not(Find_ID(QSQK)))) 
  {pTarget->DoDmg(20,DMG_Bio,pTarget,0,owner+1); fAttack=true;}
 if (fAttack)
 {
 	Jump();
 	SetXDir(GetXDir()/3);
 	SetYDir(GetYDir()/3);
	Sound("hc_attack1");
	SetAction("Attack");
	}
 if(!Random(30)) Sound("sqk_hunt*");
 if(!Random(40)) Jump(); 
 
}

//KI in der Erde
protected func Warten()
{
  //Kein Ziel? Nichts tun.
	if(!(ptarget=FindObject2(
 	Find_Distance(60),
 	Find_NoContainer(),
	Find_Or(
		Find_Func("IsBulletTarget",GetID(),this()),
		Find_OCF(OCF_Alive)),
  Find_Not(Find_Func(ObjectCall(this(),"HitExclude"))),
  Find_Not(Find_ID(QSQK)),
  Sort_Distance()))) return;
  Sound("hc_attack1");
  ClearCommands();
	SetCommand(this(),"Follow",ptarget);
	SetAction("Walk");
}

public func IsBulletTarget(id idBullet, object pBullet, object pShooter)
{
 if(GetAction() eq "Warten") return false; //Im Boden nicht zu treffen
 if(idBullet == QSQK) return false; //Nicht welcher meiner Art angreifen.
 return true;
}

func FxRemoveSqueekerStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
	pTarget->Splotsch();
}

public func IsSqueeker() { return true; }
public func IsBouncy() { return true; }

