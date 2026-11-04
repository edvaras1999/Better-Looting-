Scriptname BetterLootingNative Native Hidden
{ Funcoes nativas implementadas pela BetterLooting.dll. Servem so para o menu MCM falar com o plugin. }

; Valor atual de uma definicao (chave do BetterLooting.ini). Devolve defaultValue se a chave nao existir.
int Function GetInt(string key, int defaultValue) global native

; Valor por omissao de uma definicao.
int Function GetDefault(string key, int fallback) global native

; Altera uma definicao e grava-a no BetterLooting.ini.
Function SetInt(string key, int value) global native

; Avisa o plugin de que o visual mudou: o menu de loot e recriado na proxima abertura.
Function ApplyVisual() global native

; Escreve uma linha no log do plugin (diagnostico).
Function Log(string msg) global native
