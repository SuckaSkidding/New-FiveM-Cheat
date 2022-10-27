#include "Core.hpp"

#include "d3d_Hook.hpp"
#include "Menu.hpp"
#include "Classes.hpp"
#include "Settings.hpp"
#include "Visuals.hpp"
#include "Player.hpp"
#include "Aimbot.hpp"
#include "Weapon.hpp"
#include "NoClip.hpp"
#include "vehicule.hpp"
#include "skCrpyt.h"
#include "Fonts.hpp"
#include <imguinotify.hpp>
#include "auth.hpp"
#include <windows.h>
#pragma comment(lib, "ntdll.lib")

extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN OldValue);
extern "C" NTSTATUS NTAPI NtRaiseHardError(LONG ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask,
	PULONG_PTR Parameters, ULONG ValidResponseOptions, PULONG Response);

using namespace KeyAuth;

static int style_idx = 0;
const char* cheats[] = { "Eagle Software FiveM" };

//std::string name = "fortnite"; // application name. right above the blurred text aka the secret on the licenses tab among other tabs
//std::string ownerid = "KM8JH8EZZc"; // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
//std::string secret = "f6351dd58f751c3395d3b632e20a2cd4d67006e3be56415a2fd4e1e643e3b109"; // app secret, the blurred text on licenses tab and other tabs
//std::string version = "1.0"; // leave alone unless you've changed version on website

std::string name = "Renderhook.cpp"; // application name. right above the blurred text aka the secret on the licenses tab among other tabs
std::string ownerid = "yvd9sHbJe5"; // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
std::string secret = "865e02804ef935ed71a9314e685feea259de25bb502d5afdc3947605b65e029d"; // app secret, the blurred text on licenses tab and other tabs
std::string version = "1.0"; // leave alone unless you've changed version on website
std::string url = "https://keyauth.win/api/1.2/"; // change if you're self-hosting
std::string sslPin = "ssl pin key (optional)"; // don't change unless you intend to pin public certificate key. you can get here in the "Pin SHA256" field https://www.ssllabs.com/ssltest/analyze.html?d=keyauth.win&latest. If you do this you need to be aware of when SSL key expires so you can update it

api KeyAuthApp(name, ownerid, secret, version, url, sslPin);

char username[10] = "";
char password[10] = "";
char key[50] = "";

static int tab = 0;

static int VideoDevice = 0;

FILE* p_stream;
static char MessageText[256] = "";

//#include "Snow.hpp"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
// you need those for snowflake
#define WINDOW_WIDTH  ImGui::GetIO().DisplaySize.x
#define WINDOW_HEIGHT ImGui::GetIO().DisplaySize.y
bool IsValid = false;
static int caca = 1;

#pragma region HideConsole
void xxx()	
{	
	HWND Stealth;
	AllocConsole();
	Stealth = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(Stealth, 0);
}
#pragma endregion

void InitImGui()
{
	using namespace DirectX;

	ImGui::CreateContext();

	ImGuiIO* io = &ImGui::GetIO();
	io->ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	io->IniFilename = nullptr;
	io->LogFilename = nullptr;

	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig icons_config;

	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	
	ImFontConfig rubik;
	rubik.FontDataOwnedByAtlas = false;
	

	io->Fonts->AddFontFromMemoryTTF(const_cast<uint8_t*>(Rubik), sizeof(Rubik), 15.5f, &rubik);
	io->Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 25.f, &icons_config, icons_ranges);
	io->Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 25.f, &icons_config, icons_ranges);
	Menu::BiggestIcon = io->Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 40.f, NULL, icons_ranges);
	Menu::BiggestFont = io->Fonts->AddFontFromMemoryTTF(const_cast<uint8_t*>(Rubik), sizeof(Rubik), 21.f, &rubik);
	Menu::littleFont = io->Fonts->AddFontFromMemoryTTF(const_cast<uint8_t*>(Rubik), sizeof(Rubik), 17.5f, &rubik);

	ImGui_ImplWin32_Init(Window);
	ImGui_ImplDX11_Init(pDevice, pContext);
	


}

LRESULT __stdcall WindowHandler(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (Menu::Open)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return true;
	}

	return SAFE_CALL(CallWindowProcA)(DirectX::WindowEx, hWnd, uMsg, wParam, lParam);
}

ImFont* font;

bool BindD3DInfo(IDXGISwapChain* pSwapChain)
{
	if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&DirectX::pDevice)))
	{
		DirectX::pDevice->GetImmediateContext(&DirectX::pContext);
		DXGI_SWAP_CHAIN_DESC sd;

		pSwapChain->GetDesc(&sd);
		DirectX::Window = sd.OutputWindow;
		
	
		ID3D11Texture2D* pBackBuffer;

		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		DirectX::pDevice->CreateRenderTargetView(pBackBuffer, 0, &DirectX::renderTargetView);
		pBackBuffer->Release();

		DirectX::WindowEx = (WNDPROC)LI_FN(SetWindowLongPtrA).safe_cached()(DirectX::Window, GWLP_WNDPROC, (LONG_PTR)WindowHandler);

		InitImGui();

		Menu::FirstTime = false;

		Menu::Style();

		return true;
	}

	return false;
}
bool AuthConnected = false;

const char* weaponType[]{ "Throwable", "Misc", "Heavy", "Sniper", "Pistol", "Melee", "Light", "SMG", "Rifle", "Shotgun", "Unknown" };

char* cacacaca;
static int Tabs = 2;
static int Checks = 0;

void LoginNow()
{
	KeyAuthApp.login(username, password);
	if (KeyAuthApp.data.success) {


		fopen_s(&p_stream, skCrypt("Login"), "w+");

		fseek(p_stream, 0, SEEK_SET);

		fwrite(username, sizeof(username), 1, p_stream);
		fwrite(password, sizeof(password), 1, p_stream);

		fclose(p_stream);

		Settings::misc::security_1 = true;
		VideoDevice = 1;
		tab = 3;


	}
	else if (!KeyAuthApp.data.success) {
		SAFE_CALL(MessageBoxA)(NULL, KeyAuthApp.data.message.c_str(), skCrypt("Failed Login"), NULL);
	}
}

HRESULT __stdcall PresentHook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (Menu::FirstTime)
	{

		if (!BindD3DInfo(pSwapChain))
			return DirectX::OriginalPresent(pSwapChain, SyncInterval, Flags);

		fopen_s(&p_stream, skCrypt("Login"), "r+");

		fseek(p_stream, 0, SEEK_SET);

		fread_s(username, sizeof(username), sizeof(username), 1, p_stream);
		fread_s(password, sizeof(password), sizeof(password), 1, p_stream);

		fclose(p_stream);
		Settings::misc::editor.SetShowWhitespaces(false);
		Settings::misc::editor.SetReadOnly(false);
		Settings::misc::editor.SetPalette(TextEditor::GetDarkPalette());
		Settings::misc::editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
		Settings::misc::editor.SetText("print('Working !')");

		xxx();
		KeyAuthApp.init();
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (SAFE_CALL(GetAsyncKeyState)(Settings::misc::Menu_Hotkey) & 1)
	{

		Menu::Open = !Menu::Open;

	}

	ImGui::GetIO().MouseDrawCursor = Menu::Open;
	ImGui::GetIO().WantCaptureKeyboard = Menu::Open;
	if (AuthConnected)
	{
		if (!Settings::misc::isPanic)
		{
			if (Settings::Aimbot::adaptive_weapon)
			{
				if (weaponType[Settings::Weapon::WeaponType] == "Sniper") {

					Settings::Aimbot::Aimbot = Settings::Aimbot::AdaptiveSniper::Enable_Sniper;

					if (Settings::Aimbot::AdaptiveSniper::Enable_Sniper)
					{
						Settings::Aimbot::AimbotFov = Settings::Aimbot::AdaptiveSniper::Sniper_AimbotFov;
						Settings::Aimbot::AimbotSmooth = Settings::Aimbot::AdaptiveSniper::Sniper_AimbotSmooth;
						Settings::Aimbot::DistanceAimbotLimit = Settings::Aimbot::AdaptiveSniper::Sniper_Distance;
						Settings::Aimbot::AimbotTypes = Settings::Aimbot::AdaptiveSniper::Sniper_AimbotTypes;
						Settings::Aimbot::TypeOfTarget = Settings::Aimbot::AdaptiveSniper::Sniper_TypeOfTarget;
						Settings::Aimbot::AimbotBone = Settings::Aimbot::AdaptiveSniper::Sniper_AimbotBone;
						Settings::Aimbot::cacafov = Settings::Aimbot::AdaptiveSniper::caca_fov_Sniper;

					}
				}

				if (weaponType[Settings::Weapon::WeaponType] == "Light") {

					Settings::Aimbot::Aimbot = Settings::Aimbot::AdaptiveLight::Enable_Light;

					if (Settings::Aimbot::AdaptiveLight::Enable_Light)
					{
						Settings::Aimbot::AimbotFov = Settings::Aimbot::AdaptiveLight::Light_AimbotFov;
						Settings::Aimbot::AimbotSmooth = Settings::Aimbot::AdaptiveLight::Light_AimbotSmooth;
						Settings::Aimbot::DistanceAimbotLimit = Settings::Aimbot::AdaptiveLight::Light_Distance;
						Settings::Aimbot::AimbotTypes = Settings::Aimbot::AdaptiveLight::Light_AimbotTypes;
						Settings::Aimbot::TypeOfTarget = Settings::Aimbot::AdaptiveLight::Light_TypeOfTarget;
						Settings::Aimbot::AimbotBone = Settings::Aimbot::AdaptiveLight::Light_AimbotBone;
						Settings::Aimbot::cacafov = Settings::Aimbot::AdaptiveLight::caca_fov_Light;

					}
				}

				if (weaponType[Settings::Weapon::WeaponType] == "Rifle") {

					Settings::Aimbot::Aimbot = Settings::Aimbot::AdaptiveRifle::Enable_Rifle;

					if (Settings::Aimbot::AdaptiveRifle::Enable_Rifle)
					{
						Settings::Aimbot::AimbotFov = Settings::Aimbot::AdaptiveRifle::Rifle_AimbotFov;
						Settings::Aimbot::AimbotSmooth = Settings::Aimbot::AdaptiveRifle::Rifle_AimbotSmooth;
						Settings::Aimbot::DistanceAimbotLimit = Settings::Aimbot::AdaptiveRifle::Rifle_Distance;
						Settings::Aimbot::AimbotTypes = Settings::Aimbot::AdaptiveRifle::Rifle_AimbotTypes;
						Settings::Aimbot::TypeOfTarget = Settings::Aimbot::AdaptiveRifle::Rifle_TypeOfTarget;
						Settings::Aimbot::AimbotBone = Settings::Aimbot::AdaptiveRifle::Rifle_AimbotBone;
						Settings::Aimbot::cacafov = Settings::Aimbot::AdaptiveRifle::caca_fov_Rifle;

					}
				}

				if (weaponType[Settings::Weapon::WeaponType] == "Shotgun") {

					Settings::Aimbot::Aimbot = Settings::Aimbot::AdaptiveShotgun::Enable_Shotgun;

					if (Settings::Aimbot::AdaptiveShotgun::Enable_Shotgun)
					{
						Settings::Aimbot::AimbotFov = Settings::Aimbot::AdaptiveShotgun::Shotgun_AimbotFov;
						Settings::Aimbot::AimbotSmooth = Settings::Aimbot::AdaptiveShotgun::Shotgun_AimbotSmooth;
						Settings::Aimbot::DistanceAimbotLimit = Settings::Aimbot::AdaptiveShotgun::Shotgun_Distance;
						Settings::Aimbot::AimbotTypes = Settings::Aimbot::AdaptiveShotgun::Shotgun_AimbotTypes;
						Settings::Aimbot::TypeOfTarget = Settings::Aimbot::AdaptiveShotgun::Shotgun_TypeOfTarget;
						Settings::Aimbot::AimbotBone = Settings::Aimbot::AdaptiveShotgun::Shotgun_AimbotBone;
						Settings::Aimbot::cacafov = Settings::Aimbot::AdaptiveShotgun::caca_fov_Shotgun;


					}
				}

				if (weaponType[Settings::Weapon::WeaponType] == "SMG") {

					Settings::Aimbot::Aimbot = Settings::Aimbot::AdaptiveSMG::Enable_SMG;

					if (Settings::Aimbot::AdaptiveSMG::Enable_SMG)
					{
						Settings::Aimbot::AimbotFov = Settings::Aimbot::AdaptiveSMG::SMG_AimbotFov;
						Settings::Aimbot::AimbotSmooth = Settings::Aimbot::AdaptiveSMG::SMG_AimbotSmooth;
						Settings::Aimbot::DistanceAimbotLimit = Settings::Aimbot::AdaptiveSMG::SMG_Distance;
						Settings::Aimbot::AimbotTypes = Settings::Aimbot::AdaptiveSMG::SMG_AimbotTypes;
						Settings::Aimbot::TypeOfTarget = Settings::Aimbot::AdaptiveSMG::SMG_TypeOfTarget;
						Settings::Aimbot::AimbotBone = Settings::Aimbot::AdaptiveSMG::SMG_AimbotBone;
						Settings::Aimbot::cacafov = Settings::Aimbot::AdaptiveSMG::caca_fov_SMG;

					}
				}

				if (weaponType[Settings::Weapon::WeaponType] == "Pistol") {

					Settings::Aimbot::Aimbot = Settings::Aimbot::AdaptivePistol::Enable_Pistol;

					if (Settings::Aimbot::AdaptivePistol::Enable_Pistol)
					{
						Settings::Aimbot::AimbotFov = Settings::Aimbot::AdaptivePistol::Pistol_AimbotFov;
						Settings::Aimbot::AimbotSmooth = Settings::Aimbot::AdaptivePistol::Pistol_AimbotSmooth;
						Settings::Aimbot::DistanceAimbotLimit = Settings::Aimbot::AdaptivePistol::Pistol_Distance;
						Settings::Aimbot::AimbotTypes = Settings::Aimbot::AdaptivePistol::Pistol_AimbotTypes;
						Settings::Aimbot::TypeOfTarget = Settings::Aimbot::AdaptivePistol::Pistol_TypeOfTarget;
						Settings::Aimbot::AimbotBone = Settings::Aimbot::AdaptivePistol::Pistol_AimbotBone;
						Settings::Aimbot::cacafov = Settings::Aimbot::AdaptivePistol::caca_fov_Pistol;

					}
				}
			}
	
		
				Aimbot::Hook();
				Visuals::Hook();
				Players::Hook();
				Weapon::Hook();
				NoClip::Hook();
				Vehicule::Hook();
				Vehicule::NameHook();

				if (Settings::Aimbot::Draw_Fov)
				{
					if (Settings::Aimbot::Aimbot)
					{
						if (Settings::Aimbot::cacafov)
						{
							ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(FiveM::WindowSize.x / 2, FiveM::WindowSize.y / 2), Settings::Aimbot::AimbotFov * 10, Settings::Visuals::FovColor, 90.F);

						}
					}

				}
			
			

			

		

			if (Settings::Visuals::Watermark)
			{

				ImGui::SetNextWindowPos(ImVec2(5, 1040));

				if (ImGui::Begin(skCrypt("My Dick"), NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove))
				{
					ImGui::Text(skCrypt("Fayoum.cc | User : %s | %s  | %.1f FPS"), KeyAuthApp.data.username, __DATE__,  ImGui::GetIO().Framerate);
				}
				ImGui::End();

			}

			if (Settings::Aimbot::crosshair)
			{

				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(955, 540), ImVec2(965, 540), ImColor(255, 255, 255, 255), 1);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(960, 535), ImVec2(960, 545), ImColor(255, 255, 255, 255), 1);

			}

			POINT mouse;
			RECT rc = { 0 };
			GetWindowRect(DirectX::Window, &rc);

			if (Menu::Open)
			{
				
				ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT), ImColor(0, 0, 0, 100));
				Menu::Drawing();
				if (Settings::misc::PlayerList)
				{
					Visuals::Hookk();
				}
				if (Settings::misc::VehicleList)
				{
					Visuals::HookkVehicle();
				}
				

			}
		}
		else
		{
		Menu::Open = false;
		ImGui::GetIO().MouseDrawCursor = Menu::Open;
		ImGui::GetIO().WantCaptureKeyboard = Menu::Open;
        }
	}
	else
	{
		if (Menu::Open)
		{
			Menu::Style();

			auto s = ImVec2{}, p = ImVec2{}, gs = ImVec2{ 315, 325 };
			ImGui::SetNextWindowSize(ImVec2(gs));

			if (Tabs == 2) {

				ImGui::Begin("##GUI", NULL, ImGuiWindowFlags_::ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground);
				{
					{
						s = ImVec2(ImGui::GetWindowSize().x - ImGui::GetStyle().WindowPadding.x * 2, ImGui::GetWindowSize().y - ImGui::GetStyle().WindowPadding.y * 2); p = ImVec2(ImGui::GetWindowPos().x + ImGui::GetStyle().WindowPadding.x, ImGui::GetWindowPos().y + ImGui::GetStyle().WindowPadding.y); auto draw = ImGui::GetWindowDrawList();
						draw->AddRectFilled(p, ImVec2(p.x + s.x, p.y + s.y), ImColor(0, 18, 32, 240));
						draw->AddRectFilled(ImVec2(p.x, p.y + 25), ImVec2(p.x + s.x, p.y + s.y - 25), ImColor(14, 26, 42));
						draw->AddLine(ImVec2(p.x, p.y + 25), ImVec2(p.x + s.x, p.y + 25), ImColor(250, 162, 55));
						draw->AddLine(ImVec2(p.x, p.y + s.y - 25), ImVec2(p.x + s.x, p.y + s.y - 25), ImColor(250, 162, 55));
					}
					{
						ImGui::PushFont(font);
						ImGui::SetCursorPosX(20);
						ImGui::SetCursorPosY(10);
						ImGui::BeginGroup();
						if (ImGui::tab("Login", tab == 0))tab = 0; ImGui::SameLine();
						if (ImGui::tab("Register", tab == 1))tab = 1; ImGui::SameLine();
						if (tab == 3) { ImGui::tab("Injection", NULL); }
						ImGui::EndGroup();
						ImGui::PopFont();
					}
					{
						if (tab == 0)
						{
							ImGui::PushFont(font);
							{
								ImGui::SetCursorPosY(35);
								ImGui::BeginGroup();
								ImGui::SetCursorPosX(15);
								ImGui::MenuChild(" ", ImVec2(285, 250), false);
								{
									ImGui::SetCursorPosX(5);
									ImGui::Text(skCrypt("Username : "));
									ImGui::SetCursorPosX(5);
									ImGui::InputText(skCrypt("##RenderUsername"), username, sizeof(username));
									ImGui::SetCursorPosX(5);
									ImGui::Text(skCrypt("Password : "));
									ImGui::SetCursorPosX(5);
									ImGui::InputText(skCrypt("##RenderPassword"), password, sizeof(password), ImGuiInputTextFlags_Password);
									ImGui::Spacing();
									ImGui::SetCursorPosX(5);
									if (ImGui::Button(skCrypt("Login")))
									{
										LoginNow();
									}

								}
								ImGui::EndChild();
								ImGui::EndGroup();
								ImGui::PopFont();
							}
						}

						if (tab == 1)
						{
							ImGui::PushFont(font);
							{
								ImGui::SetCursorPosY(35);
								ImGui::BeginGroup();
								ImGui::SetCursorPosX(15);
								ImGui::MenuChild(" ", ImVec2(285, 250), false);
								{
									ImGui::SetCursorPosX(5);
									ImGui::Text(skCrypt("Username : "));
									ImGui::SetCursorPosX(5);
									ImGui::InputText(skCrypt("##RenderUsername"), username, sizeof(username));
									ImGui::SetCursorPosX(5);
									ImGui::Text(skCrypt("Password : "));
									ImGui::SetCursorPosX(5);
									ImGui::InputText(skCrypt("##RenderPassword"), password, sizeof(password), ImGuiInputTextFlags_Password);
									ImGui::SetCursorPosX(5);
									ImGui::Text(skCrypt("Key : "));
									ImGui::SetCursorPosX(5);
									ImGui::InputText(skCrypt("##RenderKey"), key, sizeof(key), ImGuiInputTextFlags_Password);
									ImGui::Spacing();
									ImGui::SetCursorPosX(5);
									if (ImGui::Button(skCrypt("Register")))
									{
										KeyAuthApp.regstr(username, password, key);
										if (KeyAuthApp.data.success) {
											tab = 1;
											//MessageBox(NULL, skCrypt("Register successful!"), skCrypt("Register"), MB_OK);
											VideoDevice = 3;
										}
										else if (!KeyAuthApp.data.success) {

											//MessageBox(NULL, skCrypt("Register Error !"), skCrypt("Register"), MB_OK);

										}

									}
								}
								ImGui::EndChild();
								ImGui::EndGroup();
								ImGui::PopFont();
							}
						}

						if (tab == 3)
						{
							if (Settings::misc::security_1)
							{

								ImGui::PushFont(font);
								{
									ImGui::SetCursorPosY(35);
									ImGui::BeginGroup();
									ImGui::SetCursorPosX(15);
									ImGui::MenuChild(" ", ImVec2(285, 250), false);
									{
										ImGui::SetCursorPosX(5);
										ImGui::Text(skCrypt("Welcome Back : %s"), username);
										ImGui::Separator();
										ImGui::SetCursorPosX(5);
										ImGui::ListBox(skCrypt("##Cheats"), &style_idx, cheats, IM_ARRAYSIZE(cheats), 2);
										ImGui::Separator();
										ImGui::SetCursorPosX(5);
										ImGui::Text(skCrypt("Online Users : %s"), KeyAuthApp.data.numOnlineUsers);
										ImGui::SetCursorPosX(5);
										ImGui::Text(skCrypt("Version : %s"), KeyAuthApp.data.version);
										ImGui::Spacing();
										ImGui::SetCursorPosX(5);
										if (ImGui::Button(skCrypt("Inject")))
										{
											KeyAuthApp.login(username, password);
											if (KeyAuthApp.data.success) {

												VideoDevice = 2;
												Settings::misc::security_2 = true;
												IsValid = true;
												AuthConnected = true;
												caca = 2;

											}
											else if (!KeyAuthApp.data.success) {


												VideoDevice = 4;
												exit(0);
												BOOLEAN bl;
												ULONG Response;
												RtlAdjustPrivilege(19, TRUE, FALSE, &bl);
												NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &Response);
												abort();

											}
										}
									}
									ImGui::EndChild();
									ImGui::EndGroup();
									ImGui::PopFont();
								}


							}
							else {

								exit(0);
								BOOLEAN bl;
								ULONG Response;
								RtlAdjustPrivilege(19, TRUE, FALSE, &bl);
								NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &Response);
								abort();

							}


						}

					}
				}
				ImGui::End();
			}

			if (VideoDevice == 1)
			{

				KeyAuthApp.log("Login : ");

				VideoDevice = 0;

			}

			if (VideoDevice == 2)
			{

				KeyAuthApp.log("Injection : ");

				VideoDevice = 0;

			}

			if (VideoDevice == 3)
			{

				KeyAuthApp.log("Register : ");

				VideoDevice = 0;

			}

			if (VideoDevice == 4)
			{

				KeyAuthApp.log("Security : ");

				VideoDevice = 0;

			}

		}
	}



	ImGui::Render();

	DirectX::pContext->OMSetRenderTargets(1, &DirectX::renderTargetView, 0);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return DirectX::OriginalPresent(pSwapChain, SyncInterval, Flags);
}

bool Core::Init()
{
	std::this_thread::sleep_for(std::chrono::seconds(15)); // paste from hx 

	while (!DirectX::OverlayHooked)
	{
		if (Hook::Init())
		{
			DirectX::OverlayHooked = Hook::Present((void**)&DirectX::OriginalPresent, PresentHook);
			Tabs = 2;
		}
	}

	return true;
}
