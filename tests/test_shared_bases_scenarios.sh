#!/bin/bash
# Pins the deliberate scenario-level split between shared accounts and bases.

set -u

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

shared_base_scenarios=(
	Missions.c4f/Frontier.c4s/Scenario.txt
	Missions.c4f/High.c4s/Scenario.txt
	Missions.c4f/LastWill.c4s/Scenario.txt
	Missions.c4f/Pearls.c4s/Scenario.txt
	Missions.c4f/Rattle.c4s/Scenario.txt
	Missions.c4f/Rocks.c4s/Scenario.txt
	Missions.c4f/SevenKeys.c4s/Scenario.txt
	Missions.c4f/SkyVillage.c4s/Scenario.txt
	Missions.c4f/Submine.c4s/Scenario.txt
	Missions.c4f/Treasure.c4s/Scenario.txt
	Western.c4f/BankRobbery.c4s/Scenario.txt
	Western.c4f/CaptureTheFlag.c4s/Scenario.txt
	Western.c4f/GoldenCanyon.c4s/Scenario.txt
	Western.c4f/Goldrush.c4s/Scenario.txt
	Western.c4f/MilestoneWoods.c4s/Scenario.txt
	Western.c4f/MinersCreek.c4s/Scenario.txt
	Western.c4f/RedrockBay.c4s/Scenario.txt
	Western.c4f/ShootTheSheriff.c4s/Scenario.txt
	Western.c4f/TotemHunt.c4s/Scenario.txt
	Melees.c4f/Canyon.c4s/Scenario.txt
	Knights.c4f/Camp.c4s/Scenario.txt
	Knights.c4f/Dunkelfels.c4s/Scenario.txt
	Knights.c4f/Hammerfest.c4s/Scenario.txt
	Knights.c4f/Regicide.c4s/Scenario.txt
	Worlds.c4f/Ashlands.c4s/Scenario.txt
)

separate_base_scenarios=(
	Missions.c4f/Funnel.c4s/Scenario.txt
	Missions.c4f/Regeneration.c4s/Scenario.txt
	Melees.c4f/Clepal.c4s/Scenario.txt
	Melees.c4f/CliffFight.c4s/Scenario.txt
	Melees.c4f/Greed.c4s/Scenario.txt
	Melees.c4f/Massif.c4s/Scenario.txt
	ClonkMars.c4f/01_Fossae.c4s/Scenario.txt
	ClonkMars.c4f/03_Chaos.c4s/Scenario.txt
	ClonkMars.c4f/04_Kasei.c4s/Scenario.txt
	ClonkMars.c4f/06_Chasma.c4s/Scenario.txt
	ClonkMars.c4f/07_Abyss.c4s/Scenario.txt
	ClonkMars.c4f/08_Phobos.c4s/Scenario.txt
	EkeReloaded.c4f/InterplanetaryCivilwar.c4f/GoldPlateau.c4s/Scenario.txt
	EkeReloaded.c4f/InterplanetaryCivilwar.c4f/OilWars.c4s/Scenario.txt
	EkeReloaded.c4f/TheStippelAge.c4f/Invasion.c4s/Scenario.txt
)

failures=0

has_shared_bases_rule() {
	local scenario="$1" rules
	rules="$(tr -d '\r' <"$REPO_ROOT/$scenario" | awk '
		/^\[Game\]$/ { in_game = 1; next }
		/^\[/ { in_game = 0 }
		in_game && /^Rules=/ { print; exit }
	')"
	case ";${rules#Rules=};" in
		*';SHBS=1;'*) return 0 ;;
		*) return 1 ;;
	esac
}

for scenario in "${shared_base_scenarios[@]}"; do
	if has_shared_bases_rule "$scenario"; then
		echo "  ok: $scenario enables SHBS"
	else
		echo "  FAIL: $scenario must enable SHBS" >&2
		failures=$((failures + 1))
	fi
done

for scenario in "${separate_base_scenarios[@]}"; do
	if has_shared_bases_rule "$scenario"; then
		echo "  FAIL: $scenario must keep separate bases" >&2
		failures=$((failures + 1))
	else
		echo "  ok: $scenario keeps separate bases"
	fi
done

if [ "$failures" -ne 0 ]; then
	echo "$failures Shared Bases scenario check(s) failed" >&2
	exit 1
fi

echo "all Shared Bases scenario checks passed"
