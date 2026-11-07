# Better Looting

An improved version of [QuickLoot IE LE](https://github.com/Mhacks79/QuickLoot-LE) by Mhacks79 for
**Skyrim Legendary Edition** (TESV 1.9.32.0.8, SKSE 1.7.3). Better Looting also adds the object
highlight of *Highlight Objects* and the lootable-corpse highlight of *Highlight Lootable Corpses*,
rebuilt for Skyrim LE, and a full MCM.

Look at a chest, a barrel or a body and its contents show up in a small window next to the crosshair.
Take one item, take everything, use or equip it, or open the container the usual way, without going through
the inventory screen. Hold a key to light up nearby loot and lootable corpses.

> Status: **v0.12**. Early versions are tested by one person; expect rough edges. See [CHANGES.md](CHANGES.md).

## Features

- Loot window for containers and corpses, with item icons, stolen / read / enchanted markers, value, weight and
  value-per-weight columns, and your carried weight.
- Two looks: **QuickLoot Redesigned** (default) and the original **QuickLoot IE** window. Switch in the MCM.
- Take, take only one, take all, use / equip, search (open the container). Every action has a remappable key
  and modifier for keyboard and mouse.
- Sorting by category and by name, value, weight or value per weight.
- **Highlight** of the object under the crosshair, with a colored glow (5 colors, 5 intensity steps), filters by
  object type and red for what would be stealing.
- **Area scan**: hold a key to light up nearby objects and **lootable corpses** (corpses you already searched are
  hidden). Works even if the crosshair highlight is off.
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

- `Data/SKSE/Plugins/BetterLooting.dll` and `BetterLooting.ini`
- `Data/BetterLooting.esp` (keep it **enabled**: it holds the MCM quest and the glow shaders) and `Data/SEQ/BetterLooting.seq`
- `Data/Scripts/BetterLootingNative.pex`, `BetterLootingMCM.pex`
- `Data/Interface/LootMenuRedesigned.swf`, `LootMenuIE.swf`
- `Data/Textures/Effects/Gradients/BetterLooting_*.dds` (glow colors and intensities)

Check `Documents/My Games/Skyrim/SKSE/BetterLooting.log`: it should say that the Papyrus functions were registered,
that the shaders and the textures were found, and the loot window functions level.

## Default controls (loot window open)

| Action | Keyboard / mouse |
|---|---|
| Take | E |
| Take only one | Alt+E |
| Use / equip | Shift+E |
| Equip in the right hand | Shift + left mouse button |
| Equip in the left hand | Shift + right mouse button |
| Take all | C |
| Search (open the container) | R |
| Scroll the list | mouse wheel / zoom keys |
| Area scan (loot window closed) | hold E for 500 ms |

All of the above, except scrolling, can be changed in the MCM (page **Keys**): each action has a key and a modifier
(none, Shift, Ctrl, Alt). Keyboard keys and mouse buttons are supported. With the window open, the game does nothing
else with a key that is bound to an action, and its own Activate and Ready Weapon keys do nothing on their own.
Holding the Ready Weapon key for 2 seconds (window closed) turns the loot window on or off.

Use / equip: armor and clothes are equipped; potions, food and ingredients are consumed; ammo is taken whole and
equipped; one-handed weapons go to the hand you choose, two-handed weapons use either. Books, keys and other items are
only taken. If your hands come out swapped, swap `iEquipSlotIdRight` and `iEquipSlotIdLeft` in the INI.

## Configuration

Everything is in the MCM (**Better Looting**) and in `BetterLooting.ini`, which the MCM writes to. Pages:
General (language, look, behavior, sorting), Window (size, position, opacity, columns), Keys, Highlight.

## Objectives

Near term:

- Full **gamepad** support: take, take all, use/equip, search and scrolling, with the right button icons.
- Filters for lootable corpses (quest items, keys, enchanted gear, minimum value, books, consumables).
- A permanent (always-on) corpse highlight and a "searched" list that is saved with the game.
- Use books and scrolls from the loot window.
- Look into the loot window showing fewer rows than expected for short lists.

Later:

- **Phase 3**: survival (EatingSleepingDrinking) and the activation animations patch, integrated into the same plugin.
- Optional KHA-OS settings page, as in QuickLoot IE LE.

## What differs from QuickLoot IE LE

Better Looting needs SkyUI and an ESP (for the MCM and the highlight), which QuickLoot IE LE avoids on purpose.
Better Looting is built on the code base of Quick Loot 1.1f and speaks QuickLoot IE's window protocol; see
[NOTICE.md](NOTICE.md). Gamepad support, visibility conditions and exclusions are not complete yet.

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

GPL-3.0-or-later with the Modding Exception and the GPL-3.0 Linking Exception, as QuickLoot IE LE.

## Credits

QuickLoot IE LE by Mhacks79; Quick Loot 1.1f for LE and libSkyrim; QuickLoot Redesigned; Highlight Objects - EdgeLight Edition; Highlight Lootable Corpses by AmazingBuff; SKSE and SkyUI teams. Developed with AI assistance (Claude).
