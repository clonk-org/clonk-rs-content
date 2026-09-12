# The Twonky Tales

Six sequential cooperative scenarios, authored as a prelude inspired by the
legendary equipment in Dragon Rock. This is new fiction, not a claim about an
established Twonky canon. Mara, Orren, Veyr and the supporting cast are original.
No Twonky film assets or dialogue were imported. Research sources and the
runtime dependencies are credited inside the campaign.

## Playing

Open **The Twonky Tales** in the scenario browser. Only **A Very Fine Shot** is
initially accessible. Winning a chapter grants the access token for the next;
completed chapters remain replayable. The last discovery stays open until all
players dismiss their pages; the ending then gets its own acknowledgement.
Unlocks use the same native mission-access
store as the challenges. Saves within a chapter retain its object references,
arrow histories, mechanisms, damage, materials and followers.

One to four players share a team. The first user slot, normally the host, is
Twonky. Companions have their own controllable knights. The bow is a real,
transferable BOW1 object, recognizable by its name. Its shot has no random aim
deviation and three times the ordinary bow's launch velocity. The three starting
arrows remain individual objects. After the grip is repaired in chapter two,
use the bow's **Recall the arrows** context action. Arrows return along their
recorded outward positions through the current landscape, stop at new walls,
and can trigger mechanisms that have moved since the outward shot. They do not
home on a player or jump into an inventory. Retrieve them to fire again.

Grab a mechanism and press **Up** to inspect or **Throw** to operate. The context
menu offers both actions. Read the goal for the chapter journal. Talk to named
non-player characters through their context menu; eligible survivors follow the
last player to speak to them. Workshop chests support ordinary grab/put/get
controls and can be pushed. Overhead mechanisms can be inspected from the
gallery floor; operating a hand control still requires being beside it.
Repair and forging recipes can draw from the actor's
inventory or a workshop chest within 45 pixels.

Failure has local, lasting costs: health, rubble, flooded chambers, seized cargo,
or additional armed guards. Repairs consume real metal and wood. They do not
correct a bad setting or erase guards, rubble or lost supplies. Every named person belongs to the continuing story: their death, or destruction
of the unique bow, presents an explicit defeat and grants no unlock. Exhausting
recoverable supplies can also strand a run. There
is no automatic rescue, arrow replacement, restart reward or invisible reset.
Disconnected players leave carried items behind.

## Design and callbacks

| Chapter | Distinct task | What returns later |
| --- | --- | --- |
| A Very Fine Shot | Solve three simultaneous water loads, release a catch, escort three survivors | The precise village geography, catch, bell and old spillway |
| The Mill Below | Repair the bow, record and reverse one shot through a movable receiver, load a physical counterweight | The bow's limit, the coupling stroke, three metal and one wooden brake |
| The King's Account | Reconcile actual cargo with three records, deliver the cart, expose a duplicate seal | Veyr's network, both makers' signatures, the player's own involvement |
| A Hero's Welcome | Present an uncomfortable complete account, operate prison ventilation, free and escort three witnesses | The two private acts of resistance and the limits of their excuses |
| The Road Back | Revisit inverted plumbing, place a return path before a catch moves, escort survivors and deliver tunnel supports | The supposedly useless spillway becomes the mountain approach |
| A Bow for Tomorrow | Combine the original balance, returning shot, moved receiver and held anchor; relinquish the equipment | A bow left for the next people entering Dragon Rock |

Twonky's change is enacted: accepting collective credit, accepting a profitable
commission, refusing to edit the names of friends out of evidence, returning to
help, and finally taking the load while another person holds the bow. Veyr uses
real culpability as an excuse for his continuing choices; the makers' later help
does not rewrite their earlier decisions. The existing Dragon Rock adventure,
its dragon and Ala Kadabra remain available separately.

Each chapter is a self-contained start with the inventory needed for that
chapter's canonical opening. Unlocks carry campaign progress; scarce materials
and injuries persist within a chapter and its saves, not as an escalating
cross-chapter inventory tax. This keeps replay and joining friends practical.
In solo play the final anchor uses the exact physical counterweight learned in
the mill. In multiplayer Twonky must remain beside the anchor without the bow,
and another player must make the recorded shot. All other machinery latches,
so coordinated teams can divide work without requiring simultaneous button
presses that would make solo play impossible.

## Spoiler walkthrough for review and playtesting

These notes are outside the shipped pack. They describe the intended deductions;
they do not replace movement, aiming, cargo handling or survival.

1. **Village:** intake 2, bypass 1, relief 1 gives loads 5, 4, 2. Commit at the
   pump, strike the school catch outward, open the old spillway, and talk to
   Pip, Iven and Nell. Escort all three to the bell and ring it. A failed pump
   test needs two metal and one wood before the next test.
2. **Mill:** repair the grip for two metal and one wood. Select RECEIVE (1),
   shoot the east witness, then select RETURN (2) and lower the receiver two
   stops. Recall the same arrow through it. Move the counterweight chest beside
   the press with exactly three metal and one wood. Operate the press and read
   the drawing beyond its gate. Repair a jam before retrying.
3. **Account:** keep three metal and two wood in the consignment. Province is
   `(3*metal+wood)%4`, road is `(metal+2*wood)%3`, toll is `metal-wood`: 3, 1, 1.
   Submit beside dispatch and deliver the full cart to the receipt desk. With
   the lamp at 0, record an outward shot through the seal. After delivery,
   switch the lamp to 1 and recall that arrow. Read the duplicate below.
4. **Prison:** declare three intakes, two warehouses, BOTH makers (2). A lie
   summons a persistent armed bailiff. Vent 1 records the outward shot through
   the pardon ratchet; vent 3 accepts its return after the docket is cleared.
   Read the original confession, talk to the three prisoners, announce escape
   and escort all three to the rendezvous.
5. **Return:** open the old spillway. Before balancing, shoot along the old
   school catch's line while it is still raised out of that line. Intake 1,
   bypass 2, relief 1 restores the original safe loads and lowers the catch.
   Recall the recorded path through it. A return shot also releases the buried
   keystone. Push the convoy beside the tunnel bench and spend two metal and
   three wood there. Bring the same three villagers home and ring the bell.
6. **Mountain:** restore 2, 1, 1. Record an outward shot at the witness; move the
   receiver two stops. In a group, hand the bow and ammunition to a companion
   before this shot, then have Twonky take and remain at the anchor. Alone,
   position the mill's exact counterweight beside the anchor and operate it.
   Recall the same arrow. Demand surrender, escort Sella to the cache and place
   the bow plus at least three arrows into the legacy chest. Seal the cache.

## Maintenance

The shared definitions live in `TwonkyTales.c4f/Tales.c4d`. Scenario scripts own
their mechanics and narrative conditions; `TTSC` owns common inventory, joining,
escorting, arrows, failure and goal plumbing. Changes affect this pack only.
Existing Dragon Rock and other imported content are untouched.

`tools/generate_twonky_maps.py` rebuilds the deterministic indexed landscapes.
`tools/generate_twonky_art.py` rebuilds the original scenery and chapter plates.
Both use Pillow; the art generator uses DejaVu Serif from the usual Arch font
path. Generated PNG/BMP assets are checked in; players need neither dependency.
Game scripts and localization tables use Latin-1/ASCII and CRLF.

The engine's `twonky_tales` integration tests load this shipped pack and exercise
native scripts and physics. They exercise every chapter ending, real outbound
and returning shots, solo and cooperative anchor operation, native mission
unlocks, material repair and seizure, NPC escorts, disconnect drops, explicit
defeat, and serialized state during a return. Normal controls also walk to and
operate the village intake and old spillway, preserve carried items while
reading a new page, and lead the final escort down the drain stairs.
Fixture positioning and inventory transfers isolate other mechanism contracts;
these are not complete input-only playthroughs.
Use the spoiler route above for multiplayer difficulty and pacing playtests.
