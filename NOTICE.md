# Notices

## This work

Better Looting is an improved version of **QuickLoot IE LE** by Mhacks79
(a port of QuickLoot IE 4.1.3 by Miss Corruption, https://github.com/MissCorruption/QuickLootIE)
for **Skyrim Legendary Edition** (TESV 1.9.32.0.8, SKSE 1.7.3). It keeps the same license terms
as that project: **GPL-3.0-or-later WITH the Modding Exception AND the GPL-3.0 Linking Exception
(with Corresponding Source)**. See [LICENSE](LICENSE) and [EXCEPTIONS.md](EXCEPTIONS.md), reproduced
unchanged. See [CHANGES.md](CHANGES.md) for what Better Looting adds.

This program comes with ABSOLUTELY NO WARRANTY, to the extent permitted by law (GPL-3.0,
sections 15 and 16).

## Lineage

QuickLoot IE is the end of a chain of forks, and Better Looting inherits all of it:

1. **QuickLootRE** by ryan-rsm-mckenzie, MIT license, 2018.
2. **QuickLoot EE** by ilian53, a fork of QuickLootRE under the same MIT license.
3. **QuickLoot IE** by Miss Corruption and contributors, relicensed under GPL-3.0-or-later with the
   two exceptions, keeping the MIT notice for the code it took over.
4. **QuickLoot IE LE** by Mhacks79, a port of QuickLoot IE 4.1.3 to Skyrim LE.
5. **Better Looting** (this project).

The MIT license lets its code be reused in a GPL project on one condition: its copyright and
permission notice must stay with every copy. That is the only reason the [MIT](MIT) file is here,
reproduced unchanged. It is not the license of Better Looting as a whole.

## Code base and what comes from where

Better Looting is a separate implementation. Its loot menu is built on the code base of
**Quick Loot 1.1f** for Skyrim LE (source files "for modders", Nexus Skyrim LE mod 73096) and the
libSkyrim library that comes with it, and it speaks the loot window protocol of QuickLoot IE, so it can
drive the original QuickLoot IE movie and the QuickLoot Redesigned movie. Ideas about the controls and
the use/equip behaviour follow QuickLoot IE LE.

| Component | Used for | Author | License |
|---|---|---|---|
| QuickLoot IE 4.1.3 | `LootMenuIE.swf` (original movie, header unchanged) and the loot window protocol | Miss Corruption and contributors | GPL-3.0-or-later with exceptions; MIT notice for inherited code |
| QuickLoot Redesigned (Nexus 192433) | `LootMenuRedesigned.swf` (shipped as the default look) | its author | see its Nexus page; it is a movie for QuickLoot IE and ships QuickLoot IE's license file |
| Quick Loot 1.1f for LE (Nexus 73096) and libSkyrim | code base of the plugin | **TO BE CONFIRMED** | **TO BE CONFIRMED** (the source files carry no license) |
| Highlight Objects - EdgeLight Edition (Nexus 183648) | shader data and the logic of the object highlight (`BetterLooting.esp`, `tools/make_esp.py`) | **TO BE CONFIRMED** | GPL-3.0-or-later with exceptions (`COPYING.txt`, `EXCEPTIONS.md` of that project) |
| Highlight Lootable Corpses (Nexus 191076) by AmazingBuff | logic of the lootable corpses highlight | AmazingBuff | GPL-3.0 (`LICENSE` of that project) |
| SKSE 1.7.3 | build dependency | Ian Patterson, Stephen Abel, Paul Connelly and the SKSE team | SKSE license |
| SkyUI | MCM (runtime dependency, not included) | SkyUI team | see its Nexus page |

Entries marked TO BE CONFIRMED must be filled in before a public release.

## Developed with AI assistance

Better Looting was developed with the help of an AI assistant (Claude, by Anthropic), under the
direction and testing of its author.
