#strict 2
local tt_key, tt_open;
public func Setup(string key) { tt_key=key; }
public func Key() { return tt_key; }
public func IsOpen() { return tt_open; }
public func Open(bool open)
{
  tt_open=open;
  if (open) { SetSolidMask(); SetClrModulation(RGBa(255,255,255,210)); }
  else { SetSolidMask(0,96,16,96,0,0); SetClrModulation(RGBa(255,255,255,0)); }
  return true;
}
