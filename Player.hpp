#pragma once  

#include "includes.hpp"
#include "Classes.hpp"
#include "native.hpp"
namespace Players
{

	void Hook(void)
	{
		if (Settings::Player::enable)
		{
	

		hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
		if (!World)
			return;

		hk_Ped* LocalPlayer = World->LocalPlayer();
		if (!LocalPlayer)
			return;
		hk_PlayerInfo* PlayerInfo = LocalPlayer->PlayerInfo();
		if (!PlayerInfo)
			return;
		hk_ObjectNavigationPed* Nav = LocalPlayer->ObjectNavigation();
		if (!LocalPlayer)
			return;



			if (Settings::Player::Semi_Godmod)
			{
				if (LocalPlayer->GetHealth() < 200)
					LocalPlayer->SetMaxHealth();

			}
			if (Settings::Player::Infinite_Armor)
			{
				LocalPlayer->SetArmor(Settings::Player::ArmorToSet);

			}

			if (!Settings::misc::untrusted_build)
			{
				auto player = PLAYER::PLAYER_PED_ID();

			if (Settings::Player::Infinite_stamina) {

				PLAYER::RESET_PLAYER_STAMINA(player);

			}

			if (Settings::Player::AntiHS)
			{

				PED::SET_PED_SUFFERS_CRITICAL_HITS(player, false);

			}

		

			

			if (Settings::Player::TpToWaypoint) {

				int WaypointHandle = HUD::GET_FIRST_BLIP_INFO_ID(8);
				if (HUD::DOES_BLIP_EXIST(WaypointHandle))
				{
					PVector3 waypoint1 = HUD::GET_BLIP_COORDS(WaypointHandle);

					PED::SET_PED_COORDS_KEEP_VEHICLE(player, waypoint1.x, waypoint1.y, waypoint1.z);

				}

				Settings::Player::TpToWaypoint = false;
			}



				if (Settings::Player::Invisible)
				{
					ENTITY::SET_ENTITY_VISIBLE(player, false, 0);

					LocalPlayer->SetInvisible(0X1);


				}
				else {

					LocalPlayer->SetInvisible(0X37);

				}

				if (Settings::Player::Godmod)
				{
					LocalPlayer->SetGoodmode(true);
					ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), true);
				}
			}


		
	

				if (Settings::Player::SetArmor)
				{
					LocalPlayer->SetArmor(Settings::Player::ArmorToSet);
					Settings::Player::SetArmor = false;
				}

			

			if (SAFE_CALL(GetAsyncKeyState)(Settings::Player::HotkeyArmor) & 0x8000) {


	
					LocalPlayer->SetArmor(Settings::Player::ArmorToSet);

		
			}

			if (SAFE_CALL(GetAsyncKeyState)(Settings::Player::HotkeyHealth) & 0x8000) {


				LocalPlayer->SetHealth(100 + Settings::Player::HealthToSet);

			}

			if (Settings::Player::SetHealth)
			{
				LocalPlayer->SetHealth(100 + Settings::Player::HealthToSet);

				Settings::Player::SetHealth = false;
			}

			if (Settings::Player::NoRagDoll)
			{
				LocalPlayer->Set_Ragdoll(true);
			}
			


			if (Settings::Player::fastrun)
			{
				PlayerInfo->SetSpeed(5);

			}
			
		
			if (Settings::Player::EnableFovChange)
			{
				DWORD64 addr = FiveM::GetCamera();
				uintptr_t CameraData = *(uintptr_t*)(addr + 0x10);
				*(float*)(CameraData + 0x30) = Settings::Player::FovValue;
				Settings::Player::EnableFovChange = false;
				//	std::cout << *(float*)(CameraData + 0x30);

				//	FiveM::SetFOVZoom(Settings::Player::FovValue);
			}
		}
	





		/*void OnlinePlayerHook()
		{
			ImGui::SetNextWindowSize(ImVec2(600, 300));

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
			ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
			ImGui::SetNextWindowBgAlpha(1.0f);
			if (ImGui::Begin("Player MENU", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
			{
				hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
				if (!World)
					return;

				hk_Ped* LocalPlayer = World->LocalPlayer();
				if (!LocalPlayer)
					return;

				hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(FiveM::ReplayInterface);
				if (!ReplayInterface)
					return;

				hk_PedInterface* PedInterface = ReplayInterface->PedInterface();
				if (!PedInterface)
					return;

				for (int index = 0; index < PedInterface->PedMaximum(); index++)
				{
					hk_Ped* Peds = PedInterface->PedList()->Ped(index);
					if (!Peds) continue;

					bool IsPed = false;

					auto ped_type = Peds->GetPedType();
					if (!ped_type)
					{
						continue;
					}
					ped_type = ped_type << 11 >> 25;

					if (ped_type != 2) {
						IsPed = true;

					}

					if (IsPed)
					{
						continue;
					}


					ImVec2 Ped_Location = FiveM::WorldToScreen(Peds->GetCoordinate());
					if (!IsOnScreen(Ped_Location)) continue;

					ImVec2 LocalPlayer_Location = FiveM::WorldToScreen(LocalPlayer->GetCoordinate());
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
					ImGui::SetCursorPosX( 10);
					const char* PlayerID = "Player ID : " + index;

					ImGui::Text(PlayerID);


				}

				ImGui::PopStyleVar(4);
				ImGui::End();
			}
		}
		*/

	}
}