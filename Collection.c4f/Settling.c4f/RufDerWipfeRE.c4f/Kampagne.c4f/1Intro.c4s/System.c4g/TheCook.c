/*-- Kochdialog --*/

#strict
#appendto CLNK

static g_CookDialog_State;

func FxIntCookDialogTimer(pTarget)
{
  if(g_fDonotspeak)
    return 0;
  if(g_CookDialog_State<20)
    g_CookDialog_State++;
  if(g_CookDialog_State==2)
    return ColorMessage("$Cook01$",this,0);
  if(g_CookDialog_State==4)
    return ColorMessage("$Cook02$",this,0);
  if(g_CookDialog_State==7)
  {
    SetCommand(pTarget,"MoveTo",0,680,630);
    return ColorMessage("$Cook03$",this,0);
  }
  if(g_CookDialog_State==9)
    return SetCommand(pTarget,"MoveTo",0,520,610);
  if(g_CookDialog_State==10)
    return ColorMessage("$Cook04$",this,0);
  if(g_CookDialog_State==12)
  {
    SetCommand(pTarget,"MoveTo",0,680,630);
    return ColorMessage("$Cook05$",this,0);
  }
  if(g_CookDialog_State==13)
  {
    SetCommand(pTarget,"MoveTo",0,530,630);
    return 1;
  }
  if(g_CookDialog_State==14)
  {
    CastPXS("Water",40,10,0,-20);
    CastPXS("Acid",20,10,0,-20);
    SetCommand(pTarget,"MoveTo",0,700,630);
    return 1;
  }
  if(g_CookDialog_State<15)
    return 1;
  if(g_CookDialog_State==15)
  {
    Schedule("Sound(\"Clonk\")",15);
    CreateParticle("Smoke", 0,-10, 0,-40, 100, RGBa(128,255,128,0));
    SetAction("Throw");
    SetCommand(pTarget,"MoveTo",0,720,630);
    return 1;
  }
  g_CookDialog_State=RandomX(15,22);
  if(g_CookDialog_State==16)
  {
    SetCommand(pTarget,"None");
    return 1;
  }
  if(g_CookDialog_State==17)
  {
    SetCommand(pTarget,"MoveTo",0,720,630);
    return 1;
  }
  if(g_CookDialog_State==18)
  {
    SetCommand(pTarget,"MoveTo",0,680,630);
    return 1;
  }
  if(g_CookDialog_State==19)
  {
    SetCommand(pTarget,"MoveTo",0,700,630);
    return 1;
  }
  if(g_CookDialog_State==20)
  {
    SetCommand(pTarget,"MoveTo",0,700,630);
    SetAction("Throw");
    Schedule("Sound(\"Clonk\")",15);
    return 1;
  }
  return(1);
}

func FxIntCookDialog2Timer(pTarget)
{
  if(g_fDonotspeak)
    return 0;
  if(g_CookDialog_State==1)
    return ColorMessage("$Cook06$",this,0);
  if(g_CookDialog_State==3)
    return ColorMessage("$Cook07$",this,0);
  if(g_CookDialog_State==5)
  {
    AddEffect("IntMakeGreen",pTarget,10,1,pTarget);
    SetCommand(pTarget,"MoveTo",0,550,610);
    return ColorMessage("$Cook08$",this,0);
  }
  if(g_CookDialog_State==11)
  {
    pTarget->SetAction("Walk",0,0,true);
    Fling(this,RandomX(-10,10),-20);
    ColorMessage("Ahh!!!",this,0);
    return 1;
  }
  if(g_CookDialog_State==13)
  {
    SetCommand(pTarget,"MoveTo",0,730,630);
    return ColorMessage("$Cook09$",this,0);
  }
  if(g_CookDialog_State==14)
  {
    SetCommand(pTarget,"MoveTo",0,900,640);
    return ColorMessage("$Cook10$",this,0);
  }
  if(g_CookDialog_State==21)
  {
    return ColorMessage("$Cook11$",this,5);
  }
  if(g_CookDialog_State==22)
  {
    if(!Random(4))
      return ColorMessage("$Cook12$",this,5);
  }
  return(1);
}

func FxIntMakeGreenStart(pClonk)
{
  SetGraphics("",pClonk,GetID(pClonk),GFX_Overlay,GFXOV_MODE_ExtraGraphics,0,GFX_BLIT_Mod2|GFX_BLIT_ClrSfc_Mod2);
  SetClrModulation(RGBa(32,200,32,255),pClonk,GFX_Overlay);
}

func FxIntMakeGreenTimer(pClonk,iNum,iTime)
{
  SetClrModulation(RGBa(18,180,18,255-Abs(Sin(iTime*4,254))),pClonk,GFX_Overlay);
  if(iTime>360)
    return -1;
}

func FxIntMakeGreenStop(pClonk)
{
  pClonk->SetAction("Dead");
  SetGraphics(0,pClonk,0,GFX_Overlay);
}
