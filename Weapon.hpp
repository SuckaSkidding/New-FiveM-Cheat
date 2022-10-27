#pragma once

#include "includes.hpp"
#include "Classes.hpp"
#include "Settings.hpp"
#include "native.hpp"



DWORD weaponsList[] = {
	0x93E220BD, 0xA0973D5E, 0x24B17070, 0x2C3731D9, 0xAB564B93, 0x787F0BB, 0xBA45E8B8, 0x23C9F95C, 0xFDBC8A50, 0x497FACC3, //Throwables
	0x34A67B97, 0xFBAB5776, 0x060EC506, //Miscellaneous
	0xB1CA77B1, 0xA284510B, 0x42BF8A85, 0x7F7497E5, 0x6D544C99, 0x63AB0442, 0x0781FE4A, 0xB62D1F67, //Heavy Weapons
	0x05FC3C11, 0x0C472FE2, 0xA914799, 0xC734385A, 0x6A6C02E0, //Sniper Rifles
	0x9D07F764, 0x7FD62962, 0xDBBD7280, 0x61012683, //Light Machine Guns
	0xBFEFFF6D, 0x394F415C, 0x83BF0278, 0xFAD1F1C9, 0xAF113F99, 0xC0A3098D, 0x969C3D67, 0x7F229F94, 0x84D6FAFD, 0x624FE830, //Assault Rifles
	0x13532244, 0x2BE6766B, 0x78A97CD0, 0xEFE7E2DF, 0x0A3D4D34, 0xDB1AA450, 0xBD248B55, 0x476BF155, //Submachine Guns
	0x1B06D571, 0xBFE256D4, 0x5EF9FEC4, 0x22D8FE39, 0x3656C8C1, 0x99AEEB3B, 0xBFD21232, 0x88374054, 0xD205520E, 0x83839C4, 0x47757124, 0xDC4DB296, 0xC1B3C3D1, 0xCB96392F, 0x97EA20B8, 0xAF3696A1, //Pistols
	0x92A27487, 0x958A4A8F, 0xF9E6AA4B, 0x84BD7BFD, 0x8BB05FD7, 0x440E4788, 0x4E875F73, 0xF9DCBF2D, 0xD8DF3C3C, 0x99B507EA, 0xDD5DF8D9, 0xDFE37640, 0x678B81B1, 0x19044EE0, 0xCD274149, 0x94117305, 0x3813FC08, //Mele
	0x1D073A89, 0x555AF99A, 0x7846A318, 0xE284C527, 0x9D61E50F, 0xA89CB99E, 0x3AABBBAA, 0xEF951FBB, 0x12E82D3D
};



namespace wepList {
	DWORD THROWABLE_WEAPON[] = { 0x93E220BD, 0xA0973D5E, 0x24B17070, 0x2C3731D9, 0xAB564B93, 0x787F0BB, 0xBA45E8B8, 0x23C9F95C, 0xFDBC8A50, 0x497FACC3 };
	DWORD MISC_WEAPON[] = { 0x34A67B97, 0xFBAB5776, 0x060EC506 };
	DWORD HEAVY_WEAPON[] = { 0xB1CA77B1, 0xA284510B, 0x42BF8A85, 0x7F7497E5, 0x6D544C99, 0x63AB0442, 0x0781FE4A, 0xB62D1F67 };
	DWORD SNIPER_WEAPON[] = { 0x05FC3C11, 0x0C472FE2, 0xA914799, 0xC734385A, 0x6A6C02E0 };
	DWORD PISTOL_WEAPON[] = { 0x1B06D571, 0xBFE256D4, 0x5EF9FEC4, 0x22D8FE39, 0x3656C8C1, 0x99AEEB3B, 0xBFD21232, 0x88374054, 0xD205520E, 0x83839C4, 0x47757124, 0xDC4DB296, 0xC1B3C3D1, 0xCB96392F, 0x97EA20B8, 0xAF3696A1 };
	DWORD MELEE_WEAPON[] = { 0x92A27487, 0x958A4A8F, 0xF9E6AA4B, 0x84BD7BFD, 0x8BB05FD7, 0x440E4788, 0x4E875F73, 0xF9DCBF2D, 0xD8DF3C3C, 0x99B507EA, 0xDD5DF8D9, 0xDFE37640, 0x678B81B1, 0x19044EE0, 0xCD274149, 0x94117305, 0x3813FC08 };
	DWORD LIGHT_WEAPON[] = { 0x9D07F764, 0x7FD62962, 0xDBBD7280, 0x61012683 };
	DWORD SMG_WEAPON[] = { 0x13532244, 0x2BE6766B, 0x78A97CD0, 0xEFE7E2DF, 0x0A3D4D34, 0xDB1AA450, 0xBD248B55, 0x476BF155 };
	DWORD RIFLE_WEAPON[] = { 0xBFEFFF6D, 0x394F415C, 0x83BF0278, 0xFAD1F1C9, 0xAF113F99, 0xC0A3098D, 0x969C3D67, 0x7F229F94, 0x84D6FAFD, 0x624FE830 };
	DWORD SHOTGUN_WEAPON[] = { 0x1D073A89, 0x555AF99A, 0x7846A318, 0xE284C527, 0x9D61E50F, 0xA89CB99E, 0x3AABBBAA, 0xEF951FBB, 0x12E82D3D };
}

auto GetWeaponType(DWORD dwWeaponHash) {
	for (int i = 0; i < sizeof(wepList::THROWABLE_WEAPON) / sizeof(DWORD); i++) {
		if (dwWeaponHash == wepList::THROWABLE_WEAPON[i]) {
			return 0;
		}
	}
	for (int i = 0; i < sizeof(wepList::MISC_WEAPON) / sizeof(DWORD); i++) {
		if (dwWeaponHash == wepList::MISC_WEAPON[i]) {
			return 1;
		}
	}
	for (int i = 0; i < sizeof(wepList::HEAVY_WEAPON) / sizeof(DWORD); i++) {
		if (dwWeaponHash == wepList::HEAVY_WEAPON[i]) {
			return 2;
		}
	}
	for (int i = 0; i < sizeof(wepList::SNIPER_WEAPON) / sizeof(DWORD); i++) {
		if (dwWeaponHash == wepList::SNIPER_WEAPON[i]) {
			return 3;
		}
	}
	for (int i = 0; i < sizeof(wepList::PISTOL_WEAPON) / sizeof(DWORD); i++) {
		if (dwWeaponHash == wepList::PISTOL_WEAPON[i]) {
			return 4;
		}
	}
	for (int i = 0; i < sizeof(wepList::MELEE_WEAPON) / sizeof(DWORD); i++) {
		if (dwWeaponHash == wepList::MELEE_WEAPON[i]) {
			return 5;
		}
	}
	for (int i = 0; i < sizeof(wepList::LIGHT_WEAPON) / sizeof(DWORD); i++) {
		if (dwWeaponHash == wepList::LIGHT_WEAPON[i]) {
			return 6;
		}
	}
	for (int i = 0; i < sizeof(wepList::SMG_WEAPON) / sizeof(DWORD); i++) {
		if (dwWeaponHash == wepList::SMG_WEAPON[i]) {
			return 7;
		}
	}
	for (int i = 0; i < sizeof(wepList::RIFLE_WEAPON) / sizeof(DWORD); i++) {
		if (dwWeaponHash == wepList::RIFLE_WEAPON[i]) {
			return 8;
		}
	}
	for (int i = 0; i < sizeof(wepList::SHOTGUN_WEAPON) / sizeof(DWORD); i++) {
		if (dwWeaponHash == wepList::SHOTGUN_WEAPON[i]) {
			return 9;
		}
	}

	return 10;
}


namespace Weapon
{
	void Hook(void)
	{

		if (!Settings::misc::untrusted_build)
		{
			auto player = PLAYER::PLAYER_PED_ID();
			auto WeaponHash = WEAPON::GET_SELECTED_PED_WEAPON(player);

			Settings::Weapon::WeaponType = GetWeaponType(WeaponHash);

			if (Settings::Weapon::DamageBoost)
			{

				WEAPON::_SET_WEAPON_DAMAGE_MODIFIER_THIS_FRAME(WeaponHash, Settings::Weapon::DamageBoost_value);

			}



			if (Settings::Weapon::InfiniteAmmo)
			{

				WEAPON::SET_PED_AMMO(player, WeaponHash, 100, true);

			}



			if (Settings::Weapon::SetAmmo)
			{


				WEAPON::SET_PED_AMMO(player, WeaponHash, Settings::Weapon::AmmoToSet, true);


				Settings::Weapon::SetAmmo = false;

			}



			if (Settings::Weapon::FastNoReload)
			{

				WEAPON::REFILL_AMMO_INSTANTLY(player);

			}
		}


	//	if (Settings::Weapon::enable)
		//{
			hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
			if (!World)
				return;

			hk_Ped* LocalPlayer = World->LocalPlayer();
			if (!LocalPlayer)
				return;

			hk_WeaponManager* WeaponManager = LocalPlayer->WeaponManager();
			if (!WeaponManager)
				return;
			hk_WeaponInfo* WeaponInfo = WeaponManager->WeaponInfo();
			if (!WeaponInfo)
				return;

			if (Settings::Weapon::No_Recoil)
			{
				WeaponInfo->SetRecoil(Settings::Weapon::RecoilManipulation);

			}

			if (Settings::Weapon::No_Spread)
			{
				WeaponInfo->SetSpread(0.f);
			}

			if (Settings::Weapon::ExplosiveAmmo)
			{
				WeaponInfo->SetAmmoType(0x5);
				WeaponInfo->SetAmmoExplosiveType(0x1);

			}
		

			if (Settings::Weapon::ReloadV2)
			{
				WeaponInfo->SetReload(Settings::Weapon::ReloadSpeed / 10);
			}

			if (!Settings::misc::untrusted_build)
			{
				if (Settings::Weapon::GiveWeapon)
				{
					WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFAD1F1C9, (int)999, (bool)false, (bool)true);
					Settings::Weapon::GiveWeapon = false;
				}
			}

			if (Settings::Weapon::NoReload)
			{
				WeaponInfo->SetReload(1000);
			}
			if (Settings::Weapon::NoRangeLimit)
			{
				WeaponInfo->SetRange(1000);
			}

			hk_AmmoInfo* AmmoInfo = WeaponInfo->AmmoInfo();
			if (!AmmoInfo)
				return;

			hk_AmmoCount* AmmoCount = AmmoInfo->AmmoCount();
			if (!AmmoCount)
				return;
			hk_FixedAmmoCount* FixedAmmoCount = AmmoCount->FixedAmmoCount();
			if (!FixedAmmoCount)
				return;

			if (Settings::Weapon::InfiniteAmmo)
			{
				FixedAmmoCount->SetAmmo(9999);
			}
			if (Settings::Weapon::rapidfire)
			{
			
					
				AmmoInfo->SetSpeedBullet(20000);
				
			}
			if (Settings::Weapon::SetAmmo)
			{
				FixedAmmoCount->SetAmmo(Settings::Weapon::AmmoToSet);
				//auto player = PLAYER::PLAYER_PED_ID();
				//auto WeaponHash = WEAPON::GET_SELECTED_PED_WEAPON(player);
				//
				Settings::Weapon::SetAmmo = false;
			}

		//}


	}
}