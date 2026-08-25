/*-- Blitz --*/

#strict

/* Locals */

local iAdvX, iVarX, // Bewegung / Abweichung X
      iAdvY, iVarY; // Bewegung / Abweichung Y
local iSize;
local iAngle;
local iChilds;
local fDoGamma;

/* Aktivierung */

public func Activate(iStartX, iStartY, iAdvanceX, iRandomX, iAdvanceY, iRandomY)
{
  // Startposition
  AddVertex(iStartX,iStartY);
  // Bewegungsverhalten
  iAdvX=iAdvanceX*2; iVarX=iRandomX;
  iAdvY=iAdvanceY*2; iVarY=iRandomY;
  // Aktion setzen
  SetAction("Advance");
  // Geräusch
  if (iSize==10) { Sound("Thunder"); fDoGamma = 1; LightningEffect(10); }
  return(1);
}

public func Launch(iX, iY, iNewAngle, iNewSize)
{
  var XDir;
  iAngle = iNewAngle;
  if(!iNewSize) iSize = 10;
  else iSize = iNewSize;
  XDir = Cos(iAngle, 10, 10);
//  if (iDir == DIR_Left()) XDir = -Cos(iAngle, 10, 10);
//  else (XDir = +Cos(iAngle, 10, 10));
  Activate(iX, iY, XDir, 0, -Sin(iAngle, 10, 10));
//  Log("%d", iSize);
  return(1);
}

private func Timer()
{
  var i=0;
  // Partikel-Effekt
  while(i<GetVertexNum()-1)
  {
  
    DrawParticleLine ("LightningSpark", GetVertex(i)-GetX(), GetVertex(i, 1)-GetY(), 
		    GetVertex(i+1)-GetX(), GetVertex(i+1,1)-GetY(), 6,15*iSize, RGB(200,200,255));
		    
 //   DrawParticleLine ("LightningSpark", GetVertex(i)-GetX(), GetVertex(i, 1)-GetY(), 
//		    GetVertex(i+1)-GetX(), GetVertex(i+1,1)-GetY(), 6,300, RGBa(0,100,255,100));
//    DrawParticleLine ("LightningSpark", GetVertex(i)-GetX(), GetVertex(i, 1)-GetY(), 
//		    GetVertex(i+1)-GetX(), GetVertex(i+1,1)-GetY(),  3, 25, RGB(250,250,255));
    DrawParticleLine2 ("LightningCore", GetVertex(i)-GetX(), GetVertex(i, 1)-GetY(), 
		    GetVertex(i+1)-GetX(), GetVertex(i+1,1)-GetY(),  3, 6*iSize, RGBa(250,250,255,200));
    i++;
  }
}


global func DrawParticleLine2 (szKind, x0, y0, x1, y1, prtdist, a, b0, b1, ydir)
  {
  // Parameter gültig?
  if (!prtdist) return(0);
  // Anzahl der benötigten Partikel berechnen
  var prtnum = Max(Distance(x0, y0, x1, y1) / prtdist, 2);
  var i=prtnum;
  var xdir = Sin(Angle(x0,y0,x1,y1),10);
  ydir = -Cos(Angle(x0,y0,x1,y1),10);
  // Partikel erzeugen!
  while (i>-1)
    {
    var i1,i2,b; i2 = i*256/prtnum; i1 = 256-i2;

    b =   ((b0&16711935)*i1 + (b1&16711935)*i2)>>8 & 16711935
        | ((b0>>8&16711935)*i1 + (b1>>8&16711935)*i2) & -16711936;
    if (!b && (b0 | b1)) ++b;
    CreateParticle(szKind, x0+(x1-x0)*i/prtnum, y0+(y1-y0)*i--/prtnum, xdir,ydir, a, b);
    }
  // Erfolg; Anzahl erzeugter Partikel zurückgeben
  return (prtnum);
  }

/* Bewegung */

private func Advance()
{
  // Einschlag
  var iVtx = GetVertexNum()-1, iVtxX = GetVertex(iVtx, 0), iVtxY = GetVertex(iVtx, 1);
  if (GBackSolid(iVtxX-GetX(), iVtxY-GetY() ))
    return(Remove());
  if (GetMaterial(iVtxX-GetX(), iVtxY-GetY()) != Material("Sky"))
  	return(Remove());

  // Ende
  if(iSize<10 && iVtx>iSize) return(Remove());

  // Neuen Eckpunkt
  if (!AddVertex( iVtxX+iAdvX+RandomX(-10,10), iVtxY+iAdvY+RandomX(-10,10)))
    return(Remove());

  SetPosition(GetVertex(GetVertexNum()), GetVertex(GetVertexNum(), 1));
  
  // Im Wasser?
  if (GBackLiquid(iVtxX-GetX(), iVtxY-GetY() ))
  {
	  var obj;
	  // Alle Lebewesen in der Nähe bekommen was ab
	  while (obj = FindObject( 0, iVtxX-GetX()-400, iVtxY-GetY()-400, 800, 800, OCF_Alive(), 0,0, NoContainer(),obj  ) )
    	  {
      	    if(!PathFree(iVtxX, iVtxY, GetX(obj), GetY(obj))) continue;
	    if(!GBackLiquid(GetX(obj)-GetX(), GetY(obj)-GetY())) continue;
  	    obj->~LightningStrike();
  	    //Punch(obj, 12);
            DrawParticleLine ("LightningSpark2", iVtxX-GetX(), iVtxY-GetY(), 
		    GetX(obj)-GetX(), GetY(obj)-GetY(), 6, 60, RGB(200,200,255));
            DrawParticleLine ("LightningSpark2", iVtxX-GetX(), iVtxY-GetY(), 
		    GetX(obj)-GetX(), GetY(obj)-GetY(),  3, 25, RGB(250,250,255));
          }
	  return(Remove());
  }
  // Verzweigung
  if ( ( (iVtx%2) && iSize==10 && iVtx<20 && Random(2)) || (!Random(10+iChilds*2) && iSize==8))
  {
  	if(!iChilds)
  	{
    	var iRand = RandomX(45,20);
    	if(!Random(2)) iRand = -iRand*10;
   		LaunchLightning(iVtxX, iVtxY, iAngle+iRand);
   	}
  }
  
  // Objektanziehung
  iVtx = GetVertexNum()-1; iVtxX = GetVertex(iVtx, 0); iVtxY = GetVertex(iVtx, 1);
  var obj;
  if (iVtx>7)
  {
    while (obj = FindObject( 0, iVtxX-GetX()-25, iVtxY-GetY()-25, 50, 50, OCF_Alive(), 0,0, NoContainer(),obj  ) )
    {
      if(!PathFree(iVtxX, iVtxY, GetX(obj), GetY(obj))) continue;
      Attraction(obj);
      break;
    }
  }
/*  for(obj in FindObjects(Find_ID(GetID()), Find_Distance(35,iVtxX-GetX(), iVtxY-GetY()), Find_Layer(GetObjectLayer()), Find_Exclude(this())))
  {
    AttractionLightning(obj);
    break;
  }*/
  
  Timer();
  if(iVtx%2) { Advance();   LightningEffect(4);  ExecLgt(); }
  // Weiter	
  return();
}

public func LaunchLightning(iX, iY, iAngle)
{
  var obj = CreateObject(LGTS, -GetX(), -GetY(), GetOwner());
  obj->Launch(iX, iY, iAngle, iSize-2);
  iChilds++;
}

global func CreateLightning(iX, iY, iAngle)
{
  var obj = CreateObject(LGTS, -GetX(), -GetY(), GetOwner());
  obj->Launch(iX, iY, (900-iAngle*10), 10);
}

private func LightningEffect(strength)
  {
  lgt_enlightment+=Random(strength)+strength/2;
  return(1);
  }

private func ExecLgt()
  {
  if (!fDoGamma) return(1);
  // Lichteffekt ausführen (Rampe 5)
  if (lgt_enlightment>0 && lgt_last_lgt_frame != FrameCounter())
    {
    var lgt=Min(lgt_enlightment*3, 255);
    SetGamma(RGB(lgt, lgt, lgt), RGB(128+lgt/2, 128+lgt/2, 128+lgt/2), 16777215, 5);
    lgt_enlightment=Max(lgt_enlightment*4/8-1); lgt_last_lgt_frame=FrameCounter();
    }
  }

/* Effekte */

private func Sparkle()
{
  Timer();
  var iVtx = GetVertexNum()-1, iVtxX = GetVertex(iVtx, 0), iVtxY = GetVertex(iVtx, 1);
  CastObjects(SPRK,1,20,iVtxX-GetX(), iVtxY-GetY());
  return(1);
}

/* Anziehung */

private func Attraction(obj)
{
  // Einschlag in Objekt
  AddVertex( GetVertex(0,0,obj)+GetX(obj),
             GetVertex(0,1,obj)+GetY(obj) );
  obj->~LightningStrike(this());
  SetAction("Connect");
  //Punch(obj, 10);
}

public func LightningStrike(obj)
{
  // Einschlag in Objekt
  AddVertex( GetVertex(GetVertexNum(obj)-1,0,obj),
             GetVertex(GetVertexNum(obj)-1,1,obj) );
  SetAction("Connect");
}

private func AttractionLightning(obj)
{
  // Einschlag in Objekt
  AddVertex( GetVertex(GetVertexNum(obj)-1,0,obj),
             GetVertex(GetVertexNum(obj)-1,1,obj) );
  obj->~LightningStrike(this());
  SetAction("Connect");
}

/* Ende */

private func Remove()
{
  var i=0;
  // Nachleuchten
  while(i<GetVertexNum()-1)
  {
    DrawParticleLine ("LightningSpark2", GetVertex(i)-GetX(), GetVertex(i, 1)-GetY(), 
		    GetVertex(i+1)-GetX(), GetVertex(i+1,1)-GetY(), 6, 15*iSize, RGBa(200,200,255,128));
//    DrawParticleLine ("LightningSpark2", GetVertex(i)-GetX(), GetVertex(i, 1)-GetY(), 
//		    GetVertex(i+1)-GetX(), GetVertex(i+1,1)-GetY(),  3, 25, RGB(250,250,255));
    DrawParticleLine2 ("LightningCore2", GetVertex(i)-GetX(), GetVertex(i, 1)-GetY(), 
		    GetVertex(i+1)-GetX(), GetVertex(i+1,1)-GetY(),  3, 6*iSize, RGBa(250,250,255,200));
    i++;
  }
  if(ObjectCount(GetID())<=1)  ResetGamma(5);
  RemoveObject();
}
