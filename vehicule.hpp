#pragma once

#include "includes.hpp"
#include "Classes.hpp"
#include "Settings.hpp"
#include <string>
#include <ctime>

LPCSTR vehModels[283] = {
	   "NINEF", "NINEF2", "BLISTA", "ASEA", "ASEA2", "T20", "BRAWLER", "COQUETTE3", "VINDICATOR"
	   "COACH", "AIRBUS", "ASTEROPE", "AIRTUG", "AMBULANCE", "BARRACKS", "BARRACKS2", "BALLER", "BALLER2",
	   "BJXL", "BANSHEE", "BENSON", "BFINJECTION", "BIFF", "BLAZER", "BLAZER2", "BLAZER3", "BISON", "BISON2",
	   "BISON3", "BOXVILLE", "BOXVILLE2", "BOXVILLE3", "BOBCATXL", "BODHI2", "BUCCANEER", "BUFFALO", "BUFFALO2", "BULLDOZER",
	   "BULLET", "BURRITO", "BURRITO2", "BURRITO3", "BURRITO4", "BURRITO5", "CAVALCADE", "CAVALCADE2", "POLICET",
	   "GBURRITO", "CABLECAR", "CADDY", "CADDY2", "CAMPER", "CARBONIZZARE", "CHEETAH", "COMET2", "COGCABRIO", "COQUETTE",
	   "CUTTER", "GRESLEY", "DILETTANTE", "DILETTANTE2", "DUNE", "DUNE2", "HOTKNIFE", "DLOADER", "DUBSTA", "DUBSTA2",
	   "DUMP", "RUBBLE", "DOCKTUG", "DOMINATOR", "EMPEROR", "EMPEROR2", "EMPEROR3", "ENTITYXF", "EXEMPLAR", "ELEGY2",
	   "F620", "FBI", "FBI2", "FELON", "FELON2", "FELTZER2", "FIRETRUK", "FLATBED", "FORKLIFT", "FQ2",
	   "FUSILADE", "FUGITIVE", "FUTO", "GRANGER", "GAUNTLET", "HABANERO", "HAULER", "HANDLER", "INFERNUS", "INGOT",
	   "INTRUDER", "ISSI2", "JACKAL", "JOURNEY", "JB700", "KHAMELION", "LANDSTALKER", "LGUARD", "MANANA", "MESA",
	   "MESA2", "MESA3", "CRUSADER", "MINIVAN", "MIXER", "MIXER2", "MONROE", "MOWER", "MULE", "MULE2",
	   "ORACLE", "ORACLE2", "PACKER", "PATRIOT", "PBUS", "PENUMBRA", "PEYOTE", "PHANTOM", "PHOENIX", "PICADOR",
	   "POUNDER", "POLICE", "POLICE4", "POLICE2", "POLICE3", "POLICEOLD1", "POLICEOLD2", "PONY", "PONY2", "PRAIRIE",
	   "PRANGER", "PREMIER", "PRIMO", "RANCHERXL", "RANCHERXL2", "RAPIDGT", "RAPIDGT2", "RADI", "RATLOADER",
	   "REBEL", "REGINA", "REBEL2", "RENTALBUS", "RUINER", "RUMPO", "RUMPO2", "RHINO", "RIOT", "RIPLEY",
	   "ROCOTO", "ROMERO", "SABREGT", "SADLER", "SADLER2", "SANDKING", "SANDKING2", "SCHAFTER2", "SCHWARZER", "SCRAP",
	   "SEMINOLE", "SENTINEL", "SENTINEL2", "ZION", "ZION2", "SERRANO", "SHERIFF", "SHERIFF2", "SPEEDO", "SPEEDO2",
	   "STANIER", "STINGER", "STINGERGT", "STOCKADE", "STOCKADE3", "STRATUM", "SULTAN", "SUPERD", "SURANO", "SURFER",
	   "SURFER2", "SURGE", "TACO", "TAILGATER", "TAXI", "TRASH", "TRACTOR", "TRACTOR2", "TRACTOR3",
	   "TIPTRUCK", "TIPTRUCK2", "TORNADO", "TORNADO2", "TORNADO3", "TORNADO4", "TOURBUS", "TOWTRUCK", "TOWTRUCK2",
	   "UTILLITRUCK", "UTILLITRUCK2", "UTILLITRUCK3", "VOODOO2", "WASHINGTON", "STRETCH", "YOUGA", "ZTYPE", "SANCHEZ", "SANCHEZ2",
	   "POLICEB", "AKUMA", "CARBONRS", "HEXER", "NEMESIS", "ADDER", "VOLTIC", "VACCA", "BIFTA",
	   "BAGGER", "BATI", "BATI2", "RUFFIAN", "DAEMON", "DOUBLE", "PCJ", "VADER", "VIGERO", "FAGGIO2",
	   "PARADISE", "KALAHARI", "JESTER", "TURISMOR", "ALPHA", "HUNTLEY", "THRUST", "MASSACRO",
	   "MASSACRO2", "ZENTORNO", "BLADE", "GLENDALE", "PANTO", "PIGALLE", "WARRENER", "RHAPSODY", "DUBSTA3", "MONSTER",
	   "SOVEREIGN", "INNOVATION", "HAKUCHOU", "FUROREGT", "COQUETTE2", "BTYPE", "BUFFALO3", "DOMINATOR2", "GAUNTLET2",
	   "MARSHALL", "DUKES", "DUKES2", "STALION", "STALION2", "BLISTA2", "BLISTA3", "VIRGO", "WINDSOR",
	   "INSURGENT", "INSURGENT2", "TECHNICAL", "KURUMA", "KURUMA2", "JESTER2", "CASCO",
	   "GUARDIAN", "ENDURO", "LECTRO", "SLAMVAN", "SLAMVAN2", "RATLOADER2", "FELTZER3", "OSIRIS"
};
namespace Vehicule
{
	void NameHook(void)
	{
		if (Settings::vehicle::NameESP)
		{


			hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(FiveM::ReplayInterface);
			if (!ReplayInterface)
				return;
			hk_VehicleInterface* VehicleInterface = ReplayInterface->VehicleInterface();
			if (!VehicleInterface)
				return;


			for (int i = 0; i < VehicleInterface->VehicleMaximum(); i++)
			{
				hk_Vehicle* Ped = VehicleInterface->VehicleList()->Vehicle(i);
				if (!Ped) return;
				ImVec2 Ped_Location = FiveM::WorldToScreen(Ped->GetCoordinate());



				auto playerName = Ped->VehicleModelInfo()->GetCharName();
				const char* c = playerName;
				const char* items[] = { c };

				std::string VehicleName = playerName;
				DrawOutlinedText(ImGui::GetFont(), VehicleName, ImVec2(Ped_Location.x, Ped_Location.y), 14.0f, ImColor(255, 255, 255), true);


			}
		}
	}
	void Hook(void)
	{
		if (Settings::vehicle::enable)
		{

			hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
			if (!World)
				return;
			hk_Ped* LocalPlayer = World->LocalPlayer();
			if (!LocalPlayer)
				return;
			

			hk_Vehicle* ActualVehicle = LocalPlayer->VehicleManager();
			if (!ActualVehicle)
				return;
			if (Settings::misc::untrusted_build)
			{
				return;
			}
			auto player = PLAYER::PLAYER_PED_ID();
			auto trying_to_open_veicle = PED::GET_VEHICLE_PED_IS_TRYING_TO_ENTER(player);
			auto current_vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), true);

			if (Settings::vehicle::unlockcar)
			{

				VEHICLE::SET_VEHICLE_DOORS_LOCKED(trying_to_open_veicle, 1);

			}

			if (Settings::vehicle::autostart_vehicle)
			{

				VEHICLE::SET_VEHICLE_ENGINE_ON(current_vehicle, true, true, false);

			}

		
			if (Settings::vehicle::warp_vehicle)
			{

				PED::SET_PED_INTO_VEHICLE(player, trying_to_open_veicle, -1);

			}

			if (Settings::vehicle::godmode)
			{

				ENTITY::SET_ENTITY_INVINCIBLE(current_vehicle, true);
				ENTITY::SET_ENTITY_PROOFS(current_vehicle, 1, 1, 1, 1, 1, 1, 1, 1);
				VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(current_vehicle, 0);
				VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(current_vehicle, 0);
				VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(current_vehicle, 0);
				VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(current_vehicle, false);
			//	VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(current_vehicle, false);
				VEHICLE::SET_VEHICLE_HAS_STRONG_AXLES(current_vehicle, true);


			}

			if (PED::IS_PED_IN_ANY_VEHICLE(player, false))
			{
				Settings::vehicle::on_car = true;


				if (Settings::vehicle::repaire)
				{

					VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(current_vehicle);

					ActualVehicle->SetEngineHealth(1000);
					ActualVehicle->SetBodyHealth(1000);
					//VEHICLE::SET_VEHICLE_BODY_HEALTH(current_vehicle, 1000.0);
					//	VEHICLE::SET_DISABLE_VEHICLE_ENGINE_FIRES(current_vehicle, true);
				//	VEHICLE::_SET_VEHICLE_WHEELS_DEAL_DAMAGE(current_vehicle, false);
				//	VEHICLE::SET_VEHICLE_FIXED(current_vehicle);
					//VEHICLE::SET_VEHICLE_TYRE_FIXED(current_vehicle, true);
					
					Settings::vehicle::repaire = false;


				}

				if (Settings::vehicle::max_current_vehicle)
				{


					VEHICLE::SET_VEHICLE_MOD(current_vehicle, Settings::vehicle::mod_idx, Settings::vehicle::add_idx, false);

					Settings::vehicle::max_current_vehicle = false;
				}

				if (Settings::vehicle::vehicle_speed_bool)
				{
	
					ActualVehicle->VehicleHandlingInfo()->SetAcceleration(Settings::vehicle::vehicle_speed_value);

				}
				if (Settings::vehicle::shift_boost)
				{
					if (SAFE_CALL(GetAsyncKeyState)(VK_SHIFT) & 0x8000) {
						//float speed = ENTITY::GET_ENTITY_SPEED(current_vehicle);
						float speeedspeed = 5;
						ActualVehicle->VehicleHandlingInfo()->SetAcceleration(1000);

					}
				}
				

				if (Settings::vehicle::update_colors)
				{

					VEHICLE::SET_VEHICLE_COLOURS(current_vehicle, Settings::vehicle::primary_color, Settings::vehicle::secondary_color);

					Settings::vehicle::update_colors = false;

				}



				if (Settings::vehicle::update_plate)
				{

					VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(current_vehicle, Settings::vehicle::plate_text);

					Settings::vehicle::update_plate = false;

				}



			}
			else {

				Settings::vehicle::on_car = false;

			}
		}
		

	}
}