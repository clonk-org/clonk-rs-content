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
    return ColorMessage("Chefchens Lieblingsessen :D",this,0);
  if(g_CookDialog_State==4)
    return ColorMessage("Mammutsuppe mit Kräuterdressing...",this,0);
  if(g_CookDialog_State==7)
  {
    SetCommand(pTarget,"MoveTo",0,680,630);
    return ColorMessage("!WAS! hast du mit der Suppe getan!!!!111einself",this,0);
  }
  if(g_CookDialog_State==9)
    return SetCommand(pTarget,"MoveTo",0,520,610);
  if(g_CookDialog_State==10)
    return ColorMessage("Wach auf Faulpelz!!1 >:/",this,0);
  if(g_CookDialog_State==12)
  {
    SetCommand(pTarget,"MoveTo",0,680,630);
    return ColorMessage("Du Idiot hast Würgwurz in die Suppe getan *grrr* >:/|Ich möchte dich heute hier nicht mehr sehen...",this,0);
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
    return ColorMessage("Was kochen wir da eigentlich? O_o",this,0);
  if(g_CookDialog_State==3)
    return ColorMessage("Und was ist sein Lieblingsessen? /:",this,0);
  if(g_CookDialog_State==5)
  {
    AddEffect("IntMakeGreen",pTarget,10,1,pTarget);
    SetCommand(pTarget,"MoveTo",0,550,610);
    return ColorMessage("<c ffffff>Das st<c eeffee>i<c ddffdd>n<c ccffcc>kk<c bbffbb>t<c aaffaa> aa<c 99ff99>bbb<c 88ff88>e<c 77ff77>rrr <c 66ff66>a<c 55ff55>r <c 44ff44>d<c 33ff33>o<c 22ff22>l<c 11ff11>l<c 00ff00>e... *würg*</c></c></c></c></c></c></c></c></c></c></c></c></c></c></c></c>",this,0);
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
    return ColorMessage("upps...",this,0);
  }
  if(g_CookDialog_State==14)
  {
    SetCommand(pTarget,"MoveTo",0,900,640);
    return ColorMessage("ähh... ok... :./",this,0);
  }
  if(g_CookDialog_State==21)
  {
    return ColorMessage("*seufz*",this,5);
  }
  if(g_CookDialog_State==22)
  {
    if(!Random(4))
      return ColorMessage(".oO(boah was für tolle Kristalle :D)",this,5);
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
