#pragma once

class TESObjectREFR;
struct SKSESerializationInterface;

// Realce de objetos: aplica um efeito de shader do jogo (BetterLooting.esp) aos objetos. A chamada usa
// a funcao nativa EffectShader.Play diretamente, sem scripts Papyrus.
//  - objeto na mira (evento da mira)
//  - varrimento por raio e cadaveres lootaveis (tecla do teclado ou do comando mantida premida)
//  - realce permanente dos cadaveres (opcional)
namespace Highlight
{
	// procura os shaders do ESP e regista o evento da mira
	void Init();

	// guarda e carrega a lista de cadaveres revistados no save (chamado uma vez, ao carregar o plugin)
	void RegisterSerialization(const SKSESerializationInterface *a_intfc);

	// estado da tecla do varrimento (chamado pelo gancho de input; nao consome a tecla)
	// a_pad: false = teclado/rato, true = comando
	void OnScanKey(bool a_down, bool a_up, float a_heldSeconds, bool a_lootMenuVisible, bool a_pad);

	// chamado a cada lote de eventos de input: faz o realce permanente dos cadaveres (limitado a 1 por segundo)
	void Tick();

	// o cadaver deixa de ser realcado (foi revistado)
	void MarkSearched(TESObjectREFR *a_ref);
}
