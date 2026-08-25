/*-- Tunnelzauber --*/

#strict

// Aktivierung durch Zaubern
public func Activate(object caster, object real_caster)
{
  // Eigentlichen Zauberer rausfinden
  var base_caster = caster;
  if(real_caster) caster = real_caster;

  // Zielen wenn möglich
  if(caster->~DoSpellAim(this(), base_caster) ) return(1);

  // Clonk kann nicht zielen: Tunnel geht geradeaus
  StartTunnel(caster, GetDir(caster)*180 - 90);

  // Zauberobjekt ist nicht mehr vonnoeten
  RemoveObject();

  // Zauber erfolgreich
  return(1);
}

// Aktivierung nach Zielen
public func ActivateAngle(object caster, int angle)
{
  // Akustik muss sein
  Sound("Magic*");

  // Auf gehts
  StartTunnel(caster, angle);

  // Objekt löschen
  RemoveObject();

  // Zauber erfolgreich
  return(1);
}

// Starten des Tunnels
public func StartTunnel(object caster, int angle)
{
  // Effekt anwerfen, der uebernimmt den Rest
  AddEffect("TunnelUSpell", 0, 260, 1, 0, GetID(), caster, angle);
}

// Effektparameter
static const MTNL_Height = 30; // Höhe des Tunnels
static const MTNL_Jitter = 2; // Franseliger Rand am Gang (unten und oben ist schlecht zu unterscheiden, weil gedreht)
static const MTNL_Length = 120; // Länge des Tunnels
static const MTNL_Duration = 250; // Wartezeit zwischen Auf- und Abbau
static const MTNL_DataIndex = 4; // Stelle, ab welcher Pixeldaten gespeichert werden (Peter: Gib Arrays!)

// Und los gehts!
public func FxTunnelUSpellStart(object target, int effect_number, int temp, object caster, int angle)
{
  // Temporaerer Aufruf? Nix fuer uns.
  if(temp) return(0);

  // Startpunkt und -winkel speichern
  EffectVar(0, target, effect_number) = Sin(angle, -10) + GetX(caster); // Start-X
  EffectVar(1, target, effect_number) = -Cos(angle, -10) + GetY(caster); // Start-Y
  EffectVar(2, target, effect_number) = ((360 - angle) + 360) % 360; // Winkel (Der Zauber will ihn CCW, wir haben ihn CW)
  EffectVar(3, target, effect_number) = MTNL_DataIndex; // Aktueller Daten-Index

  // Erwartetes Ende der Datenliste beschreiben, damit die Liste nicht bei jedem Aufruf neu allokiert werden muss
  EffectVar(MTNL_DataIndex + 2 * (MTNL_Length * MTNL_Height), target, effect_number) = 1;

  // Fertig, der Timer uebernimmt den Rest
  return(1);
}

global func FxTunnelUSpellTimer(object target, int effect_number, int effect_time)
{
  // Aktueller Status (0: Aufbau, 1: Warten, 2: Abbau)
  var cur_state = 0;
  if(effect_time >= 1 + MTNL_Length) cur_state = 1;
  if(effect_time >= 1 + MTNL_Length + MTNL_Duration) cur_state = 2;
  if(effect_time >= 1 + MTNL_Length * 2 + MTNL_Duration) return(-1);

  // Warten?
  if(cur_state == 1)
  {
    // Daten-Index resetten, damit er beim Abbau ausgelesen werden kann.
    EffectVar(3, target, effect_number) = MTNL_DataIndex;
    // Warten.
    return(1);
  }


  // Tunnel-Y anhand Effekt-Zeit rausfinden
  var y = effect_time - 1;
  if(cur_state == 2) y = effect_time - 1 - (MTNL_Length + MTNL_Duration);
  
  // Rotationszentrum
  var rot_x = EffectVar(0, target, effect_number);
  var rot_y = EffectVar(1, target, effect_number);
  
  // Rotationswinkel (* 100 fuer mehr Genauigkeit)
  var angle = EffectVar(2, target, effect_number) * 100;
  
  // Rechter und linker Aussenpunkt auf aktueller Hoehe (y)
  var left_point = -MTNL_Height / 2;
  var right_point = MTNL_Height / 2;

  // Rundung am Anfang
  var dy;
  if(y < MTNL_Height/2)
  {
    dy = MTNL_Height/2 - y;
    right_point = Sqrt(MTNL_Height*MTNL_Height/4 - dy*dy);
    left_point =- right_point;
  }
  
  // Rundung am Ende
  dy = y - MTNL_Length + MTNL_Height/2;
  if(dy > 0)
  {
    right_point = Sqrt(MTNL_Height*MTNL_Height/4 - dy*dy);
    left_point =- right_point;
  }
  
  // Wir berechnen jetzt den Winkel vom Rotationszentrum zu den Aussenpunkten
  var left_angle = Angle(0, 0, left_point, y, 100);
  var right_angle = Angle(0, 0, right_point, y, 100);
  
  // Abstand zu den Aussenpunkten (sollte bei beiden der selbe sein)
  var dist = Distance(0, 0, left_point, y);
  
  // Jetzt werden die Aussenpunkte rotiert. Dazu nehmen wir den Winkel vom Rotationszentrum zu den Punkten und addieren
  // Den gewueschten Winkel dazu. Ein Winkel von 45 (vom Rotationszentrum aus) wird bei einer Drehung von 90 Grad also
  // zu 135 (vom Rotationszentrum aus). Damit ist der gedreht, die Entfernung zum Rotationszentrum haben wir vorhin ja auch
  // schon ausgerechnet. Damit koennen wir die neue Position bestimmen 
  var left_x = rot_x + Sin(left_angle + angle, dist, 100);
  var left_y = rot_y + Cos(left_angle + angle, dist, 100);
//  if (left_x < 0 || left_x > 100) Log("!!!!");
  var right_x = rot_x + Sin(right_angle + angle, dist, 100);
  var right_y = rot_y + Cos(right_angle + angle, dist, 100);
  
  // Zwischen diesen beiden Punkten kann nun eine Linie gezogen werden. Macht man das aber fuer jeden Pixel einmal, kann es
  // dabei bei krummen Winkeln zu einzelnen Pixeln kommen, die nicht erfasst werden. Deshalb wird eine zweite Linie ein wenig
  // versetzt gezeichnet. Die Versetzung wird hier bestimmt. Versetzt man aber in beide Richtung (X und Y), so kommt wieder
  // Murks raus, da das meistens die Zeile ist, die bei der naechsten Iteration erfasst wuerde und die Zwischenpixel hat man
  // immer noch nicht. Deshalb nur entweder in X oder in Y verschieben.
  var off_y, off_x = BoundBy(Sin(angle, 10, 100) / 5, -1, 1);
  if(!off_x) off_y = BoundBy(Cos(angle, 10, 100) / 5, -1, 1);
 
  // Die beiden Linien werden jetzt gezeichnet und fuer jeden Pixel StorePixel oder RestorePixel (je nach Status) aufgerufen
  var function = "StorePixel";
  if(cur_state == 2) function = "RestorePixel";

  EffectCall(target, effect_number, "DrawLine", left_x, left_y, right_x, right_y, function);
  EffectCall(target, effect_number, "DrawLine", left_x + off_x, left_y + off_y, right_x + off_x, right_y + off_y, function);

  // Fertig 
  return(1);
}

public func FxTunnelUSpellStorePixel(object target, int effect_number, int x, int y, int pos, int len)
{ 
  // Pixeldaten speichern
  var pixel_data = GetMaterial(x, y); // GetMaterial gibt 7 Bit Material-Index
  
  // Bestimmte Dinge ignorieren
  if(pixel_data == Material("Tunnel") || pixel_data == Material("Vehicle") || pixel_data == Material("Sky") )
    pixel_data = -1;
  else
    if(!GBackSky(x, y) ) pixel_data |= (1 << 7); // Das 8. Bit speichert also Unter-/Obergrund
    
  // Rand ausfranzen
  // Unten
  if(pos < MTNL_Jitter && Random(MTNL_Jitter * 4) )
  {
    if(pos == 0)
      pixel_data = -1;
    else
      if(EffectVar(EffectVar(3, target, effect_number) - 1, target, effect_number) == -1)
        pixel_data = -1;
  }
  // Oben
  if(pos > len - MTNL_Jitter)
  {
    if(EffectVar(EffectVar(3, target, effect_number) - 1, target, effect_number) == -1)
      pixel_data = -1;
    else
      if(Random((MTNL_Jitter - (len - pos)) * 2) )
        pixel_data = -1;
  }

  // Daten setzen und Daten-Index hochzaehlen, fuer den naechsten Pixel
  EffectVar(EffectVar(3, target, effect_number)++, target, effect_number) = pixel_data;
  // Pixel loeschen
  if(pixel_data != -1)
  {
    FreeRect(x, y, 1, 1);
    // Effekt
    if(!Random(9) )
      CreateParticle("MaterialSpark", x, y, 0, RandomX(-1, 1), RandomX(5*3, 5*14), RGBa(255, 128, 128, Random(128)) );
  }
}

public func FxTunnelUSpellRestorePixel(object target, int effect_number, int x, int y, int pos, int len)
{
  // Pixeldaten extrahieren und Daten-Index hochzaehlen, fuer den naechsten Pixel
  var pixel_data = EffectVar(EffectVar(3, target, effect_number)++, target, effect_number);
  // -1? Nix zu tun.
  if(pixel_data == -1) return(0);
  // Unterirdisch (8. Bit)
  var sub = (pixel_data & (1 << 7) );
  // Rest ist das Material 
  pixel_data &= (~(1 << 7) );
  // Pixel widerherstellen
  // TODO: Use Burned texture
  DrawMaterialQuad(Format("%s-Smooth", MaterialName(pixel_data)), x, y, x+1, y, x+1, y+1, x, y+1, sub);
  // Effekt
  if(!Random(9) )
    CreateParticle("MaterialSpark", x, y, 0, RandomX(-1, 1), RandomX(5*3, 5*14), RGBa(255, 128, 128, Random(128)) );
}

/*func EffectCall() {
  Log("%s %d %s %d %d %d %d %d", GetName(Par()),Par(1),Par(2),Par(3),Par(4), Par(5), Par(6), Par(7));
  inherited(Par(0), Par(1), Par(2), Par(3), Par(4), Par(5), Par(6), Par(7), Par(8), Par(9));
}*/

// Der Linienmalalgorithmus (Bresenhams Linienalgorithmus)
public func FxTunnelUSpellDrawLine(object target, int effect_number, int fx, int fy, int tx, int ty, string function)
{
  // Enginebugworkaround
/*  if (fx < 0) return();
  if (fy < 0) return();
  if (tx < 0) return();
  if (ty < 0) return();*/
  var dx = Abs(fx - tx);
  var dy = Abs(fy - ty);
  
  var xval = 1, yval = 1;
  if(fx > tx) xval = -1;
  if(fy > ty) yval = -1;
  
  if(dx >= dy)
  {
    var error = 0;
    var cx = fx, cy = fy;
    
    for(var i = 0; i < dx; ++ i)
    {
      EffectCall(target, effect_number, function, cx, cy, i, dx);
      cx += xval;
      error += dy;
      if(error >= dx)
      {
        error -= dx;
        cy += yval;
      }
    }
  }
  else
  {    
    var error = 0;
    var cx = fx, cy = fy;
    
    for(var i = 0; i < dy; ++ i)
    {
      EffectCall(target, effect_number, function, cx, cy, i, dy);
      cy += yval;
      error += dx;
      if(error >= dy)
      {
        error -= dy;
        cx += xval;
      }
    }
  }
}

// Vertauscht den Inhalt zweiter Variablen mittels einer tempoaeren dritten
public func SwapVar(&a, &b)
{
  var f = a;
  a = b;
  b = f;
}

public func IsAimerSpell() { return(1); }

// Zaubereigenschaften
public func GetSpellClass() { return(EART); }
public func GetSpellCombo() { return("633"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
