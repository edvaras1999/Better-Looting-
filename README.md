# Better Looting (Skyrim LE)

Plugin SKSE (1.7.3) para Skyrim LE que junta, num unico plugin, o menu de loot do QuickLootIE
(com o visual do QuickLoot Redesigned), o realce de objetos (Highlight Objects) e a
sobrevivencia (EatingSleepingDrinking), com os patches de animacao.

Estado: **v0.9** - menu de loot completo (com peso do jogador), MCM, dois visuais, ordenacao, realce de objetos na mira, varrimento por raio e cadaveres com loot. O plugin parte do codigo do
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

## Realce de objetos (Highlight)

O objeto para que olhas ganha um brilho (efeito de shader do jogo, aplicado pela DLL sem scripts Papyrus).
Cor e duracao configuraveis; o que seria roubo fica a vermelho; filtro por tipo de objeto (ativadores,
contentores, portas, flora, mobilia, armaduras, armas, municao, livros, ingredientes, chaves, misc, pocoes,
pergaminhos, gemas de alma). Os shaders vivem no `BetterLooting.esp` (gerado por `tools/make_esp.py` a partir
dos dados do Highlight Objects EdgeLight Edition). A cor do brilho vem de 5 paletas (`dist/Data/Textures/Effects/Gradients/BetterLooting_<cor>.dds`,
geradas tambem pelo `make_esp.py`). O contorno em D3D11 do original nao existe no LE.

**Varrimento por raio:** mantendo o **E** premido (500 ms por omissao; tecla e tempo no MCM) realca os objetos
a volta (raio 1500, 3 s) e tambem os **cadaveres com loot** (verde por omissao). Manter premido refresca o brilho.
So com o menu de loot fechado; um toque curto em E continua a ativar normalmente. Cadaveres de que se pegou
algo (ou abertos com Procurar) deixam de ser realcados, nesta sessao de jogo.
Ainda por fazer: filtros de valor dos cadaveres (itens de missao, chaves, equipamento encantado, valor minimo,
livros, consumiveis) e realce permanente dos cadaveres.

## Configuracao

- **MCM** (SkyUI): "Better Looting", com paginas General/Geral, Window/Janela, Keys/Teclas e Highlight/Realce, em
  English e Portugues. Todas as opcoes gravam no `BetterLooting.ini`.
- **Visual**: Redesigned (padrao) ou QuickLootIE original. A troca aplica-se na proxima vez que o
  menu de loot abre (o menu e recriado).
- `BetterLooting.ini` fica ao lado da DLL (`Data/SKSE/Plugins`) e tem uma seccao `[Settings]`.

