#!/bin/bash
# Pins the third-party layout: private definitions live with their scenarios,
# only genuinely shared definitions occupy the data root, and the curated
# collection does not repeat scenarios already offered elsewhere.

set -u

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
failures=0

fail() {
	echo "  FAIL: $*" >&2
	failures=$((failures + 1))
}

expect_present() {
	local path="$1"
	if [ -e "$REPO_ROOT/$path" ]; then
		echo "  ok present: $path"
	else
		fail "missing $path"
	fi
}

expect_absent() {
	local path="$1"
	if [ -e "$REPO_ROOT/$path" ]; then
		fail "must not exist: $path"
	else
		echo "  ok absent: $path"
	fi
}

# Read from packs.toml, where each nested definition is recorded next
# to the pack that carries it.
nested_definitions=()
while IFS= read -r path; do
	nested_definitions+=("$path")
done < <("${PYTHON:-python3}" "$REPO_ROOT/tools/packs.py" nested-definitions)
if [ "${#nested_definitions[@]}" -eq 0 ]; then
	echo "  FAIL: packs.toml lists no nested definitions" >&2
	exit 1
fi

echo "scenario-private definitions are nested beside their consumers:"
for path in "${nested_definitions[@]}"; do
	expect_present "$path"
	expect_absent "${path##*/}"
done

echo "definitions shared by separate scenario families stay at the data root:"
for path in \
	Golems.c4d \
	ModernCombat.c4d \
	RopepackRemake.c4d \
	WesternBalancing.c4d \
	WesternBugfixes.c4d
do
	expect_present "$path"
done

echo "unreferenced packs rejected by the compilation stay out:"
for path in EkeBot.c4d KnightMelees.c4d KnightSkirmishs.c4d ZombieWars.c4d; do
	expect_absent "$path"
done

echo "the collection does not repeat scenarios available in another pack:"
duplicate_scenarios=(
	Collection.c4f/Adventures.c4f/Arso-Morf.c4s
	Collection.c4f/Adventures.c4f/Drachenfels.c4s
	Collection.c4f/Adventures.c4f/Goldrush.c4s
	Collection.c4f/Hazard.c4f/Hazard.c4f
	Collection.c4f/Knights.c4f/Queron3_41.c4s
	Collection.c4f/ModernCombat.c4f/ModernCombat.c4f
	Collection.c4f/Settling.c4f/ClonkMars.c4f
)
for path in "${duplicate_scenarios[@]}"; do
	expect_absent "$path"
done

echo "the retained authoritative copies remain available:"
for path in \
	ClonkMars.c4f/01_Fossae.c4s \
	EkeReloaded.c4f/TheStippelAge.c4f/Arso-Morf.c4s \
	Fantasy.c4f/Drachenfels.c4s \
	Hazard.c4f/AH_Predator.c4s \
	Melees.c4f/Queron3.c4s \
	ModernCombat.c4f/CMC_AtlanticCrisis.c4s \
	Western.c4f/Goldrush.c4s
do
	expect_present "$path"
done

echo "E.P.I.C. ships as one self-contained scenario pack:"
for path in \
	E.P.I.C.c4f/E.P.I.C.c4d \
	E.P.I.C.c4f/Death_Canyon.c4s \
	E.P.I.C.c4f/Hills_of_gore.c4s \
	E.P.I.C.c4f/Minor_Wound_Battle.c4s \
	'E.P.I.C.c4f/Test map.c4s' \
	E.P.I.C.c4f/Wound_Valley_King.c4s
do
	expect_present "$path"
done

# Only inside the listed packs: the repository also holds tooling whose build
# output legitimately contains empty directories.
echo "deduplication leaves no empty content directories:"
while IFS= read -r path; do
	fail "empty directory: ${path#"$REPO_ROOT/"}"
done < <(
	"${PYTHON:-python3}" "$REPO_ROOT/tools/packs.py" roots | while IFS= read -r root; do
		[ -d "$REPO_ROOT/$root" ] && find "$REPO_ROOT/$root" -type d -empty
	done
)

if [ "$failures" -ne 0 ]; then
	echo "$failures content layout check(s) failed" >&2
	exit 1
fi
echo "all content layout checks passed"
