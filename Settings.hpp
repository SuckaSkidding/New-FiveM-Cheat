#pragma once
#include "editor.h"
namespace Settings
{


	inline ImVec2 WindowsPosActual = ImVec2(0, 0);
	namespace Player
	{


		inline bool enable = false;

		inline bool Godmod = false;
		inline bool Semi_Godmod = false;
		inline bool Infinite_Armor = false;
		inline bool SetHealth = false;
		inline bool fastrun = false;
		inline bool SetArmor = false; 
		inline int RunSpeedToSet = 1;

		inline int HealthToSet = 100;
		inline int ArmorToSet = 100;
		inline bool NoClip = false; 
		inline int noclipspeed = false;
		inline int isNoclipWorking = true;
		inline bool Invisible = false;
		inline bool AntiHS = false;
		inline bool Infinite_stamina = false;
		inline int HotkeyArmor = 0;
		inline int HotkeyHealth = 0;
		inline bool flymode = false;
		inline int ForwardHotkey = 90;
		inline int BackwardHotkey = 0x53;

		inline bool EnableFovChange = false;
		inline bool SuperJump = false;
		inline bool TpToWaypoint = false;
		inline bool NoRagDoll = false;
		inline float FovValue = 50;
		inline bool NoClipSpeed_bool = false;
		inline int Speed = 0;
		inline bool Gravity = false;
		inline bool AirStuck = false;
		inline int Airstuck_hotkey = 0;
		inline bool godnoclip = false;

		

	}
	namespace Weapon
	{
		inline bool enable = false;


		static int weapon_current = 0;
		inline const char* weapon_list[]{ "Dagger", "Bat", "Knife", "Machete", "Pistol", "Pistol MK2", "Combat Pistol", "AP Pistol", "Stungun", "Pistol 50", "SNS PISTOL", "SNS Pistol MK2", "Heavy Pistol", "Vintage Pisol", "Flare Gun", "Marksman Pistol", "Revolver", "Revolver MK2", "Double Action", "Micro Smg", "Smg", "Smg MK2", "Assault Smg", "Combat PDW", "Machine Pistol", "Mini Smg", "Pump Shotgun", "Pump Shotgun MK2", "Sawnoff Shotgun", "Assault Shotgun", "Bullpup Shotgun", "Musket", "Heavy Shotgun", "DB Shotgun", "Auto Shotgun", "Assault Rifle", "Assault Rifle MK2", "Carbine Rifle", "Carbine Rifle MK2", "Advanced Rifle", "Special Carbine", "Special Carbine MK2", "Bullpup Rifle", "Bullpup Rifle MK2", "Compact Rifle", "Machine Gun", "Combat MG", "Combat MG MK2", "GUSENBERG", "Sniper Rifle", "AWP", "AWP MK2", "Marksman Rifle", "Marksman Rifle MK2", "RPG", "Grenade Launcher", "MiniGun", "FireWork", "RailGun", "Homing Launcher", "Compact Launcher", "Grenade", "BZGAS", "Smoke Grenade", "Flare", "Molotov", "Sticky BOMB", "Prox Mine", "SnowBall", "Pipe Bomb", "Ball", "Petrol Can", "Fire Extinguisher", "Parachute" };
		inline bool No_Recoil = false;
		inline bool No_Spread = false;
		inline bool InfiniteAmmo = false;
		inline bool NoRangeLimit = false;
		inline bool SetAmmo = false;
		inline bool ExplosiveAmmo = false;
		inline bool InstantHitAmmo = false;
		inline bool rapidfire = false;
		inline bool NoReload = false;
		inline bool FastNoReload = false;

		inline int AmmoToSet = 30;
		inline bool GiveWeapon = false;
		inline bool GiveAllWeapon = false;
		inline int WeaponType = 10;
		inline bool DamageBoost = false;
		inline int DamageBoost_value = 0;
		inline bool ReloadV2 = false;
		inline int ReloadSpeed = 0;
		inline int RecoilManipulation = 0;

	}

	namespace Aimbot
	{
		inline bool Aimbot = false;
		inline bool Prediction = false;
		inline bool VisibleCheck = false;
		inline bool TriggerBot = false;
		inline int AimbotPrediction = 1;
		inline int AimbotTick = 16;

		inline bool Draw_Fov = false;
		inline bool PreviewTarget = false;
		inline int AimbotFov = 30;
		inline int AimbotSmooth = 1;
		inline int TypeOfTarget = 0;
		inline const char *AimbotTarget[]{ "All", "Only Player", "Only Ped" };
		inline int DistanceAimbotLimit = 150;
		inline int AimbotBone = 0;
		inline const char* AimbotBones[]{ "Head", "Body", "Leg left", "Leg right", "Arm right", "Arm left" };
		inline int AimbotTypes = 0;
		inline const char* AimbotType[]{ "Mouse aim", "Memory aim" ,"Magic Bullet (not stable)" };
		inline bool adaptive_weapon = false;
		inline bool cacafov = false;
		inline bool cacasmooth = false;
		inline bool cacadistance = false;
		inline bool AutoShoot = false;
		inline bool MagicLikeEAGLE = false;
		inline bool SilentLikeEAGLE = false;


		namespace AdaptiveSniper
		{

			inline bool Enable_Sniper;
			inline int Sniper_AimbotFov = 4;
			inline int Sniper_AimbotSmooth = 1;
			inline int Sniper_TypeOfTarget = 0;
			inline int Sniper_AimbotBone = 0;
			inline int Sniper_Distance = 150;
			inline int Sniper_AimbotTypes = 0;
			inline const char* Sniper_AimbotType[]{ "Mouse aim", "Memory aim" ,"Magic Bullet (not stable)" };
			inline const char* Sniper_AimbotTarget[]{ "All", "Only Player", "Only Ped" };
			inline const char* Sniper_AimbotBones[]{ "Head", "Body", "Leg left", "Leg right", "Arm right", "Arm left" };

			inline bool caca_fov_Sniper = false;
			inline bool caca_distance_Sniper = false;
			inline bool caca_smooth_Sniper = false;


		}

		namespace AdaptiveShotgun
		{

			inline bool Enable_Shotgun;
			inline int Shotgun_AimbotFov = 4;
			inline int Shotgun_AimbotSmooth = 1;
			inline int Shotgun_TypeOfTarget = 0;
			inline int Shotgun_AimbotBone = 0;
			inline int Shotgun_Distance = 150;
			inline int Shotgun_AimbotTypes = 0;
			inline const char* Shotgun_AimbotType[]{ "Mouse aim", "Memory aim" ,"Magic Bullet (not stable)"};
			inline const char* Shotgun_AimbotTarget[]{ "All", "Only Player", "Only Ped" };
			inline const char* Shotgun_AimbotBones[]{ "Head", "Body", "Leg left", "Leg right", "Arm right", "Arm left" };

			inline bool caca_fov_Shotgun = false;
			inline bool caca_distance_Shotgun = false;
			inline bool caca_smooth_Shotgun = false;

		}

		namespace AdaptivePistol
		{

			inline bool Enable_Pistol;
			inline int Pistol_AimbotFov = 4;
			inline int Pistol_AimbotSmooth = 1;
			inline int Pistol_TypeOfTarget = 0;
			inline int Pistol_AimbotBone = 0;
			inline int Pistol_Distance = 150;
			inline int Pistol_AimbotTypes = 0;
			inline const char* Pistol_AimbotType[]{ "Mouse aim", "Memory aim" ,"Magic Bullet (not stable)"};
			inline const char* Pistol_AimbotTarget[]{ "All", "Only Player", "Only Ped" };
			inline const char* Pistol_AimbotBones[]{ "Head", "Body", "Leg left", "Leg right", "Arm right", "Arm left" };

			inline bool caca_fov_Pistol = false;
			inline bool caca_distance_Pistol = false;
			inline bool caca_smooth_Pistol = false;

		}

		namespace AdaptiveSMG
		{

			inline bool Enable_SMG;
			inline int SMG_AimbotFov = 4;
			inline int SMG_AimbotSmooth = 1;
			inline int SMG_TypeOfTarget = 0;
			inline int SMG_AimbotBone = 0;
			inline int SMG_Distance = 150;
			inline int SMG_AimbotTypes = 0;
			inline const char* SMG_AimbotType[]{ "Mouse aim", "Memory aim" ,"Magic Bullet (not stable)" };
			inline const char* SMG_AimbotTarget[]{ "All", "Only Player", "Only Ped" };
			inline const char* SMG_AimbotBones[]{ "Head", "Body", "Leg left", "Leg right", "Arm right", "Arm left" };

			inline bool caca_fov_SMG = false;
			inline bool caca_distance_SMG = false;
			inline bool caca_smooth_SMG = false;

		}

		namespace AdaptiveLight
		{

			inline bool Enable_Light;
			inline int Light_AimbotFov = 4;
			inline int Light_AimbotSmooth = 1;
			inline int Light_TypeOfTarget = 0;
			inline int Light_AimbotBone = 0;
			inline int Light_Distance = 150;
			inline int Light_AimbotTypes = 0;
			inline const char* Light_AimbotType[]{ "Mouse aim", "Memory aim" ,"Magic Bullet (not stable)" };
			inline const char* Light_AimbotTarget[]{ "All", "Only Player", "Only Ped" };
			inline const char* Light_AimbotBones[]{ "Head", "Body", "Leg left", "Leg right", "Arm right", "Arm left" };

			inline bool caca_fov_Light = false;
			inline bool caca_distance_Light = false;
			inline bool caca_smooth_Light = false;

		}


		namespace AdaptiveRifle
		{

			inline bool Enable_Rifle;
			inline int Rifle_AimbotFov = 4;
			inline int Rifle_AimbotSmooth = 1;
			inline int Rifle_TypeOfTarget = 0;
			inline int Rifle_AimbotBone = 0;
			inline int Rifle_Distance = 150;
			inline int Rifle_AimbotTypes = 0;
			inline const char* Rifle_AimbotType[]{ "Mouse aim", "Memory aim" , "Magic Bullet (not stable) "};
			inline const char* Rifle_AimbotTarget[]{ "All", "Only Player", "Only Ped" };
			inline const char* Rifle_AimbotBones[]{ "Head", "Body", "Leg left", "Leg right", "Arm right", "Arm left" };

			inline bool caca_fov_Rifle = false;
			inline bool caca_distance_Rifle = false;
			inline bool caca_smooth_Rifle = false;

		}

		inline int Hotkey = 2;
		inline bool rapidfire = false;
		inline bool crosshair = false;
		inline bool silentaim = false;

		inline bool magic = false;
		inline bool aimmousewhilesilent = false;

	}

	namespace Visuals
	{

		inline bool enable = true;
		inline bool Box = false;
		inline bool Filled = false;
		inline bool Corner = false; 
		inline bool TroiDBox = false;
		inline bool Skeleton = false;
		inline bool Snapline = false;
		inline bool flags = false;
		inline bool HealthBar = false;
		inline bool ArmorBar = false;
		inline bool Weapon = false;
		inline bool DistanceESP = false;
		inline bool NameEsp = false;
		inline bool PreviewEsp = false;
		inline bool tpped = false;
		inline bool invisibleped = false; 
		inline bool killped = false;
		inline bool PlayerName = false;
		inline bool VisibleCheck = false;
		inline float CornerSize = 1;
		inline float BoxSize = 1;

		inline int Skeleton_idx = 0;
		inline int HealthBar_idx = 0;
		inline bool HealthBar_Multicolor = false;

		inline const char* HealthbarStyle[]{ "Right","Left","Down","Up"};

		inline bool Skeleton_Detailled = false;
		inline bool Keybinds = false;
		inline bool Watermark = false;

		inline bool Draw_LocalPlayer = false;
		inline bool Draw_Dead = false;
		inline bool Draw_Ped = false; 
		inline bool Draw_Animals = false;
		inline bool StaticESP = false;

		inline int Distance = 150;
		inline int FillBoxAlpha = 35;

		inline ImColor FovColor = ImColor(255, 255, 255, 255);
		inline ImColor BoxColor = ImColor(255, 255, 255, 255);
		inline ImColor BoxColorVisible = ImColor(252, 3, 23, 255);

		inline ImColor CornerColor = ImColor(33, 140, 176, 255);
		inline ImColor CornerColorVisible = ImColor(252, 3, 23, 255);

		inline ImColor SkelColor = ImColor(255, 255, 255, 255);

		inline ImColor SkelColorVisible = ImColor(252, 3, 23, 255);

		inline ImColor MenuColor = ImColor(255, 255, 255, 255);
	}

	namespace vehicle
	{
		inline bool enable = false;
		inline bool stickycar = false;
		inline bool godmode = false;
		inline bool fastspeed = false; 
		inline bool nogravity = false; 
		inline bool setmaxhealth = false;
		inline int Speed = 10; 
		inline bool unlockcar = false;
		inline bool autostart_vehicle = false;
		inline bool warp_vehicle = false;
		inline bool repaire = false;
		inline bool shift_boost = false;
		inline bool vehicle_speed_bool = false;
		inline int vehicle_speed_value = 0;
		inline bool max_current_vehicle = false;
		inline bool on_car = false;
		inline bool NameESP = false;


		inline int mod_idx = 0;
		inline int primary_color = 0;
		inline int secondary_color = 0;
		inline int add_idx = 0;
		inline bool autodrive = false;

		inline bool spawn_vehicle = false;
		inline char VehicleHashString[15] = "";
		inline char plate_text[8] = "";
		inline bool update_colors = false;
		inline bool update_plate = false;


	}

	namespace misc
	{
		inline bool menu = true;
		inline bool customization = false; 
		inline bool informations = false;
		inline bool LuaExecutor = false;
		inline TextEditor editor;
		inline bool isPanic = false;
		inline int Menu_Hotkey = 45;
		inline bool PlayerList = false;
		inline bool PedList = false;
		inline bool VehicleList = false;
		inline bool untrusted_build = true;
		inline int NoclipKey = 0;
		inline bool security_1 = false;
		inline bool security_2 = false;
		inline char cfg_name[10] = "";
		inline bool EnableChat = false;

	}

	namespace TempColor
	{
		inline float Box[4] = { 255.f, 255.f, 255.f, 255.f };
		inline float BoxVisible[4] = { 255.f, 255.f, 255.f, 255.f };

		inline float Corner[4] = { 33.f, 140.f, 176.f, 255.f };
		inline float CornerVisible[4] = { 33.f, 140.f, 176.f, 255.f };

		inline float Fov[4] = { 255.f, 255.f, 255.f, 255.f };
		inline float Skeleton[4] = { 255.f, 255.f, 255.f, 255.f };
		inline float SkeletonVisible[4] = { 255.f, 255.f, 255.f, 255.f };

		inline float Menu[4] = { 255.f, 255.f, 255.f, 255.f };


	}
}