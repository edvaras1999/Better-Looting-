# Better Looting (Skyrim LE)

Plugin SKSE (1.7.3) para Skyrim LE que junta, num unico plugin, o menu de loot do QuickLootIE
(com o visual do QuickLoot Redesigned), o realce de objetos (Highlight Objects) e a
sobrevivencia (EatingSleepingDrinking), com os patches de animacao.

Estado: **v0.4** - menu de loot completo, MCM, dois visuais e ordenacao. O plugin parte do codigo do
QuickLoot 1.1f (LE) e fala o protocolo do `LootMenuIE.swf` do QuickLootIE.

## Teclas (menu de loot aberto)

- Ativar (E) pega o item selecionado; roda do rato / zoom deslocam a selecao.
- **C**: Pegar tudo.
- **R**: Procurar, ou seja, abre o contentor normal (um toque por omissao; o tempo configura-se).
- Com o menu aberto, C e R sao retiradas do jogo: nao fazem o que fariam com o menu fechado.
- Com o menu fechado, R mantido 2 s liga/desliga o menu de loot (aviso no ecra).

## Ordenacao

Por categoria (ouro, gemas, gemas de alma, lockpicks, barras, minerio, pocoes, comida e bebida, livros,
notas, pergaminhos, flechas e virotes, joias, armas, armaduras, roupas, resto) e, dentro de cada uma, por
nome, valor, peso ou valor por peso (ascendente ou descendente; itens sem peso primeiro, se quiser).
Itens de missao ficam sempre primeiro. Tudo no MCM (pagina General) e no INI.

## Configuracao

- **MCM** (SkyUI): "Better Looting", com paginas General/Geral, Window/Janela e Keys/Teclas, em
  English e Portugues. Todas as opcoes gravam no `BetterLooting.ini`.
- **Visual**: Redesigned (padrao) ou QuickLootIE original. A troca aplica-se na proxima vez que o
  menu de loot abre (o menu e recriado).
- `BetterLooting.ini` fica ao lado da DLL (`Data/SKSE/Plugins`) e tem uma seccao `[Settings]`.

## Estrutura

- `betterlooting/` - codigo C++ do plugin e o projeto Visual Studio
- `papyrus/src` - scripts (`BetterLootingNative.psc`, `BetterLootingMCM.psc`); `papyrus/stubs` so serve para compilar
- `tools/make_esp.py` - gera `BetterLooting.esp` (so a quest do MCM) e o `.seq`
- `dist/Data/` - ficheiros de dados que vao no pacote (SWFs, INI, ESP, SEQ)
- `third_party/` - **coloque aqui** `Quick_Loot_1_1f_Source_Files__for_modders_-73096-1-1f.7z`
  (a libSkyrim usada para compilar; o workflow extrai-o e compila-o)
- `.github/workflows/build.yml` - compila no GitHub Actions e gera o artefacto
  `BetterLooting-instalacao`

## Como testar

1. Faca upload dos ficheiros para o repositorio (podem ir soltos na raiz: o workflow arruma-os).
2. Na aba Actions, abra a execucao e baixe `BetterLooting-instalacao`.
3. Instale com o MO2, deixe o `BetterLooting.esp` ativo e **desative o QuickLoot 1.1f**
   (QuickLoot.dll, QuickLoot.ini, LootMenu.swf).
4. No log do SKSE (`BetterLooting.log`) procure `Funcoes Papyrus registadas` e
   `LootMenuIE.swf: nivel de funcoes = 6` (o nome da linha e o mesmo para os dois visuais).

## Licencas

O codigo parte de QuickLoot 1.1f e QuickLootIE (GPL-3.0-or-later). Os autores originais deram
permissao desde que o resultado seja para Skyrim LE ou VR.
