#include "Core.hpp"

#include "Classes.hpp"
#include <native.hpp>
#include "func.hpp"
#include <process.h>
bool executedGiveWeaponToPed = false;
#pragma warning(disable : 6031)


BOOLEAN APIENTRY DllMain(HINSTANCE hk_dll, DWORD hk_reason, LPVOID hk_lpReserved)
{
	UNREFERENCED_PARAMETER(hk_lpReserved);

	if (hk_reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hk_dll);
		if (DEBUG == 1)
		{

				typedef BOOL(__stdcall* AllocConsole_t)();
				AllocConsole_t AllocConsole_o = (AllocConsole_t)SAFE_CALL(GetProcAddress)(SAFE_CALL(GetModuleHandleA)((E("kernel32.dll"))), E("AllocConsole"));

				AllocConsole_o();
				freopen_s((FILE**)stdout, E("CONOUT$"), E("w"), stdout);
		}




		FiveM::World = Memory::PatternScan(E("48 8B 05 ? ? ? ? 48 8B 48 08 48 85 C9 74 52 8B 81"), NULL, 7);
		FiveM::ReplayInterface = Memory::PatternScan(E("48 8D 0D ? ? ? ? 89 44 24 30 E8 ? ? ? ? 48 83 C4 28 C3 48 8B 05"), NULL, 7);
		FiveM::PlayerInterface = Memory::PatternScan(E("48 8B 0D ? ? ? ? E8 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 8B CF"), NULL, 7);

		FiveM::W2S = Memory::PatternScan(E("48 89 5C 24 ?? 55 56 57 48 83 EC 70 65 4C 8B 0C 25"), NULL, NULL);

		FiveM::BonePos = Memory::PatternScan(E("48 89 5C 24 ?? 48 89 6C 24 ?? 48 89 74 24 ?? 57 48 83 EC 60 48 8B 01 41 8B E8 48 8B F2 48 8B F9 33 DB"), NULL, NULL);
		FiveM::Camera = Memory::PatternScan(E("48 8B 05 ? ? ? ? 48 8B 98 ? ? ? ? EB"), NULL, 7);
		FiveM::Waypoint = Memory::PatternScan(E("4C 8D 05 ? ? ? ? 0F B7 C1"), NULL, 7);
		FiveM::pointer_to_handle = reinterpret_cast<FiveM::pizza_to_spaghetti_t>(Memory::PatternScan(E("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 8B 15 ? ? ? ? 48 8B F9 48 83 C1 10 33 DB"), NULL, NULL));
		FiveM::raycast_result_func = reinterpret_cast<FiveM::tGET_RAYCAST_RESULT>(Memory::PatternScan(E("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 8B 15 ? ? ? ? 48 8B F9 48 83 C1 10 33 DB"), NULL, NULL));

     //   FiveM::IsOnFiveM = (bool)Memory::PatternScan(E("48 89 5C 24 ?? 55 56 57 48 83 EC 70 65 4C 8B 0C 25"), NULL, NULL);

		if (SAFE_CALL(GetModuleHandleA)("FiveM_GameProcess.exe") || SAFE_CALL(GetModuleHandleA)("FiveM_GTAProcess.exe"))
		{
			FiveM::EntityType = 0x10A8;
			FiveM::Armor = 0x14B8;

			FiveM::WeaponManager = 0x10C8;
			FiveM::PlayerInfo = 0x10B8;
			FiveM::PlayerID = 0x68;

			FiveM::Recoil = 0x2E8;
			FiveM::Spread = 0x74;
			FiveM::ReloadMultiplier = 0x12C;
			FiveM::AmmoType = 0x20;
			FiveM::AmmoExplosiveType = 0x24;
			FiveM::WeaponName = 0x5E0;
			FiveM::IsInAVehicule = 0x146B;
			FiveM::Range = 0x25C;


		}
		if (SAFE_CALL(GetModuleHandleA)("FiveM_b2060_GameProcess.exe") || SAFE_CALL(GetModuleHandleA)("FiveM_b2060_GTAProcess.exe"))
		{
			FiveM::EntityType = 0x10B8;
			FiveM::Armor = 0x14E0;

			FiveM::WeaponManager = 0x10D8;
			FiveM::PlayerInfo = 0x10B8;
			FiveM::PlayerID = 0x88;

			FiveM::Recoil = 0x2F4;
			FiveM::Spread = 0x84;
			FiveM::ReloadMultiplier = 0x134;
			FiveM::AmmoType = 0x20;
			FiveM::AmmoExplosiveType = 0x24;

			FiveM::WeaponName = 0x5F0;
			FiveM::IsInAVehicule = 0x146B;
			FiveM::Range = 0x28C;

		}
		if (SAFE_CALL(GetModuleHandleA)("FiveM_b2545_GameProcess.exe") || SAFE_CALL(GetModuleHandleA)("FiveM_b2545_GTAProcess.exe"))
		{
			FiveM::EntityType = 0x10B8;
			FiveM::Armor = 0x14E0 + 0x50;

			FiveM::WeaponManager = 0x10D8;
			FiveM::PlayerID = 0x88;

			FiveM::PlayerInfo = 0x10C8;
			FiveM::Recoil = 0x2F4;
			FiveM::Spread = 0x84;
			FiveM::ReloadMultiplier = 0x134;
			FiveM::AmmoType = 0x20;
			FiveM::AmmoExplosiveType = 0x24;
			FiveM::WeaponName = 0x5F0;
			FiveM::IsInAVehicule = 0x146B;
			FiveM::Range = 0x28C;


		}
		if (SAFE_CALL(GetModuleHandleA)("FiveM_b2612_GameProcess.exe") || SAFE_CALL(GetModuleHandleA)("FiveM_b2612_GTAProcess.exe") || SAFE_CALL(GetModuleHandleA)("FiveM_b2699_GTAProcess.exe") || SAFE_CALL(GetModuleHandleA)("FiveM_b2699_GameProcess.exe"))
		{
			FiveM::EntityType = 0x10B8;
			FiveM::Armor = 0x1530;

			FiveM::WeaponManager = 0x10D8;
			FiveM::PlayerInfo = 0x10C8;
			FiveM::Recoil = 0x2F4;
			FiveM::PlayerID = 0x88;

			FiveM::Spread = 0x84;
			FiveM::ReloadMultiplier = 0x134;
			FiveM::AmmoType = 0x20;
			FiveM::AmmoExplosiveType = 0x24;
			FiveM::WeaponName = 0x5F0;
			FiveM::IsInAVehicule = 0x146B;
			FiveM::Range = 0x28C;


		}
		if (SAFE_CALL(GetModuleHandleA)("FiveM_b2189_GameProcess.exe") || SAFE_CALL(GetModuleHandleA)("FiveM_b2189_GTAProcess.exe"))
		{
			FiveM::EntityType = 0x10B8;
			FiveM::Armor = 0x14E0;

			FiveM::WeaponManager = 0x10D8;
			FiveM::PlayerInfo = 0x10C8;
			FiveM::PlayerID = 0x68;

			FiveM::Recoil = 0x2F4;
			FiveM::Spread = 0x84;
			FiveM::ReloadMultiplier = 0x134;
			FiveM::AmmoType = 0x20;
			FiveM::AmmoExplosiveType = 0x24;
			FiveM::WeaponName = 0x5F0;
			FiveM::IsInAVehicule = 0x146B;
			FiveM::Range = 0x28C;


		}
		if (SAFE_CALL(GetModuleHandleA)("FiveM_b2372_GameProcess.exe") || SAFE_CALL(GetModuleHandleA)("FiveM_b2372_GTAProcess.exe"))
		{
			FiveM::EntityType = 0x10B8;
			FiveM::Armor = 0x14E0;

			FiveM::WeaponManager = 0x10D8;
			FiveM::PlayerInfo = 0x10C8;
			FiveM::Recoil = 0x2F4;
			FiveM::PlayerID = 0x88;

			FiveM::Spread = 0x84;
			FiveM::ReloadMultiplier = 0x134;
			FiveM::AmmoType = 0x20;
			FiveM::AmmoExplosiveType = 0x24;
			FiveM::WeaponName = 0x5F0;
			FiveM::IsInAVehicule = 0x146B;
			FiveM::Range = 0x28C;
		}
       

		//std::cout << "\n Game successfully inited !" << std::endl;

		SAFE_CALL(_beginthreadex)(0, 0, (_beginthreadex_proc_type)Core::Init, 0, 0, 0);

	}

	return TRUE;
}