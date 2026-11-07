# Changes

Better Looting is an improved version of QuickLoot IE LE (see [NOTICE.md](NOTICE.md)). This file lists what
it adds or changes, newest first.

## 0.12

- **Remappable keys.** Every action of the loot window (take, take one, use/equip, take all, search, equip in
  the right hand, equip in the left hand) has its own key and modifier (Shift, Ctrl or Alt), for keyboard and
  mouse. All of them are on the **Keys** page of the MCM.
- **Use / equip** from the loot window (Shift+E): armor and clothes are equipped, potions, food and
  ingredients are consumed, ammo is taken whole and equipped. **Shift + left mouse button** equips a
  one-handed weapon in the right hand, **Shift + right mouse button** in the left hand; two-handed weapons
  use either. Books, keys and other items are only taken.
- **Take only one** moves to **Alt+E** (it was Shift+E).
- The game's own Activate and Ready Weapon keys no longer do anything on their own while the loot window is
  open on the keyboard, so remapping is strict.
- MCM rebuilt: all key options are on the Keys page; the dead "take whole stack" option is gone.
- README, INI and license documents in English; license documents follow QuickLoot IE LE.

## 0.11

- Glow intensity is now the brightness of the glow color (5 steps: 100, 60, 35, 18 and 8%), because the
  shader blends additively and ignores alpha.

## 0.9 - 0.10

- Glow intensity steps, scan independent of the crosshair highlight, texture check at start-up.

## 0.8

- Colored glow: one solid-color palette texture per color; lootable corpses glow under the crosshair too.

## 0.6 - 0.7

- Area scan: hold a key to light up nearby objects and lootable corpses (corpses already searched are hidden).

## 0.5

- Player weight in the loot window.
- Highlight of the object under the crosshair, with filters by object type and red for what would be stealing.
  The highlight uses the game's own effect shaders (`BetterLooting.esp`), applied by calling the native
  `EffectShader.Play` directly: no Papyrus scripts.

## 0.4

- Sorting by category (gold, gems, soul gems, lockpicks, ingots, ores, potions, food, books, notes, scrolls,
  ammo, jewelry, weapons, armor, clothes, other) and by name, value, weight or value per weight.

## 0.1 - 0.3

- Loot window with the QuickLoot IE protocol (two looks: QuickLoot Redesigned, the default, and the original),
  take all (C), search (R), MCM in English and Portuguese, options saved in `BetterLooting.ini`.
