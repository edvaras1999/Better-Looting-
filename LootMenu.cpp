#include "LootMenu.h"
#include "Settings.h"
#include <SKSE/PluginAPI.h>
#include <SKSE/DebugLog.h>
#include <SKSE/GameRTTI.h>
#include <SKSE/GameForms.h>
#include <SKSE/GameObjects.h>
#include <SKSE/GameExtraData.h>
#include <SKSE/GameInput.h>
#include <SKSE/GameSettings.h>
#include <SKSE/Scaleform.h>
#include <SKSE/NiNodes.h>
#include <SKSE/NiControllers.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <algorithm>
#include <memory>


static void QueueUITask(void (LootMenu::*fn)(GFxMovieView *));

LootMenu * LootMenu::ms_pSingleton = nullptr;
BSSpinLock LootMenu::ms_lock;


static bool IsDownLShift()
{
	if (Settings::bDisableLootSingle)
		return false;

	InputManager *input = InputManager::GetSingleton();

	//BSInputDevice *keyboard = input->keyboard;
	BSWin32KeyboardDevice *keyboard = DYNAMIC_CAST<BSWin32KeyboardDevice *>(input->keyboard);
	if (keyboard && keyboard->IsEnabled())
	{
		static UInt32 keyRun = 0;
		if (keyRun == 0)
		{
			InputStringHolder *holder = InputStringHolder::GetSingleton();
			keyRun = InputMappingManager::GetSingleton()->GetMappedKey(holder->run, BSInputDevice::kType_Keyboard);
		}

		if (keyRun != InputMappingManager::kInvalid && keyboard->IsPressed(keyRun))
			return true;
	}

	//BSInputDevice *gamepad = input->gamepad;
	BSWin32GamepadDevice *gamepad = DYNAMIC_CAST<BSWin32GamepadDevice *>(input->gamepad);
	if (gamepad && gamepad->IsEnabled())
	{
		static UInt32 keySprint = 0;
		if (keySprint == 0)
		{
			InputStringHolder *holder = InputStringHolder::GetSingleton();
			keySprint = InputMappingManager::GetSingleton()->GetMappedKey(holder->sprint, BSInputDevice::kType_Gamepad);
		}

		if (keySprint != InputMappingManager::kInvalid && gamepad->IsPressed(keySprint))
			return true;
	}

	return false;
}


// Opens LootMenu when LoadingMenu is closed.
class AutoOpenLootMenuSink : public BSTEventSink<MenuOpenCloseEvent>
{
public:
	EventResult ReceiveEvent(MenuOpenCloseEvent *evn, BSTEventSource<MenuOpenCloseEvent> *src)
	{
		MenuManager *mm = MenuManager::GetSingleton();
		UIStringHolder *holder = UIStringHolder::GetSingleton();

		if (evn->menuName == holder->loadingMenu && !evn->opening)
		{
			LootMenu *lootMenu = LootMenu::GetSingleton();

			if (lootMenu || LootMenu::SendRequestStart())
			{
				mm->BSTEventSource<MenuOpenCloseEvent>::RemoveEventSink(this);
				delete this;
			}
		}

		return kEvent_Continue;
	}

	static void Register()
	{
		MenuManager *mm = MenuManager::GetSingleton();
		if (mm)
		{
			AutoOpenLootMenuSink *sink = new AutoOpenLootMenuSink();
			if (sink)
			{
				mm->BSTEventSource<MenuOpenCloseEvent>::AddEventSink(sink);
			}
		}
	}
};


class HasActivateChoiceVisitor : public PerkEntryVisitor
{
public:
	HasActivateChoiceVisitor(Actor *a_actor, TESObjectREFR *a_target)
	{
		m_actor = a_actor;
		m_target = a_target;
		m_result = false;
	}

	virtual UInt32 Visit(BGSPerkEntry *perkEntry) override
	{
		if (perkEntry->CanProcess(2, &m_actor))
		{
			BGSEntryPointPerkEntry *entryPoint = (BGSEntryPointPerkEntry *)perkEntry;
			if (entryPoint->functionData)
			{
				BGSEntryPointFunctionDataActivateChoice *activateChoice = (BGSEntryPointFunctionDataActivateChoice*)entryPoint->functionData;
				BGSPerk *perk = entryPoint->perk;

				if ((activateChoice->flags & BGSEntryPointFunctionDataActivateChoice::kFlag_RunImmediately) == 0)
				{
					enum {
						VampireFeed = 0x0CF02C,
						Cannibalism = 0x0EE5C3
					};

					switch (perk->formID)
					{
					case VampireFeed:
					case Cannibalism:
						break;
					default:
						m_result = true;
					}
				}
				if ((activateChoice->flags & BGSEntryPointFunctionDataActivateChoice::kFlag_ReplaceDefault) != 0)
				{
					m_result = true;
				}
			}
		}

		return 1;
	}

	bool GetResult() const
	{
		return m_result;
	}

protected:
	Actor			* m_actor;
	TESObjectREFR	* m_target;
	bool			m_result;
};


class DelayedUpdater : public TaskDelegate
{
public:
	virtual void Run() override
	{
		LootMenu *lootMenu = LootMenu::GetSingleton();
		if (lootMenu)
		{
			lootMenu->Update();
		}
	}
	virtual void Dispose() override
	{
	}

	static void Register()
	{
		static DelayedUpdater singleton;

		const SKSEPlugin *plugin = SKSEPlugin::GetSingleton();
		const SKSETaskInterface *task = plugin->GetInterface<SKSETaskInterface>();
		
		task->AddTask(&singleton);
	}
};


//========================================================
// constructor & destructor
//========================================================

LootMenu::LootMenu() : m_targetRef(nullptr), m_containerRef(nullptr), m_owner(nullptr), m_items(), m_selectedIndex(-1),
	m_bNowTaking(false), m_bOpenAnim(false), m_bUpdateRequest(false), m_bDisabled(false), m_mcRoot(nullptr), m_visual(Settings::iVisual)
{
	// visual 0 = Redesigned (padrao), 1 = QuickLootIE original
	const char *swfPath = (m_visual == 1) ? "LootMenuIE" : "LootMenuRedesigned";

	_MESSAGE("LootMenu::LootMenu()");

	if (LoadMovie(swfPath, GFxMovieView::SM_ShowAll, 0.0f))
	{
		_MESSAGE("  loaded Inteface/%s.swf successfully", swfPath);

		menuDepth = 2;			// set this lower than that of Fader Menu (3)
		flags = kType_DoNotDeleteOnClose | kType_DoNotPreventGameSave | kType_Unk10000;
		m_mcRoot = "_root.lootMenu";

		m_items.reserve(128);
	}

	_MESSAGE("");
}


LootMenu::~LootMenu()
{
	_MESSAGE("LootMenu::~LootMenu()");

	Clear();

	_MESSAGE("");
}


bool LootMenu::Install()
{
	_MESSAGE("LootMenu::Install()");

	MenuManager *mm = MenuManager::GetSingleton();
	if (!mm)
		return false;

	mm->Register("Better Looting Menu", []() -> IMenu * {
		return new LootMenu();
	});

	AutoOpenLootMenuSink::Register();

	_MESSAGE("  LootMenu has been Installed\n");
	
	return true;
}


bool LootMenu::SendRequestStart()
{
	if (ms_pSingleton)
		return false;

	MenuManager *mm = MenuManager::GetSingleton();
	UIStringHolder *holder = UIStringHolder::GetSingleton();
	if (mm->IsMenuOpen(holder->mainMenu))
		return false;

	UIManager *ui = UIManager::GetSingleton();
	ui->AddMessage("Better Looting Menu", UIMessage::kMessage_Open, nullptr);
	return true;
}


bool LootMenu::SendRequestStop()
{
	if (!ms_pSingleton)
		return false;

	UIManager *ui = UIManager::GetSingleton();
	ui->AddMessage("Better Looting Menu", UIMessage::kMessage_Close, nullptr);
	return true;
}


//===================================================
//
//===================================================

UInt32 LootMenu::ProcessMessage(UIMessage *message)
{
	UInt32 result = 2;

	if (message->type == UIMessage::kMessage_Open)
	{
		_MESSAGE("OPEN MESSAGE RECEIVED: (name=%s, view=%p)", message->name.c_str(), view);
		OnMenuOpen();
		_MESSAGE("");
	}
	else if (message->type == UIMessage::kMessage_Close)
	{
		_MESSAGE("CLOSE MESSAGE RECEIVED: (name=%s)", message->name.c_str());
		OnMenuClose();
		_MESSAGE("");
	}
	else if (message->type == UIMessage::kMessage_Scaleform)
	{
		if (view && view->GetVisible() && message->data)
		{
			BSUIScaleformData *scaleformData = (BSUIScaleformData *)message->data;
			GFxEvent *event = scaleformData->event;

			if (event->type == GFxEvent::MouseWheel)
			{
				GFxMouseEvent *mouse = (GFxMouseEvent *)event;
				if (mouse->scrollDelta > 0)
					SetIndex(-1);
				else if (mouse->scrollDelta < 0)
					SetIndex(1);
			}
			else if (event->type == GFxEvent::KeyDown)
			{
				GFxKeyEvent *key = (GFxKeyEvent *)event;
				if (key->keyCode == GFxKey::Up)
					SetIndex(-1);
				else if (key->keyCode == GFxKey::Down)
					SetIndex(1);
			}
			else if (event->type == GFxEvent::CharEvent)
			{
				GFxCharEvent *charEvent = (GFxCharEvent *)event;
				_MESSAGE("wchar = %p", charEvent->wcharCode);
			}
		}
	}

	return result;
}


void LootMenu::Setup()
{
	if (view)
		QueueUITask(&LootMenu::UI_Init);
}


void LootMenu::RequestVisualReload()
{
	// A troca e feita em OnMenuOpen, na proxima vez que o menu abrir.
	_MESSAGE("Visual alterado para %d: o menu sera recriado na proxima abertura", Settings::iVisual);
}


void LootMenu::OnMenuOpen()
{
	// O SWF so e lido quando o menu e criado. Se o visual mudou no MCM, tira-se a marca
	// que impede o jogo de apagar o menu e fecha-se: na proxima abertura cria-se um novo.
	if (m_visual != Settings::iVisual)
	{
		_MESSAGE("  visual alterado (%d -> %d): a recriar o menu", m_visual, Settings::iVisual);
		flags &= ~kType_DoNotDeleteOnClose;
		UIManager::GetSingleton()->AddMessage("Better Looting Menu", UIMessage::kMessage_Close, nullptr);
		return;
	}

	if (view)
	{
		BSSpinLockGuard guard(ms_lock);

		_MESSAGE("  registering MenuEventHandler...");
		MenuControls *controls = MenuControls::GetSingleton();
		controls->RegisterHandler(this);

		_MESSAGE("  registering skse crosshair event...");
		const SKSEPlugin *plugin = SKSEPlugin::GetSingleton();
		const SKSEMessagingInterface *mes = plugin->GetInterface(SKSEMessagingInterface::Version_1);
		BSTEventSource<SKSECrosshairRefEvent> *source = mes->GetEventDispatcher<SKSECrosshairRefEvent>();
		source->AddEventSink(this);

		Setup();

		ms_pSingleton = this;

		_MESSAGE("  LootMenu is opened successfully.");
	}
}


void LootMenu::OnMenuClose()
{
	if (ms_pSingleton)
	{
		BSSpinLockGuard guard(ms_lock);

		ms_pSingleton = nullptr;

		Clear();
		view->SetVisible(false);

		_MESSAGE("  unregistering MenuEventHandler");
		MenuControls *controls = MenuControls::GetSingleton();
		controls->RemoveHandler(this);

		_MESSAGE("  unregistering skse crosshair event");
		const SKSEPlugin *plugin = SKSEPlugin::GetSingleton();
		const SKSEMessagingInterface *mes = plugin->GetInterface(SKSEMessagingInterface::Version_1);
		BSTEventSource<SKSECrosshairRefEvent> *source = mes->GetEventDispatcher<SKSECrosshairRefEvent>();
		source->RemoveEventSink(this);

		_MESSAGE("  LootMenu is closed successfully.");
	}

	AutoOpenLootMenuSink::Register();
}


//===================================================
// input
//===================================================

bool LootMenu::CanProcess(InputEvent *evn)
{
	if (!IsVisible())
		return false;

	if (evn->eventType != InputEvent::kEventType_Button)
		return false;

	ButtonEvent *button = static_cast<ButtonEvent *>(evn);
	if (evn->deviceType == BSInputDevice::kType_Gamepad)
	{
		return (button->keyMask == KeyCode::Gamepad_Up || button->keyMask == KeyCode::Gamepad_Down);
	}
	else if (evn->deviceType == BSInputDevice::kType_Mouse)
	{
		return (button->keyMask == KeyCode::Mouse_WheelDown || button->keyMask == KeyCode::Mouse_WheelUp);
	}
	else if (evn->deviceType == BSInputDevice::kType_Keyboard)
	{
		InputStringHolder *holder = InputStringHolder::GetSingleton();
		return (evn->GetControlID() == holder->zoomIn || evn->GetControlID() == holder->zoomOut);
	}

	return false;
}


bool LootMenu::ProcessButton(ButtonEvent *evn)
{
	if (!evn->IsDown())
		return true;

	if (evn->deviceType == BSInputDevice::kType_Gamepad)
	{
		if (evn->keyMask == KeyCode::Gamepad_Up)
			SetIndex(-1);
		else if (evn->keyMask == KeyCode::Gamepad_Down)
			SetIndex(1);
	}
	else if (evn->deviceType == BSInputDevice::kType_Mouse)
	{
		if (evn->keyMask == KeyCode::Mouse_WheelUp)
			SetIndex(-1);
		else if (evn->keyMask == KeyCode::Mouse_WheelDown)
			SetIndex(1);
	}
	else if (evn->deviceType == BSInputDevice::kType_Keyboard)
	{
		InputStringHolder *holder = InputStringHolder::GetSingleton();
		if (evn->GetControlID() == holder->zoomIn)
			SetIndex(-1);
		else if (evn->GetControlID() == holder->zoomOut)
			SetIndex(1);
	}

	return true;
}

//===================================================
// event handlers
//===================================================

EventResult LootMenu::ReceiveEvent(TESContainerChangedEvent *evn, BSTEventSource<TESContainerChangedEvent> *src)
{
	BSSpinLockGuard guard(ms_lock);

	if (m_containerRef && !m_bNowTaking)
	{
		FormID containerFormID = m_containerRef->formID;
		if (evn->from == containerFormID || evn->to == containerFormID)
		{
			if (m_bUpdateRequest == false)
			{
				_MESSAGE("CONTAINER HAS BEEN UPDATED WITHOUT QUICK LOOTING");
				_MESSAGE("    %p %s\n", containerFormID, m_containerRef->GetReferenceName());
			}

			MenuManager *mm = MenuManager::GetSingleton();
			if (mm->numPauseGame > 0)
			{
				// 現在ゲーム停止中なら、更新予約をする
				// 最後のポーズメニューが閉じた時に、まとめて更新する。
				m_bUpdateRequest = true;
			}
			else
			{
				// 直ちに更新する
				//Update();
				DelayedUpdater::Register();
			}
		}
	}

	return kEvent_Continue;
}

EventResult LootMenu::ReceiveEvent(MenuOpenCloseEvent *evn, BSTEventSource<MenuOpenCloseEvent> *src)
{
	BSSpinLockGuard guard(ms_lock);

	if (IsOpen())
	{
		UIStringHolder *holder = UIStringHolder::GetSingleton();
		MenuManager *mm = MenuManager::GetSingleton();

		if (evn->opening)
		{
			IMenu *menu = mm->GetMenu(evn->menuName);
			if (menu)
			{
				if ((menu->flags & kType_StopCrosshairUpdate) != 0)
				{
					// DialogueMenu, CraftingMenu
					Close();
				}
				else if ((menu->flags & kType_PauseGame) != 0)
				{
					view->SetVisible(false);
				}
			}
		}
		else	// close
		{
			if (mm->numPauseGame == 0 && view->GetVisible() == false)
			{
				view->SetVisible(true);

				if (m_bUpdateRequest)
				{
					_MESSAGE("UPDATING CONTAINER");
					_MESSAGE("    %p %s", m_containerRef->formID, m_containerRef->GetReferenceName());

					//Update();
					DelayedUpdater::Register();
					_MESSAGE("");
				}
			}

			if (evn->menuName == holder->containerMenu)
			{
				m_bOpenAnim = false;
			}
		}
	}

	return kEvent_Continue;
}


EventResult LootMenu::ReceiveEvent(SKSECrosshairRefEvent *evn, BSTEventSource<SKSECrosshairRefEvent> *src)
{
	if (evn->crosshairRef && CanOpen(evn->crosshairRef))
	{
		Open(evn->crosshairRef);
	}
	else
	{
		if (IsOpen())
			Close();
	}

	return kEvent_Continue;
}


//===================================================
//
//===================================================

bool LootMenu::CanOpen(TESObjectREFR *a_ref)
{
	if (!ms_pSingleton || ms_pSingleton->m_bDisabled)
		return false;

	if (!a_ref || !a_ref->baseForm)
		return false;

	MenuManager *mm = MenuManager::GetSingleton();
	if (mm && mm->numPauseGame + mm->numStopCrosshairUpdate > 0)
		return false;

	InputMappingManager *mapping = InputMappingManager::GetSingleton();
	if (!mapping || !mapping->IsMovementControlsEnabled())
		return false;

	// player is grabbing / in favor state / in killmove
	if (g_thePlayer->GetGrabbedRef() || g_thePlayer->GetActorInFavorState() || g_thePlayer->IsInKillMove())
		return false;

	bool bAnimationDriven;
	static BSFixedString strAnimationDriven = "bAnimationDriven";
	if (g_thePlayer->GetAnimationVariableBool(strAnimationDriven, bAnimationDriven) && bAnimationDriven)
		return false;

	// in combat
	if (Settings::bDisableInCombat && g_thePlayer->IsInCombat())
		return false;

	// theft
	if (Settings::bDisableTheft && a_ref->IsOffLimits())
		return false;


	TESObjectREFR *containerRef = nullptr;
	TESForm *baseForm = a_ref->baseForm;

	if (baseForm->Is(FormType::Activator))
	{
		RefHandle refHandle;
		if (a_ref->extraData.GetAshPileRefHandle(refHandle) && refHandle != g_invalidRefHandle)
		{
			TESObjectREFRPtr refPtr;
			if (TESObjectREFR::LookupByHandle(refHandle, refPtr))
				containerRef = refPtr;
		}
	}
	else  if (baseForm->Is(FormType::Container))
	{
		if (!a_ref->IsLocked())
			containerRef = a_ref;
	}
	else if (baseForm->Is(FormType::NPC))
	{
		if (a_ref->IsDead(true))
			containerRef = a_ref;
	}
	if (!containerRef)
		return false;

	UInt32 numItems = containerRef->GetNumItems(false, false);

	// empty container
	if (Settings::bDisableIfEmpty && numItems == 0)
		return false;

	// container with meny item
	if (Settings::iItemLimit > 0 && numItems >= Settings::iItemLimit)
		return false;

	//// do not open it which is replaced activate label.
	//if (containerRef)
	//{
	//	BSString label;
	//	BGSEntryPointPerkEntry::Calculate(BGSEntryPointPerkEntry::kEntryPoint_Set_Activate_Label, g_thePlayer, a_ref, &label);
	//	if (label.GetLength() > 0)
	//		return false;
	//}

	// do not open it which has any activate choice or replaced default.
	if (g_thePlayer->CanProcessEntryPointPerkEntry(BGSEntryPointPerkEntry::kEntryPoint_Activate))
	{
		HasActivateChoiceVisitor visitor(g_thePlayer, a_ref);
		g_thePlayer->VisitEntryPointPerkEntries(BGSEntryPointPerkEntry::kEntryPoint_Activate, visitor);
		if (visitor.GetResult())
			return false;
	}

	return true;
}


bool LootMenu::IsOpen() const
{
	BSSpinLockGuard guard(ms_lock);

	return m_containerRef != nullptr;
}


bool LootMenu::IsVisible() const
{
	BSSpinLockGuard guard(ms_lock);

	return (m_containerRef && view && view->GetVisible());
}


bool LootMenu::Open(TESObjectREFR *a_ref)
{
	BSSpinLockGuard guard(ms_lock);

	if (IsOpen())	// is another container already opened ?
	{
		if (m_containerRef->IsNot(FormType::Character))
			PlayAnimationClose();
	}
	else
	{
		MenuManager *mm = MenuManager::GetSingleton();
		mm->BSTEventSource<MenuOpenCloseEvent>::AddEventSink(this);
		g_containerChangedEventSource.AddEventSink(this);
	}

	m_targetRef = a_ref;
	m_containerRef = a_ref;

	if (m_targetRef->baseForm->Is(FormType::Activator))		// is target a ash pile ?
	{
		RefHandle refHandle;
		if (m_targetRef->extraData.GetAshPileRefHandle(refHandle) && refHandle != g_invalidRefHandle)
		{
			TESObjectREFRPtr refPtr;
			if (TESObjectREFR::LookupByHandle(refHandle, refPtr))
				m_containerRef = refPtr;
		}
	}

	m_selectedIndex = 0;
	
	Update();
	view->SetVisible(true);

	return true;
}

void LootMenu::Close()
{
	if (!IsOpen())
		return;

	BSSpinLockGuard guard(ms_lock);

	if (m_containerRef->IsNot(FormType::Character))
		PlayAnimationClose();

	Clear();

	InvokeScaleform_Close();
	view->SetVisible(false);
}


static bool IsValidItem(TESForm *item)
{
	if (!item)
		return false;

	if (item->Is(FormType::LeveledItem))
		return false;

	if (item->Is(FormType::Light))
	{
		TESObjectLIGH *light = static_cast<TESObjectLIGH *>(item);
		if (!light->CanBeCarried())
			return false;
	}
	else
	{
		if (!item->IsPlayable())
			return false;
	}

	TESFullName *fullName = DYNAMIC_CAST<TESFullName*>(item);
	if (!fullName)
		return false;

	const char *name = fullName->GetFullName();
	if (!name || !name[0])
		return false;

	return true;
}


void LootMenu::Update()
{
	BSSpinLockGuard guard(ms_lock);

	if (!IsOpen() || m_bDisabled)
		return;

	// clear all item info before update
	m_items.clear();

	UInt32 numItems = m_containerRef->GetNumItems(false, false);
	if (numItems > m_items.capacity())
	{
		m_items.reserve(numItems);
	}

	// set owner
	m_owner = nullptr;
	if (m_containerRef->IsNot(FormType::Character))
	{
		m_owner = m_containerRef->GetOwner();
	}
	

	//===================================
	// default items
	//===================================
	BSTHashMap<TESForm *, SInt32> itemMap;

	TESContainer *container = m_containerRef->GetContainer();
	TESContainer::Entry *entry;
	UInt32 index = 0;
	while (container->GetContainerItemAt(index++, entry))
	{
		if (!entry)
			continue;

		if (!IsValidItem(entry->form))
			continue;

		itemMap.SetAt(entry->form, entry->count);
	}


	//================================
	// changes
	//================================
	ExtraContainerChanges *exChanges = m_containerRef->extraData.GetExtraData<ExtraContainerChanges>();
	InventoryChanges *changes = (exChanges) ? exChanges->changes : nullptr;
	if (!changes)
	{
		_MESSAGE("FORCES CONTAINER TO SPAWN");
		_MESSAGE("  %p [%s]\n", m_containerRef->formID, m_containerRef->GetReferenceName());
		
		changes = new InventoryChanges(m_containerRef);
		m_containerRef->extraData.SetInventoryChanges(changes);
		changes->InitContainer();
	}
	
	if (changes->entryList)
	{
		for (InventoryEntryData *pEntry : *changes->entryList)
		{
			if (!pEntry)
				continue;

			TESForm *item = pEntry->baseForm;
			if (!IsValidItem(item))
				continue;

			SInt32 totalCount = pEntry->GetCount();
			SInt32 baseCount = 0;
			if (itemMap.GetAt(item, baseCount))
			{
				if (baseCount < 0)			// this entry is already processed. skip.
					continue;

				if (!item->IsGold())		// gold is special case.
					totalCount += baseCount;
			}
			itemMap.SetAt(item, -1);		// mark as processed.

			if (totalCount <= 0)
				continue;
			
			InventoryEntryData *defaultEntry = nullptr;
			if (!item->IsGold() && pEntry->extraList)
			{
				for (BaseExtraList *extraList : *pEntry->extraList)
				{
					if (!extraList)
						continue;

					int count = extraList->GetItemCount();
					if (count <= 0)
					{
						totalCount += count;
						continue;
					}
					totalCount -= count;

					InventoryEntryData *pNewEntry = nullptr;
					if (extraList->HasType<ExtraTextDisplayData>() || extraList->HasType<ExtraHealth>())
					{
						pNewEntry = new InventoryEntryData(item, count);
						pNewEntry->AddEntryList(extraList);
					}
					else if (extraList->HasType<ExtraOwnership>())
					{
						pNewEntry = new InventoryEntryData(item, count);
						pNewEntry->AddEntryList(extraList);
					}
					else
					{
						if (!defaultEntry)
							defaultEntry = new InventoryEntryData(item, 0);

						defaultEntry->AddEntryList(extraList);
						defaultEntry->countDelta += count;
					}

					if (pNewEntry)
					{
						m_items.emplace_back(pNewEntry, m_owner);
					}
				}
			}

			if (totalCount > 0)	// rest
			{
				if (!defaultEntry)
					defaultEntry = new InventoryEntryData(item, 0);
				
				defaultEntry->countDelta += totalCount;
			}

			if (defaultEntry)
			{
				m_items.emplace_back(defaultEntry, m_owner);
			}
		}
	}

	//================================
	// default items that were not processed
	//================================
	for (auto &node : itemMap)
	{
		if (node.value <= 0)
			continue;

		if (!IsValidItem(node.key))
			continue;

		InventoryEntryData *entry = new InventoryEntryData(node.key, node.value);
		m_items.emplace_back(entry, m_owner);
	}

	//================================
	// dropped items
	//================================
	ExtraDroppedItemList *exDroppedItemList = m_containerRef->extraData.GetExtraData<ExtraDroppedItemList>();
	if (exDroppedItemList)
	{
		for (RefHandle handle : exDroppedItemList->handles)
		{
			if (handle == g_invalidRefHandle)
				continue;

			TESObjectREFRPtr refPtr;
			if (!TESObjectREFR::LookupByHandle(handle, refPtr))
				continue;

			if (!IsValidItem(refPtr->baseForm))
				continue;

			InventoryEntryData *entry = new InventoryEntryData(refPtr->baseForm, 1);
			entry->AddEntryList(&refPtr->extraData);
			m_items.emplace_back(entry, m_owner);
		}
	}


	if (!m_items.empty())
	{
		Sort();
	}

	InvokeScaleform_Open();

	m_bUpdateRequest = false;
}


void LootMenu::Sort()
{
	//std::sort(m_items.begin(), m_items.end());

	qsort(&m_items[0], m_items.size(), sizeof(ItemData), [](const void *pA, const void *pB) -> int {
		const ItemData &a = *(const ItemData *)pA;
		const ItemData &b = *(const ItemData *)pB;

		if (a.pEntry == b.pEntry)
			return 0;
		return ItemSortLess(a, b) ? -1 : 1;
	});
}


void LootMenu::SetIndex(SInt32 index)
{
	BSSpinLockGuard guard(ms_lock);

	if (IsOpen())
	{
		const int tail = m_items.size() - 1;
		m_selectedIndex += index;
		if (m_selectedIndex > tail)
			m_selectedIndex = tail;
		else if (m_selectedIndex < 0)
			m_selectedIndex = 0;

		InvokeScaleform_SetIndex();
	}
}


void LootMenu::TakeItem(bool a_all)
{
	BSSpinLockGuard guard(ms_lock);

	if (!IsOpen())
		return;

	if (m_selectedIndex < 0 || m_items.size() <= m_selectedIndex)
		return;

	m_bNowTaking = true;

	auto it = m_items.begin() + m_selectedIndex;
	InventoryEntryData *item = it->pEntry;
	
	bool bTakeSingle = false;

	BaseExtraList *extraData = nullptr;
	if (item->extraList && !item->extraList->empty())
		extraData = item->extraList->front();

	if (extraData && extraData->HasType<ExtraItemDropper>())
	{
		// picks up the weapons that dropped on the ground. 

		TESObjectREFR* refItem = (TESObjectREFR*)((UInt32)extraData - 0x48);
		g_thePlayer->PickUpItem(refItem, 1, false, true);
	}
	else
	{
		UInt32 lootMode = TESObjectREFR::kRemoveType_Take;			// take
		UInt32 numItems = item->countDelta;

		if (m_containerRef->IsDead(false))		// dead body
		{
			g_thePlayer->PlayPickupEvent(item->baseForm, m_owner, m_containerRef, 6);
		}
		else									// container
		{
			g_thePlayer->PlayPickupEvent(item->baseForm, m_owner, m_containerRef, 5);

			if (m_containerRef->IsOffLimits())
				lootMode = TESObjectREFR::kRemoveType_Steal;		// steal
			
			if (!m_bOpenAnim)
			{
				SendChestLootedEvent();
			}
		}

		if (!a_all && numItems > 1 && IsDownLShift())
		{
			bTakeSingle = true;
			numItems = 1;
		}
		
		RefHandle handle = 0;
		m_containerRef->RemoveItem(&handle, item->baseForm, numItems, lootMode, extraData, g_thePlayer, 0, 0);

		// remove arrow projectile 3D.
		static_cast<TESBoundObject*>(item->baseForm)->OnRemovedFrom(m_containerRef);

		if (m_containerRef->Is(FormType::Character))
		{
			Actor *actor = static_cast<Actor*>(m_containerRef);
			if (actor->processManager)
			{
				// dispel worn item enchants
				typedef void(*FnDispelWornItemEnchants)(Actor*);
				const FnDispelWornItemEnchants dispelWornItemEnchants = (FnDispelWornItemEnchants)0x00664B90;

				dispelWornItemEnchants(actor);
				actor->processManager->UpdateEquipment(actor);
			}
		}
		else
		{
			if (m_containerRef->IsOffLimits())
			{
				_MESSAGE("SEND STEAL ALERM");
				UInt32 totalValue = item->GetValue() * numItems;
				g_thePlayer->SendStealAlerm(m_containerRef, nullptr, 0, totalValue, m_owner, true);
			}

			PlayAnimationOpen();
		}

		// plays item pickup sound
		PlaySound(item->baseForm);
	}

	m_bNowTaking = false;

	if (bTakeSingle)
	{
		Update();
		//DelayedUpdater::Register();
	}
	else
	{
		m_items.erase(it);
		InvokeScaleform_Open();
	}
}


void LootMenu::TakeAllItems()
{
	BSSpinLockGuard guard(ms_lock);

	if (!IsOpen())
		return;

	// cada TakeItem retira o primeiro item da lista; o limite evita ciclos infinitos
	size_t remaining = m_items.size();
	while (remaining-- > 0 && !m_items.empty() && IsOpen())
	{
		m_selectedIndex = 0;
		TakeItem(true);
	}
}


void LootMenu::Clear()
{
	if (m_containerRef)
	{
		MenuManager *mm = MenuManager::GetSingleton();
		mm->BSTEventSource<MenuOpenCloseEvent>::AddEventSink(this);
		g_containerChangedEventSource.RemoveEventSink(this);
	}

	m_targetRef = nullptr;
	m_containerRef = nullptr;
	m_owner = nullptr;
	m_selectedIndex = -1;

	m_items.clear();

	m_bNowTaking = false;
	m_bOpenAnim = false;
	m_bUpdateRequest = false;
}



//===================================================
// scaleform
//===================================================

class LootMenuUITask : public UIDelegate
{
public:
	typedef void (LootMenu::*FnUI)(GFxMovieView *);

	FnUI	m_fn;

	LootMenuUITask(FnUI fn) : m_fn(fn)
	{
	}

	TES_FORMHEAP_REDEFINE_NEW();

	void Run() override
	{
		LootMenu *lootMenu = LootMenu::GetSingleton();
		if (lootMenu)
		{
			GFxMovieView *view = lootMenu->GetMovieView();
			if (view)
				(lootMenu->*m_fn)(view);
		}
	}

	void Dispose() override
	{
		delete this;
	}
};

static void QueueUITask(void (LootMenu::*fn)(GFxMovieView *))
{
	const SKSEPlugin *plugin = SKSEPlugin::GetSingleton();
	const SKSETaskInterface *task = plugin->GetInterface(SKSETaskInterface::Version_2);
	if (task)
		task->AddUITask(new LootMenuUITask(fn));
}


bool IsUTF8(const char *str);	// UTF8.cpp


static void SetGFxString(GFxMovieView *view, GFxValue &value, const char *str)
{
	UInt32 codePage = (Settings::bForceAnsi || !IsUTF8(str)) ? CP_ACP : CP_UTF8;

	//if (codePage == CP_ACP)
	//	Settings::bForceAnsi = true;

	if (codePage == CP_UTF8)
	{
		//view->CreateString(&value, str);
		value.SetString(str);
	}
	else
	{
		// ANSI => UTF8
		const static std::size_t bufSize = 128;

		int sizeUnicode = MultiByteToWideChar(codePage, 0, str, -1, nullptr, 0);
		if (sizeUnicode >= bufSize)
		{
			// overflows loccal stack. use heap.

			// ANSI => Unicode
			wchar_t *strUnicode = static_cast<wchar_t *>(g_formHeap->Allocate(sizeof(wchar_t) * sizeUnicode, alignof(wchar_t), true));
			MultiByteToWideChar(codePage, 0, str, -1, strUnicode, sizeUnicode);

			// Unicode => UTF-8
			int sizeUTF8 = WideCharToMultiByte(CP_UTF8, 0, strUnicode, sizeUnicode, nullptr, 0, nullptr, nullptr);
			char *strUTF8 = static_cast<char *>(FormHeap_Allocate(sizeUTF8));
			WideCharToMultiByte(CP_UTF8, 0, strUnicode, sizeUnicode, strUTF8, sizeUTF8, nullptr, nullptr);

			view->CreateString(&value, strUTF8);

			FormHeap_Free(strUnicode);
			FormHeap_Free(strUTF8);
		}
		else
		{
			wchar_t strUnicode[bufSize];
			char	strUTF8[bufSize * 6];

			// ANSI => Unicode
			MultiByteToWideChar(codePage, 0, str, -1, strUnicode, bufSize);

			// Unicode => UTF-8
			WideCharToMultiByte(CP_UTF8, 0, strUnicode, sizeUnicode, strUTF8, sizeof(strUTF8), nullptr, nullptr);

			view->CreateString(&value, strUTF8);
		}
	}

	return;
}


void LootMenu::InvokeScaleform_Open()
{
	// fecha automaticamente quando fica vazio
	if (m_items.empty() && Settings::bCloseIfEmpty)
	{
		g_thePlayer->OnCrosshairRefChanged();
		return;
	}

	if (m_selectedIndex >= m_items.size())
		m_selectedIndex = m_items.size() - 1;
	else if (m_selectedIndex < 0)
		m_selectedIndex = 0;

	QueueUITask(&LootMenu::UI_Open);
}


void LootMenu::InvokeScaleform_Close()
{
	QueueUITask(&LootMenu::UI_Close);
}


void LootMenu::InvokeScaleform_SetIndex()
{
	QueueUITask(&LootMenu::UI_SetIndex);
}


//===================================================
// protocolo do SWF do QuickLootIE (LootMenuIE.swf)
// Tudo isto corre na thread de UI.
//===================================================

static const char kSwfRoot[] = "_root.lootMenu";

// IE: o SWF recebe um objecto de definicoes em init()
void LootMenu::UI_Init(GFxMovieView *a_view)
{
	BSSpinLockGuard guard(ms_lock);

	// diagnostico: o SWF foi carregado e tem o objecto esperado?
	GFxValue lootMenuObj;
	if (!a_view->GetVariable(&lootMenuObj, kSwfRoot) || !lootMenuObj.IsObject())
	{
		_MESSAGE("ERRO: _root.lootMenu nao existe em LootMenuIE.swf (SWF incompativel?)");
		return;
	}

	GFxValue version;
	if (a_view->Invoke("_root.lootMenu.getVersion", &version, nullptr, 0) && version.IsNumber())
		_MESSAGE("LootMenuIE.swf: nivel de funcoes = %d", (int)version.GetNumber());
	else
		_MESSAGE("AVISO: LootMenuIE.swf nao respondeu a getVersion()");

	// definicoes da janela (BetterLooting.ini)
	double scale = 1.0;
	if (Settings::iScale >= 0)
	{
		double s = Settings::iScale;
		if (s < 25) s = 25;
		else if (s > 400) s = 400;
		scale = s * 0.01;
	}

	double maxLines = Settings::iMaxLines;
	if (maxLines < 1) maxLines = 1;
	if (maxLines > 25) maxLines = 25;

	double minLines = Settings::iMinLines;
	if (minLines < 0) minLines = 0;
	if (minLines > maxLines) minLines = maxLines;

	// ancora: 0 = esquerda/cima, 1 = centro, 2 = direita/baixo
	int ax = Settings::iAnchorX; if (ax < 0) ax = 0; if (ax > 2) ax = 2;
	int ay = Settings::iAnchorY; if (ay < 0) ay = 0; if (ay > 2) ay = 2;

	double alphaNormal = Settings::iAlphaNormal;
	if (alphaNormal < 10) alphaNormal = 10; if (alphaNormal > 100) alphaNormal = 100;
	double alphaEmpty = Settings::iAlphaEmpty;
	if (alphaEmpty < 0) alphaEmpty = 0; if (alphaEmpty > 100) alphaEmpty = 100;

	GFxValue settings;
	a_view->CreateObject(&settings);
	settings.SetMember("minLines", GFxValue(minLines));
	settings.SetMember("maxLines", GFxValue(maxLines));
	settings.SetMember("offsetX", GFxValue((double)Settings::iOffsetX));
	settings.SetMember("offsetY", GFxValue((double)Settings::iOffsetY));
	settings.SetMember("scale", GFxValue(scale));
	settings.SetMember("alphaNormal", GFxValue(alphaNormal * 0.01));
	settings.SetMember("alphaEmpty", GFxValue(alphaEmpty * 0.01));
	settings.SetMember("anchorFractionX", GFxValue(ax * 0.5));
	settings.SetMember("anchorFractionY", GFxValue(ay * 0.5));

	GFxValue infoColumns;
	a_view->CreateArray(&infoColumns);
	if (Settings::bShowValue)
		infoColumns.PushBack(GFxValue("value"));
	if (Settings::bShowWeight)
		infoColumns.PushBack(GFxValue("weight"));
	if (Settings::bShowValuePerWeight)
		infoColumns.PushBack(GFxValue("valuePerWeight"));
	settings.SetMember("infoColumns", infoColumns);

	settings.SetMember("showItemIcons", GFxValue(Settings::bShowItemIcons));

	// as listas de dados da barra de botoes e da barra de info (a barra de botoes enche em UI_Open)
	GFxValue emptyButtons;
	a_view->CreateArray(&emptyButtons);
	a_view->SetVariable("_root.lootMenu.buttonBar.dataProvider", emptyButtons);

	GFxValue emptyInfo;
	a_view->CreateArray(&emptyInfo);
	a_view->SetVariable("_root.lootMenu.infoBar.dataProvider", emptyInfo);

	GFxValue autoLeft("left");
	a_view->SetVariable("_root.lootMenu.title.autoSize", autoLeft);

	a_view->Invoke("_root.lootMenu.init", nullptr, &settings, 1);

	GFxValue hidden(false);
	a_view->SetVariable("_root.lootMenu._visible", hidden);

	_MESSAGE("UI_Init concluido");
}


// IE: cada item e um objecto com displayName, count, value, weight, stolen, iconLabel, ...
void LootMenu::UI_Open(GFxMovieView *a_view)
{
	BSSpinLockGuard guard(ms_lock);

	if (!m_targetRef || !m_containerRef)
		return;

	// titulo
	GFxValue title;
	SetGFxString(a_view, title, m_targetRef->GetReferenceName());
	a_view->SetVariable("_root.lootMenu.title.text", title);

	// o peso do jogador ainda nao e mostrado na v0.1
	GFxValue noText("");
	a_view->SetVariable("_root.lootMenu.weight.text", noText);

	// itens
	GFxValue items;
	a_view->CreateArray(&items);

	for (ItemData &itemData : m_items)
	{
		GFxValue name;
		SetGFxString(a_view, name, itemData.GetName());
		GFxValue count = (double)itemData.GetCount();
		GFxValue value = (double)itemData.GetValue();
		GFxValue weight = itemData.GetWeight();
		GFxValue stolen = itemData.IsStolen();
		GFxValue iconLabel = itemData.GetIcon();
		GFxValue white = (double)0xFFFFFF;
		GFxValue processed = true;

		GFxValue item;
		a_view->CreateObject(&item);
		item.SetMember("displayName", name);
		item.SetMember("text", name);
		item.SetMember("count", count);
		item.SetMember("value", value);
		item.SetMember("weight", weight);
		item.SetMember("stolen", stolen);
		item.SetMember("isStolen", stolen);
		item.SetMember("iconLabel", iconLabel);
		item.SetMember("iconColor", white);
		item.SetMember("textColor", white);
		item.SetMember("skyui_itemDataProcessed", processed);

		TESForm *form = itemData.pEntry->baseForm;
		if (form->Is(FormType::Book))
		{
			TESObjectBOOK *book = static_cast<TESObjectBOOK*>(form);
			GFxValue isRead = book->IsRead();
			item.SetMember("read", isRead);
		}

		if (form->IsArmor() || form->IsWeapon())
		{
			GFxValue isEnchanted = itemData.IsEnchanted();
			item.SetMember("enchanted", isEnchanted);
		}

		items.PushBack(item);
	}

	a_view->SetVariable("_root.lootMenu.itemList.dataProvider", items);
	a_view->Invoke("_root.lootMenu.itemList.invalidateData", nullptr, nullptr, 0);

	// barra de botoes: o indice e o numero da tecla (o SWF desenha o icone dessa tecla)
	{
		static char *sSteal = g_gameSettingCollection->Get("sSteal")->data.s;
		static char *sTake = g_gameSettingCollection->Get("sTake")->data.s;
		static char *sSearch = g_gameSettingCollection->Get("sSearch")->data.s;
		Setting *takeAllSetting = g_gameSettingCollection->Get("sTakeAll");
		const char *sTakeAll = (takeAllSetting && takeAllSetting->data.s) ? takeAllSetting->data.s : "Take All";

		bool isStolen = m_containerRef->IsOffLimits();

		InputStringHolder *holder = InputStringHolder::GetSingleton();
		UInt32 keyTake = InputMappingManager::GetSingleton()->GetMappedKey(holder->activate, BSInputDevice::kType_Keyboard);
		if (keyTake == InputMappingManager::kInvalid)
			keyTake = 18;	// E

		GFxValue buttons;
		a_view->CreateArray(&buttons);

		const char *labels[3] = { isStolen ? sSteal : sTake, sTakeAll, sSearch };
		double keys[3] = { (double)keyTake, (double)Settings::iKeyTakeAll, (double)Settings::iKeySearch };
		for (int i = 0; i < 3; ++i)
		{
			GFxValue label;
			SetGFxString(a_view, label, labels[i]);
			GFxValue index = keys[i];
			GFxValue stolenFlag = isStolen;

			GFxValue button;
			a_view->CreateObject(&button);
			button.SetMember("label", label);
			button.SetMember("index", index);
			button.SetMember("stolen", stolenFlag);
			buttons.PushBack(button);
		}

		a_view->SetVariable("_root.lootMenu.buttonBar.dataProvider", buttons);
		a_view->Invoke("_root.lootMenu.buttonBar.invalidateData", nullptr, nullptr, 0);
	}

	a_view->Invoke("_root.lootMenu.refresh", nullptr, nullptr, 0);

	GFxValue index = (double)m_selectedIndex;
	a_view->SetVariable("_root.lootMenu.itemList.selectedIndex", index);

	GFxValue shown(true);
	a_view->SetVariable("_root.lootMenu._visible", shown);
}


void LootMenu::UI_Close(GFxMovieView *a_view)
{
	BSSpinLockGuard guard(ms_lock);

	GFxValue hidden(false);
	a_view->SetVariable("_root.lootMenu._visible", hidden);
}


void LootMenu::UI_SetIndex(GFxMovieView *a_view)
{
	BSSpinLockGuard guard(ms_lock);

	GFxValue index = (double)m_selectedIndex;
	a_view->SetVariable("_root.lootMenu.itemList.selectedIndex", index);
}


//===================================================
// dumps log for debugging purpose
//===================================================

void LootMenu::Dump()
{
	BSSpinLockGuard guard(ms_lock);

	if (!IsOpen())
		return;

	_MESSAGE("=== DUMP CONTAINER ===");
	_MESSAGE("%p [%s] numItems=%d owner=%p", m_targetRef->formID, m_targetRef->GetReferenceName(), m_items.size(), (m_owner ? m_owner->formID : 0));
	for (ItemData &itemData : m_items)
	{
		InventoryEntryData *pEntry = itemData.pEntry;
		TESForm *form = pEntry->baseForm;

		_MESSAGE("    %p [%s], count=%d, icon=%s, priority=%d isStolen=%d",
			form->formID,
			itemData.GetName(),
			itemData.GetCount(),
			itemData.GetIcon(),
			itemData.priority,
			itemData.IsStolen()
		);

		if (form->Is(FormType::Book))
		{
		}

		if (form->IsArmor() || form->IsWeapon())
		{
		}
	}

	TESContainer *container = m_containerRef->GetContainer();
	if (container)
	{
		_MESSAGE("DEFAULT ITEMS");
		TESContainer::Entry *entry;
		UInt32 lvl = 0;
		UInt32 idx = 0;
		while (container->GetContainerItemAt(idx++, entry))
		{
			if (!entry || !entry->form)
			{
				_MESSAGE("    INVALID ENTRY");
				continue;
			}

			TESForm *item = entry->form;

			if (item->Is(FormType::LeveledItem))
			{
				_MESSAGE("    %08X LeveledItem %d", item->formID, lvl);
				++lvl;
			}
			else
			{
				bool bPlayable = item->IsPlayable();
				TESFullName *name = DYNAMIC_CAST<TESFullName*>(item);
				_MESSAGE("    %08X [%s] count=%d playable=%d", item->formID, name->GetFullName(), entry->count, bPlayable);
			}
		}
	}

	if (m_containerRef->extraData.HasType<ExtraContainerChanges>())
	{
		_MESSAGE("CONTAINER CHANGES");

		ExtraContainerChanges *exChanges = m_containerRef->extraData.GetByType<ExtraContainerChanges>();
		InventoryChanges *changes = exChanges ? exChanges->changes : nullptr;

		if (changes && changes->entryList)
		{
			for (InventoryEntryData *pEntry : *changes->entryList)
			{
				if (!pEntry || !pEntry->baseForm)
				{
					_MESSAGE("    INVALID ENTRY");
					continue;
				}

				TESForm *item = pEntry->baseForm;

				bool bPlayable = item->IsPlayable();
				TESFullName *name = DYNAMIC_CAST<TESFullName*>(item);
				_MESSAGE("    %08X [%s] %p count=%d playable=%d", item->formID, name->GetFullName(), pEntry, pEntry->countDelta, bPlayable);

				if (!pEntry->extraList)
					continue;

				for (BaseExtraList *extraList : *pEntry->extraList)
				{
					if (!extraList)
					{
						_MESSAGE("      INVALID ENTRY");
						continue;
					}

					_MESSAGE("      extraList - %p", extraList);
					for (BSExtraData *extraData : *extraList)
					{
						const char *className = GetObjectClassName(extraData);
						if (extraData->GetType() == ExtraCount::kExtraTypeID)
							_MESSAGE("        %02X %s (%d)", extraData->GetType(), className, ((ExtraCount*)extraData)->count);
						else if (extraData->GetType() == ExtraLeveledItem::kExtraTypeID)
							_MESSAGE("        %02X %s (%d)", extraData->GetType(), className, ((ExtraLeveledItem*)extraData)->index);
						else
							_MESSAGE("        %02X %s", extraData->GetType(), className);
					}
				}
			}
		}
	}

	_MESSAGE("EXTRA DATA");
	for (BSExtraData *extraData : m_containerRef->extraData)
	{
		if (!extraData)
		{
			_MESSAGE("    INVALID ENTRY");
			continue;
		}

		UInt32 type = extraData->GetType();
		_MESSAGE("    %02X %s", type, GetObjectClassName(extraData));
		if (type == ExtraAshPileRef::kExtraTypeID)
		{
			ExtraAshPileRef *exAshPileRef = static_cast<ExtraAshPileRef *>(extraData);
			RefHandle handle = exAshPileRef->refHandle;
			if (handle && handle != g_invalidRefHandle)
			{
				TESObjectREFRPtr refPtr;
				if (TESObjectREFR::LookupByHandle(handle, refPtr))
				{
					_MESSAGE("      %p [%s]", refPtr->formID, refPtr->GetReferenceName());
				}
			}
		}
		else if (type == ExtraOwnership::kExtraTypeID)
		{
			ExtraOwnership *exOwnership = static_cast<ExtraOwnership *>(extraData);
			if (exOwnership && exOwnership->owner)
			{
				TESFullName *fullName = DYNAMIC_CAST<TESFullName *>(exOwnership->owner);
				_MESSAGE("      %p [%s]", exOwnership->owner->formID, fullName->GetFullName());
			}
		}
	}

	_MESSAGE("======================");
}


//===================================================
// misc
//===================================================

void LootMenu::SendItemsPickpocketedEvent(UInt32 numItems)
{
	typedef void(*FnSendItemsPickpocketedEvent)(UInt32);
	const FnSendItemsPickpocketedEvent fnSendItemsPickpocketedEvent = (FnSendItemsPickpocketedEvent)0x0084A650;

	(*fnSendItemsPickpocketedEvent)(numItems);
}

void LootMenu::SendChestLootedEvent()
{
	typedef void(*FnSendChestLootedEvent)();
	const FnSendChestLootedEvent fnSendChestLootedEvent = (FnSendChestLootedEvent)0x0084A580;

	(*fnSendChestLootedEvent)();
}

void LootMenu::PlaySound(TESForm *item)
{
	typedef bool(*FnPlaySound)(BGSSoundDescriptorForm *, UInt32 flag, const NiPoint3 *, NiNode *);
	const FnPlaySound fnPlaySound = (FnPlaySound)0x00653A60;

	BGSPickupPutdownSounds *sounds = DYNAMIC_CAST<BGSPickupPutdownSounds*>(item);
	if (sounds && sounds->pickUp)
		fnPlaySound(sounds->pickUp, false, &m_containerRef->pos, g_thePlayer->GetNiNode());
}


void LootMenu::PlayAnimation(const char *fromName, const char *toName)
{
	NiNode *niNode = m_containerRef->GetNiNode();
	if (!niNode)
		return;
	NiTimeController *controller = niNode->GetController();
	if (!controller)
		return;
	NiControllerManager* manager = NiDynamicCast<NiControllerManager*>(controller);
	if (!manager)
		return;
	NiControllerSequence *fromSeq = manager->GetSequenceByName(fromName);
	NiControllerSequence *toSeq = manager->GetSequenceByName(toName);
	if (!fromSeq || !toSeq)
		return;

	typedef void(*Fn)(TESObjectREFR *, NiControllerManager *, NiControllerSequence *, NiControllerSequence *, bool);
	const Fn fn = (Fn)0x0044B8C0;

	fn(m_containerRef, manager, toSeq, fromSeq, false);
}

void LootMenu::PlayAnimationOpen()
{
	if (m_containerRef && !m_bOpenAnim)
	{
		PlayAnimation("Close", "Open");
		if (m_containerRef->IsNot(FormType::Character))
			m_containerRef->ActivateRefChildren(g_thePlayer);
	}
	m_bOpenAnim = true;
}

void LootMenu::PlayAnimationClose()
{
	if (m_containerRef && m_bOpenAnim)
		PlayAnimation("Open", "Close");
	m_bOpenAnim = false;
}
