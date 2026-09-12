"""Distribution and progression contracts for the authored campaign."""
import configparser
from pathlib import Path
import struct
import unittest

ROOT = Path(__file__).resolve().parents[1] / "TwonkyTales.c4f"
CHAPTERS = ["01FineShot", "02MillBelow", "03KingsAccount", "04HerosWelcome", "05RoadBack", "06BowForTomorrow"]


class TwonkyCampaignTests(unittest.TestCase):
    def test_sequential_access_uses_unique_native_mission_tokens(self):
        for index, chapter in enumerate(CHAPTERS, 1):
            with self.subTest(chapter=chapter):
                core = configparser.ConfigParser()
                core.read(ROOT / f"{chapter}.c4s" / "Scenario.txt", encoding="latin1")
                self.assertEqual(core["Head"].get("MissionAccess"), None if index == 1 else f"TwonkyTales{index}")
                self.assertEqual(core["Head"]["MaxPlayer"], "4")
                self.assertEqual(core["Head"]["ForcedNoCrew"], "1")
                self.assertEqual(core["Definitions"]["Definition3"], "TwonkyTales.c4f\\Tales.c4d")

    def test_all_six_indexed_landscapes_are_distinct_and_match_their_core(self):
        landscapes = []
        for chapter in CHAPTERS:
            with self.subTest(chapter=chapter):
                bitmap = (ROOT / f"{chapter}.c4s" / "Map.bmp").read_bytes()
                self.assertEqual(bitmap[:2], b"BM")
                self.assertEqual(struct.unpack_from("<ii", bitmap, 18), (320, 180))
                self.assertEqual(struct.unpack_from("<H", bitmap, 28)[0], 8)
                landscapes.append(bitmap)
        self.assertEqual(len(set(landscapes)), 6)

    def test_each_chapter_has_both_languages_and_a_separate_ending(self):
        endings = []
        for chapter in CHAPTERS:
            for language in ("US", "DE"):
                with self.subTest(chapter=chapter, language=language):
                    folder = ROOT / f"{chapter}.c4s"
                    entries = dict(line.split("=", 1) for line in (folder / f"StringTbl{language}.txt").read_text(encoding="latin1").splitlines() if "=" in line)
                    for key in ("Introduction", "Journal", "Ending"):
                        self.assertTrue(entries[key].strip())
                    self.assertTrue((folder / f"Desc{language}.txt").is_file())
                    self.assertTrue((folder / "Title.png").is_file())
                    endings.append(entries["Ending"])
        self.assertEqual(len(set(endings)), 12)


if __name__ == "__main__":
    unittest.main()
