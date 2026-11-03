# Better Looting (Skyrim LE)

Plugin SKSE (1.7.3) para Skyrim LE que junta, num unico plugin, o menu de loot do QuickLootIE
(com o visual do QuickLoot Redesigned), o realce de objetos (Highlight Objects) e a
sobrevivencia (EatingSleepingDrinking), com os patches de animacao.

Estado: **v0.1** - so o menu de loot. O plugin parte do codigo do QuickLoot 1.1f (LE) e passa a
falar o protocolo do `LootMenuIE.swf` do QuickLootIE.

## Estrutura

- `betterlooting/` - codigo C++ do plugin e o projeto Visual Studio
- `dist/Data/` - ficheiros de dados (SWF e INI) que vao no pacote
- `third_party/` - **coloque aqui** `Quick_Loot_1_1f_Source_Files__for_modders_-73096-1-1f.7z`
  (a libSkyrim usada para compilar; o workflow extrai-o e compila-o)
- `.github/workflows/build.yml` - compila no GitHub Actions e gera o artefacto
  `BetterLooting-instalacao`

## Como testar

1. Faca upload dos ficheiros para o repositorio (podem ir soltos na raiz: o workflow arruma-os).
2. Na aba Actions, abra a execucao e baixe `BetterLooting-instalacao`.
3. Instale com o MO2 e **desative o QuickLoot 1.1f** (QuickLoot.dll, QuickLoot.ini, LootMenu.swf).
4. Abra o jogo, olhe para um cadaver ou contentor e confirme que a lista aparece.
5. No log do SKSE (`BetterLooting.log`) procure `LootMenuIE.swf: nivel de funcoes = 6`.

## Licencas

O codigo parte de QuickLoot 1.1f e QuickLootIE (GPL-3.0-or-later). Os autores originais deram
permissao desde que o resultado seja para Skyrim LE ou VR.
