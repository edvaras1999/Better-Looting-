Scriptname BetterLootingMCM extends SKI_ConfigBase
{ Menu MCM do Better Looting (SkyUI). Textos em English e Portugues (sem acentos, por causa das fontes do jogo).
  Todas as opcoes gravam no BetterLooting.ini atraves da DLL; nada fica guardado no save. }

int lang = 0

; ---------------- interruptores ----------------
string[] tKey
string[] tEN
string[] tPT
int[] tOpt

; ---------------- sliders ----------------
string[] sKey
string[] sEN
string[] sPT
float[] sMin
float[] sMax
float[] sStep
string[] sFmt
int[] sOpt

; ---------------- teclas (tecla + modificador) ----------------
string[] aKey
string[] aMod
string[] aEN
string[] aPT
int[] aOptKey
int[] aOptMod
string[] pKey
string[] pMod
int[] pOptKey
int[] pOptMod
int[] padMask
string[] padName

; ---------------- apanhar automatico ----------------
string[] rName
string[] rEN
string[] rPT
int[] rExtra
int[] oPick
int[] oGlow
int[] oColor
int[] oMinVal
int[] oMinVpw
int[] oMinX
int oALKeyToggle
int oALKeyHold
int oALPadToggle
int oALPadHold

; ---------------- menus ----------------
int oLang
int oVisual
int oAnchorX
int oAnchorY
int oSortBy
int oHLColor
int oHLCorpseColor

int Function GetVersion()
	return 2
EndFunction

Event OnConfigInit()
	ModName = "Better Looting"
	BuildPages()
	BuildTables()
EndEvent

Event OnVersionUpdate(int a_version)
	BuildPages()
	BuildTables()
EndEvent

Function BuildPages()
	Pages = new string[8]
	Pages[0] = "General / Geral"
	Pages[1] = "Window / Janela"
	Pages[2] = "Keys / Teclas"
	Pages[3] = "Highlight / Realce"
	Pages[4] = "Auto-loot / Apanhar"
	Pages[5] = "Auto types / Tipos"
	Pages[6] = "Auto colors / Cores"
	Pages[7] = "Auto limits / Limiares"
EndFunction

string Function L(string en, string pt)
	if lang == 1
		return pt
	endif
	return en
EndFunction

Function BuildTables()
	tKey = new string[43]
	tEN = new string[43]
	tPT = new string[43]
	tOpt = new int[43]
	tKey[0] = "bDisableInCombat"
	tEN[0] = "Disable in combat"
	tPT[0] = "Desativar em combate"
	tKey[1] = "bDisableTheft"
	tEN[1] = "Disable when stealing"
	tPT[1] = "Desativar em roubo"
	tKey[2] = "bDisablePickpocketing"
	tEN[2] = "Disable when pickpocketing"
	tPT[2] = "Desativar em pickpocket"
	tKey[3] = "bDisableIfEmpty"
	tEN[3] = "Disable if the container is empty"
	tPT[3] = "Desativar se o contentor estiver vazio"
	tKey[4] = "bCloseIfEmpty"
	tEN[4] = "Close when it becomes empty"
	tPT[4] = "Fechar quando ficar vazio"
	tKey[5] = "bShowValue"
	tEN[5] = "Show value column"
	tPT[5] = "Mostrar coluna de valor"
	tKey[6] = "bShowWeight"
	tEN[6] = "Show weight column"
	tPT[6] = "Mostrar coluna de peso"
	tKey[7] = "bShowValuePerWeight"
	tEN[7] = "Show value per weight column"
	tPT[7] = "Mostrar coluna de valor por peso"
	tKey[8] = "bShowItemIcons"
	tEN[8] = "Show item icons"
	tPT[8] = "Mostrar icones dos itens"
	tKey[9] = "bSortByCategory"
	tEN[9] = "Group by category (gold, gems, potions, weapons...)"
	tPT[9] = "Agrupar por categoria (ouro, gemas, pocoes, armas...)"
	tKey[10] = "bSortDescending"
	tEN[10] = "Descending order"
	tPT[10] = "Ordem descendente"
	tKey[11] = "bSortWeightlessFirst"
	tEN[11] = "Weightless items first"
	tPT[11] = "Itens sem peso primeiro"
	tKey[12] = "bHLEnabled"
	tEN[12] = "Highlight objects under the crosshair"
	tPT[12] = "Realcar objetos na mira"
	tKey[13] = "bHLRedStealable"
	tEN[13] = "Red highlight for what would be stealing"
	tPT[13] = "Realce vermelho para o que seria roubo"
	tKey[14] = "bHLIgnoreUnnamed"
	tEN[14] = "Ignore objects without a name"
	tPT[14] = "Ignorar objetos sem nome"
	tKey[15] = "bHLActivator"
	tEN[15] = "Activators"
	tPT[15] = "Ativadores"
	tKey[16] = "bHLTalkingActivator"
	tEN[16] = "Talking activators"
	tPT[16] = "Ativadores falantes"
	tKey[17] = "bHLContainer"
	tEN[17] = "Containers"
	tPT[17] = "Contentores"
	tKey[18] = "bHLDoor"
	tEN[18] = "Doors"
	tPT[18] = "Portas"
	tKey[19] = "bHLFlora"
	tEN[19] = "Flora and trees"
	tPT[19] = "Flora e arvores"
	tKey[20] = "bHLFurniture"
	tEN[20] = "Furniture"
	tPT[20] = "Mobilia"
	tKey[21] = "bHLArmor"
	tEN[21] = "Armor and clothes"
	tPT[21] = "Armaduras e roupas"
	tKey[22] = "bHLWeapon"
	tEN[22] = "Weapons"
	tPT[22] = "Armas"
	tKey[23] = "bHLAmmo"
	tEN[23] = "Ammo"
	tPT[23] = "Municao"
	tKey[24] = "bHLBook"
	tEN[24] = "Books"
	tPT[24] = "Livros"
	tKey[25] = "bHLIngredient"
	tEN[25] = "Ingredients"
	tPT[25] = "Ingredientes"
	tKey[26] = "bHLKey"
	tEN[26] = "Keys"
	tPT[26] = "Chaves"
	tKey[27] = "bHLMisc"
	tEN[27] = "Misc items"
	tPT[27] = "Itens diversos"
	tKey[28] = "bHLPotion"
	tEN[28] = "Potions"
	tPT[28] = "Pocoes"
	tKey[29] = "bHLScroll"
	tEN[29] = "Scrolls"
	tPT[29] = "Pergaminhos"
	tKey[30] = "bHLSoulGem"
	tEN[30] = "Soul gems"
	tPT[30] = "Gemas de alma"
	tKey[31] = "bHLScanEnabled"
	tEN[31] = "Area scan (hold the key)"
	tPT[31] = "Varrimento por raio (manter a tecla)"
	tKey[32] = "bHLCorpses"
	tEN[32] = "Highlight lootable corpses (crosshair and scan)"
	tPT[32] = "Realcar cadaveres com loot (mira e varrimento)"
	tKey[33] = "bHLCorpseHideSearched"
	tEN[33] = "Hide corpses already searched"
	tPT[33] = "Esconder cadaveres ja revistados"
	tKey[34] = "bHLCorpseFilter"
	tEN[34] = "Only highlight corpses with interesting loot"
	tPT[34] = "Realcar so cadaveres com loot interessante"
	tKey[35] = "bHLCFQuest"
	tEN[35] = "Interesting: quest items"
	tPT[35] = "Interessante: itens de missao"
	tKey[36] = "bHLCFKey"
	tEN[36] = "Interesting: keys"
	tPT[36] = "Interessante: chaves"
	tKey[37] = "bHLCFEnchanted"
	tEN[37] = "Interesting: enchanted weapons and armor"
	tPT[37] = "Interessante: armas e armaduras encantadas"
	tKey[38] = "bHLCFBooks"
	tEN[38] = "Interesting: books"
	tPT[38] = "Interessante: livros"
	tKey[39] = "bHLCFConsumables"
	tEN[39] = "Interesting: potions, food, ingredients and scrolls"
	tPT[39] = "Interessante: pocoes, comida, ingredientes e pergaminhos"
	tKey[40] = "bHLCorpseAlways"
	tEN[40] = "Always highlight lootable corpses (no key needed)"
	tPT[40] = "Realcar sempre os cadaveres com loot (sem tecla)"
	tKey[41] = "bAutoLoot"
	tEN[41] = "Auto-loot (picks up and lights what your rules allow)"
	tPT[41] = "Apanhar automatico (apanha e da brilho ao que as regras permitem)"
	tKey[42] = "bALAlways"
	tEN[42] = "Always on (no key needed)"
	tPT[42] = "Sempre ligado (sem tecla)"
	sKey = new string[21]
	sEN = new string[21]
	sPT = new string[21]
	sMin = new float[21]
	sMax = new float[21]
	sStep = new float[21]
	sFmt = new string[21]
	sOpt = new int[21]
	sKey[0] = "iScale"
	sEN[0] = "Window scale"
	sPT[0] = "Escala da janela"
	sMin[0] = 25.0
	sMax[0] = 400.0
	sStep[0] = 5.0
	sFmt[0] = "{0}%"
	sKey[1] = "iMaxLines"
	sEN[1] = "Visible lines (maximum)"
	sPT[1] = "Linhas visiveis (maximo)"
	sMin[1] = 1.0
	sMax[1] = 25.0
	sStep[1] = 1.0
	sFmt[1] = "{0}"
	sKey[2] = "iMinLines"
	sEN[2] = "Visible lines (minimum)"
	sPT[2] = "Linhas visiveis (minimo)"
	sMin[2] = 0.0
	sMax[2] = 25.0
	sStep[2] = 1.0
	sFmt[2] = "{0}"
	sKey[3] = "iItemLimit"
	sEN[3] = "Maximum items in the list (0 = no limit)"
	sPT[3] = "Maximo de itens na lista (0 = sem limite)"
	sMin[3] = 0.0
	sMax[3] = 200.0
	sStep[3] = 1.0
	sFmt[3] = "{0}"
	sKey[4] = "iOffsetX"
	sEN[4] = "Horizontal offset"
	sPT[4] = "Deslocamento horizontal"
	sMin[4] = -1000.0
	sMax[4] = 1000.0
	sStep[4] = 10.0
	sFmt[4] = "{0} px"
	sKey[5] = "iOffsetY"
	sEN[5] = "Vertical offset"
	sPT[5] = "Deslocamento vertical"
	sMin[5] = -1000.0
	sMax[5] = 1000.0
	sStep[5] = 10.0
	sFmt[5] = "{0} px"
	sKey[6] = "iAlphaNormal"
	sEN[6] = "Opacity"
	sPT[6] = "Opacidade"
	sMin[6] = 10.0
	sMax[6] = 100.0
	sStep[6] = 5.0
	sFmt[6] = "{0}%"
	sKey[7] = "iAlphaEmpty"
	sEN[7] = "Opacity when empty"
	sPT[7] = "Opacidade quando vazio"
	sMin[7] = 0.0
	sMax[7] = 100.0
	sStep[7] = 5.0
	sFmt[7] = "{0}%"
	sKey[8] = "iSearchHoldMs"
	sEN[8] = "Search: time to hold the key (0 = one tap)"
	sPT[8] = "Procurar: tempo a manter a tecla (0 = um toque)"
	sMin[8] = 0.0
	sMax[8] = 1000.0
	sStep[8] = 50.0
	sFmt[8] = "{0} ms"
	sKey[9] = "iHLCrosshairMs"
	sEN[9] = "Crosshair highlight duration"
	sPT[9] = "Duracao do realce na mira"
	sMin[9] = 100.0
	sMax[9] = 10000.0
	sStep[9] = 100.0
	sFmt[9] = "{0} ms"
	sKey[10] = "iHLScanHoldMs"
	sEN[10] = "Scan: time to hold the key (0 = one tap)"
	sPT[10] = "Varrimento: tempo a manter a tecla (0 = um toque)"
	sMin[10] = 0.0
	sMax[10] = 2000.0
	sStep[10] = 50.0
	sFmt[10] = "{0} ms"
	sKey[11] = "iHLScanRadius"
	sEN[11] = "Scan radius"
	sPT[11] = "Raio do varrimento"
	sMin[11] = 300.0
	sMax[11] = 5000.0
	sStep[11] = 100.0
	sFmt[11] = "{0}"
	sKey[12] = "iHLScanMs"
	sEN[12] = "Scan glow duration"
	sPT[12] = "Duracao do brilho do varrimento"
	sMin[12] = 500.0
	sMax[12] = 10000.0
	sStep[12] = 500.0
	sFmt[12] = "{0} ms"
	sKey[13] = "iHLScanMax"
	sEN[13] = "Maximum objects per scan"
	sPT[13] = "Maximo de objetos por varrimento"
	sMin[13] = 8.0
	sMax[13] = 128.0
	sStep[13] = 8.0
	sFmt[13] = "{0}"
	sKey[14] = "iHLCorpseMaxDist"
	sEN[14] = "Corpse highlight distance"
	sPT[14] = "Distancia do realce de cadaveres"
	sMin[14] = 500.0
	sMax[14] = 5000.0
	sStep[14] = 100.0
	sFmt[14] = "{0}"
	sKey[15] = "iHLFillPercent"
	sEN[15] = "Glow strength"
	sPT[15] = "Intensidade do brilho"
	sMin[15] = 5.0
	sMax[15] = 100.0
	sStep[15] = 5.0
	sFmt[15] = "{0}%"
	sKey[16] = "iHLCMinValue"
	sEN[16] = "Interesting: minimum item value (0 = off)"
	sPT[16] = "Interessante: valor minimo do item (0 = desligado)"
	sMin[16] = 0.0
	sMax[16] = 5000.0
	sStep[16] = 50.0
	sFmt[16] = "{0}"
	sKey[17] = "iALPickupRadius"
	sEN[17] = "Auto-loot pick-up radius"
	sPT[17] = "Raio de apanhar do apanhar automatico"
	sMin[17] = 50.0
	sMax[17] = 1000.0
	sStep[17] = 10.0
	sFmt[17] = "{0}"
	sKey[18] = "iALGlowRadius"
	sEN[18] = "Auto-loot glow radius"
	sPT[18] = "Raio do brilho do apanhar automatico"
	sMin[18] = 300.0
	sMax[18] = 5000.0
	sStep[18] = 100.0
	sFmt[18] = "{0}"
	sKey[19] = "iALMaxPerTick"
	sEN[19] = "Maximum pick-ups every half second"
	sPT[19] = "Maximo de apanhas a cada meio segundo"
	sMin[19] = 1.0
	sMax[19] = 10.0
	sStep[19] = 1.0
	sFmt[19] = "{0}"
	sKey[20] = "iALHoldMs"
	sEN[20] = "Time to hold the hold key (0 = one tap)"
	sPT[20] = "Tempo a manter a tecla premida (0 = um toque)"
	sMin[20] = 0.0
	sMax[20] = 2000.0
	sStep[20] = 50.0
	sFmt[20] = "{0} ms"
	aKey = new string[8]
	aMod = new string[8]
	aEN = new string[8]
	aPT = new string[8]
	aOptKey = new int[8]
	aOptMod = new int[8]
	aKey[0] = "iKeyTake"
	aMod[0] = "iModTake"
	aEN[0] = "Take"
	aPT[0] = "Pegar"
	aKey[1] = "iKeyTakeOne"
	aMod[1] = "iModTakeOne"
	aEN[1] = "Take only one"
	aPT[1] = "Pegar so um"
	aKey[2] = "iKeyUse"
	aMod[2] = "iModUse"
	aEN[2] = "Use / equip"
	aPT[2] = "Usar / equipar"
	aKey[3] = "iKeyTakeAll"
	aMod[3] = "iModTakeAll"
	aEN[3] = "Take all"
	aPT[3] = "Pegar tudo"
	aKey[4] = "iKeySearch"
	aMod[4] = "iModSearch"
	aEN[4] = "Search (open the container)"
	aPT[4] = "Procurar (abrir o contentor)"
	aKey[5] = "iKeyEquipRight"
	aMod[5] = "iModEquipRight"
	aEN[5] = "Equip in the right hand"
	aPT[5] = "Equipar na mao direita"
	aKey[6] = "iKeyEquipLeft"
	aMod[6] = "iModEquipLeft"
	aEN[6] = "Equip in the left hand"
	aPT[6] = "Equipar na mao esquerda"
	aKey[7] = "iHLScanKey"
	aMod[7] = ""
	aEN[7] = "Area scan key (hold)"
	aPT[7] = "Tecla do varrimento (manter)"
	pKey = new string[8]
	pMod = new string[8]
	pOptKey = new int[8]
	pOptMod = new int[8]
	pKey[0] = "iPadTake"
	pMod[0] = "iPadModTake"
	pKey[1] = "iPadTakeOne"
	pMod[1] = "iPadModTakeOne"
	pKey[2] = "iPadUse"
	pMod[2] = "iPadModUse"
	pKey[3] = "iPadTakeAll"
	pMod[3] = "iPadModTakeAll"
	pKey[4] = "iPadSearch"
	pMod[4] = "iPadModSearch"
	pKey[5] = "iPadEquipRight"
	pMod[5] = "iPadModEquipRight"
	pKey[6] = "iPadEquipLeft"
	pMod[6] = "iPadModEquipLeft"
	pKey[7] = "iHLScanPad"
	pMod[7] = ""
	padMask = new int[17]
	padName = new string[17]
	padMask[0] = 0
	padName[0] = "None"
	padMask[1] = 4096
	padName[1] = "A"
	padMask[2] = 8192
	padName[2] = "B"
	padMask[3] = 16384
	padName[3] = "X"
	padMask[4] = 32768
	padName[4] = "Y"
	padMask[5] = 256
	padName[5] = "LB"
	padMask[6] = 512
	padName[6] = "RB"
	padMask[7] = 9
	padName[7] = "LT"
	padMask[8] = 10
	padName[8] = "RT"
	padMask[9] = 32
	padName[9] = "Back"
	padMask[10] = 16
	padName[10] = "Start"
	padMask[11] = 64
	padName[11] = "L stick click"
	padMask[12] = 128
	padName[12] = "R stick click"
	padMask[13] = 1
	padName[13] = "D-pad up"
	padMask[14] = 2
	padName[14] = "D-pad down"
	padMask[15] = 4
	padName[15] = "D-pad left"
	padMask[16] = 8
	padName[16] = "D-pad right"

	; ids de opcoes por preencher: -1 (o id 0 e valido, por isso nao serve de "vazio")
	int n = 0
	while n < tOpt.Length
		tOpt[n] = -1
		n += 1
	endwhile
	n = 0
	while n < sOpt.Length
		sOpt[n] = -1
		n += 1
	endwhile
	n = 0
	while n < aOptKey.Length
		aOptKey[n] = -1
		aOptMod[n] = -1
		n += 1
	endwhile
	oLang = -1
	oVisual = -1
	oAnchorX = -1
	oAnchorY = -1
	oSortBy = -1
	oHLColor = -1
	oHLCorpseColor = -1
	rName = new string[30]
	rEN = new string[30]
	rPT = new string[30]
	rExtra = new int[30]
	oPick = new int[30]
	oGlow = new int[30]
	oColor = new int[30]
	oMinVal = new int[30]
	oMinVpw = new int[30]
	oMinX = new int[30]
	rName[0] = "Weapon1H"
	rEN[0] = "One-handed weapons"
	rPT[0] = "Armas de uma mao"
	rExtra[0] = 2
	rName[1] = "Weapon2H"
	rEN[1] = "Two-handed weapons"
	rPT[1] = "Armas de duas maos"
	rExtra[1] = 2
	rName[2] = "Bow"
	rEN[2] = "Bows and crossbows"
	rPT[2] = "Arcos e bestas"
	rExtra[2] = 2
	rName[3] = "Staff"
	rEN[3] = "Staves"
	rPT[3] = "Cajados"
	rExtra[3] = 2
	rName[4] = "Ammo"
	rEN[4] = "Ammo"
	rPT[4] = "Municao"
	rExtra[4] = 2
	rName[5] = "ArmorHeavy"
	rEN[5] = "Heavy armor"
	rPT[5] = "Armadura pesada"
	rExtra[5] = 1
	rName[6] = "ArmorLight"
	rEN[6] = "Light armor"
	rPT[6] = "Armadura leve"
	rExtra[6] = 1
	rName[7] = "Clothing"
	rEN[7] = "Clothing"
	rPT[7] = "Roupas"
	rExtra[7] = 1
	rName[8] = "Shield"
	rEN[8] = "Shields"
	rPT[8] = "Escudos"
	rExtra[8] = 1
	rName[9] = "Jewelry"
	rEN[9] = "Jewelry"
	rPT[9] = "Joias"
	rExtra[9] = 1
	rName[10] = "Potion"
	rEN[10] = "Potions"
	rPT[10] = "Pocoes"
	rExtra[10] = 0
	rName[11] = "Poison"
	rEN[11] = "Poisons"
	rPT[11] = "Venenos"
	rExtra[11] = 0
	rName[12] = "Food"
	rEN[12] = "Food and drinks"
	rPT[12] = "Comida e bebidas"
	rExtra[12] = 0
	rName[13] = "Ingredient"
	rEN[13] = "Ingredients"
	rPT[13] = "Ingredientes"
	rExtra[13] = 0
	rName[14] = "Scroll"
	rEN[14] = "Scrolls"
	rPT[14] = "Pergaminhos"
	rExtra[14] = 0
	rName[15] = "SpellTome"
	rEN[15] = "Spell tomes"
	rPT[15] = "Tomos de feitico"
	rExtra[15] = 0
	rName[16] = "SkillBook"
	rEN[16] = "Skill books"
	rPT[16] = "Livros de skill"
	rExtra[16] = 0
	rName[17] = "BookOther"
	rEN[17] = "Other books and notes"
	rPT[17] = "Outros livros e notas"
	rExtra[17] = 0
	rName[18] = "Key"
	rEN[18] = "Keys"
	rPT[18] = "Chaves"
	rExtra[18] = 0
	rName[19] = "SoulGemEmpty"
	rEN[19] = "Empty soul gems"
	rPT[19] = "Gemas de alma vazias"
	rExtra[19] = 0
	rName[20] = "SoulGemFilled"
	rEN[20] = "Filled soul gems"
	rPT[20] = "Gemas de alma cheias"
	rExtra[20] = 0
	rName[21] = "Gem"
	rEN[21] = "Gems"
	rPT[21] = "Gemas"
	rExtra[21] = 0
	rName[22] = "Ingot"
	rEN[22] = "Ingots"
	rPT[22] = "Barras"
	rExtra[22] = 0
	rName[23] = "Ore"
	rEN[23] = "Loose ore"
	rPT[23] = "Minerio solto"
	rExtra[23] = 0
	rName[24] = "Lockpick"
	rEN[24] = "Lockpicks"
	rPT[24] = "Lockpicks"
	rExtra[24] = 0
	rName[25] = "Gold"
	rEN[25] = "Gold"
	rPT[25] = "Ouro"
	rExtra[25] = 0
	rName[26] = "Torch"
	rEN[26] = "Torches and lights"
	rPT[26] = "Tochas e luzes"
	rExtra[26] = 0
	rName[27] = "MiscOther"
	rEN[27] = "Other misc items"
	rPT[27] = "Outros itens diversos"
	rExtra[27] = 0
	rName[28] = "Flora"
	rEN[28] = "Flora (harvest)"
	rPT[28] = "Flora (colher)"
	rExtra[28] = 0
	rName[29] = "OreVein"
	rEN[29] = "Ore veins"
	rPT[29] = "Filoes de minerio"
	rExtra[29] = 0
	n = 0
	while n < 30
		oPick[n] = -1
		oGlow[n] = -1
		oColor[n] = -1
		oMinVal[n] = -1
		oMinVpw[n] = -1
		oMinX[n] = -1
		n += 1
	endwhile
	oALKeyToggle = -1
	oALKeyHold = -1
	oALPadToggle = -1
	oALPadHold = -1
	n = 0
	while n < pOptKey.Length
		pOptKey[n] = -1
		pOptMod[n] = -1
		n += 1
	endwhile
EndFunction

string Function VisualName(int v)
	if v == 1
		return "Original (QuickLootIE)"
	endif
	return "Redesigned"
EndFunction

string Function LangName(int v)
	if v == 1
		return "Portugues"
	endif
	return "English"
EndFunction

string Function ModName2(int v)
	if v == 1
		return "Shift"
	elseif v == 2
		return "Ctrl"
	elseif v == 3
		return "Alt"
	endif
	return L("None", "Nenhum")
EndFunction

string Function ColorName(int v)
	if v == 1
		return L("White", "Branco")
	elseif v == 2
		return L("Green", "Verde")
	elseif v == 3
		return L("Cyan", "Ciano")
	elseif v == 4
		return L("Red", "Vermelho")
	endif
	return L("Gold", "Dourado")
EndFunction

string Function SortByName(int v)
	if v == 1
		return L("Value", "Valor")
	elseif v == 2
		return L("Weight", "Peso")
	elseif v == 3
		return L("Value per weight", "Valor por peso")
	endif
	return L("Name", "Nome")
EndFunction

string Function AnchorXName(int v)
	if v == 1
		return L("Center", "Centro")
	elseif v == 2
		return L("Right", "Direita")
	endif
	return L("Left", "Esquerda")
EndFunction

string Function AnchorYName(int v)
	if v == 1
		return L("Center", "Centro")
	elseif v == 2
		return L("Bottom", "Base")
	endif
	return L("Top", "Topo")
EndFunction

int Function PadIndex(int mask)
	int i = 0
	while i < padMask.Length
		if padMask[i] == mask
			return i
		endif
		i += 1
	endwhile
	return 0
EndFunction

string Function PadModName(int mask)
	if mask == 256
		return "LB"
	elseif mask == 512
		return "RB"
	elseif mask == 9
		return "LT"
	elseif mask == 10
		return "RT"
	endif
	return L("None", "Nenhum")
EndFunction

int Function PadModMask(int index)
	if index == 1
		return 256
	elseif index == 2
		return 512
	elseif index == 3
		return 9
	elseif index == 4
		return 10
	endif
	return 0
EndFunction

int Function PadModIndex(int mask)
	if mask == 256
		return 1
	elseif mask == 512
		return 2
	elseif mask == 9
		return 3
	elseif mask == 10
		return 4
	endif
	return 0
EndFunction

Function AddPadRow(int i)
	pOptKey[i] = AddMenuOption(L(aEN[i], aPT[i]) + " - " + L("gamepad", "comando"), padName[PadIndex(BetterLootingNative.GetInt(pKey[i], 0))])
	if pMod[i] != ""
		pOptMod[i] = AddMenuOption(L(aEN[i], aPT[i]) + " - " + L("gamepad modifier", "modificador do comando"), PadModName(BetterLootingNative.GetInt(pMod[i], 0)))
	else
		AddEmptyOption()
	endif
EndFunction

Function AddToggleRange(int first, int last)
	int i = first
	while i <= last
		tOpt[i] = AddToggleOption(L(tEN[i], tPT[i]), BetterLootingNative.GetInt(tKey[i], 0) != 0)
		i += 1
	endwhile
EndFunction

Function AddSliderRange(int first, int last)
	int i = first
	while i <= last
		sOpt[i] = AddSliderOption(L(sEN[i], sPT[i]), BetterLootingNative.GetInt(sKey[i], 0) as float, sFmt[i])
		i += 1
	endwhile
EndFunction

Function AddKeyRow(int i)
	aOptKey[i] = AddKeyMapOption(L(aEN[i], aPT[i]), BetterLootingNative.GetInt(aKey[i], 18))
	if aMod[i] != ""
		aOptMod[i] = AddMenuOption(L(aEN[i], aPT[i]) + " - " + L("modifier", "modificador"), ModName2(BetterLootingNative.GetInt(aMod[i], 0)))
	else
		AddEmptyOption()
	endif
EndFunction

Event OnPageReset(string a_page)
	BuildTables()
	lang = BetterLootingNative.GetInt("iLanguage", 0)
	SetCursorFillMode(TOP_TO_BOTTOM)

	if a_page == Pages[0]
		AddHeaderOption(L("Language and look", "Idioma e visual"))
		oLang = AddMenuOption(L("Language", "Idioma"), LangName(lang))
		oVisual = AddMenuOption(L("Loot window look", "Visual da janela de loot"), VisualName(BetterLootingNative.GetInt("iVisual", 0)))
		AddHeaderOption(L("Behavior", "Comportamento"))
		AddToggleRange(0, 4)
		AddHeaderOption(L("Sorting", "Ordenacao"))
		AddToggleRange(9, 9)
		oSortBy = AddMenuOption(L("Sort by", "Ordenar por"), SortByName(BetterLootingNative.GetInt("iSortBy", 0)))
		AddToggleRange(10, 11)
	elseif a_page == Pages[1]
		AddHeaderOption(L("Size and lines", "Tamanho e linhas"))
		AddSliderRange(0, 3)
		AddHeaderOption(L("Position", "Posicao"))
		oAnchorX = AddMenuOption(L("Horizontal anchor", "Ancora horizontal"), AnchorXName(BetterLootingNative.GetInt("iAnchorX", 0)))
		oAnchorY = AddMenuOption(L("Vertical anchor", "Ancora vertical"), AnchorYName(BetterLootingNative.GetInt("iAnchorY", 0)))
		AddSliderRange(4, 5)
		AddHeaderOption(L("Opacity", "Opacidade"))
		AddSliderRange(6, 7)
		AddHeaderOption(L("Columns and icons", "Colunas e icones"))
		AddToggleRange(5, 8)
	elseif a_page == Pages[2]
		SetCursorFillMode(LEFT_TO_RIGHT)
		AddHeaderOption(L("Loot window keys", "Teclas da janela de loot"))
		AddHeaderOption(L("Modifiers", "Modificadores"))
		int i = 0
		while i < 7
			AddKeyRow(i)
			i += 1
		endwhile
		AddHeaderOption(L("Timing", "Tempos"))
		AddHeaderOption("")
		AddSliderRange(8, 8)
		AddEmptyOption()
		AddHeaderOption(L("Area scan (outside the loot window)", "Varrimento (fora da janela de loot)"))
		AddHeaderOption("")
		AddKeyRow(7)
		AddSliderRange(10, 10)
		AddHeaderOption(L("Gamepad (loot window open)", "Comando (janela de loot aberta)"))
		AddHeaderOption(L("Gamepad modifiers", "Modificadores do comando"))
		i = 0
		while i < 7
			AddPadRow(i)
			i += 1
		endwhile
		AddHeaderOption(L("Area scan on the gamepad", "Varrimento no comando"))
		AddHeaderOption("")
		AddPadRow(7)
	elseif a_page == Pages[3]
		AddHeaderOption(L("Crosshair highlight", "Realce na mira"))
		AddToggleRange(12, 12)
		oHLColor = AddMenuOption(L("Highlight color", "Cor do realce"), ColorName(BetterLootingNative.GetInt("iHLColor", 0)))
		AddToggleRange(13, 14)
		AddSliderRange(9, 9)
		AddSliderRange(15, 15)
		AddHeaderOption(L("Object types", "Tipos de objeto"))
		AddToggleRange(15, 30)
		AddHeaderOption(L("Area scan", "Varrimento por raio"))
		AddToggleRange(31, 31)
		AddSliderRange(11, 13)
		AddHeaderOption(L("Lootable corpses", "Cadaveres com loot"))
		AddToggleRange(32, 33)
		AddToggleRange(40, 40)
		oHLCorpseColor = AddMenuOption(L("Corpse color", "Cor dos cadaveres"), ColorName(BetterLootingNative.GetInt("iHLCorpseColor", 2)))
		AddSliderRange(14, 14)
		AddHeaderOption(L("Corpse loot filters", "Filtros do loot dos cadaveres"))
		AddToggleRange(34, 39)
		AddSliderRange(16, 16)
	elseif a_page == Pages[4]
		AddHeaderOption(L("Auto-loot", "Apanhar automatico"))
		AddToggleRange(41, 42)
		AddSliderRange(17, 20)
		AddHeaderOption(L("Activation (0 / None = off)", "Ativacao (0 / Nenhum = desligado)"))
		oALKeyToggle = AddKeyMapOption(L("Toggle key (press to turn on/off)", "Tecla de ligar/desligar"), BetterLootingNative.GetInt("iALToggleKey", 0))
		oALKeyHold = AddKeyMapOption(L("Hold key (works while pressed)", "Tecla para manter premida"), BetterLootingNative.GetInt("iALHoldKey", 0))
		oALPadToggle = AddMenuOption(L("Gamepad toggle button", "Botao do comando: ligar/desligar"), padName[PadIndex(BetterLootingNative.GetInt("iALPadToggle", 0))])
		oALPadHold = AddMenuOption(L("Gamepad hold button", "Botao do comando: manter premido"), padName[PadIndex(BetterLootingNative.GetInt("iALPadHold", 0))])
	elseif a_page == Pages[5]
		SetCursorFillMode(LEFT_TO_RIGHT)
		AddHeaderOption(L("Pick up", "Apanhar"))
		AddHeaderOption(L("Glow", "Brilho"))
		int r = 0
		while r < 30
			oPick[r] = AddToggleOption(L(rEN[r], rPT[r]) + " - " + L("pick up", "apanhar"), BetterLootingNative.GetRule("Type." + rName[r], "Pick", 0) != 0)
			oGlow[r] = AddToggleOption(L(rEN[r], rPT[r]) + " - " + L("glow", "brilho"), BetterLootingNative.GetRule("Type." + rName[r], "Glow", 0) != 0)
			r += 1
		endwhile
	elseif a_page == Pages[6]
		AddHeaderOption(L("Glow color per type", "Cor do brilho por tipo"))
		int c = 0
		while c < 30
			oColor[c] = AddMenuOption(L(rEN[c], rPT[c]), ColorName(BetterLootingNative.GetRule("Type." + rName[c], "Color", 0)))
			c += 1
		endwhile
	elseif a_page == Pages[7]
		SetCursorFillMode(LEFT_TO_RIGHT)
		AddHeaderOption(L("Minimum value / value per weight", "Valor minimo / valor por peso"))
		AddHeaderOption(L("Minimum armor rating / damage", "Armadura minima / dano minimo"))
		int s = 0
		while s < 28
			oMinVal[s] = AddSliderOption(L(rEN[s], rPT[s]) + " - " + L("min value", "valor min."), BetterLootingNative.GetRule("Type." + rName[s], "MinValue", 0) as float, "{0}")
			oMinVpw[s] = AddSliderOption(L(rEN[s], rPT[s]) + " - " + L("min value/weight", "valor/peso min."), BetterLootingNative.GetRule("Type." + rName[s], "MinValuePerWeight", 0) as float, "{0}")
			if rExtra[s] != 0
				oMinX[s] = AddSliderOption(L(rEN[s], rPT[s]) + " - " + ExtraLabel(s), BetterLootingNative.GetRule("Type." + rName[s], ExtraKey(s), 0) as float, "{0}")
				AddEmptyOption()
			endif
			s += 1
		endwhile
	endif
EndEvent

int Function FindToggle(int a_option)
	int i = 0
	while i < tOpt.Length
		if tOpt[i] == a_option
			return i
		endif
		i += 1
	endwhile
	return -1
EndFunction

int Function FindSlider(int a_option)
	int i = 0
	while i < sOpt.Length
		if sOpt[i] == a_option
			return i
		endif
		i += 1
	endwhile
	return -1
EndFunction

int Function FindKey(int a_option)
	int i = 0
	while i < aOptKey.Length
		if aOptKey[i] == a_option
			return i
		endif
		i += 1
	endwhile
	return -1
EndFunction

int Function FindPadKey(int a_option)
	int i = 0
	while i < pOptKey.Length
		if pOptKey[i] == a_option
			return i
		endif
		i += 1
	endwhile
	return -1
EndFunction

int Function FindPadMod(int a_option)
	int i = 0
	while i < pOptMod.Length
		if pOptMod[i] == a_option
			return i
		endif
		i += 1
	endwhile
	return -1
EndFunction

int Function FindMod(int a_option)
	int i = 0
	while i < aOptMod.Length
		if aOptMod[i] == a_option
			return i
		endif
		i += 1
	endwhile
	return -1
EndFunction

Function SetKey(string a_key, int a_option, int a_code)
	int code = a_code
	if code < 1
		code = BetterLootingNative.GetDefault(a_key, 1)
	endif
	BetterLootingNative.SetInt(a_key, code)
	SetKeyMapOptionValue(a_option, code)
EndFunction

string Function ExtraKey(int i)
	if rExtra[i] == 1
		return "MinArmor"
	endif
	return "MinDamage"
EndFunction

string Function ExtraLabel(int i)
	if rExtra[i] == 1
		return L("min armor", "armadura min.")
	endif
	return L("min damage", "dano min.")
EndFunction

string Function RuleSection(int i)
	return "Type." + rName[i]
EndFunction

int Function FindIn(int[] arr, int a_option)
	int i = 0
	while i < arr.Length
		if arr[i] == a_option
			return i
		endif
		i += 1
	endwhile
	return -1
EndFunction

Function ToggleRule(int i, string key, int a_option)
	int v = 1
	if BetterLootingNative.GetRule(RuleSection(i), key, 0) != 0
		v = 0
	endif
	BetterLootingNative.SetRule(RuleSection(i), key, v)
	SetToggleOptionValue(a_option, v != 0)
EndFunction

bool Function ResetAutoOption(int a_option)
	int i = FindIn(oPick, a_option)
	if i >= 0
		BetterLootingNative.SetRule(RuleSection(i), "Pick", 0)
		SetToggleOptionValue(a_option, false)
		return true
	endif
	i = FindIn(oGlow, a_option)
	if i >= 0
		BetterLootingNative.SetRule(RuleSection(i), "Glow", 0)
		SetToggleOptionValue(a_option, false)
		return true
	endif
	i = FindIn(oColor, a_option)
	if i >= 0
		BetterLootingNative.SetRule(RuleSection(i), "Color", 0)
		SetMenuOptionValue(a_option, ColorName(0))
		return true
	endif
	i = FindIn(oMinVal, a_option)
	if i >= 0
		BetterLootingNative.SetRule(RuleSection(i), "MinValue", 0)
		SetSliderOptionValue(a_option, 0.0, "{0}")
		return true
	endif
	i = FindIn(oMinVpw, a_option)
	if i >= 0
		BetterLootingNative.SetRule(RuleSection(i), "MinValuePerWeight", 0)
		SetSliderOptionValue(a_option, 0.0, "{0}")
		return true
	endif
	i = FindIn(oMinX, a_option)
	if i >= 0
		BetterLootingNative.SetRule(RuleSection(i), ExtraKey(i), 0)
		SetSliderOptionValue(a_option, 0.0, "{0}")
		return true
	endif
	if a_option == oALKeyToggle
		SetKey("iALToggleKey", a_option, 0)
		return true
	elseif a_option == oALKeyHold
		SetKey("iALHoldKey", a_option, 0)
		return true
	elseif a_option == oALPadToggle
		BetterLootingNative.SetInt("iALPadToggle", 0)
		SetMenuOptionValue(a_option, padName[0])
		return true
	elseif a_option == oALPadHold
		BetterLootingNative.SetInt("iALPadHold", 0)
		SetMenuOptionValue(a_option, padName[0])
		return true
	endif
	return false
EndFunction

bool Function AutoSliderOpen(int a_option)
	int i = FindIn(oMinVal, a_option)
	if i >= 0
		SetSliderDialogStartValue(BetterLootingNative.GetRule(RuleSection(i), "MinValue", 0) as float)
		SetSliderDialogDefaultValue(0.0)
		SetSliderDialogRange(0.0, 5000.0)
		SetSliderDialogInterval(10.0)
		return true
	endif
	i = FindIn(oMinVpw, a_option)
	if i >= 0
		SetSliderDialogStartValue(BetterLootingNative.GetRule(RuleSection(i), "MinValuePerWeight", 0) as float)
		SetSliderDialogDefaultValue(0.0)
		SetSliderDialogRange(0.0, 500.0)
		SetSliderDialogInterval(5.0)
		return true
	endif
	i = FindIn(oMinX, a_option)
	if i >= 0
		SetSliderDialogStartValue(BetterLootingNative.GetRule(RuleSection(i), ExtraKey(i), 0) as float)
		SetSliderDialogDefaultValue(0.0)
		if rExtra[i] == 1
			SetSliderDialogRange(0.0, 200.0)
		else
			SetSliderDialogRange(0.0, 300.0)
		endif
		SetSliderDialogInterval(5.0)
		return true
	endif
	return false
EndFunction

bool Function AutoSliderAccept(int a_option, float a_value)
	int i = FindIn(oMinVal, a_option)
	if i >= 0
		BetterLootingNative.SetRule(RuleSection(i), "MinValue", a_value as int)
		SetSliderOptionValue(a_option, a_value, "{0}")
		return true
	endif
	i = FindIn(oMinVpw, a_option)
	if i >= 0
		BetterLootingNative.SetRule(RuleSection(i), "MinValuePerWeight", a_value as int)
		SetSliderOptionValue(a_option, a_value, "{0}")
		return true
	endif
	i = FindIn(oMinX, a_option)
	if i >= 0
		BetterLootingNative.SetRule(RuleSection(i), ExtraKey(i), a_value as int)
		SetSliderOptionValue(a_option, a_value, "{0}")
		return true
	endif
	return false
EndFunction

bool Function AutoMenuOpen(int a_option)
	string[] opts
	int i = FindIn(oColor, a_option)
	if i >= 0
		opts = new string[5]
		opts[0] = ColorName(0)
		opts[1] = ColorName(1)
		opts[2] = ColorName(2)
		opts[3] = ColorName(3)
		opts[4] = ColorName(4)
		SetMenuDialogOptions(opts)
		SetMenuDialogStartIndex(BetterLootingNative.GetRule(RuleSection(i), "Color", 0))
		SetMenuDialogDefaultIndex(0)
		return true
	endif
	if a_option == oALPadToggle || a_option == oALPadHold
		opts = new string[17]
		int j = 0
		while j < 17
			opts[j] = padName[j]
			j += 1
		endwhile
		SetMenuDialogOptions(opts)
		if a_option == oALPadToggle
			SetMenuDialogStartIndex(PadIndex(BetterLootingNative.GetInt("iALPadToggle", 0)))
		else
			SetMenuDialogStartIndex(PadIndex(BetterLootingNative.GetInt("iALPadHold", 0)))
		endif
		SetMenuDialogDefaultIndex(0)
		return true
	endif
	return false
EndFunction

bool Function AutoMenuAccept(int a_option, int a_index)
	int i = FindIn(oColor, a_option)
	if i >= 0
		BetterLootingNative.SetRule(RuleSection(i), "Color", a_index)
		SetMenuOptionValue(a_option, ColorName(a_index))
		return true
	endif
	if a_option == oALPadToggle
		BetterLootingNative.SetInt("iALPadToggle", padMask[a_index])
		SetMenuOptionValue(a_option, padName[a_index])
		return true
	elseif a_option == oALPadHold
		BetterLootingNative.SetInt("iALPadHold", padMask[a_index])
		SetMenuOptionValue(a_option, padName[a_index])
		return true
	endif
	return false
EndFunction

bool Function AutoHighlight(int a_option)
	if a_option == oALKeyToggle || a_option == oALPadToggle
		SetInfoText(L("One press turns auto-loot on or off. It needs the Auto-loot option turned on.", "Um toque liga ou desliga o apanhar automatico. Precisa da opcao Apanhar automatico ligada."))
		return true
	elseif a_option == oALKeyHold || a_option == oALPadHold
		SetInfoText(L("Auto-loot works while you hold this key or button.", "O apanhar automatico funciona enquanto mantiveres esta tecla ou botao premido."))
		return true
	endif
	int i = FindIn(oPick, a_option)
	if i >= 0
		SetInfoText(L("Pick up this type by itself. Never touches anything owned by someone, corpses or containers. Books open when picked up.", "Apanhar este tipo sozinho. Nunca toca no que tem dono, em cadaveres nem em contentores. Os livros abrem ao serem apanhados."))
		return true
	endif
	i = FindIn(oGlow, a_option)
	if i >= 0
		SetInfoText(L("Make this type glow when it is near you and passes the limits.", "Dar brilho a este tipo quando esta perto e passa nos limiares."))
		return true
	endif
	i = FindIn(oMinVal, a_option)
	if i < 0
		i = FindIn(oMinVpw, a_option)
	endif
	if i < 0
		i = FindIn(oMinX, a_option)
	endif
	if i >= 0
		SetInfoText(L("0 = off. All the limits of the type must be met. Name filters and forbid lists are in BetterLooting_AutoLoot.ini.", "0 = desligado. Todos os limiares do tipo tem de ser cumpridos. Os filtros de nome e as listas de proibicao estao no BetterLooting_AutoLoot.ini."))
		return true
	endif
	return false
EndFunction

Event OnOptionSelect(int a_option)
	int i = FindToggle(a_option)
	if i >= 0
		int v = 1
		if BetterLootingNative.GetInt(tKey[i], 0) != 0
			v = 0
		endif
		BetterLootingNative.SetInt(tKey[i], v)
		SetToggleOptionValue(a_option, v != 0)
		return
	endif
	i = FindIn(oPick, a_option)
	if i >= 0
		ToggleRule(i, "Pick", a_option)
		return
	endif
	i = FindIn(oGlow, a_option)
	if i >= 0
		ToggleRule(i, "Glow", a_option)
	endif
EndEvent

Event OnOptionDefault(int a_option)
	if ResetAutoOption(a_option)
		return
	endif
	int i = FindToggle(a_option)
	if i >= 0
		int d = BetterLootingNative.GetDefault(tKey[i], 0)
		BetterLootingNative.SetInt(tKey[i], d)
		SetToggleOptionValue(a_option, d != 0)
		return
	endif
	i = FindSlider(a_option)
	if i >= 0
		int ds = BetterLootingNative.GetDefault(sKey[i], 0)
		BetterLootingNative.SetInt(sKey[i], ds)
		SetSliderOptionValue(a_option, ds as float, sFmt[i])
		return
	endif
	i = FindKey(a_option)
	if i >= 0
		SetKey(aKey[i], a_option, 0)
		return
	endif
	i = FindPadKey(a_option)
	if i >= 0
		int dpk = BetterLootingNative.GetDefault(pKey[i], 0)
		BetterLootingNative.SetInt(pKey[i], dpk)
		SetMenuOptionValue(a_option, padName[PadIndex(dpk)])
		return
	endif
	i = FindPadMod(a_option)
	if i >= 0
		int dpm = BetterLootingNative.GetDefault(pMod[i], 0)
		BetterLootingNative.SetInt(pMod[i], dpm)
		SetMenuOptionValue(a_option, PadModName(dpm))
		return
	endif
	i = FindMod(a_option)
	if i >= 0
		int dm = BetterLootingNative.GetDefault(aMod[i], 0)
		BetterLootingNative.SetInt(aMod[i], dm)
		SetMenuOptionValue(a_option, ModName2(dm))
		return
	endif
	if a_option == oAnchorX
		BetterLootingNative.SetInt("iAnchorX", 0)
		SetMenuOptionValue(a_option, AnchorXName(0))
	elseif a_option == oAnchorY
		BetterLootingNative.SetInt("iAnchorY", 0)
		SetMenuOptionValue(a_option, AnchorYName(0))
	elseif a_option == oSortBy
		BetterLootingNative.SetInt("iSortBy", 0)
		SetMenuOptionValue(a_option, SortByName(0))
	elseif a_option == oHLColor
		BetterLootingNative.SetInt("iHLColor", 0)
		SetMenuOptionValue(a_option, ColorName(0))
	elseif a_option == oHLCorpseColor
		BetterLootingNative.SetInt("iHLCorpseColor", 2)
		SetMenuOptionValue(a_option, ColorName(2))
	elseif a_option == oVisual
		BetterLootingNative.SetInt("iVisual", 0)
		BetterLootingNative.ApplyVisual()
		SetMenuOptionValue(a_option, VisualName(0))
	elseif a_option == oLang
		BetterLootingNative.SetInt("iLanguage", 0)
		lang = 0
		ForcePageReset()
	endif
EndEvent

Event OnOptionSliderOpen(int a_option)
	if AutoSliderOpen(a_option)
		return
	endif
	int i = FindSlider(a_option)
	if i >= 0
		SetSliderDialogStartValue(BetterLootingNative.GetInt(sKey[i], 0) as float)
		SetSliderDialogDefaultValue(BetterLootingNative.GetDefault(sKey[i], 0) as float)
		SetSliderDialogRange(sMin[i], sMax[i])
		SetSliderDialogInterval(sStep[i])
	endif
EndEvent

Event OnOptionSliderAccept(int a_option, float a_value)
	if AutoSliderAccept(a_option, a_value)
		return
	endif
	int i = FindSlider(a_option)
	if i >= 0
		BetterLootingNative.SetInt(sKey[i], a_value as int)
		SetSliderOptionValue(a_option, a_value, sFmt[i])
	endif
EndEvent

Event OnOptionMenuOpen(int a_option)
	if AutoMenuOpen(a_option)
		return
	endif
	string[] opts
	int cur = 0
	int dflt = 0
	int pk = FindPadKey(a_option)
	int pm = FindPadMod(a_option)
	int k = FindMod(a_option)
	if pk >= 0
		opts = new string[17]
		int j = 0
		while j < 17
			opts[j] = padName[j]
			j += 1
		endwhile
		cur = PadIndex(BetterLootingNative.GetInt(pKey[pk], 0))
		dflt = PadIndex(BetterLootingNative.GetDefault(pKey[pk], 0))
	elseif pm >= 0
		opts = new string[5]
		opts[0] = PadModName(0)
		opts[1] = PadModName(256)
		opts[2] = PadModName(512)
		opts[3] = PadModName(9)
		opts[4] = PadModName(10)
		cur = PadModIndex(BetterLootingNative.GetInt(pMod[pm], 0))
		dflt = PadModIndex(BetterLootingNative.GetDefault(pMod[pm], 0))
	elseif k >= 0
		opts = new string[4]
		opts[0] = ModName2(0)
		opts[1] = ModName2(1)
		opts[2] = ModName2(2)
		opts[3] = ModName2(3)
		cur = BetterLootingNative.GetInt(aMod[k], 0)
		dflt = BetterLootingNative.GetDefault(aMod[k], 0)
	elseif a_option == oLang
		opts = new string[2]
		opts[0] = "English"
		opts[1] = "Portugues"
		cur = lang
	elseif a_option == oVisual
		opts = new string[2]
		opts[0] = VisualName(0)
		opts[1] = VisualName(1)
		cur = BetterLootingNative.GetInt("iVisual", 0)
	elseif a_option == oAnchorX
		opts = new string[3]
		opts[0] = AnchorXName(0)
		opts[1] = AnchorXName(1)
		opts[2] = AnchorXName(2)
		cur = BetterLootingNative.GetInt("iAnchorX", 0)
	elseif a_option == oAnchorY
		opts = new string[3]
		opts[0] = AnchorYName(0)
		opts[1] = AnchorYName(1)
		opts[2] = AnchorYName(2)
		cur = BetterLootingNative.GetInt("iAnchorY", 0)
	elseif a_option == oSortBy
		opts = new string[4]
		opts[0] = SortByName(0)
		opts[1] = SortByName(1)
		opts[2] = SortByName(2)
		opts[3] = SortByName(3)
		cur = BetterLootingNative.GetInt("iSortBy", 0)
	elseif a_option == oHLColor
		opts = new string[5]
		opts[0] = ColorName(0)
		opts[1] = ColorName(1)
		opts[2] = ColorName(2)
		opts[3] = ColorName(3)
		opts[4] = ColorName(4)
		cur = BetterLootingNative.GetInt("iHLColor", 0)
	elseif a_option == oHLCorpseColor
		opts = new string[5]
		opts[0] = ColorName(0)
		opts[1] = ColorName(1)
		opts[2] = ColorName(2)
		opts[3] = ColorName(3)
		opts[4] = ColorName(4)
		cur = BetterLootingNative.GetInt("iHLCorpseColor", 2)
		dflt = 2
	else
		return
	endif
	SetMenuDialogOptions(opts)
	SetMenuDialogStartIndex(cur)
	SetMenuDialogDefaultIndex(dflt)
EndEvent

Event OnOptionMenuAccept(int a_option, int a_index)
	if a_index < 0
		return
	endif
	if AutoMenuAccept(a_option, a_index)
		return
	endif
	int pk = FindPadKey(a_option)
	int pm = FindPadMod(a_option)
	int k = FindMod(a_option)
	if pk >= 0
		BetterLootingNative.SetInt(pKey[pk], padMask[a_index])
		SetMenuOptionValue(a_option, padName[a_index])
	elseif pm >= 0
		BetterLootingNative.SetInt(pMod[pm], PadModMask(a_index))
		SetMenuOptionValue(a_option, PadModName(PadModMask(a_index)))
	elseif k >= 0
		BetterLootingNative.SetInt(aMod[k], a_index)
		SetMenuOptionValue(a_option, ModName2(a_index))
	elseif a_option == oLang
		BetterLootingNative.SetInt("iLanguage", a_index)
		lang = a_index
		ForcePageReset()
	elseif a_option == oVisual
		BetterLootingNative.SetInt("iVisual", a_index)
		BetterLootingNative.ApplyVisual()
		SetMenuOptionValue(a_option, VisualName(a_index))
	elseif a_option == oAnchorX
		BetterLootingNative.SetInt("iAnchorX", a_index)
		SetMenuOptionValue(a_option, AnchorXName(a_index))
	elseif a_option == oAnchorY
		BetterLootingNative.SetInt("iAnchorY", a_index)
		SetMenuOptionValue(a_option, AnchorYName(a_index))
	elseif a_option == oSortBy
		BetterLootingNative.SetInt("iSortBy", a_index)
		SetMenuOptionValue(a_option, SortByName(a_index))
	elseif a_option == oHLColor
		BetterLootingNative.SetInt("iHLColor", a_index)
		SetMenuOptionValue(a_option, ColorName(a_index))
	elseif a_option == oHLCorpseColor
		BetterLootingNative.SetInt("iHLCorpseColor", a_index)
		SetMenuOptionValue(a_option, ColorName(a_index))
	endif
EndEvent

Event OnOptionKeyMapChange(int a_option, int a_keyCode, string a_conflictControl, string a_conflictName)
	if a_option == oALKeyToggle
		SetKey("iALToggleKey", a_option, a_keyCode)
		return
	elseif a_option == oALKeyHold
		SetKey("iALHoldKey", a_option, a_keyCode)
		return
	endif
	int i = FindKey(a_option)
	if i >= 0
		SetKey(aKey[i], a_option, a_keyCode)
	endif
EndEvent

Event OnOptionHighlight(int a_option)
	if AutoHighlight(a_option)
		return
	endif
	int i = FindKey(a_option)
	if i >= 0
		if i == 7
			SetInfoText(L("Hold this key to light up nearby objects and lootable corpses. Holding longer keeps refreshing the glow. A short tap still does its normal action.", "Mantenha esta tecla premida para realcar objetos e cadaveres com loot por perto. Manter mais tempo continua a refrescar o brilho. Um toque curto faz a acao normal."))
		else
			SetInfoText(L("Key of this action while the loot window is open. Keyboard keys and mouse buttons work. The game does not do anything else with this key while the window is open.", "Tecla desta acao com a janela de loot aberta. Funcionam teclas do teclado e botoes do rato. O jogo nao faz mais nada com esta tecla enquanto a janela esta aberta."))
		endif
		return
	endif
	i = FindPadKey(a_option)
	if i >= 0
		SetInfoText(L("Gamepad button of this action while the loot window is open. None disables it on the gamepad.", "Botao do comando desta acao com a janela de loot aberta. Nenhum desativa-a no comando."))
		return
	endif
	i = FindPadMod(a_option)
	if i >= 0
		SetInfoText(L("Button that must be held together with the gamepad button (None, LB, RB, LT or RT).", "Botao que tem de estar premido junto com o botao do comando (Nenhum, LB, RB, LT ou RT)."))
		return
	endif
	i = FindMod(a_option)
	if i >= 0
		SetInfoText(L("Modifier that must be held together with the key (None, Shift, Ctrl or Alt).", "Modificador que tem de estar premido junto com a tecla (Nenhum, Shift, Ctrl ou Alt)."))
		return
	endif
	if a_option == oVisual
		SetInfoText(L("Look of the loot window. The change applies the next time the loot menu opens.", "Visual da janela de loot. A mudanca aplica-se na proxima vez que o menu de loot abrir."))
	elseif a_option == oLang
		SetInfoText(L("Language of this menu.", "Idioma deste menu."))
	elseif a_option == oAnchorX
		SetInfoText(L("Where on the screen the window is anchored, horizontally.", "Ponto do ecra onde a janela fica ancorada, na horizontal."))
	elseif a_option == oAnchorY
		SetInfoText(L("Where on the screen the window is anchored, vertically.", "Ponto do ecra onde a janela fica ancorada, na vertical."))
	elseif a_option == oHLCorpseColor
		SetInfoText(L("Color of the glow on lootable corpses.", "Cor do brilho nos cadaveres com loot."))
	elseif a_option == oHLColor
		SetInfoText(L("Color of the glow on the object you look at. What would be stealing uses red if that option is on.", "Cor do brilho no objeto para que olhas. O que seria roubo usa vermelho, se essa opcao estiver ligada."))
	elseif a_option == oSortBy
		SetInfoText(L("Order of the items inside each category (or of the whole list if grouping is off). Quest items always come first.", "Ordem dos itens dentro de cada categoria (ou de toda a lista, se o agrupamento estiver desligado). Itens de missao ficam sempre primeiro."))
	elseif a_option == sOpt[3]
		SetInfoText(L("Stops the list after this many items. 0 means all items are listed.", "Para a lista neste numero de itens. 0 lista todos os itens."))
	elseif a_option == sOpt[16]
		SetInfoText(L("With the filters on, a corpse also counts as interesting if it has an item worth at least this much.", "Com os filtros ligados, um cadaver tambem conta como interessante se tiver um item que valha pelo menos isto."))
	elseif a_option == sOpt[15]
		SetInfoText(L("How strong the glow is. It applies in 5 steps (100, 60, 35, 18 and 8%). Lower it to see the details of the objects.", "Define quanto forte e o brilho. Aplica-se em 5 niveis (100, 60, 35, 18 e 8%). Reduza para ver os detalhes dos objetos."))
	endif
EndEvent
