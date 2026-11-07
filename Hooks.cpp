#include <SKSE.h>
#include <SKSE/DebugLog.h>
#include <SKSE/GameForms.h>
#include <SKSE/GameObjects.h>
#include <SKSE/GameEvents.h>
#include <SKSE/GameMenus.h>
#include <SKSE/GameInput.h>
#include <SKSE/GameRTTI.h>
#include <SKSE/SafeWrite.h>

#include "Hooks.h"
#include "LootMenu.h"
#include "Settings.h"
#include "Highlight.h"

//========================================================
// input patches
//========================================================

// ルートメニューが開いてる場合は、マウスホイールでカメラが移動しないよう書き換える
template <UInt32 VTBL>
class CameraStateHandler : public PlayerInputHandler
{
public:
	typedef bool(CameraStateHandler::*FnCanProcess)(InputEvent* evn);

	static FnCanProcess fnCanProcessOld;

	bool CanProcess_Hook(InputEvent* evn)
	{
		bool result = (this->*fnCanProcessOld)(evn);
		LootMenu *lootMenu = LootMenu::GetSingleton();

		if (result && lootMenu && lootMenu->IsVisible())
		{
			result = (evn->GetControlID() == InputStringHolder::GetSingleton()->togglePOV);
		}

		return result;
	}

	static void InitHook()
	{
		fnCanProcessOld = SafeWrite32(VTBL + 4, &CanProcess_Hook);
	}
};

template <UInt32 VTBL> typename CameraStateHandler<VTBL>::FnCanProcess CameraStateHandler<VTBL>::fnCanProcessOld;

typedef CameraStateHandler<0x010E29D4> ThirdPersonStateHandler;
typedef CameraStateHandler<0x010E30B0> FirstPersonStateHandler;


// ルートメニューが開いている場合は、箱コン上下ボタンでお気に入りが開かないよう書き換える
class FavoritesHandler : public MenuEventHandler
{
public:
	typedef bool(FavoritesHandler::*FnCanProcess)(InputEvent *);

	static FnCanProcess fnCanProcess;

	bool CanProcess_Hook(InputEvent *evn)
	{
		bool result = (this->*fnCanProcess)(evn);
		LootMenu *lootMenu = LootMenu::GetSingleton();

		if (result && lootMenu && lootMenu->IsVisible())
		{
			if (evn->deviceType == BSInputDevice::kType_Gamepad && evn->eventType == InputEvent::kEventType_Button)
			{
				ButtonEvent *button = static_cast<ButtonEvent *>(evn);
				result = (button->keyMask != KeyCode::Gamepad_Up && button->keyMask != KeyCode::Gamepad_Down);
			}
		}

		return result;
	}

	static void InitHook()
	{
		fnCanProcess = SafeWrite32(0x010E6A38 + 0x01 * 4, &CanProcess_Hook);
	}
};

FavoritesHandler::FnCanProcess FavoritesHandler::fnCanProcess;



#ifdef USE_JUMP_KEY
// ルートメニューが開いている場合は、ジャンプしないよう書き換える
class JumpHandlerEx : public JumpHandler
{
public:
	typedef bool(JumpHandlerEx::*FnCanProcess)(InputEvent* evn);

	static FnCanProcess fnCanProcessOld;

	bool CanProcess_Hook(InputEvent* evn)
	{
		LootMenu *lootMenu = LootMenu::GetSingleton();
		return (lootMenu && lootMenu->IsVisible()) ? false : (this->*fnCanProcessOld)(evn);
	}

	static void InitHook()
	{
		fnCanProcessOld = SafeWrite32(0x010D460C + 4, &CanProcess_Hook);
	}
};

JumpHandlerEx::FnCanProcess JumpHandlerEx::fnCanProcessOld;
#endif


// ルートメニューが開いている場合は、武器構えボタンでコンテナが開くよう書き換える
class ReadyWeaponHandlerEx : public ReadyWeaponHandler
{
public:
	typedef void(ReadyWeaponHandlerEx::*FnProcessButton)(ButtonEvent* evn, PlayerControls::Data14 *data);

	static FnProcessButton fnProcessButtonOld;

	void ProcessButton_Hook(ButtonEvent* evn, PlayerControls::Data14 *data)
	{
		LootMenu *lootMenu = LootMenu::GetSingleton();

		static bool bProcessLongTap = false;

		if (!evn->IsDown())
		{
			if (bProcessLongTap && evn->timer > 2.0f)
			{
				bProcessLongTap = false;

				if (lootMenu && !lootMenu->IsVisible())
				{
					bool disabled = !lootMenu->IsDisabled();
					lootMenu->SetDisabled(disabled);
					
					typedef void(*FnDebug_Notification)(const char *, bool, bool);
					const FnDebug_Notification fnDebug_Notification = (FnDebug_Notification)0x008997A0;

					fnDebug_Notification((disabled ? "Better Looting has been disabled." : "Better Looting has been enabled."), false, true);
				}
			}

			return;
		}

		bProcessLongTap = true;

		if (lootMenu && lootMenu->IsVisible())
		{
			g_thePlayer->StartActivation();
		}
		else
		{
			(this->*fnProcessButtonOld)(evn, data);
		}
	}

	static void InitHook()
	{
		fnProcessButtonOld = SafeWrite32(0x010D45C4 + 0x04 * 4, &ProcessButton_Hook);
	}
};

ReadyWeaponHandlerEx::FnProcessButton ReadyWeaponHandlerEx::fnProcessButtonOld;


// アクティベート時の動作を書き換える
class PlayerCharacterEx : public PlayerCharacter
{
public:
	typedef void(PlayerCharacterEx::*FnStartActivation)();

	static FnStartActivation fnStartActivationOld;

	// abre o contentor/cadaver normalmente (sem passar pelo menu de loot)
	static void OpenContainer()
	{
		if (fnStartActivationOld)
			(static_cast<PlayerCharacterEx *>(g_thePlayer)->*fnStartActivationOld)();
	}

	void StartActivation_Hook()
	{
		LootMenu *lootMenu = LootMenu::GetSingleton();

		if (lootMenu && lootMenu->IsVisible())
		{
			lootMenu->TakeItem();
		}
		else
		{
			(this->*fnStartActivationOld)();
		}
	}

	static void InitHook()
	{
		fnStartActivationOld = WriteRelCall(0x0077265A, &StartActivation_Hook);
	}
};

PlayerCharacterEx::FnStartActivation PlayerCharacterEx::fnStartActivationOld;


// Teclas da janela de loot (v0.12). Com a janela aberta, cada acao tem a sua tecla e o seu modificador
// (Shift, Ctrl ou Alt), configuraveis. Codigo da tecla: 1-255 = teclado (DirectInput), 256+ = rato.
// As teclas que correspondem a uma acao sao retiradas da lista de eventos, para o jogo nao fazer o que
// elas fariam com a janela fechada (ativar, empunhar arma, atacar...). A tecla de Ativar e a de Empunhar
// arma do jogo tambem sao retiradas no teclado, para nao pegarem itens por conta propria.
class PlayerControlsEx : public PlayerControls
{
public:
	typedef EventResult(PlayerControlsEx::*FnReceiveEvent)(InputEvent **evn, BSTEventSource<InputEvent *> *source);

	static FnReceiveEvent fnReceiveEventOld;

	enum Action
	{
		kAct_Take,
		kAct_TakeOne,
		kAct_Use,
		kAct_TakeAll,
		kAct_Search,
		kAct_EquipRight,
		kAct_EquipLeft,
		kAct_Count
	};

	EventResult ReceiveEvent_Hook(InputEvent **evn, BSTEventSource<InputEvent *> *source)
	{
		if (evn && *evn)
		{
			FilterEvents(evn);
			if (!*evn)
				return kEvent_Continue;
		}

		return (this->*fnReceiveEventOld)(evn, source);
	}

	static int KeyOf(int a_action)
	{
		switch (a_action)
		{
		case kAct_Take:			return Settings::iKeyTake;
		case kAct_TakeOne:		return Settings::iKeyTakeOne;
		case kAct_Use:			return Settings::iKeyUse;
		case kAct_TakeAll:		return Settings::iKeyTakeAll;
		case kAct_Search:		return Settings::iKeySearch;
		case kAct_EquipRight:	return Settings::iKeyEquipRight;
		case kAct_EquipLeft:	return Settings::iKeyEquipLeft;
		}
		return 0;
	}

	static int ModOf(int a_action)
	{
		switch (a_action)
		{
		case kAct_Take:			return Settings::iModTake;
		case kAct_TakeOne:		return Settings::iModTakeOne;
		case kAct_Use:			return Settings::iModUse;
		case kAct_TakeAll:		return Settings::iModTakeAll;
		case kAct_Search:		return Settings::iModSearch;
		case kAct_EquipRight:	return Settings::iModEquipRight;
		case kAct_EquipLeft:	return Settings::iModEquipLeft;
		}
		return 0;
	}

	// 0 = nenhum, 1 = Shift, 2 = Ctrl, 3 = Alt; 99 = combinacao que nao usamos
	static int ActiveModifier(const bool *a_down)
	{
		const bool shift = a_down[42] || a_down[54];
		const bool ctrl = a_down[29] || a_down[157];
		const bool alt = a_down[56] || a_down[184];

		if (!shift && !ctrl && !alt) return 0;
		if (shift && !ctrl && !alt) return 1;
		if (ctrl && !shift && !alt) return 2;
		if (alt && !shift && !ctrl) return 3;
		return 99;
	}

	static void FilterEvents(InputEvent **evn)
	{
		static bool s_down[256] = {};			// estado das teclas do teclado (para os modificadores)
		static bool s_consumed[512] = {};		// teclas cujo premir foi consumido (consome-se tambem a repeticao e o soltar)
		static int s_searchCode = -1;			// tecla de Procurar premida e consumida
		static bool s_searchDone = false;		// Procurar ja disparou neste premir

		LootMenu *lootMenu = LootMenu::GetSingleton();
		const bool visible = (lootMenu && lootMenu->IsVisible());

		// teclas de Ativar e de Empunhar arma do jogo (teclado)
		int gameActivate = -1;
		int gameReady = -1;
		if (visible)
		{
			InputStringHolder *holder = InputStringHolder::GetSingleton();
			InputMappingManager *mapping = InputMappingManager::GetSingleton();
			if (holder && mapping)
			{
				const UInt32 a = mapping->GetMappedKey(holder->activate, BSInputDevice::kType_Keyboard);
				const UInt32 r = mapping->GetMappedKey(holder->readyWeapon, BSInputDevice::kType_Keyboard);
				if (a != InputMappingManager::kInvalid) gameActivate = (int)a;
				if (r != InputMappingManager::kInvalid) gameReady = (int)r;
			}
		}

		InputEvent *prev = nullptr;
		InputEvent *cur = *evn;
		while (cur)
		{
			InputEvent *next = cur->next;
			bool consume = false;

			const bool isKeyboard = (cur->deviceType == BSInputDevice::kType_Keyboard);
			const bool isMouse = (cur->deviceType == BSInputDevice::kType_Mouse);

			if (cur->eventType == InputEvent::kEventType_Button && (isKeyboard || isMouse))
			{
				ButtonEvent *btn = static_cast<ButtonEvent *>(cur);
				const UInt32 code = isKeyboard ? btn->keyMask : (256 + btn->keyMask);

				// estado dos modificadores (Shift, Ctrl, Alt)
				if (isKeyboard && btn->keyMask < 256)
					s_down[btn->keyMask] = !btn->IsUp();

				// varrimento: so observa a tecla (nao a consome), por isso o E continua a ativar normalmente
				if (code == (UInt32)Settings::iHLScanKey)
					Highlight::OnScanKey(btn->IsDown(), btn->IsUp(), btn->timer, visible);

				if (code < 512)
				{
					if (s_consumed[code])
					{
						// repeticao ou soltar de uma tecla ja consumida
						consume = true;

						if ((int)code == s_searchCode)
						{
							const bool reached = (Settings::iSearchHoldMs <= 0) ||
								(btn->IsPressed() && btn->timer * 1000.0f >= (float)Settings::iSearchHoldMs);
							if (!s_searchDone && reached && !btn->IsUp())
							{
								s_searchDone = true;
								Highlight::MarkSearched(lootMenu ? lootMenu->GetContainerRef() : nullptr);
								PlayerCharacterEx::OpenContainer();
							}
						}

						if (btn->IsUp())
						{
							s_consumed[code] = false;
							if ((int)code == s_searchCode)
								s_searchCode = -1;
						}
					}
					else if (visible && btn->IsDown())
					{
						const int modifier = ActiveModifier(s_down);

						int matched = -1;
						for (int act = 0; act < kAct_Count; ++act)
						{
							if ((UInt32)KeyOf(act) == code && ModOf(act) == modifier)
							{
								matched = act;
								break;
							}
						}

						if (matched >= 0)
						{
							consume = true;
							s_consumed[code] = true;

							switch (matched)
							{
							case kAct_Take:			lootMenu->TakeItem(false, false);	break;
							case kAct_TakeOne:		lootMenu->TakeItem(false, true);	break;
							case kAct_Use:			lootMenu->UseSelected(0);			break;
							case kAct_TakeAll:		lootMenu->TakeAllItems();			break;
							case kAct_EquipRight:	lootMenu->UseSelected(1);			break;
							case kAct_EquipLeft:	lootMenu->UseSelected(2);			break;
							case kAct_Search:
								s_searchCode = (int)code;
								s_searchDone = false;
								if (Settings::iSearchHoldMs <= 0)
								{
									s_searchDone = true;
									Highlight::MarkSearched(lootMenu->GetContainerRef());
									PlayerCharacterEx::OpenContainer();
								}
								break;
							}
						}
						else if (isKeyboard && ((int)code == gameActivate || (int)code == gameReady))
						{
							// a tecla de Ativar/Empunhar do jogo nao faz nada por conta propria com a janela aberta
							consume = true;
							s_consumed[code] = true;
						}
					}
				}
			}

			if (consume)
			{
				if (prev)
					prev->next = next;
				else
					*evn = next;
			}
			else
			{
				prev = cur;
			}

			cur = next;
		}
	}

	static void InitHook()
	{
		fnReceiveEventOld = SafeWrite32(0x010D4680 + 0x04 * 1, &ReceiveEvent_Hook);
	}
};

PlayerControlsEx::FnReceiveEvent PlayerControlsEx::fnReceiveEventOld;


//========================================================
// crosshair text patches
//========================================================

template <UInt32 VTBL>
class TESBoundObjectEx : public TESObjectCONT
{
public:
	typedef bool(TESBoundObjectEx::*FnGetCrosshairText)(TESObjectREFR *ref, BSString * dst, bool arg3);

	static FnGetCrosshairText fnGetCrosshairText;

	bool GetCrosshairText_Hook(TESObjectREFR *ref, BSString * dst, bool unk)
	{
		UInt32 retnAddr = *(UInt32*)(&ref - 1);

		if (retnAddr == 0x0073A136 && LootMenu::CanOpen(ref))
			return false;

		return (this->*fnGetCrosshairText)(ref, dst, unk);
	}

	static void InitHook()
	{
		fnGetCrosshairText = SafeWrite32(VTBL + 0x4D * 4, &GetCrosshairText_Hook);
	}
};

template <UInt32 VTBL>
typename TESBoundObjectEx<VTBL>::FnGetCrosshairText TESBoundObjectEx<VTBL>::fnGetCrosshairText;

typedef TESBoundObjectEx<0x01084D44> TESObjectCONTEx;
typedef TESBoundObjectEx<0x010841B4> TESObjectACTIEx;	// for ash pile
typedef TESBoundObjectEx<0x010A5964> TESNPCEx;


//========================================================
// HDT patch
//========================================================

#include <map>

class FreezeEventHandler : public BSTEventSink<MenuOpenCloseEvent>
{
public:
	typedef EventResult(FreezeEventHandler::*FnReceiveEvent)(MenuOpenCloseEvent *evn, BSTEventSource<MenuOpenCloseEvent> *src);

	static std::map<UInt32, FnReceiveEvent> ms_handlerMap;

	UInt32 GetVPtr() const
	{
		return *(UInt32*)this;
	}

	EventResult ReceiveEvent_Hook(MenuOpenCloseEvent *evn, BSTEventSource<MenuOpenCloseEvent> *src)
	{
		static BSFixedString menuName = "Better Looting Menu";

		if (evn->menuName == menuName)
		{
			_MESSAGE("* hdt-pe sink is skipped");
			return kEvent_Continue;
		}

		auto it = ms_handlerMap.find(GetVPtr());
		if (it == ms_handlerMap.end())
			return kEvent_Continue;

		FnReceiveEvent handler = it->second;
		return (this->*handler)(evn, src);
	}

	void InstallHook()
	{
		UInt32 vptr = GetVPtr();
		FreezeEventHandler::FnReceiveEvent pFn = SafeWrite32(vptr + 4, &FreezeEventHandler::ReceiveEvent_Hook);
		ms_handlerMap[vptr] = pFn;
	}
};

std::map<UInt32, FreezeEventHandler::FnReceiveEvent> FreezeEventHandler::ms_handlerMap;


class MenuOpenCloseEventSource : public BSTEventSource<MenuOpenCloseEvent>
{
public:
	void ProcessHook()
	{
		lock.Lock();

		BSTEventSink<MenuOpenCloseEvent> *sink;
		UInt32 idx = 0;
		while (eventSinks.GetAt(idx, sink))
		{
			const char * className = GetObjectClassName(sink);
			if (strcmp(className, "class FreezeEventHandler") == 0)
			{
				FreezeEventHandler *freezeEventHandler = static_cast<FreezeEventHandler *>(sink);
				_MESSAGE("<< hdtPhysicsExtensions detected %p>>", freezeEventHandler->GetVPtr());

				freezeEventHandler->InstallHook();
			}

			++idx;
		}

		lock.Unlock();
	}


	static void InitHook()
	{
		MenuManager *mm = MenuManager::GetSingleton();
		if (mm)
		{
			MenuOpenCloseEventSource *pThis = static_cast<MenuOpenCloseEventSource*>(mm->GetMenuOpenCloseEventSource());
			pThis->ProcessHook();
		}
	}
};

//========================================================

namespace Hooks
{
	void Install()
	{
		// input patches
		if (InputManager::GetSingleton()->IsGamepadEnabled())
		{
			FavoritesHandler::InitHook();			// directional pad
		}
		else
		{
			ThirdPersonStateHandler::InitHook();	// mouse wheel
			FirstPersonStateHandler::InitHook();	// mouse wheel
		}
#ifdef USE_JUMP_KEY
		JumpHandlerEx::InitHook();					// [Space] or (Y)
#endif
		ReadyWeaponHandlerEx::InitHook();			// [R] or (X)
		PlayerCharacterEx::InitHook();				// [E] or (A)
		PlayerControlsEx::InitHook();				// teclas da janela de loot (teclado e rato)

		// crosshair text patches
		TESObjectCONTEx::InitHook();
		TESObjectACTIEx::InitHook();
		TESNPCEx::InitHook();

		// HDT patch
		MenuOpenCloseEventSource::InitHook();
	}
}
