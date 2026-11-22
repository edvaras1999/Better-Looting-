# Better Looting

An improved version of [QuickLoot IE LE](https://github.com/Mhacks79/QuickLoot-LE) by Mhacks79 for
**Skyrim Legendary Edition** (TESV 1.9.32.0.8, SKSE 1.7.3). Better Looting also adds the object
highlight of *Highlight Objects* and the lootable-corpse highlight of *Highlight Lootable Corpses*,
rebuilt for Skyrim LE, and a full MCM.

Look at a chest, a barrel or a body and its contents show up in a small window next to the crosshair.
Take one item, take everything, use or equip it, or open the container the usual way, without going through
the inventory screen. Hold a key to light up nearby loot and lootable corpses.

> Status: **v0.14**. Early versions are tested by one person; expect rough edges. See [CHANGES.md](CHANGES.md).

## Features

- Loot window for containers and corpses, with item icons, stolen / read / enchanted markers, value, weight and
  value-per-weight columns, and your carried weight.
- Two looks: **QuickLoot Redesigned** (default) and the original **QuickLoot IE** window. Switch in the MCM.
- Take, take only one, take all, use / equip, search (open the container). Every action has a remappable key
  and modifier for keyboard and mouse, and a remappable button and modifier for the **gamepad**, with the right
  icons in the button bar.
- Sorting by category and by name, value, weight or value per weight.
- **Highlight** of the object under the crosshair, with a colored glow (5 colors, 5 intensity steps), filters by
  object type and red for what would be stealing.
- **Area scan**: hold a key (or a gamepad button) to light up nearby objects and **lootable corpses**. Works even if
  the crosshair highlight is off.
- **Lootable corpses**: optional filters (quest items, keys, enchanted gear, books, consumables, minimum value), an
  optional always-on highlight, and the list of corpses you already searched is saved with your game.
- **Auto-loot** (off by default): picks up loose objects and harvests flora by itself, and makes them glow, following
  your rules: 30 item types, each with its own pick-up and glow switches, glow color and thresholds (minimum value,
  value per weight, armor rating, weapon damage), plus name filters and forbid lists. See below.
- Full MCM (SkyUI) in English and Portuguese; options are saved in `BetterLooting.ini`.
- No Papyrus polling: the highlight and use/equip call the game's native functions from the plugin.

## Requirements

- Skyrim Legendary Edition 1.9.32.0.8
- [SKSE](https://skse.silverlock.org/) 1.7.3
- [SkyUI](https://www.nexusmods.com/skyrim/mods/3863) (for the MCM)
- **Disable Quick Loot 1.1f** (`QuickLoot.dll`, `QuickLoot.ini`, `LootMenu.swf`) and any other loot plugin:
  they compete for the same menu.

## Installation

Install the release archive with your mod manager. It contains:

- `Data/SKSE/Plugins/BetterLooting.dll`, `BetterLooting.ini` and `BetterLooting_AutoLoot.ini`
- `Data/BetterLooting.esp` (keep it **enabled**: it holds the MCM quest and the glow shaders) and `Data/SEQ/BetterLooting.seq`
- `Data/Scripts/BetterLootingNative.pex`, `BetterLootingMCM.pex`
- `Data/Interface/LootMenuRedesigned.swf`, `LootMenuIE.swf`
- `Data/Textures/Effects/Gradients/BetterLooting_*.dds` (glow colors and intensities)

Check `Documents/My Games/Skyrim/SKSE/BetterLooting.log`: it should say that the Papyrus functions were registered,
that the shaders and the textures were found, and the loot window functions level.

## Default controls (loot window open)

| Action | Keyboard / mouse | Gamepad |
|---|---|---|
| Take | E | A |
| Take only one | Alt+E | (none) |
| Use / equip | Shift+E | Y |
| Equip in the right hand | Shift + left mouse button | RB + Y |
| Equip in the left hand | Shift + right mouse button | LB + Y |
| Take all | C | X |
| Search (open the container) | R | Back |
| Scroll the list | mouse wheel / zoom keys | D-pad |
| Area scan (loot window closed) | hold E for 500 ms | (none by default) |

All of the above, except scrolling, can be changed in the MCM (page **Keys**): each action has a key and a modifier
(none, Shift, Ctrl, Alt) for keyboard and mouse, and a button and a modifier (none, LB, RB, LT, RT) for the gamepad.
With the window open, the game does nothing else with a key or button that is bound to an action, and its own Activate
and Ready Weapon keys and buttons do nothing on their own. Holding the Ready Weapon key for 2 seconds (window closed)
turns the loot window on or off.

Use / equip: armor and clothes are equipped; potions, food and ingredients are consumed; scrolls and ammo are equipped
(ammo is taken whole); books are taken and then used, which is **experimental** (if a book is only taken, tell me).
One-handed weapons go to the hand you choose, two-handed weapons use either. Other items are only taken. If your hands
come out swapped, swap `iEquipSlotIdRight` and `iEquipSlotIdLeft` in the INI.

## Auto-loot

Turn it on in the MCM (page **Auto-loot**) and choose what you want in the pages **Auto types**, **Auto colors** and
**Auto limits**. Everything is off until you turn a type on.

- **What it touches:** loose objects, flora (it harvests them) and, optionally, ore veins (you need a pickaxe equipped).
  **Never** corpses, containers, doors, levers or anything owned by someone (that would be stealing). It does nothing
  in combat or with the loot window open.
- **How it decides**, for each object near you: the forbid lists always win; a match in the allow lists is always
  picked up and lit; otherwise the type decides (Pick, Glow, color) and every limit of the type must be met.
- **30 types:** one-handed and two-handed weapons, bows, staves, ammo, heavy and light armor, clothing, shields,
  jewelry, potions, poisons, food, ingredients, scrolls, spell tomes, skill books, other books, keys, empty and filled
  soul gems, gems, ingots, loose ore, lockpicks, gold, torches, other misc items, flora, ore veins.
- **Per type:** pick up, glow, color, minimum value, minimum value per weight, minimum armor rating (armor types),
  minimum damage (weapons and ammo).
- **Rules file:** `BetterLooting_AutoLoot.ini` holds the per-type values (the MCM edits them) and the lists you edit by
  hand: name filters (`exact:`, `contains:`, `starts:`, `ends:`), form filters (`Plugin.esp|0x...`), and forbid lists for
  forms, names and activators (flora and ore veins).
- **Activation:** always on (MCM), a key or gamepad button that turns it on and off, or a key or button you hold. By
  default there are no keys: set them in the MCM.
- **Notes:** books open when they are picked up (so those types are better left off); pick-ups are rate limited and an
  object that cannot be picked up is not retried for 15 seconds.
- **Not included:** a classified storage (everything goes into your inventory) and emptying corpses or containers.

## Configuration

Everything is in the MCM (**Better Looting**) and in `BetterLooting.ini` and `BetterLooting_AutoLoot.ini`, which the MCM
writes to. Pages: General (language, look, behavior, sorting), Window (size, position, opacity, columns), Keys, Highlight,
and four for auto-loot (Auto-loot, Auto types, Auto colors, Auto limits).

## Objectives

Done in this version: auto-loot with per-type rules, thresholds, name and form filters and forbid lists.
Done before: full gamepad support with icons, filters for lootable corpses, an always-on corpse highlight, the
searched-corpses list saved with the game, and use for books and scrolls.

Next:

- **Phase 3**: survival (EatingSleepingDrinking) and the activation animations patch, integrated into the same plugin.
- Confirm that books are read when used from the loot window, and improve it if not.
- Auto-loot: classified storage, and an optional spell as a second way to trigger it.
- Optional KHA-OS settings page, as in QuickLoot IE LE.
- Fill in the remaining authors in `NOTICE.md` before a public release.

Known limitations: with the **Redesigned** look, a short list can show fewer rows than expected (the original look does
not do this); it comes from that movie, which has no source.

## What differs from QuickLoot IE LE

Better Looting needs SkyUI and an ESP (for the MCM and the highlight), which QuickLoot IE LE avoids on purpose.
Better Looting is built on the code base of Quick Loot 1.1f and speaks QuickLoot IE's window protocol; see
[NOTICE.md](NOTICE.md). Visibility conditions and exclusions are not implemented.

## Building from source

Releases are built by GitHub Actions (`.github/workflows/build.yml`), Win32 / Release / v143:

1. Put the Quick Loot 1.1f source archive (`Quick_Loot_1_1f_Source_Files__for_modders_-73096-1-1f.7z`) in `third_party/`.
2. Push. The workflow extracts it, compiles libSkyrim and the plugin, compiles the Papyrus scripts with Caprica
   and builds the installation package (artifact `BetterLooting-instalacao`).

`tools/make_esp.py` generates `BetterLooting.esp`, the `.seq` file and the glow textures.

## Repository layout

- `betterlooting/` C++ source and Visual Studio project
- `papyrus/` scripts (`src`) and the stubs used only to compile them
- `tools/make_esp.py` ESP, SEQ and texture generator
- `dist/Data/` files that go in the package (looks, INI, ESP, SEQ, textures)
- `third_party/` the Quick Loot 1.1f source archive used to build

## License

GPL-3.0-or-later with the Modding Exception and the GPL-3.0 Linking Exception, as QuickLoot IE and QuickLoot IE LE.
See [LICENSE](LICENSE), [EXCEPTIONS.md](EXCEPTIONS.md), [MIT](MIT) and [NOTICE.md](NOTICE.md).

## Credits

QuickLoot IE LE by Mhacks79; QuickLoot IE by Miss Corruption and contributors; QuickLootEE by ilian53; QuickLootRE by
ryan-rsm-mckenzie; Quick Loot 1.1f for LE and libSkyrim; QuickLoot Redesigned; Highlight Objects - EdgeLight Edition;
Highlight Lootable Corpses by AmazingBuff; SKSE and SkyUI teams. Developed with AI assistance (Claude).
