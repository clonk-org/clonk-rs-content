#strict

/*// Zeichnet einen Materialkreis (Danke an Clonk-Karl :)
global func DrawMaterialCircle(sMaterial, iX, iY, iRad)
{
 for(var x=iX-iRad;x<iX+iRad;++x)
  for(var y=iY-iRad;y<iY+iRad;++y)
   if((x-iX)**2 + (y-iY)**2 < iRad**2)
    DrawMaterialQuad(sMaterial,x,y,x+1,y,x+1,y+1,x,y+1);
 return(1);
}
*/

global func DigFreeCircle(int radius, int x, int y) //  Danke an jsb
{
  for(var ox = x-radius; ox < x+radius; ++ox)
    for(var oy = y-radius; oy < y+radius; ++oy)
      if((ox-x)**2 + (oy-y)**2 < radius**2)
        if(!GBackSky(ox, oy))
          DrawMaterialQuad("Tunnel-Smooth", ox,oy, ox+1,oy, ox+1,oy+1, ox,oy+1);
}
