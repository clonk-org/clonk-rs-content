#!/usr/bin/env python3
"""Original pixel scenery and chapter plates for The Twonky Tales (Pillow)."""
from math import cos, sin, pi
from pathlib import Path
from PIL import Image, ImageDraw, ImageFont

ROOT = Path(__file__).resolve().parents[1] / "TwonkyTales.c4f"
ART = ROOT / "Tales.c4d" / "Scenery.c4d"
CHAPTERS = ("01FineShot", "02MillBelow", "03KingsAccount", "04HerosWelcome", "05RoadBack", "06BowForTomorrow")
INK, STONE, LIGHT, WOOD, BRASS, BLUE = "#26313b", "#657078", "#d7c7a2", "#695143", "#c3a36a", "#557f8b"


def gear(draw, x, y, radius, color=BRASS):
    points = []
    for i in range(64):
        a = i*pi/32
        r = radius if i % 4 < 2 else radius*0.8
        points.append((x+cos(a)*r, y+sin(a)*r))
    draw.polygon(points, fill=color)
    draw.ellipse((x-radius*0.62, y-radius*0.62, x+radius*0.62, y+radius*0.62), fill=INK)
    for i in range(6):
        a=i*pi/3
        draw.line((x,y,x+cos(a)*radius*0.7,y+sin(a)*radius*0.7),fill=color,width=max(1,int(radius/8)))
    draw.ellipse((x-3,y-3,x+3,y+3),fill=LIGHT)


def scenery(kind):
    im=Image.new("RGBA",(160,120));d=ImageDraw.Draw(im)
    if kind in ("House","School","Ruin"):
        d.rectangle((18,51,143,119),fill=WOOD,outline=INK,width=3)
        d.polygon([(8,54),(79,9),(152,54)],fill=INK)
        d.line((8,54,79,9,152,54),fill=BRASS,width=3)
        d.rectangle((70,82,92,119),fill=INK)
        for x in (29,105):
            d.rectangle((x,68,x+20,91),fill=BLUE,outline=LIGHT,width=2)
            d.line((x+10,69,x+10,90),fill=INK,width=2)
        for y in (58,100,114): d.line((19,y,142,y),fill=BRASS,width=2)
        for x in (23,64,99,137): d.line((x,55,x,119),fill=INK,width=3)
        if kind=="School":
            d.rectangle((60,63,101,78),fill=LIGHT,outline=INK)
            d.line((66,71,76,67,83,71,94,66),fill=WOOD,width=2)
        if kind=="Ruin":
            d.polygon([(80,10),(130,42),(99,81),(64,61)],fill=(0,0,0,0))
            d.line((62,53,100,74),fill=BRASS,width=2)
    elif kind=="Mill":
        d.rectangle((12,45,108,119),fill=STONE,outline=INK,width=3)
        d.polygon([(4,48),(60,9),(116,48)],fill=INK)
        d.line((4,48,60,9,116,48),fill=BRASS,width=3)
        for y in (60,80,100): d.line((15,y,106,y),fill=INK,width=2)
        gear(d,119,86,31)
        d.rectangle((32,67,53,92),fill=BLUE,outline=LIGHT,width=2)
    elif kind=="Archive":
        d.rectangle((12,24,149,119),fill=INK,outline=BRASS,width=3)
        for y in (30,55,80):
            for x in range(20,140,13):
                d.rectangle((x,y,x+9,y+18),fill=WOOD if x%3 else BLUE,outline=LIGHT)
            d.line((14,y+21,146,y+21),fill=BRASS,width=3)
    elif kind=="Prison":
        d.rectangle((9,14,152,119),fill=STONE,outline=INK,width=3)
        for x in (21,68,115):
            d.rounded_rectangle((x,39,x+28,108),radius=10,fill=INK)
            for bar in (x+7,x+20): d.line((bar,40,bar,108),fill=BRASS,width=2)
        d.polygon([(13,8),(149,8),(136,32),(29,32)],fill=WOOD,outline=BRASS)
    elif kind=="Engine":
        d.rectangle((4,104,155,119),fill=INK,outline=BRASS,width=2)
        gear(d,46,65,42);gear(d,118,82,29)
        d.rectangle((24,9,40,69),fill=STONE,outline=BRASS,width=2)
        d.line((80,113,80,28,146,28,146,97),fill=BLUE,width=5)
    elif kind=="Tank":
        d.rounded_rectangle((8,26,151,115),radius=16,fill=INK,outline=BRASS,width=3)
        d.rectangle((15,69,144,99),fill=BLUE)
        for x in (31,126):d.line((x,32,x,115),fill=BRASS,width=4)
        d.line((2,16,80,16,80,28),fill=STONE,width=5)
    return im


def plate(chapter):
    im=Image.new("RGB",(1024,576),"#15212b");d=ImageDraw.Draw(im)
    accent=(BRASS,BLUE,"#b9a279","#af7966",BLUE,BRASS)[chapter-1]
    # Finely spaced, deterministic engraving lines are deliberately code-native art.
    for y in range(24,554,7):
        d.line((24,y,1000,y),fill="#1b2933")
    d.rectangle((24,24,999,551),outline=accent,width=2)
    d.rectangle((35,35,988,540),outline="#3d484b")
    if chapter in (1,5):
        d.polygon([(70,379),(250,283),(440,370),(630,242),(945,355),(945,499),(70,499)],fill="#35454b")
        for x in range(80,950,38):d.line((x,435,x+19,423,x+36,435),fill=BLUE,width=2)
        for x in (175,345,680):
            house=scenery("Ruin" if chapter==5 and x==345 else "House").resize((224,168),Image.Resampling.NEAREST)
            im.paste(house,(x-65,244 if x!=680 else 207),house)
        d.arc((310,120,790,474),188,342,fill=accent,width=4)
        if chapter==5:d.polygon([(329,350),(345,331),(351,356)],fill=accent)
        else:d.polygon([(770,341),(749,328),(752,354)],fill=accent)
    elif chapter==2:
        gear(d,375,293,139);gear(d,609,358,104);gear(d,702,171,73)
        d.line((157,445,845,445),fill=accent,width=4)
        d.arc((133,111,879,468),191,346,fill=LIGHT,width=3)
    elif chapter==3:
        for y in (201,297,393):d.line((100,y,924,y),fill=accent,width=3)
        d.polygon([(300,95),(722,130),(677,475),(277,432)],fill="#b6a380",outline=BRASS)
        for y in range(166,406,35):d.line((345,y,641,y+29),fill=WOOD,width=3)
        d.ellipse((550,322,650,422),outline="#7c4b3e",width=7)
        d.line((548,350,654,394),fill="#7c4b3e",width=4)
    elif chapter==4:
        d.rounded_rectangle((268,75,755,501),radius=100,fill="#3b4649",outline=BRASS,width=4)
        for x in range(305,752,65):d.line((x,137,x,500),fill=INK,width=16)
        d.polygon([(124,180),(900,96),(871,209),(151,263)],fill="#9b6954",outline=BRASS)
        d.line((314,466,689,297),fill=LIGHT,width=5)
    else:
        gear(d,299,345,100,STONE);gear(d,716,345,100,STONE)
        d.arc((308,66,657,505),263,98,fill=BRASS,width=12)
        d.line((469,74,469,491),fill=LIGHT,width=3)
        d.line((186,344,830,232),fill=LIGHT,width=4)
        d.polygon([(844,230),(810,216),(815,252)],fill=LIGHT)
        d.arc((180,153,842,469),5,175,fill=BLUE,width=3)
    # Language-independent chapter numbers; the launcher supplies localized titles.
    font=ImageFont.truetype('/usr/share/fonts/TTF/DejaVuSerif.ttf',32)
    d.text((62,56),("I","II","III","IV","V","VI")[chapter-1],fill=LIGHT,font=font)
    return im


def main():
    ART.mkdir(parents=True,exist_ok=True)
    for kind in ("House","School","Ruin","Mill","Archive","Prison","Engine","Tank"):
        scenery(kind).save(ART/("Graphics"+kind+".png"))
    scenery("House").save(ART/"Graphics.png")
    for n,chapter in enumerate(CHAPTERS,1):
        plate(n).save(ROOT/(chapter+".c4s")/"Title.png")
    plate(6).save(ROOT/"Title.png")


if __name__=="__main__":main()
