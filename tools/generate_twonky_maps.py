#!/usr/bin/env python3
"""Rebuild the six original indexed landscapes. Requires Pillow; no random input.

Coordinates below are world pixels. The two village maps deliberately share
foundations; other rooms, waterworks, shafts and sight lines differ by chapter.
The native minimum 5x map zoom keeps every mechanism aligned with a hand-authored landing.
"""
from pathlib import Path
from PIL import Image, ImageDraw

ROOT = Path(__file__).resolve().parents[1] / "TwonkyTales.c4f"
ZOOM = 5
CHAPTERS = ("01FineShot", "02MillBelow", "03KingsAccount", "04HerosWelcome", "05RoadBack", "06BowForTomorrow")
SKY, TUNNEL, EARTH, GRANITE, ROCK, WATER, BRICK, GOLD, ORE, COAL = 0, 129, 130, 131, 132, 133, 134, 135, 136, 137


def build(chapter):
    im = Image.new("P", (320, 180), GRANITE)
    palette = [(0, 0, 0)] * 256
    colors = [(112, 155, 180), (39, 37, 38), (96, 72, 47), (83, 85, 89), (99, 94, 83), (40, 104, 153), (111, 67, 53), (225, 171, 54), (109, 109, 96), (31, 32, 34)]
    palette[0] = colors[0]
    for index, color in enumerate(colors[1:], 1):
        palette[index] = palette[index + 128] = color
    im.putpalette([c for color in palette for c in color])
    draw = ImageDraw.Draw(im)

    def rect(box, fill):
        draw.rectangle(tuple(v // ZOOM for v in box), fill=fill)

    def ramp(start, end):
        x1, y1 = start; x2, y2 = end
        # A raised landing leaves a traversable underpass where stairs meet a gallery.
        y2 -= 35
        draw.polygon([(x1//ZOOM,(y1-70)//ZOOM), (x2//ZOOM,(y2-70)//ZOOM), (x2//ZOOM,y2//ZOOM), (x1//ZOOM,y1//ZOOM)], fill=TUNNEL)
        draw.polygon([(x1//ZOOM,y1//ZOOM), (x2//ZOOM,y2//ZOOM), (x2//ZOOM,(y2+5)//ZOOM), (x1//ZOOM,(y1+5)//ZOOM)], fill=GRANITE)

    rect((0, 0, 1599, 198), SKY)
    # Foundation strata stay visible in the cut walls and outside the corridors.
    for y in range(214, 900, 42):
        for x in range((y // 42 % 2) * 44, 1600, 88):
            rect((x, y, x+76, y+2), ROCK if chapter in (1, 5) else BRICK)
    rect((80, 330, 1490, 428), TUNNEL)
    rect((180, 550, 1480, 648), TUNNEL)
    rect((80, 740, 1410, 838), TUNNEL)
    ramp((90, 198), (500, 428))
    ramp((280, 428), (700, 648))
    ramp((220, 648), (600, 838))
    # Upper entrance has a broad lip, so a loaded chest can be pushed down.
    rect((180, 200, 400, 210), GRANITE)
    rect((80, 154, 175, 245), SKY)
    if chapter in (1, 5):
        # Reservoir, school foundations and the buried, older spillway.
        rect((760, 220, 1030, 308), TUNNEL)
        rect((765, 250 if chapter == 1 else 230, 1025, 307), WATER)
        rect((1090, 305, 1470, 328), BRICK)
        rect((1140, 265, 1440, 304), ROCK)
        rect((270, 690, 360, 738), TUNNEL)
        rect((280, 695, 350, 726), WATER)
        for x in (1150, 1230, 1310, 1390):
            rect((x, 470, x+26, 520), EARTH)
        if chapter == 5:
            # Shell craters are new. The village's fixed landmarks remain recognizable.
            for x in (680, 880, 1340):
                rect((x, 170, x+44, 220), SKY)
                rect((x, 218, x+44, 224), ROCK)
            rect((1330, 746, 1408, 836), TUNNEL)
            rect((1420, 730, 1540, 852), BRICK)
    elif chapter == 2:
        # A mill shaft with three stone flywheel wells and an observation gallery.
        for x in (610, 880, 1160):
            draw.ellipse((x//ZOOM, 220//ZOOM, (x+170)//ZOOM, 314//ZOOM), fill=TUNNEL, outline=BRICK, width=4)
        rect((1020, 306, 1410, 329), TUNNEL)
        rect((830, 460, 1200, 525), TUNNEL)
        rect((850, 500, 1160, 524), WATER)
        rect((400, 754, 760, 838), TUNNEL)
        for x in range(840, 1400, 80):
            rect((x, 688, x+8, 738), BRICK)
    elif chapter == 3:
        # Parallel trade roads; colored masonry distinguishes accounts, not solutions.
        for y in (430, 650, 840):
            rect((500, y, 1450, y+6), BRICK)
        rect((490, 280, 760, 329), TUNNEL)
        rect((1070, 510, 1430, 549), TUNNEL)
        for x in (540, 670, 800):
            rect((x, 690, x+80, 726), ORE)
        rect((1170, 265, 1420, 312), GOLD)
    elif chapter == 4:
        # Prison windows sit over a continuous service duct. Floors cannot be mined through.
        for x in (1100, 1220, 1340):
            rect((x, 484, x+82, 549), TUNNEL)
            for bar in range(x+8, x+80, 16):
                rect((bar, 485, bar+4, 530), BRICK)
        rect((400, 708, 740, 739), TUNNEL)
        rect((760, 462, 1000, 510), TUNNEL)
        rect((785, 485, 975, 509), WATER)
        for x in range(450, 1500, 110):
            rect((x, 220, x+65, 252), BRICK)
    else:
        # Mountain engine: the open arrow gallery, water accumulator and final archive.
        rect((960, 280, 1460, 329), TUNNEL)
        rect((830, 456, 1080, 518), WATER)
        rect((795, 450, 824, 534), BRICK)
        rect((1086, 450, 1118, 534), BRICK)
        for x in (1120, 1200, 1280, 1360):
            rect((x, 682, x+32, 722), GOLD)
        rect((1040, 708, 1300, 738), TUNNEL)
        # The old drain exits to daylight; this is the route later rebels inherit.
        rect((1412, 742, 1599, 838), SKY)
    return im


def main():
    for chapter, name in enumerate(CHAPTERS, 1):
        build(chapter).save(ROOT / f"{name}.c4s" / "Map.bmp")


if __name__ == "__main__":
    main()
