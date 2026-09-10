from pathlib import Path
import sys
import unittest
import tempfile

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import rights_inventory


class RightsInventoryTests(unittest.TestCase):
    def test_packed_dependencies_protect_the_physical_container(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / "Pack.c4f").mkdir()
            (root / "Pack.c4f/Round.c4s").write_bytes(b"packed scenario")
            (root / "Objects.c4d").write_bytes(b"packed definitions")
            references = {
                "Pack.c4f/Round.c4s": [
                    {
                        "reference": "Objects.c4d/Item.c4d",
                        "resolved": "Objects.c4d/Item.c4d",
                    }
                ]
            }
            self.assertEqual(
                rights_inventory.physical_dependencies(root, references),
                {"Pack.c4f/Round.c4s": ["Objects.c4d"]},
            )

    def test_scope_digest_changes_when_covered_resource_bytes_change(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            path = "Round.c4s"
            (root / path).write_bytes(b"original")
            scopes = {path: {"rights": "assumed", "reason": "maintainer decision"}}
            before = rights_inventory.scope_snapshots(root, scopes, [path])
            (root / path).write_bytes(b"modified")
            after = rights_inventory.scope_snapshots(root, scopes, [path])
            self.assertEqual(before[path]["rights"], "assumed")
            self.assertNotEqual(before[path]["sha256"], after[path]["sha256"])

    def test_dependency_inventory_preserves_missing_references(self):
        rows = [
            {"path": "Objects.c4d", "texts": {}},
            {
                "path": "Pack.c4f/Round.c4s",
                "texts": {
                    "Scenario.txt": "[Definitions]\nDefinition1=objects.c4d\nDefinition2=Missing.c4d\n"
                },
            },
        ]
        self.assertEqual(
            rights_inventory.resolve_dependencies(rows),
            {
                "Pack.c4f/Round.c4s": [
                    {"reference": "objects.c4d", "resolved": "Objects.c4d"},
                    {"reference": "Missing.c4d", "resolved": None},
                ]
            },
        )
