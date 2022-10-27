#pragma once

#include "includes.hpp"
#include "Classes.hpp"
#include <sstream>

#include "native.hpp"
#include "func.hpp"
#include <fa_solid_900.h>
std::string GetPlayerNameNew(DWORD64 pPed) {
	if (Settings::misc::untrusted_build)
	{
		return "Safe Mode";
	}
	auto handle = FiveM::pointer_to_handle(pPed);
	if (handle > 1) {
		int NetworkGetPlayerIndex = NETWORK::NETWORK_GET_PLAYER_INDEX_FROM_PED(handle);
		if (NetworkGetPlayerIndex > 1)
			return std::string(Get_player_name(NetworkGetPlayerIndex));
	}
	return "**Invalid**";
}

bool IsVisible(DWORD64 pPed, DWORD64 Peddd) {
	auto handle = FiveM::pointer_to_handle(pPed);
	auto handle1 = FiveM::pointer_to_handle(Peddd);
	if (Settings::misc::untrusted_build)
	{
		return true;
	}
	if (ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY(handle1, handle, 17)) {

		return true;
	}
	return false;
}


auto DrawOutlinedText(ImFont* font, const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center) -> VOID
{
	std::stringstream stream(text);
	std::string line;

	float y = 0.0f;
	int index = 0;

	while (std::getline(stream, line))
	{
		ImVec2 textSize = font->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

		if (center)
		{
			ImGui::GetBackgroundDrawList()->AddText(font, size+ 1 , ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * index)), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetBackgroundDrawList()->AddText(font, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * index), ImGui::GetColorU32(color), line.c_str());
		}
		else
		{
			ImGui::GetBackgroundDrawList()->AddText(font, size + 1, ImVec2((pos.x) + 1, (pos.y + textSize.y * index)), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetBackgroundDrawList()->AddText(font, size, ImVec2(pos.x, pos.y + textSize.y * index), ImGui::GetColorU32(color), line.c_str());
		}

		y = pos.y + textSize.y * (index + 1);
		index++;
	}
}
PVector3 TotPVector3(Vector3 pVec)
{
	return PVector3(pVec.x, pVec.y, pVec.z);
}

Vector3 TotVector3(PVector3 pVec)
{
	return Vector3(pVec.x, pVec.y, pVec.z);
}
static hk_Ped* selected = 0;
static int selected_index = 1;
static int selectedVeh_index = 1;

namespace Visuals
{

	void HookkVehicle(void)
	{


		if (Settings::misc::VehicleList)
		{

			ImGui::SetNextWindowSize(ImVec2(600, 500));

		

				if (!ImGui::Begin("Albania - Vehicles List", &Settings::misc::VehicleList, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
				{



				}
				else
				{
					hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
					if (!World)
						return;
					hk_Ped* LocalPlayer = World->LocalPlayer();
				
					if (LocalPlayer)
					{

			


						hk_ObjectNavigationPed* Nav = LocalPlayer->ObjectNavigation();
						if (!Nav)
							return;

						hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(FiveM::ReplayInterface);
						if (!ReplayInterface)
							return;

						hk_VehicleInterface* VehicleInterface = ReplayInterface->VehicleInterface();
						if (!VehicleInterface)
							return;






						if (ImGui::ListBoxHeader("    ", ImVec2(200, 400)))
						{
							for (int i = 0; i < VehicleInterface->VehicleMaximum(); i++) {

								hk_Vehicle* Peds = VehicleInterface->VehicleList()->Vehicle(i);
								if (!Peds) continue;
								auto playerName = Peds->VehicleModelInfo()->GetCharName() ;
								const char* c = playerName;
								const char* items[] = { c };
				
								std::string VehicleName = playerName;
								VehicleName.append(" ##");
								VehicleName.append(std::to_string(i));
								bool is_selected = (selectedVeh_index == i);
								if (ImGui::Selectable(VehicleName.c_str(), is_selected))
								{

									selectedVeh_index = i;

								}
							}


							ImGui::ListBoxFooter();
						}

						std::string SelectedString = "Selected one is : " + std::to_string(selectedVeh_index);
						ImGui::Text(SelectedString.c_str());
						hk_Vehicle* SelectedPed = VehicleInterface->VehicleList()->Vehicle(selectedVeh_index);
						if (SelectedPed->GetCoordinate().x != 0)
						{
							ImGui::SetCursorPosY(30);
							ImGui::SetCursorPosX(210);
							ImGui::BeginChild("Settings", ImVec2(350, 400), false);
							{
								if (!Settings::misc::untrusted_build)
								{

									if (ImGui::TreeNode("Troll"))
									{
										if (ImGui::Button("Destroy car"))
										{
											auto player = PLAYER::PLAYER_PED_ID();
											auto WeaponHash = WEAPON::GET_SELECTED_PED_WEAPON(player);
											auto WeaponDamage = WEAPON::GET_WEAPON_DAMAGE(0xBFEFFF6D, NULL);
											MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z + 2, SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z - 1, (int)1000000, false, 0xBFEFFF6D, player, true, false, 3.f);

										}
										if (ImGui::Button("Lock vehicle"))
										{
											VEHICLE::SET_VEHICLE_DOORS_LOCKED(FiveM::pointer_to_handle((DWORD64)SelectedPed), 4);

										}


										ImGui::TreePop();
									}
									if (ImGui::TreeNode("Utils"))
									{
										if (ImGui::Button("TP To Him"))
										{

											Nav->SetRotation({ 0, 0, 0, 0 });
											LocalPlayer->SetVelocity();
											LocalPlayer->SetCoordinate(SelectedPed->GetCoordinate());
											Nav->SetCoordinate(SelectedPed->GetCoordinate());


										}
										if (ImGui::Button("Repair"))
										{
											VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(FiveM::pointer_to_handle((DWORD64)SelectedPed));

											VEHICLE::SET_VEHICLE_ENGINE_HEALTH(FiveM::pointer_to_handle((DWORD64)SelectedPed), 1000);

											VEHICLE::SET_VEHICLE_BODY_HEALTH(FiveM::pointer_to_handle((DWORD64)SelectedPed), 1000.0);
											//	VEHICLE::SET_DISABLE_VEHICLE_ENGINE_FIRES(FiveM::pointer_to_handle((DWORD64)SelectedPed), true);
											//	VEHICLE::_SET_VEHICLE_WHEELS_DEAL_DAMAGE(FiveM::pointer_to_handle((DWORD64)SelectedPed), false);
											//	VEHICLE::SET_VEHICLE_FIXED(FiveM::pointer_to_handle((DWORD64)SelectedPed));
											//	VEHICLE::SET_VEHICLE_TYRE_FIXED(FiveM::pointer_to_handle((DWORD64)SelectedPed), true);
										}
										if (ImGui::Button("Warp"))
										{
											auto player = PLAYER::PLAYER_PED_ID();
											auto vehicle = FiveM::pointer_to_handle((DWORD64)SelectedPed);

											if (VEHICLE::IS_VEHICLE_SEAT_FREE(FiveM::pointer_to_handle((DWORD64)SelectedPed), -1, false)) {

												PED::SET_PED_INTO_VEHICLE(player, FiveM::pointer_to_handle((DWORD64)SelectedPed), -1);

											}
											else if (VEHICLE::IS_VEHICLE_SEAT_FREE(FiveM::pointer_to_handle((DWORD64)SelectedPed), 0, false)) {


												PED::SET_PED_INTO_VEHICLE(player, FiveM::pointer_to_handle((DWORD64)SelectedPed), 0);

											}
											else if (VEHICLE::IS_VEHICLE_SEAT_FREE(FiveM::pointer_to_handle((DWORD64)SelectedPed), 1, false)) {


												PED::SET_PED_INTO_VEHICLE(player, FiveM::pointer_to_handle((DWORD64)SelectedPed), 1);

											}
											else if (VEHICLE::IS_VEHICLE_SEAT_FREE(FiveM::pointer_to_handle((DWORD64)SelectedPed), 2, false)) {


												PED::SET_PED_INTO_VEHICLE(player, FiveM::pointer_to_handle((DWORD64)SelectedPed), 2);

											}
										}
										if (ImGui::Button("Shoot bullet"))
										{
											auto player = PLAYER::PLAYER_PED_ID();
											auto WeaponHash = WEAPON::GET_SELECTED_PED_WEAPON(player);
											auto WeaponDamage = WEAPON::GET_WEAPON_DAMAGE(0xBFEFFF6D, NULL);
											MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z + 2, SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z - 1, (int)WeaponDamage, false, 0xBFEFFF6D, player, true, false, 3.f);
										}
										ImGui::TreePop();
										std::string healthtoshow = "Health : " + std::to_string(SelectedPed->GetHealth());
										ImGui::Text(healthtoshow.c_str());
										std::string coordstoshow = "X : " + std::to_string(SelectedPed->GetCoordinate().x) + "  Y : " + std::to_string(SelectedPed->GetCoordinate().y) + "  Z : " + std::to_string(SelectedPed->GetCoordinate().z);
										ImGui::Text(coordstoshow.c_str());

									}
								}
							}
							ImGui::EndChild();
						}
					
					}
					else {

						ImGui::Text("You are not ingame !");

					}
				}
				ImGui::End();
			
		}
	}


	void Hookk(void)
	{


		if (Settings::misc::PlayerList)
		{

			ImGui::SetNextWindowSize(ImVec2(600, 500));

		

				if (!ImGui::Begin("Albania - Players List", &Settings::misc::PlayerList, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
				{



					ImGui::End();
				}
				else
				{

					if (ENTITY::GET_ENTITY_TYPE(PLAYER::PLAYER_PED_ID()) == 1)
					{

						hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
						if (!World)
							return;

						hk_Ped* LocalPlayer = World->LocalPlayer();
						if (!LocalPlayer)
							return;

						hk_ObjectNavigationPed* Nav = LocalPlayer->ObjectNavigation();
						if (!Nav)
							return;

						hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(FiveM::ReplayInterface);
						if (!ReplayInterface)
							return;

						hk_PedInterface* PedInterface = ReplayInterface->PedInterface();
						if (!PedInterface)
							return;






						if (ImGui::ListBoxHeader(" ", ImVec2(200, 400)))
						{
							for (int i = 0; i < PedInterface->PedMaximum(); i++) {

								hk_Ped* Peds = PedInterface->PedList()->Ped(i);
								if (!Peds) continue;
								int PlayerID = FiveM::pointer_to_handle((DWORD64)Peds);
								auto playerName = GetPlayerNameNew((DWORD64)Peds);
								const char* c = playerName.c_str();
								const char* items[] = { c };
								if (playerName == "**Invalid**")
								{
									if (!Settings::misc::PedList)
										continue;
									playerName = "Peds ##" + std::to_string(i);
                                      
								}

								bool is_selected = (selected_index == i);
								if (ImGui::Selectable(playerName.c_str(), is_selected))
								{

									selected_index = i;

								}
							}


							ImGui::ListBoxFooter();
						}
						
						std::string SelectedString = "Selected one is : " + std::to_string(selected_index);
						ImGui::Text(SelectedString.c_str());
						ImGui::Checkbox(("Show Ped "), &Settings::misc::PedList);
						hk_Ped* SelectedPed = PedInterface->PedList()->Ped(selected_index);
						if (SelectedPed->GetCoordinate().x != 0)
						{
							ImGui::SetCursorPosY(30);
							ImGui::SetCursorPosX(210);
							ImGui::BeginChild("Settings", ImVec2(350, 400), false);
							{
								if (!Settings::misc::untrusted_build)
								{
									if (ImGui::TreeNode("Troll"))
									{
										if (ImGui::Button("Destroy car"))
										{
											auto player = PLAYER::PLAYER_PED_ID();
											auto WeaponDamage = WEAPON::GET_WEAPON_DAMAGE(0xBFEFFF6D, NULL);
											MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z + 2, SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z - 1, (int)10000, false, (Hash)0xBFEFFF6D, player, true, false, 3.f);

										}
										if (ImGui::Button("Lock vehicle"))
										{
											auto vehicle = PED::GET_VEHICLE_PED_IS_IN(FiveM::pointer_to_handle((DWORD64)SelectedPed), false);
											VEHICLE::SET_VEHICLE_DOORS_LOCKED(vehicle, 4);

										}

										if (ImGui::Button("HandCuff"))
										{
											PED::SET_ENABLE_HANDCUFFS(FiveM::pointer_to_handle((DWORD64)SelectedPed), true);

										}
										if (ImGui::Button("Clone pedestrian"))
										{
											auto player = PLAYER::PLAYER_PED_ID();
											PED::CLONE_PED_TO_TARGET(FiveM::pointer_to_handle((DWORD64)SelectedPed), player);

										}
										if (ImGui::Button("Taze player"))
										{
											auto player = PLAYER::PLAYER_PED_ID();
											auto WeaponHash = WEAPON::GET_SELECTED_PED_WEAPON(player);
											auto WeaponDamage = WEAPON::GET_WEAPON_DAMAGE(0x8BB05FD7, NULL);
											MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z + 2, SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z - 1, (int)WeaponDamage, false, 0x8BB05FD7, player, true, false, 3.f);
										}
										ImGui::TreePop();
									}
									if (ImGui::TreeNode("Utils"))
									{
										if (ImGui::Button("TP To Him"))
										{
											if (SelectedPed != LocalPlayer)
											{
												Nav->SetRotation({ 0, 0, 0, 0 });
												LocalPlayer->SetVelocity();
												LocalPlayer->SetCoordinate(SelectedPed->GetCoordinate());
												Nav->SetCoordinate(SelectedPed->GetCoordinate());
											}

										}
										if (ImGui::Button("Spectate"))
										{
											auto player = PLAYER::PLAYER_PED_ID();
											NETWORK::NETWORK_SET_IN_SPECTATOR_MODE((bool)true, FiveM::pointer_to_handle((DWORD64)SelectedPed));

										}
										ImGui::SameLine();
										if (ImGui::Button("Off Spectate"))
										{
											auto player = PLAYER::PLAYER_PED_ID();
											NETWORK::NETWORK_SET_IN_SPECTATOR_MODE((bool)false, FiveM::pointer_to_handle((DWORD64)SelectedPed));
										}
										if (ImGui::Button("Warp"))
										{
											auto player = PLAYER::PLAYER_PED_ID();
											auto vehicle = PED::GET_VEHICLE_PED_IS_IN(FiveM::pointer_to_handle((DWORD64)SelectedPed), false);

											if (VEHICLE::IS_VEHICLE_SEAT_FREE(FiveM::pointer_to_handle((DWORD64)SelectedPed), -1, false)) {

												PED::SET_PED_INTO_VEHICLE(player, FiveM::pointer_to_handle((DWORD64)SelectedPed), -1);

											}
											else if (VEHICLE::IS_VEHICLE_SEAT_FREE(FiveM::pointer_to_handle((DWORD64)SelectedPed), 0, false)) {


												PED::SET_PED_INTO_VEHICLE(player, FiveM::pointer_to_handle((DWORD64)SelectedPed), 0);

											}
											else if (VEHICLE::IS_VEHICLE_SEAT_FREE(FiveM::pointer_to_handle((DWORD64)SelectedPed), 1, false)) {


												PED::SET_PED_INTO_VEHICLE(player, FiveM::pointer_to_handle((DWORD64)SelectedPed), 1);

											}
											else if (VEHICLE::IS_VEHICLE_SEAT_FREE(FiveM::pointer_to_handle((DWORD64)SelectedPed), 2, false)) {


												PED::SET_PED_INTO_VEHICLE(player, FiveM::pointer_to_handle((DWORD64)SelectedPed), 2);

											}


										}

										if (ImGui::Button("Shoot bullet"))
										{
											auto player = PLAYER::PLAYER_PED_ID();
											auto WeaponHash = WEAPON::GET_SELECTED_PED_WEAPON(player);
											auto WeaponDamage = WEAPON::GET_WEAPON_DAMAGE(0xBFEFFF6D, NULL);
											MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z + 2, SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z - 1, (int)WeaponDamage, false, 0xBFEFFF6D, player, true, false, 3.f);
										}
										int PlayerID = FiveM::pointer_to_handle((DWORD64)SelectedPed);

										if (ImGui::Button("Print Id"))
										{
											std::cout << FiveM::pointer_to_handle((DWORD64)SelectedPed);
											std::cout << " + ";
											std::cout << std::to_string(FiveM::pointer_to_handle((DWORD64)SelectedPed));
										}

										auto it = std::find(FiveM::Friend.begin(), FiveM::Friend.end(), GetPlayerNameNew((DWORD64)SelectedPed));
										if (it != FiveM::Friend.end())
										{
											if (ImGui::Button("Remove Friend"))
											{
												//	auto it = std::find(FiveM::Friend.begin(), FiveM::Friend.end(), (DWORD64)SelectedPed);
												int index = it - FiveM::Friend.begin();
												FiveM::Friend.erase(FiveM::Friend.begin() + index);
											}
										}
										else
										{
											if (ImGui::Button("Add Friend"))
											{
												FiveM::Friend.push_back(GetPlayerNameNew((DWORD64)SelectedPed));
											}
										}

										ImGui::TreePop();
										std::string healthtoshow = "Health : " + std::to_string(SelectedPed->GetHealth()) + "  Armor : " + std::to_string(SelectedPed->GetArmor());
										ImGui::Text(healthtoshow.c_str());
										std::string coordstoshow = "X : " + std::to_string(SelectedPed->GetCoordinate().x) + "  Y : " + std::to_string(SelectedPed->GetCoordinate().y) + "  Z : " + std::to_string(SelectedPed->GetCoordinate().z);
										ImGui::Text(coordstoshow.c_str());


										std::string ID = "Real ID / Network ID : " + std::to_string(selected_index) + " + " + std::to_string(NETWORK::NETWORK_GET_PLAYER_INDEX_FROM_PED(FiveM::pointer_to_handle((DWORD64)SelectedPed))) + "" + std::to_string(SelectedPed->PlayerInfo()->GetHealth());
										ImGui::Text(ID.c_str());

									}
								}
								ImGui::EndChild();
						}
						

						
						}

					}
					else {

						ImGui::Text("You are not ingame !");

					}
				}
				ImGui::End();
			
		}
	}




	void Hook(void)
	{


		if (Settings::Visuals::enable)
		{

			hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
			if (!World)
				return;

			hk_Ped* LocalPlayer = World->LocalPlayer();
			if (!LocalPlayer)
				return;


			/*
					hk_PlayerInterface* PlayerInterface = (hk_PlayerInterface*)*(uint64_t*)(FiveM::PlayerInterface);
			if (!PlayerInterface)
				return;

			for (int index = 0; index < PlayerInterface->PlayerMaximum(); index++)
			{
				uintptr_t pCNetGamePlayer = *(uintptr_t*)(PlayerInterface + 0x178 + (index * 0x8));

				hk_Ped* Peds = (hk_Ped*)pCNetGamePlayer;
				//hk_Ped* Peds = PedInterface->PedList()->Ped(index);
				if (!Peds) continue;
				*/
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


				if (LocalPlayer->GetHealth() <= 100) continue;
				if (!Settings::Visuals::Draw_Dead)
				{
					if (Peds->GetHealth() <= 0) continue;
				}

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
				if (Settings::Visuals::Draw_Ped == false)
				{
					if (IsPed)
					{
						continue;
					}
				}
				if (!Settings::Visuals::Draw_LocalPlayer)
				{
					if (Peds == LocalPlayer) continue;
				}
				if (Settings::Visuals::Draw_Animals)
				{
					if (IsPed)
					{
						if (PED::IS_PED_HUMAN(FiveM::pointer_to_handle((DWORD64)Peds)))
						{
							continue;
						}
				 }
				}

				ImVec2 Ped_Location = FiveM::WorldToScreen(Peds->GetCoordinate());
				if (!IsOnScreen(Ped_Location)) continue;

				ImVec2 LocalPlayer_Location = FiveM::WorldToScreen(LocalPlayer->GetCoordinate());



				Vector3 DistanceCalculation = (LocalPlayer->GetCoordinate() - Peds->GetCoordinate());

				double Distance = sqrtf(DistanceCalculation.x * DistanceCalculation.x + DistanceCalculation.y * DistanceCalculation.y + DistanceCalculation.z * DistanceCalculation.z);

				if (Distance < Settings::Visuals::Distance)
				{
					ImVec2 Head = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0x796E);
					if (!IsOnScreen(Head)) continue;

					ImVec2 Left_Foot = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0x3779);
					if (!IsOnScreen(Left_Foot)) continue;

					ImVec2 Right_Foot = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0xCC4D);
					if (!IsOnScreen(Right_Foot)) continue;		

					float Foot_Middle = (Left_Foot.y + Right_Foot.y) / 2.f;
					float Height = abs(Head.y - Foot_Middle) * 1.35f;
					float Width = (Height / 1.80f);
					float HealthDistance = Width / 16;

					if (Settings::Visuals::StaticESP)
					{
						Vector3 mins = Peds->PedModelInfo()->GetMin();
						Vector3 maxs = Peds->PedModelInfo()->GetMax();

						Vector3 min = (Peds->GetCoordinate() + mins);
						Vector3 max = (Peds->GetCoordinate() + maxs);
						Vector3 points[] = {
								Vector3(min.x, min.y, min.z),
								Vector3(min.x, max.y, min.z),
								Vector3(max.x, max.y, min.z),
								Vector3(max.x, min.y, min.z),
								Vector3(max.x, max.y, max.z),
								Vector3(min.x, max.y, max.z),
								Vector3(min.x, min.y, max.z),
								Vector3(max.x, min.y, max.z)
						};

						ImVec2 w2s_points[8];
						auto index = 0;
						for (Vector3 point : points) {
							w2s_points[index] = FiveM::WorldToScreen(point);
							if (w2s_points[index].x == 0.f && w2s_points[index].y == 0.f)
								continue;
							index++;
						}

						float x = w2s_points[0].x;
						float y = w2s_points[0].y;
						float width = w2s_points[0].x;
						float height = w2s_points[0].y;
						for (auto point : w2s_points) {
							if (x > point.x)
								x = point.x;
							if (width < point.x)
								width = point.x;
							if (y > point.y)
								y = point.y;
							if (height < point.y)
								height = point.y;
						}

						int out_x = x;
						int out_y = y;
						int out_w = width - x;
						int out_h = height - y;
						Height = out_h;
						Width = out_w;

						Width = (Height / 1.80f);
						if (((Ped_Location.x + Width / 2) - (Ped_Location.x - Width / 2)) > 600)
							continue;
					}

					if (Settings::Visuals::Snapline)
					{

						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(FiveM::WindowSize.x / 2, FiveM::WindowSize.y / 2), Ped_Location, ImColor(255, 255, 255, 255), 1.f);

					}
					ImVec2 CordsToDraw = ImVec2(Ped_Location.x - Width / 2, Ped_Location.y - Height / 2);
				

					if (Settings::Visuals::Box)
					{
						ImColor caca = { 0, 0, 0 ,255 };
						if (IsVisible((DWORD64)LocalPlayer, (DWORD64)Peds))
						{
							caca = Settings::Visuals::BoxColor;
						}
						else
						{
							caca = Settings::Visuals::BoxColorVisible;
						}
				


							ImGui::GetBackgroundDrawList()->AddRect(ImVec2(Ped_Location.x - Width / 2, Ped_Location.y - Height / 2), ImVec2(Ped_Location.x + Width / 2, Ped_Location.y + Height / 2), ImColor(0, 0, 0, 255), 0, 0, 0.9 * Settings::Visuals::BoxSize);

							ImGui::GetBackgroundDrawList()->AddRect(ImVec2(Ped_Location.x - Width / 2, Ped_Location.y - Height / 2), ImVec2(Ped_Location.x + Width / 2, Ped_Location.y + Height / 2), caca, 0, 0, 0.5 * Settings::Visuals::BoxSize);
						
					}
					if (Settings::Visuals::Corner)
					{
						ImColor caca = { 0, 0, 0 ,255 };
						if (IsVisible((DWORD64)LocalPlayer, (DWORD64)Peds))
						{
							caca = Settings::Visuals::CornerColor;
						}
						else
						{
							caca = Settings::Visuals::CornerColorVisible;
						}

						float CornerSize = (Width) / 4.f;
					
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Ped_Location.x - Width / 2, Ped_Location.y - Height / 2), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y - Height / 2) + CornerSize), ImColor(0, 0, 0, 255), 1.8 * Settings::Visuals::CornerSize);
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Ped_Location.x - Width / 2, Ped_Location.y - Height / 2), ImVec2((Ped_Location.x - Width / 2) + CornerSize, Ped_Location.y - Height / 2), ImColor(0, 0, 0, 255), 1.8 * Settings::Visuals::CornerSize);

							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Ped_Location.x + Width / 2, Ped_Location.y - Height / 2), ImVec2(Ped_Location.x + Width / 2, (Ped_Location.y - Height / 2) + CornerSize), ImColor(0, 0, 0, 255), 1.8 * Settings::Visuals::CornerSize);
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Ped_Location.x + Width / 2, Ped_Location.y - Height / 2), ImVec2((Ped_Location.x + Width / 2) - CornerSize, Ped_Location.y - Height / 2), ImColor(0, 0, 0, 255), 1.8 * Settings::Visuals::CornerSize);

							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Ped_Location.x - Width / 2, Ped_Location.y + Height / 2), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y + Height / 2) - CornerSize), ImColor(0, 0, 0, 255), 1.8 * Settings::Visuals::CornerSize);
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Ped_Location.x - Width / 2, Ped_Location.y + Height / 2), ImVec2((Ped_Location.x - Width / 2) + CornerSize, Ped_Location.y + Height / 2), ImColor(0, 0, 0, 255), 1.8 * Settings::Visuals::CornerSize);

							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Ped_Location.x + Width / 2, Ped_Location.y + Height / 2), ImVec2(Ped_Location.x + Width / 2, (Ped_Location.y + Height / 2) - CornerSize), ImColor(0, 0, 0, 255), 1.8 * Settings::Visuals::CornerSize);
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Ped_Location.x + Width / 2, Ped_Location.y + Height / 2), ImVec2((Ped_Location.x + Width / 2) - CornerSize, Ped_Location.y + Height / 2), ImColor(0, 0, 0, 255), 1.8 * Settings::Visuals::CornerSize);

							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Ped_Location.x - Width / 2, Ped_Location.y - Height / 2), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y - Height / 2) + CornerSize), caca, 1 * Settings::Visuals::CornerSize);
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Ped_Location.x - Width / 2, Ped_Location.y - Height / 2), ImVec2((Ped_Location.x - Width / 2) + CornerSize, Ped_Location.y - Height / 2), caca, 1 * Settings::Visuals::CornerSize);

							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Ped_Location.x + Width / 2, Ped_Location.y - Height / 2), ImVec2(Ped_Location.x + Width / 2, (Ped_Location.y - Height / 2) + CornerSize), caca, 1 * Settings::Visuals::CornerSize);
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Ped_Location.x + Width / 2, Ped_Location.y - Height / 2), ImVec2((Ped_Location.x + Width / 2) - CornerSize, Ped_Location.y - Height / 2), caca, 1 * Settings::Visuals::CornerSize);

							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Ped_Location.x - Width / 2, Ped_Location.y + Height / 2), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y + Height / 2) - CornerSize), caca, 1 * Settings::Visuals::CornerSize);
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Ped_Location.x - Width / 2, Ped_Location.y + Height / 2), ImVec2((Ped_Location.x - Width / 2) + CornerSize, Ped_Location.y + Height / 2), caca, 1 * Settings::Visuals::CornerSize);

							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Ped_Location.x + Width / 2, Ped_Location.y + Height / 2), ImVec2(Ped_Location.x + Width / 2, (Ped_Location.y + Height / 2) - CornerSize), caca, 1 * Settings::Visuals::CornerSize);
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Ped_Location.x + Width / 2, Ped_Location.y + Height / 2), ImVec2((Ped_Location.x + Width / 2) - CornerSize, Ped_Location.y + Height / 2), caca, 1 * Settings::Visuals::CornerSize);
						









					}
					if (Settings::Visuals::Box || Settings::Visuals::Corner)
					{


						if (Settings::Visuals::Filled)
						{
						
								ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(Ped_Location.x - Width / 2, Ped_Location.y - Height / 2), ImVec2(Ped_Location.x + Width / 2, Ped_Location.y + Height / 2), ImColor(0, 0, 0, Settings::Visuals::FillBoxAlpha));
							
						}
					}



					if (Settings::Visuals::Skeleton)
					{
						if (Settings::Visuals::Skeleton_Detailled)
						{


							ImVec2 Pelvis = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0x2e28);
							if (!IsOnScreen(Pelvis)) continue;

							ImVec2 Neck = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0x9995);
							if (!IsOnScreen(Neck)) continue;

							ImVec2 Clavicle_Left = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0xFCD9);
							if (!IsOnScreen(Clavicle_Left)) continue;

							ImVec2 Clavicle_Right = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0x29D2);
							if (!IsOnScreen(Clavicle_Right)) continue;

							ImVec2 Uperarm_left = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0xB1C5);
							if (!IsOnScreen(Uperarm_left)) continue;

							ImVec2 Uperarm_right = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0x9D4D);
							if (!IsOnScreen(Uperarm_right)) continue;

							ImVec2 Elbow_l = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0x58B7);
							if (!IsOnScreen(Elbow_l)) continue;

							ImVec2 Elbow_r = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0xBB0);
							if (!IsOnScreen(Elbow_r)) continue;

							ImVec2 R_FormArm = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0x6E5C);
							if (!IsOnScreen(R_FormArm)) continue;

							ImVec2 L_FormArm = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0xEEEB);
							if (!IsOnScreen(L_FormArm)) continue;

							ImVec2 SKEL_R_Hand = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0xDEAD);
							if (!IsOnScreen(SKEL_R_Hand)) continue;

							ImVec2 SKEL_L_Hand = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0x49D9);
							if (!IsOnScreen(SKEL_L_Hand)) continue;


							ImVec2 SKEL_L_Thigh = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0xE39F);
							if (!IsOnScreen(SKEL_L_Thigh)) continue;

							ImVec2 SKEL_L_Calf = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0xF9BB);
							if (!IsOnScreen(SKEL_L_Calf)) continue;

							ImVec2 SKEL_R_Thigh = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0xCA72);
							if (!IsOnScreen(SKEL_R_Thigh)) continue;

							ImVec2 SKEL_R_Calf = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0x9000);
							if (!IsOnScreen(SKEL_R_Calf)) continue;

							ImVec2 SKEL_L_Foot = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0x3779);
							if (!IsOnScreen(SKEL_L_Foot)) continue;

							ImVec2 SKEL_R_Foot = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0xCC4D);
							if (!IsOnScreen(SKEL_R_Foot)) continue;

							ImColor caca = { 0, 0, 0 ,255 };
							if (IsVisible((DWORD64)LocalPlayer, (DWORD64)Peds))
							{
								caca = Settings::Visuals::SkelColor;
							}
							else
							{
								caca = Settings::Visuals::SkelColorVisible;
							}
							ImGui::GetBackgroundDrawList()->AddLine(Neck, Clavicle_Right, caca, 1.f);
							ImGui::GetBackgroundDrawList()->AddLine(Neck, Clavicle_Left, caca, 1.f);

							ImGui::GetBackgroundDrawList()->AddLine(Clavicle_Right, Uperarm_right, caca, 1.f);
							ImGui::GetBackgroundDrawList()->AddLine(Clavicle_Left, Uperarm_left, caca, 1.f);

							ImGui::GetBackgroundDrawList()->AddLine(Uperarm_right, Elbow_r, caca, 1.f);
							ImGui::GetBackgroundDrawList()->AddLine(Uperarm_left, Elbow_l, caca, 1.f);

							ImGui::GetBackgroundDrawList()->AddLine(Elbow_r, R_FormArm, caca, 1.f);
							ImGui::GetBackgroundDrawList()->AddLine(Elbow_l, L_FormArm, caca, 1.f);

							ImGui::GetBackgroundDrawList()->AddLine(R_FormArm, SKEL_R_Hand, caca, 1.f);
							ImGui::GetBackgroundDrawList()->AddLine(L_FormArm, SKEL_L_Hand, caca, 1.f);

							ImGui::GetBackgroundDrawList()->AddLine(Neck, Pelvis, caca, 1.f);

							ImGui::GetBackgroundDrawList()->AddLine(Pelvis, SKEL_L_Thigh, caca, 1.f);
							ImGui::GetBackgroundDrawList()->AddLine(Pelvis, SKEL_R_Thigh, caca, 1.f);

							ImGui::GetBackgroundDrawList()->AddLine(SKEL_L_Thigh, SKEL_L_Calf, caca, 1.f);
							ImGui::GetBackgroundDrawList()->AddLine(SKEL_R_Thigh, SKEL_R_Calf, caca, 1.f);

							ImGui::GetBackgroundDrawList()->AddLine(SKEL_L_Calf, SKEL_L_Foot, caca, 1.f);
							ImGui::GetBackgroundDrawList()->AddLine(SKEL_R_Calf, SKEL_R_Foot, caca, 1.f);


						}
						else {

							ImColor caca = { 0, 0, 0 ,255 };
							if (IsVisible((DWORD64)LocalPlayer, (DWORD64)Peds))
							{
								caca = Settings::Visuals::SkelColor;
							}
							else
							{
								caca = Settings::Visuals::SkelColorVisible;
							}
							ImVec2 Pelvis = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0x2e28);
							if (!IsOnScreen(Pelvis)) continue;

							ImVec2 Neck = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0x9995);
							if (!IsOnScreen(Neck)) continue;
							ImVec2 Left_Hand = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0x49D9);
							if (!IsOnScreen(Left_Hand)) continue;

							ImVec2 Right_Hand = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0xDEAD);
							if (!IsOnScreen(Right_Hand)) continue;


							//ImGui::GetBackgroundDrawList()->AddLine(Head,Neck,ImColor(255, 255, 255, 255), 1.f);
							ImGui::GetBackgroundDrawList()->AddLine(Neck, Left_Hand, caca, 1.f);
							ImGui::GetBackgroundDrawList()->AddLine(Neck, Right_Hand, caca, 1.f);

							ImGui::GetBackgroundDrawList()->AddLine(Neck, Pelvis, caca, 1.f);
							ImGui::GetBackgroundDrawList()->AddLine(Pelvis, Left_Foot, caca, 1.f);
							ImGui::GetBackgroundDrawList()->AddLine(Pelvis, Right_Foot, caca, 1.f);

						}
					}
					if (Settings::Visuals::flags)
					{
						if(static_cast<int>(Peds->GetHealth()) >= 175)
						{
							DrawOutlinedText(ImGui::GetFont(), ICON_FA_BATTERY_FULL, ImVec2((Ped_Location.x + Width / 2 + 20), Ped_Location.y - Height / 2 + 15), 14, ImColor(255, 255, 255), true);
						}
					else if (static_cast<int>(Peds->GetHealth()) > 150 && static_cast<int>(Peds->GetHealth()) < 175) // entre 160 et 180
					{

						DrawOutlinedText(ImGui::GetFont(), ICON_FA_BATTERY_THREE_QUARTERS, ImVec2((Ped_Location.x + Width / 2 + 20), Ped_Location.y - Height / 2 + 15), 14, ImColor(255, 255, 255), true);

					}
					else if (static_cast<int>(Peds->GetHealth()) > 125 && static_cast<int>(Peds->GetHealth()) < 150) // entre 160 et 180
					{

						DrawOutlinedText(ImGui::GetFont(), ICON_FA_BATTERY_HALF , ImVec2((Ped_Location.x + Width / 2 + 20), Ped_Location.y - Height / 2 + 15), 14, ImColor(255, 255, 255), true);

					}
					else if (static_cast<int>(Peds->GetHealth()) > 110 && static_cast<int>(Peds->GetHealth()) < 125) // entre 160 et 180
					{

						DrawOutlinedText(ImGui::GetFont(), ICON_FA_BATTERY_QUARTER, ImVec2((Ped_Location.x + Width / 2 + 20), Ped_Location.y - Height / 2 + 15), 14, ImColor(255, 255, 255), true);

					}
					else if (static_cast<int>(Peds->GetHealth()) >= 100 && static_cast<int>(Peds->GetHealth()) < 110) // entre 160 et 180
					{

						DrawOutlinedText(ImGui::GetFont(), ICON_FA_BATTERY_EMPTY, ImVec2((Ped_Location.x + Width / 2 + 20), Ped_Location.y - Height / 2 + 15),14, ImColor(255, 255, 255), true);

					}


					if (Peds->GetArmor() > 1) {

						DrawOutlinedText(ImGui::GetFont(), ICON_FA_SHIELD, ImVec2((Ped_Location.x + Width / 2 + 20), Ped_Location.y - Height / 2 + 40), 14, ImColor(255, 255, 255), true);

					}
					if (Peds->IsInAVehicule())
					{
						DrawOutlinedText(ImGui::GetFont(), "CAR", ImVec2((Ped_Location.x + Width / 2 + 20), Ped_Location.y - Height / 2 + 70), 14, ImColor(255, 255, 255), true);

					}
					}
					bool IsOnGlife = false;
					if (Peds->GetMaxHealth() > 201)
						IsOnGlife = true;
					if (Settings::Visuals::HealthBar && !IsPed)
					{

						float HealthPercentage = Height / 100 * (Peds->GetHealth() - 100);
						float HealthPercentagee = Height / 100 * 100;
						if(IsOnGlife)
							 HealthPercentage = Height / 100 * ((Peds->GetHealth() - 100 )  / 2 );

						float fixedhealth = Peds->GetHealth() / 2;
						if(IsOnGlife)
							(Peds->GetHealth() - 100) / 2;
						float fixedhealthh = 100;

						ImColor	col = ImColor(52, 176, 48, 255);
						ImColor coll = ImColor(0, 0, 0, 255);

						float  caca = (Width) / 8.f;
						if (Distance < 20)
						{
							caca = (Width) / 10.f;
						}
						float HealthPercentageeee = Width / 100 * (Peds->GetHealth() - 100);



						if (Settings::Visuals::HealthBar_idx == 0)
						{
							if (fixedhealth != 0)
								DrawHealthBar(ImVec2((Ped_Location.x + Width / 2) + HealthDistance, Ped_Location.y + Height / 2), ImVec2(2, HealthPercentagee), coll);
							DrawHealthBar(ImVec2((Ped_Location.x + Width / 2) + HealthDistance, Ped_Location.y + Height / 2), ImVec2(1.8, HealthPercentage), col);

						}
						else if (Settings::Visuals::HealthBar_idx == 1)
						{
							if (fixedhealth != 0)
								DrawHealthBar(ImVec2((Ped_Location.x - Width / 2) - HealthDistance, Ped_Location.y + Height / 2), ImVec2(2, HealthPercentagee), coll);
							DrawHealthBar(ImVec2((Ped_Location.x - Width / 2) - HealthDistance, Ped_Location.y + Height / 2), ImVec2(1.8, HealthPercentage), col);

						}
						else if (Settings::Visuals::HealthBar_idx == 2)
						{

							if (fixedhealth != 0)
								DrawHealthBarhorizontal(ImVec2((Ped_Location.x - Width / 2), (Ped_Location.y + Height / 2) + caca  + 5), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y + Height / 2) + caca  + 5), ImVec2(2, HealthPercentageeee), coll);
							DrawHealthBarhorizontal(ImVec2((Ped_Location.x - Width / 2) , (Ped_Location.y + Height / 2) + caca + 5),ImVec2(Ped_Location.x - Width /2, (Ped_Location.y + Height / 2) + caca + 5), ImVec2(1.8, HealthPercentageeee ), col);

						}
						else if (Settings::Visuals::HealthBar_idx == 3)
						{
							if (fixedhealth != 0)
								DrawHealthBarhorizontal(ImVec2((Ped_Location.x - Width / 2), (Ped_Location.y - Height / 2) - 20), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y - Height / 2) - 20  ), ImVec2(2, HealthPercentageeee), coll);
							DrawHealthBarhorizontal(ImVec2((Ped_Location.x - Width / 2), (Ped_Location.y - Height / 2) - 20), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y - Height / 2) - 20), ImVec2(1.8, HealthPercentageeee), col);

						}



						//DrawMultiColorHealthBar(ImVec2((Ped_Location.x + Width / 2) + HealthDistance + 2, Ped_Location.y - Height / 2), ImVec2((Ped_Location.x + Width / 2) + HealthDistance, (Ped_Location.y + Height / 2) - HealthPercentage), col, ImColor(255, 0, 0, 255));

					}
					if (Settings::Visuals::ArmorBar && !IsPed)
					{

						float HealthPercentage = Height / 100 * (Peds->GetArmor());
						float HealthPercentagee = Height / 100 * 100;

						float fixedhealth = Peds->GetArmor();
						
						float fixedhealthh = 100;

						ImColor col = ImColor(10, 145, 255, 255);
						ImColor coll = ImColor(0, 0, 0, 255);

						float  caca = (Width) / 8.f;
						if (Distance < 20)
						{
							caca = (Width) / 10.f;
						}
						float HealthPercentageeee = Width / 100 * (Peds->GetArmor());


							//DrawHealthBar(ImVec2((Ped_Location.x +  Width / 2) + HealthDistance , Ped_Location.y + Height / 2), ImVec2(1.8, HealthPercentagee), coll);
						if (Settings::Visuals::HealthBar_idx == 0)
						{
							if (fixedhealth != 0)
								DrawHealthBar(ImVec2((Ped_Location.x + Width / 2) + HealthDistance, Ped_Location.y + Height / 2), ImVec2(2, HealthPercentagee), coll);
							DrawHealthBar(ImVec2((Ped_Location.x + Width / 2) + HealthDistance, Ped_Location.y + Height / 2), ImVec2(1.8, HealthPercentage), col);

						}
						else if (Settings::Visuals::HealthBar_idx == 1)
						{
							if (fixedhealth != 0)
								DrawHealthBar(ImVec2((Ped_Location.x - Width / 2) - HealthDistance, Ped_Location.y + Height / 2), ImVec2(2, HealthPercentagee), coll);
							DrawHealthBar(ImVec2((Ped_Location.x - Width / 2) - HealthDistance, Ped_Location.y + Height / 2), ImVec2(1.8, HealthPercentage), col);

						}
						else if (Settings::Visuals::HealthBar_idx == 2)
						{

							if (fixedhealth != 0)
								DrawHealthBarhorizontal(ImVec2((Ped_Location.x - Width / 2), (Ped_Location.y + Height / 2) + caca + 5), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y + Height / 2) + caca + 5), ImVec2(2, HealthPercentageeee), coll);
							DrawHealthBarhorizontal(ImVec2((Ped_Location.x - Width / 2), (Ped_Location.y + Height / 2) + caca + 5), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y + Height / 2) + caca + 5), ImVec2(1.8, HealthPercentageeee), col);

						}
						else if (Settings::Visuals::HealthBar_idx == 3)
						{
							if (fixedhealth != 0)
								DrawHealthBarhorizontal(ImVec2((Ped_Location.x - Width / 2), (Ped_Location.y - Height / 2) - 20), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y - Height / 2) - 20), ImVec2(2, HealthPercentageeee), coll);
							DrawHealthBarhorizontal(ImVec2((Ped_Location.x - Width / 2), (Ped_Location.y - Height / 2) - 20), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y - Height / 2) - 20), ImVec2(1.8, HealthPercentageeee), col);

						}




					}
				
					std::string str = "";

					if (Settings::Visuals::Weapon)
					{
						hk_WeaponManager* PedsWeaponManager = Peds->WeaponManager();
						if (!PedsWeaponManager) continue;
						hk_WeaponInfo* PedsWeaponInfo = PedsWeaponManager->WeaponInfo();
						if (!PedsWeaponInfo) continue;
						uint64_t Hash = PedsWeaponInfo->GetHash();
						str.append(get_weapon_name(Hash));

						//DrawOutlinedText(ImGui::GetFont(), get_weapon_name(Hash), ImVec2(Ped_Location.x, Ped_Location.y + Height / 2 + 5), 15.0f, ImColor(255, 255, 255), true);

					}

					if (Settings::Visuals::DistanceESP) {

						int Ddistance = sqrtf(DistanceCalculation.x * DistanceCalculation.x + DistanceCalculation.y * DistanceCalculation.y + DistanceCalculation.z * DistanceCalculation.z);

						std::string strr = " | " + std::to_string(Ddistance) + " m";
						if (!Settings::Visuals::Weapon)
							strr = std::to_string(Ddistance) + " m ";
						str.append(strr);

					}
					if (Settings::Visuals::DistanceESP || Settings::Visuals::Weapon)
					{
						DrawOutlinedText(ImGui::GetFont(), str, ImVec2(Ped_Location.x, Ped_Location.y + Height / 2 + 5), 15.0f, ImColor(255, 255, 255), true);

					}
					if (Settings::Visuals::PlayerName)
					{



						auto playerName = GetPlayerNameNew((DWORD64)Peds);


						if (Peds != LocalPlayer && IsPed)
						{
							playerName = "Ped";
						}


						//DrawOutlinedText(ImGui::GetFont(), playerName, ImVec2(Ped_Location.x, Ped_Location.y - Height / 2 - 10), 15.0f, ImColor(255, 255, 255), true);
						//std::string NewPlayerName = playerName + " | " + get_weapon_name(Hash) + " | " + distance;

							ImVec2 TextSize = ImGui::CalcTextSize((char*)playerName.c_str());

							ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(Ped_Location.x - (TextSize.x / 2), Ped_Location.y - Height / 2 - 5 - (TextSize.y / 2) - 1), ImVec2(Ped_Location.x + (TextSize.x / 2), Ped_Location.y - Height / 2 - 5 + (TextSize.y / 2) - 1), ImColor(0, 0, 0, 150));
							DrawOutlinedText(ImGui::GetFont(), playerName, ImVec2(Ped_Location.x, Ped_Location.y - Height / 2  - 5- 10), 14, ImColor(255, 255, 255), true);


					}




					if (Settings::Visuals::tpped || Settings::Visuals::killped || Settings::Visuals::invisibleped)
					{
						if (IsPed && Peds != LocalPlayer)
						{
							if (Settings::Visuals::tpped)
							{
								Peds->SetVelocity();
								DWORD64 addr = FiveM::GetCamera();

								Vector3 ActualPos = LocalPlayer->GetCoordinate();
								Peds->SetCoordinate(ActualPos + Vector3(0, 0, 1));
								Peds->SetHealth(0);
								Peds->Set_Ragdoll(false);
							}
							if (Settings::Visuals::invisibleped)
							{
								Peds->SetInvisible(0X37);
							}
							if (Settings::Visuals::killped)
							{
								Peds->SetHealth(0);

							}

						}

					}
				}
			}

		}
	}
}
