/* Wiederbelebung */

#strict 2

static g_aMapList;

global func SaveMap()
{
	g_aMapList = CreateArray(LandscapeHeight()/10);
	for(var i = 0; i < LandscapeHeight()/10; i++)
	{
		var row = CreateArray(LandscapeWidth()/10);
		for(var j = 0; j < LandscapeWidth()/10; j++)
		{
			row[j] = GetMaterial(j*10+5-GetX(), i*10+5-GetY());
		}
		g_aMapList[i] = row;
	}
}

global func LoadMap()
{
	for(var a = 128; a > 0; a--)
	for(var i = 0; i < LandscapeHeight()/10; i++)
	{
		for(var j = 0; j < LandscapeWidth()/10; j++)
		{
			if(g_aMapList[i][j] == a)
				RestoreChunk(i, j);
		}
	}
	g_aMapList = 0;
}

global func RestoreChunk(szMaterial, szMaterial2, i, j, back)
{
	if(GetMaterialVal("Shape", "Material", Material(szMaterial2)) == 0)
		DrawMaterialRect(szMaterial, i*10, j*10, 10, 10, back);
	if(GetMaterialVal("Shape", "Material", Material(szMaterial2))== 2)
		DrawMatChunk(szMaterial, i*10, j*10, 0, back);
	if(GetMaterialVal("Shape", "Material", Material(szMaterial2)) == 3)
		DrawMatChunk(szMaterial, i*10, j*10, 1, back);
}
/*
global func RestoreChunk(i, j)
{
	if(GetMaterialVal("Shape", "Material", g_aMapList[i][j]) == 0)
		DrawMaterialRect(MaterialName(g_aMapList[i][j]), j*10+5, i*10+5, 10, 10);
	if(GetMaterialVal("Shape", "Material", g_aMapList[i][j]) == 2)
		DrawMatChunk(MaterialName(g_aMapList[i][j]), j*10+5, i*10+5);
	if(GetMaterialVal("Shape", "Material", g_aMapList[i][j]) == 3)
		DrawMatChunk(MaterialName(g_aMapList[i][j]), j*10+5, i*10+5, 1);
}*/

/* -------------------------------------------------------------------------------------------------------- */
/* -------------------------------- Materialfunktionen ---------------------------------------------------- */
/* -------------------------------------------------------------------------------------------------------- */

// Male ein Rechteck \o/
global func DrawMaterialRect(string sMat, int x, int y, int w, int h, bool bg)
{ 
   return DrawMaterialQuad(sMat, x, y, x+w-1, y, x+w-1, y+h, x, y+h, bg);
}

// oder ein Dreieck
global func DrawMaterialTriangle(string sMat, int x, int y, int w, int h, int iIndex, bool bg)
{ 
  if(iIndex == 0) DrawMaterialQuad(sMat, x, y, x, y, x+w-1, y+h, x, y+h, 1);
  if(iIndex == 1) DrawMaterialQuad(sMat, x+w-1, y, x+w-1, y, x+w-1, y+h, x, y+h, 1);
  if(iIndex == 2) DrawMaterialQuad(sMat, x, y, x+w-1, y, x, y+h, x, y+h, 1);
  if(iIndex == 3) DrawMaterialQuad(sMat, x, y, x+w-1, y, x+w-1, y+h, x+w-1, y+h, 1);
}

// Oder Chunks!
static g_ChunkSeed;

global func ChunkyRandom(int &iOffset, int iRange)
{
	if (!iRange) return 0;
	if (!g_ChunkSeed) g_ChunkSeed = Random(999999);
	iOffset+=3;
	return (iOffset^g_ChunkSeed)%iRange;
}

global func DrawMatChunk(iMat, iX, iY, fStrong, fBack)
{
  var wdt = 10;
  var hgt = 10;
  
  var top_rough = 1; var side_rough = 1+fStrong;

  var cro = (iX<<2)+iY;

  var aPoints = CreateArray(8);
  var rx=Max(wdt/2,1);
  
  aPoints[0] = [iX-ChunkyRandom(cro,rx/2),              iY-ChunkyRandom(cro,rx/2*top_rough) ];
  aPoints[1] = [iX-ChunkyRandom(cro,rx*side_rough),     iY+hgt/2];
  aPoints[2] = [iX-ChunkyRandom(cro,rx),                iY+hgt+ChunkyRandom(cro,rx)];
  aPoints[3] = [iX+wdt/2,                               iY+hgt+ChunkyRandom(cro,2*rx)];
  aPoints[4] = [iX+wdt+ChunkyRandom(cro,rx),            iY+hgt+ChunkyRandom(cro,rx)];
  aPoints[5] = [iX+wdt+ChunkyRandom(cro,rx*side_rough), iY+hgt/2];
  aPoints[6] = [iX+wdt+ChunkyRandom(cro,rx/2),          iY-ChunkyRandom(cro,rx/2*top_rough)];
  aPoints[7] = [iX+wdt/2,                               iY-ChunkyRandom(cro,rx*top_rough)];
  
  DrawMaterialQuad(iMat,  aPoints[0][0], aPoints[0][1], aPoints[1][0], aPoints[1][1],
  aPoints[2][0], aPoints[2][1], iX+wdt/2, iY+hgt/2, fBack);
  DrawMaterialQuad(iMat,  aPoints[2][0], aPoints[2][1], aPoints[3][0], aPoints[3][1],
  aPoints[4][0], aPoints[4][1], iX+wdt/2, iY+hgt/2, fBack);
  DrawMaterialQuad(iMat,  aPoints[4][0], aPoints[4][1], aPoints[5][0], aPoints[5][1],
  aPoints[6][0], aPoints[6][1], iX+wdt/2, iY+hgt/2, fBack);
  DrawMaterialQuad(iMat,  aPoints[6][0], aPoints[6][1], aPoints[7][0], aPoints[7][1],
  aPoints[0][0], aPoints[0][1], iX+wdt/2, iY+hgt/2, fBack);
  return 1;
}