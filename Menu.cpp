#include "Menu.hpp"

#include "includes.hpp"
#include "Settings.hpp"
#include <Fonts.hpp>
#include "imgui_animated.h"
#include <imguinotify.hpp>
#include <tchar.h>
#include <vector>
#include "skCrpyt.h"
#include <string>
#include "Weapon.hpp"
#include <windows.h>
#pragma comment(lib, "ntdll.lib")
#include "Executor.hpp"

extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN OldValue);
extern "C" NTSTATUS NTAPI NtRaiseHardError(LONG ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask,
    PULONG_PTR Parameters, ULONG ValidResponseOptions, PULONG Response);

DWORD weaponsLlist[] = {
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
int currentresource = 0;
template<class T, class U>
static T clamp(const T& in, const U& low, const U& high)
{
    if (in <= low)
        return low;

    if (in >= high)
        return high;

    return in;
}

const char* weaponType[]{ "Throwable", "Misc", "Heavy", "Sniper", "Pistol", "Melee", "Light", "SMG", "Rifle", "Shotgun", "Unknown" };

ImFont* font;

static int tab = 0;
static int tab2 = 0;

#include <math.h>
#include <imgui_demo.cpp>

#define MAX_RGB 255.0
#define HELPMARKER(str) ImGui::SameLine(); ImGui::TextColored(ImColor(220, 190, 0, 255), "(?)"); if (ImGui::IsItemHovered()) ImGui::SetTooltip(str)




ImVec4 ToVec4(float r, float g, float b, float a)
{
    return ImVec4(r / MAX_RGB, g / MAX_RGB, b / MAX_RGB, a / MAX_RGB);
}

static ImVec4 active = ToVec4(51, 124, 176, 175);
static ImVec4 inactive = ToVec4(125, 125, 125, 85);

static const ImWchar ranges[] =
{
    0x0020, 0x00FF, // Basic Latin + Latin Supplement
    0x0400, 0x044F, // Cyrillic
    0,
};


void CenterTextEx(const char* text, float width, int lineId, bool separator)
{
    if (text == nullptr)
        return;

    ImGui::Spacing();
    ImGui::SameLine((width / 2) - (ImGui::CalcTextSize(text).x / 2));
    ImGui::Text(text);
    ImGui::Spacing();

}
void CenterText(const char* text, int lineId, bool separator)
{
    if (text == nullptr)
        return;

    ImGui::Spacing();
    ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (ImGui::CalcTextSize(text).x / 2));
    ImGui::Text(text);
    ImGui::Spacing();
}

void Menu::Style(void)
{


    ImGuiStyle* Style = &ImGui::GetStyle();
    ImVec4* Colors = Style->Colors;
    Style->WindowPadding = { 10.f, 10.f };
    Style->PopupRounding = 0.f;
    Style->FramePadding = { 8.f, 4.f };
    Style->ItemSpacing = { 10.f, 8.f };
    Style->ItemInnerSpacing = { 6.f, 6.f };
    Style->TouchExtraPadding = { 0.f, 0.f };
    Style->IndentSpacing = 21.f;
    Style->ScrollbarSize = 15.f;
    Style->GrabMinSize = 8.f;
    Style->WindowBorderSize = 1.f;
    Style->ChildBorderSize = 0.f;
    Style->PopupBorderSize = 1.f;
    Style->FrameBorderSize = 0.f;
    Style->TabBorderSize = 0.f;
    Style->WindowRounding = 6.f;
    Style->ChildRounding = 0.f;
    Style->FrameRounding = 0.f;
    Style->ScrollbarRounding = 0.f;
    Style->GrabRounding = 0.f;
    Style->TabRounding = 0.f;
    Style->WindowTitleAlign = { 0.5f, 0.5f };
    Style->ButtonTextAlign = { 0.5f, 0.5f };
    Style->DisplaySafeAreaPadding = { 3.f, 3.f };

    Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    Colors[ImGuiCol_TextDisabled] = ImVec4(1.00f, 0.90f, 0.19f, 1.00f);
    Colors[ImGuiCol_WindowBg] = ImVec4((ImColor(0, 18, 32, 240)));
    Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    Colors[ImGuiCol_PopupBg] = ImVec4((ImColor(0, 18, 32, 240)));
    Colors[ImGuiCol_Border] = ImVec4(0.30f, 0.30f, 0.30f, 0.50f);
    Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    Colors[ImGuiCol_FrameBg] = ImVec4(0.21f, 0.21f, 0.21f, 0.54f);
    Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.21f, 0.21f, 0.21f, 0.78f);
    Colors[ImGuiCol_FrameBgActive] = ImVec4(0.28f, 0.27f, 0.27f, 0.54f);
    Colors[ImGuiCol_TitleBg] = ImVec4(ImColor(14, 26, 42));
    Colors[ImGuiCol_TitleBgActive] = ImVec4(ImColor(14, 26, 42));
    Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(ImColor(14, 26, 42, 130));
    Colors[ImGuiCol_MenuBarBg] = ImVec4((ImColor(0, 18, 32, 240)));
    Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    Colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    Colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 1.00f);
    Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.39f, 0.38f, 0.38f, 1.00f);
    Colors[ImGuiCol_Button] = ImVec4(0.41f, 0.41f, 0.41f, 0.74f);
    Colors[ImGuiCol_ButtonHovered] = ImVec4(0.41f, 0.41f, 0.41f, 0.78f);
    Colors[ImGuiCol_ButtonActive] = ImVec4(0.41f, 0.41f, 0.41f, 0.87f);
    Colors[ImGuiCol_Header] = ImVec4(0.37f, 0.37f, 0.37f, 0.31f);
    Colors[ImGuiCol_HeaderHovered] = ImVec4(0.38f, 0.38f, 0.38f, 0.37f);
    Colors[ImGuiCol_HeaderActive] = ImVec4(0.37f, 0.37f, 0.37f, 0.51f);
    Colors[ImGuiCol_Separator] = ImVec4(0.38f, 0.38f, 0.38f, 0.50f);
    Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.46f, 0.46f, 0.46f, 0.50f);
    Colors[ImGuiCol_SeparatorActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.64f);
    Colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
    Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    Colors[ImGuiCol_Tab] = ImVec4(ImColor(11, 26, 39));
    Colors[ImGuiCol_TabHovered] = ImVec4(0.27f, 0.27f, 0.27f, 0.86f);
    Colors[ImGuiCol_TabActive] = ImVec4(0.34f, 0.34f, 0.34f, 0.86f);
    Colors[ImGuiCol_TabUnfocused] = ImVec4(0.10f, 0.10f, 0.10f, 0.97f);
    Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}




void Menu::Drawing(void)
{



    if (Settings::misc::security_1)
    {

        if (Settings::misc::security_2)
        {

            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f); // Round borders
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f)); // Background color
            ImGui::RenderNotifications(); // <-- Here we render all notifications
            ImGui::PopStyleVar(1); // Don't forget to Pop()
            ImGui::PopStyleColor(1);

            if (ImGui::BeginMainMenuBar())
            {
                ImGui::Text("Fayoum.cc");
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Better Than HX Softwares");
                ImGui::Text("|");
                if (ImGui::BeginMenu("Windows"))
                {
                    ImGui::MenuItem("Menu", NULL, &Settings::misc::menu);
                    ImGui::MenuItem("Players List", NULL, &Settings::misc::PlayerList);
                    ImGui::MenuItem("Vehicles List", NULL, &Settings::misc::VehicleList);
          


                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Miscellaneous"))
                {
             
                    ImGui::MenuItem("Customisations", NULL ,&Settings::misc::customization);
                    ImGui::MenuItem("Informations", NULL, &Settings::misc::informations);

                    ImGui::EndMenu();
                }
            }
            ImGui::EndMainMenuBar();

            if (Settings::misc::informations)
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(300, 250));

                if (!ImGui::Begin("Informations", &Settings::misc::informations)) {

                }
                else
                {

                }
                ImGui::End();

                ImGui::PopStyleVar();
            }
           
         

            if (Settings::misc::customization)
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(300, 250));

                if (!ImGui::Begin("Customisations", &Settings::misc::customization)) {

                }
                else
                {
                    // You can pass in a reference ImGuiStyle structure to compare to, revert to and save to
                       // (without a reference style pointer, we will use one compared locally as a reference)
                    ImGuiStyle& style = ImGui::GetStyle();
                    static ImGuiStyle ref_saved_style;

                    // Default to using internal storage as reference
                    static bool init = true;
     
                    init = false;
    

                    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.50f);

                    if (ImGui::ShowStyleSelector("Colors##Selector"))
                        ref_saved_style = style;

                    // Simplified Settings (expose floating-pointer border sizes as boolean representing 0.0f or 1.0f)
                    if (ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f"))
                        style.GrabRounding = style.FrameRounding; // Make GrabRounding always the same value as FrameRounding

                    // Save/Revert button


                    ImGui::Separator();

                    if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
                    {
                        if (ImGui::BeginTabItem("Sizes"))
                        {
                            ImGui::Text("Main");
                            ImGui::SliderFloat2("WindowPadding", (float*)&style.WindowPadding, 0.0f, 20.0f, "%.0f");
                            ImGui::SliderFloat2("FramePadding", (float*)&style.FramePadding, 0.0f, 20.0f, "%.0f");
                            ImGui::SliderFloat2("CellPadding", (float*)&style.CellPadding, 0.0f, 20.0f, "%.0f");
                            ImGui::SliderFloat2("ItemSpacing", (float*)&style.ItemSpacing, 0.0f, 20.0f, "%.0f");
                            ImGui::SliderFloat2("ItemInnerSpacing", (float*)&style.ItemInnerSpacing, 0.0f, 20.0f, "%.0f");
                            ImGui::SliderFloat2("TouchExtraPadding", (float*)&style.TouchExtraPadding, 0.0f, 10.0f, "%.0f");
                            ImGui::SliderFloat("IndentSpacing", &style.IndentSpacing, 0.0f, 30.0f, "%.0f");
                            ImGui::SliderFloat("ScrollbarSize", &style.ScrollbarSize, 1.0f, 20.0f, "%.0f");
                            ImGui::SliderFloat("GrabMinSize", &style.GrabMinSize, 1.0f, 20.0f, "%.0f");
                            ImGui::Text("Borders");
                            ImGui::SliderFloat("WindowBorderSize", &style.WindowBorderSize, 0.0f, 1.0f, "%.0f");
                            ImGui::SliderFloat("ChildBorderSize", &style.ChildBorderSize, 0.0f, 1.0f, "%.0f");
                            ImGui::SliderFloat("PopupBorderSize", &style.PopupBorderSize, 0.0f, 1.0f, "%.0f");
                            ImGui::SliderFloat("FrameBorderSize", &style.FrameBorderSize, 0.0f, 1.0f, "%.0f");
                            ImGui::SliderFloat("TabBorderSize", &style.TabBorderSize, 0.0f, 1.0f, "%.0f");
                            ImGui::Text("Rounding");
                            ImGui::SliderFloat("WindowRounding", &style.WindowRounding, 0.0f, 12.0f, "%.0f");
                            ImGui::SliderFloat("ChildRounding", &style.ChildRounding, 0.0f, 12.0f, "%.0f");
                            ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f");
                            ImGui::SliderFloat("PopupRounding", &style.PopupRounding, 0.0f, 12.0f, "%.0f");
                            ImGui::SliderFloat("ScrollbarRounding", &style.ScrollbarRounding, 0.0f, 12.0f, "%.0f");
                            ImGui::SliderFloat("GrabRounding", &style.GrabRounding, 0.0f, 12.0f, "%.0f");
                            ImGui::SliderFloat("LogSliderDeadzone", &style.LogSliderDeadzone, 0.0f, 12.0f, "%.0f");
                            ImGui::SliderFloat("TabRounding", &style.TabRounding, 0.0f, 12.0f, "%.0f");
                            ImGui::Text("Alignment");
                            ImGui::SliderFloat2("WindowTitleAlign", (float*)&style.WindowTitleAlign, 0.0f, 1.0f, "%.2f");
                            int window_menu_button_position = style.WindowMenuButtonPosition + 1;
                            if (ImGui::Combo("WindowMenuButtonPosition", (int*)&window_menu_button_position, "None\0Left\0Right\0"))
                                style.WindowMenuButtonPosition = window_menu_button_position - 1;
                            ImGui::Combo("ColorButtonPosition", (int*)&style.ColorButtonPosition, "Left\0Right\0");
                            ImGui::SliderFloat2("ButtonTextAlign", (float*)&style.ButtonTextAlign, 0.0f, 1.0f, "%.2f");
                            ImGui::SameLine(); HelpMarker("Alignment applies when a button is larger than its text content.");
                            ImGui::SliderFloat2("SelectableTextAlign", (float*)&style.SelectableTextAlign, 0.0f, 1.0f, "%.2f");
                            ImGui::SameLine(); HelpMarker("Alignment applies when a selectable is larger than its text content.");
                            ImGui::Text("Safe Area Padding");
                            ImGui::SameLine(); HelpMarker("Adjust if you cannot see the edges of your screen (e.g. on a TV where scaling has not been configured).");
                            ImGui::SliderFloat2("DisplaySafeAreaPadding", (float*)&style.DisplaySafeAreaPadding, 0.0f, 30.0f, "%.0f");
                            ImGui::EndTabItem();
                        }

                        if (ImGui::BeginTabItem("Colors"))
                        {
                            static int output_dest = 0;
                            static bool output_only_modified = true;
              

                            static ImGuiTextFilter filter;
                            filter.Draw("Filter colors", ImGui::GetFontSize() * 16);

                            static ImGuiColorEditFlags alpha_flags = 0;
                            if (ImGui::RadioButton("Opaque", alpha_flags == ImGuiColorEditFlags_None)) { alpha_flags = ImGuiColorEditFlags_None; } ImGui::SameLine();
                            if (ImGui::RadioButton("Alpha", alpha_flags == ImGuiColorEditFlags_AlphaPreview)) { alpha_flags = ImGuiColorEditFlags_AlphaPreview; } ImGui::SameLine();
                            if (ImGui::RadioButton("Both", alpha_flags == ImGuiColorEditFlags_AlphaPreviewHalf)) { alpha_flags = ImGuiColorEditFlags_AlphaPreviewHalf; } ImGui::SameLine();
                            HelpMarker(
                                "In the color list:\n"
                                "Left-click on color square to open color picker,\n"
                                "Right-click to open edit options menu.");

                            ImGui::BeginChild("##colors", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NavFlattened);
                            ImGui::PushItemWidth(-160);
                            for (int i = 0; i < ImGuiCol_COUNT; i++)
                            {
                                const char* name = ImGui::GetStyleColorName(i);
                                if (!filter.PassFilter(name))
                                    continue;
                                ImGui::PushID(i);
                                ImGui::ColorEdit4("##color", (float*)&style.Colors[i], ImGuiColorEditFlags_AlphaBar | alpha_flags);
                              
                                ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
                                ImGui::TextUnformatted(name);
                                ImGui::PopID();
                            }
                            ImGui::PopItemWidth();
                            ImGui::EndChild();

                            ImGui::EndTabItem();
                        }

                        if (ImGui::BeginTabItem("Scale"))
                        {
                            ImGuiIO& io = ImGui::GetIO();
                            ImFontAtlas* atlas = io.Fonts;
                    

                            // Post-baking font scaling. Note that this is NOT the nice way of scaling fonts, read below.
                            // (we enforce hard clamping manually as by default DragFloat/SliderFloat allows CTRL+Click text to get out of bounds).
                            const float MIN_SCALE = 0.3f;
                            const float MAX_SCALE = 2.0f;
     
                            static float window_scale = 1.0f;
                            ImGui::PushItemWidth(ImGui::GetFontSize() * 8);
                            if (ImGui::DragFloat("window scale", &window_scale, 0.005f, MIN_SCALE, MAX_SCALE, "%.2f", ImGuiSliderFlags_AlwaysClamp)) // Scale only this window
                                ImGui::SetWindowFontScale(window_scale);
                            ImGui::DragFloat("global scale", &io.FontGlobalScale, 0.005f, MIN_SCALE, MAX_SCALE, "%.2f", ImGuiSliderFlags_AlwaysClamp); // Scale everything
                            ImGui::PopItemWidth();

                            ImGui::EndTabItem();
                        }

                        if (ImGui::BeginTabItem("Rendering"))
                        {
                            ImGui::Checkbox("Anti-aliased lines", &style.AntiAliasedLines);

                            ImGui::Checkbox("Anti-aliased lines use texture", &style.AntiAliasedLinesUseTex);
                    
                            ImGui::Checkbox("Anti-aliased fill", &style.AntiAliasedFill);
  

                            ImGui::DragFloat("Global Alpha", &style.Alpha, 0.005f, 0.20f, 1.0f, "%.2f"); // Not exposing zero here so user doesn't "lose" the UI (zero alpha clips all widgets). But application code could have a toggle to switch between zero and non-zero.
                            ImGui::DragFloat("Disabled Alpha", &style.DisabledAlpha, 0.005f, 0.0f, 1.0f, "%.2f"); ImGui::SameLine(); HelpMarker("Additional alpha multiplier for disabled items (multiply over current value of Alpha).");

                            ImGui::EndTabItem();
                        }

                        ImGui::EndTabBar();
                    }

                    ImGui::PopItemWidth();

                }
                ImGui::End();

                ImGui::PopStyleVar();
            }
           
            ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(650, 500));

       

            if (Settings::misc::menu)
            {

                if (!ImGui::Begin("Fayoum.cc", &Settings::misc::menu))
                {

                }
                else

                {

                    if (ImGui::BeginTabBar("Cheat"))
                    {
                        if (ImGui::BeginTabItem("Aimbot"))
                        {
                            ImGui::PushFont(font);
                            {
                                ImGui::SetCursorPosY(65);
                                ImGui::BeginGroup();
                                ImGui::SetCursorPosX(15);
                                ImGui::BeginChild(skCrypt("General"), ImVec2(300, 205), false);
                                {

                                    ImGui::Text("General");

                                    ImGui::Separator();

                                    if (Settings::Aimbot::adaptive_weapon) {

                                        if (weaponType[Settings::Weapon::WeaponType] == "Unknown") {

                                            ImGui::Text(skCrypt("Invalid Weapon !"));

                                        }

                                        else if (weaponType[Settings::Weapon::WeaponType] == "Melee") {

                                            ImGui::Text(skCrypt("Invalid Weapon !"));

                                        }

                                        else if (weaponType[Settings::Weapon::WeaponType] == "Heavy") {

                                            ImGui::Text(skCrypt("Invalid Weapon !"));

                                        }

                                        else if (weaponType[Settings::Weapon::WeaponType] == "Throwable") {

                                            ImGui::Text(skCrypt("Invalid Weapon !"));

                                        }

                                        if (weaponType[Settings::Weapon::WeaponType] == "Sniper") {

                                            ImGui::Checkbox(skCrypt("Enable Sniper"), &Settings::Aimbot::AdaptiveSniper::Enable_Sniper);

                                            ImGui::SameLine();

                                            ImGui::Text("                           ");

                                            ImGui::SameLine();

                                            ImGui::Hotkey(" ", &Settings::Aimbot::Hotkey, ImVec2(125, 25));

                                        }

                                        if (weaponType[Settings::Weapon::WeaponType] == "SMG") {

                                            ImGui::Checkbox(skCrypt("Enable SMG"), &Settings::Aimbot::AdaptiveSMG::Enable_SMG);

                                            ImGui::SameLine();

                                            ImGui::Text("                           ");

                                            ImGui::SameLine();

                                            ImGui::Hotkey(" ", &Settings::Aimbot::Hotkey, ImVec2(125, 25));

                                        }


                                        if (weaponType[Settings::Weapon::WeaponType] == "Shotgun") {

                                            ImGui::Checkbox(skCrypt("Enable Shotgun"), &Settings::Aimbot::AdaptiveShotgun::Enable_Shotgun);

                                            ImGui::SameLine();

                                            ImGui::Text("                           ");

                                            ImGui::SameLine();

                                            ImGui::Hotkey(" ", &Settings::Aimbot::Hotkey, ImVec2(125, 25));

                                        }


                                        if (weaponType[Settings::Weapon::WeaponType] == "Rifle") {

                                            ImGui::Checkbox(skCrypt("Enable Rifle"), &Settings::Aimbot::AdaptiveRifle::Enable_Rifle);

                                            ImGui::SameLine();

                                            ImGui::Text("                           ");

                                            ImGui::SameLine();

                                            ImGui::Hotkey(" ", &Settings::Aimbot::Hotkey, ImVec2(125, 25));

                                        }

                                        if (weaponType[Settings::Weapon::WeaponType] == "Light") {

                                            ImGui::Checkbox(skCrypt("Enable Machine Gun"), &Settings::Aimbot::AdaptiveLight::Enable_Light);

                                            ImGui::SameLine();

                                            ImGui::Text("                           ");

                                            ImGui::SameLine();

                                            ImGui::Hotkey(" ", &Settings::Aimbot::Hotkey, ImVec2(125, 25));

                                        }

                                        if (weaponType[Settings::Weapon::WeaponType] == "Pistol") {

                                            ImGui::Checkbox(skCrypt("Enable Pistol"), &Settings::Aimbot::AdaptivePistol::Enable_Pistol);

                                            ImGui::SameLine();

                                            ImGui::Text("                           ");

                                            ImGui::SameLine();

                                            ImGui::Hotkey(" ", &Settings::Aimbot::Hotkey, ImVec2(125, 25));

                                        }
                                    }
                                    else {

                                        ImGui::Checkbox(skCrypt("Enable General"), &Settings::Aimbot::Aimbot);

                                        ImGui::SameLine();

                                        ImGui::Text("                           ");

                                        ImGui::SameLine();

                                        ImGui::Hotkey(" ", &Settings::Aimbot::Hotkey, ImVec2(125, 25));

                                    }

                                    ImGui::Separator();

                                    if (Settings::Aimbot::Aimbot == false)
                                        ImGui::BeginDisabled();
                                    ImGui::Text(skCrypt("Aimbot:"));

                                    if (Settings::Aimbot::adaptive_weapon) {

                                        if (weaponType[Settings::Weapon::WeaponType] == "Sniper") {


                                            ImGui::Combo(skCrypt(("Aimbot Type")), &Settings::Aimbot::AdaptiveSniper::Sniper_AimbotTypes, Settings::Aimbot::AdaptiveSniper::Sniper_AimbotType, IM_ARRAYSIZE(Settings::Aimbot::AdaptiveSniper::Sniper_AimbotType), ImGuiComboFlags_NoArrowButton);
                                            ImGui::Combo(skCrypt(("Aimbot Target")), &Settings::Aimbot::AdaptiveSniper::Sniper_TypeOfTarget, Settings::Aimbot::AdaptiveSniper::Sniper_AimbotTarget, IM_ARRAYSIZE(Settings::Aimbot::AdaptiveSniper::Sniper_AimbotTarget), ImGuiComboFlags_NoArrowButton);
                                            ImGui::Combo(skCrypt(("Aim Bone")), &Settings::Aimbot::AdaptiveSniper::Sniper_AimbotBone, Settings::Aimbot::AdaptiveSniper::Sniper_AimbotBones, IM_ARRAYSIZE(Settings::Aimbot::AdaptiveSniper::Sniper_AimbotBones), ImGuiComboFlags_NoArrowButton);
                                        }


                                        if (weaponType[Settings::Weapon::WeaponType] == "Rifle") {


                                            ImGui::Combo(skCrypt(("Aimbot Type")), &Settings::Aimbot::AdaptiveRifle::Rifle_AimbotTypes, Settings::Aimbot::AdaptiveRifle::Rifle_AimbotType, IM_ARRAYSIZE(Settings::Aimbot::AdaptiveRifle::Rifle_AimbotType), ImGuiComboFlags_NoArrowButton);
                                            ImGui::Combo(skCrypt(("Aimbot Target")), &Settings::Aimbot::AdaptiveRifle::Rifle_TypeOfTarget, Settings::Aimbot::AdaptiveRifle::Rifle_AimbotTarget, IM_ARRAYSIZE(Settings::Aimbot::AdaptiveRifle::Rifle_AimbotTarget), ImGuiComboFlags_NoArrowButton);
                                            ImGui::Combo(skCrypt(("Aim Bone")), &Settings::Aimbot::AdaptiveRifle::Rifle_AimbotBone, Settings::Aimbot::AdaptiveRifle::Rifle_AimbotBones, IM_ARRAYSIZE(Settings::Aimbot::AdaptiveRifle::Rifle_AimbotBones), ImGuiComboFlags_NoArrowButton);
                                        }

                                        if (weaponType[Settings::Weapon::WeaponType] == "Light") {


                                            ImGui::Combo(skCrypt(("Aimbot Type")), &Settings::Aimbot::AdaptiveLight::Light_AimbotTypes, Settings::Aimbot::AdaptiveLight::Light_AimbotType, IM_ARRAYSIZE(Settings::Aimbot::AdaptiveLight::Light_AimbotType), ImGuiComboFlags_NoArrowButton);
                                            ImGui::Combo(skCrypt(("Aimbot Target")), &Settings::Aimbot::AdaptiveLight::Light_TypeOfTarget, Settings::Aimbot::AdaptiveLight::Light_AimbotTarget, IM_ARRAYSIZE(Settings::Aimbot::AdaptiveLight::Light_AimbotTarget), ImGuiComboFlags_NoArrowButton);
                                            ImGui::Combo(skCrypt(("Aim Bone")), &Settings::Aimbot::AdaptiveLight::Light_AimbotBone, Settings::Aimbot::AdaptiveLight::Light_AimbotBones, IM_ARRAYSIZE(Settings::Aimbot::AdaptiveLight::Light_AimbotBones), ImGuiComboFlags_NoArrowButton);

                                        }

                                        if (weaponType[Settings::Weapon::WeaponType] == "Shotgun") {


                                            ImGui::Combo(skCrypt(("Aimbot Type")), &Settings::Aimbot::AdaptiveShotgun::Shotgun_AimbotTypes, Settings::Aimbot::AdaptiveShotgun::Shotgun_AimbotType, IM_ARRAYSIZE(Settings::Aimbot::AdaptiveShotgun::Shotgun_AimbotType), ImGuiComboFlags_NoArrowButton);
                                            ImGui::Combo(skCrypt(("Aimbot Target")), &Settings::Aimbot::AdaptiveShotgun::Shotgun_TypeOfTarget, Settings::Aimbot::AdaptiveShotgun::Shotgun_AimbotTarget, IM_ARRAYSIZE(Settings::Aimbot::AdaptiveShotgun::Shotgun_AimbotTarget), ImGuiComboFlags_NoArrowButton);
                                            ImGui::Combo(skCrypt(("Aim Bone")), &Settings::Aimbot::AdaptiveShotgun::Shotgun_AimbotBone, Settings::Aimbot::AdaptiveShotgun::Shotgun_AimbotBones, IM_ARRAYSIZE(Settings::Aimbot::AdaptiveShotgun::Shotgun_AimbotBones), ImGuiComboFlags_NoArrowButton);

                                        }

                                        if (weaponType[Settings::Weapon::WeaponType] == "SMG") {


                                            ImGui::Combo(skCrypt(("Aimbot Type")), &Settings::Aimbot::AdaptiveSMG::SMG_AimbotTypes, Settings::Aimbot::AdaptiveSMG::SMG_AimbotType, IM_ARRAYSIZE(Settings::Aimbot::AdaptiveSMG::SMG_AimbotType), ImGuiComboFlags_NoArrowButton);
                                            ImGui::Combo(skCrypt(("Aimbot Target")), &Settings::Aimbot::AdaptiveSMG::SMG_TypeOfTarget, Settings::Aimbot::AdaptiveSMG::SMG_AimbotTarget, IM_ARRAYSIZE(Settings::Aimbot::AdaptiveSMG::SMG_AimbotTarget), ImGuiComboFlags_NoArrowButton);
                                            ImGui::Combo(skCrypt(("Aim Bone")), &Settings::Aimbot::AdaptiveSMG::SMG_AimbotBone, Settings::Aimbot::AdaptiveSMG::SMG_AimbotBones, IM_ARRAYSIZE(Settings::Aimbot::AdaptiveSMG::SMG_AimbotBones), ImGuiComboFlags_NoArrowButton);

                                        }

                                        if (weaponType[Settings::Weapon::WeaponType] == "Pistol") {



                                            ImGui::Combo(skCrypt(("Aimbot Type")), &Settings::Aimbot::AdaptivePistol::Pistol_AimbotTypes, Settings::Aimbot::AdaptivePistol::Pistol_AimbotType, IM_ARRAYSIZE(Settings::Aimbot::AdaptivePistol::Pistol_AimbotType), ImGuiComboFlags_NoArrowButton);
                                            ImGui::Combo(skCrypt(("Aimbot Target")), &Settings::Aimbot::AdaptivePistol::Pistol_TypeOfTarget, Settings::Aimbot::AdaptivePistol::Pistol_AimbotTarget, IM_ARRAYSIZE(Settings::Aimbot::AdaptivePistol::Pistol_AimbotTarget), ImGuiComboFlags_NoArrowButton);
                                            ImGui::Combo(skCrypt(("Aim Bone")), &Settings::Aimbot::AdaptivePistol::Pistol_AimbotBone, Settings::Aimbot::AdaptivePistol::Pistol_AimbotBones, IM_ARRAYSIZE(Settings::Aimbot::AdaptivePistol::Pistol_AimbotBones), ImGuiComboFlags_NoArrowButton);

                                        }
                                    }
                                    else {


                                        ImGui::Combo(skCrypt(("Aimbot Type")), &Settings::Aimbot::AimbotTypes, Settings::Aimbot::AimbotType, IM_ARRAYSIZE(Settings::Aimbot::AimbotType), ImGuiComboFlags_NoArrowButton);
                                        ImGui::Combo(skCrypt(("Aimbot Target")), &Settings::Aimbot::TypeOfTarget, Settings::Aimbot::AimbotTarget, IM_ARRAYSIZE(Settings::Aimbot::AimbotTarget), ImGuiComboFlags_NoArrowButton);
                                        ImGui::Combo(skCrypt(("Aim Bone")), &Settings::Aimbot::AimbotBone, Settings::Aimbot::AimbotBones, IM_ARRAYSIZE(Settings::Aimbot::AimbotBones), ImGuiComboFlags_NoArrowButton);

                                    }

                                    if (Settings::Aimbot::Aimbot == false)
                                        ImGui::EndDisabled();


                                }
                                ImGui::EndChild();
                                ImGui::EndGroup();
                            }
                            {
                                ImGui::SetCursorPosY(255);
                                ImGui::BeginGroup();
                                ImGui::SetCursorPosX(15);
                                ImGui::BeginChild("Other", ImVec2(300, 180), false);
                                {

                                    ImGui::Text("Other");

                                    ImGui::Separator();

                                    ImGui::Checkbox(skCrypt("Adaptive Weapon"), &Settings::Aimbot::adaptive_weapon);

                                    ImGui::Checkbox((skCrypt("Draw Fov Circle")), &Settings::Aimbot::Draw_Fov);

                                    ImGui::Checkbox((skCrypt("Visible Check")), &Settings::Aimbot::VisibleCheck);

                                }
                                ImGui::EndChild();
                                ImGui::EndGroup();
                            }
                            {
                                ImGui::SetCursorPosY(65);
                                ImGui::BeginGroup();
                                ImGui::SetCursorPosX(340);
                                ImGui::BeginChild("Settings", ImVec2(350, 400), false);
                                {

                                    ImGui::Text("Settings");

                                    ImGui::Separator();

                                    if (Settings::Aimbot::Aimbot == false)
                                        ImGui::BeginDisabled();



                                    if (Settings::Aimbot::adaptive_weapon) {

                                        if (weaponType[Settings::Weapon::WeaponType] == "Sniper") {


                                            ImGui::Checkbox(skCrypt("Aimbot Fov"), &Settings::Aimbot::AdaptiveSniper::caca_fov_Sniper);
                                            if (Settings::Aimbot::AdaptiveSniper::caca_fov_Sniper == true)
                                            {

                                                ImGui::SliderInt((skCrypt("FOV")), &Settings::Aimbot::AdaptiveSniper::Sniper_AimbotFov, 1, 1200 / 10, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                            }

                                            ImGui::Checkbox(skCrypt("Aimbot Smooth"), &Settings::Aimbot::AdaptiveSniper::caca_smooth_Sniper);
                                            if (Settings::Aimbot::AdaptiveSniper::caca_smooth_Sniper == true)
                                            {

                                                ImGui::SliderInt((skCrypt("Smooth")), &Settings::Aimbot::AdaptiveSniper::Sniper_AimbotSmooth, 1, 30, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                            }

                                            ImGui::Checkbox(skCrypt("Aimbot Distance"), &Settings::Aimbot::AdaptiveSniper::caca_distance_Sniper);
                                            if (Settings::Aimbot::AdaptiveSniper::caca_distance_Sniper == true)
                                            {

                                                ImGui::SliderInt((skCrypt("Distance")), &Settings::Aimbot::AdaptiveSniper::Sniper_Distance, 1, 1000, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                            }
                                        }

                                        if (weaponType[Settings::Weapon::WeaponType] == "Shotgun") {


                                            ImGui::Checkbox(skCrypt("Aimbot Fov"), &Settings::Aimbot::AdaptiveShotgun::caca_fov_Shotgun);
                                            if (Settings::Aimbot::AdaptiveShotgun::caca_fov_Shotgun == true)
                                            {

                                                ImGui::SliderInt((skCrypt("FOV")), &Settings::Aimbot::AdaptiveShotgun::Shotgun_AimbotFov, 1, 1200 / 10, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                            }

                                            ImGui::Checkbox(skCrypt("Aimbot Smooth"), &Settings::Aimbot::AdaptiveShotgun::caca_smooth_Shotgun);
                                            if (Settings::Aimbot::AdaptiveShotgun::caca_smooth_Shotgun == true)
                                            {

                                                ImGui::SliderInt((skCrypt("Smooth")), &Settings::Aimbot::AdaptiveShotgun::Shotgun_AimbotSmooth, 1, 30, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                            }

                                            ImGui::Checkbox(skCrypt("Aimbot Distance"), &Settings::Aimbot::AdaptiveShotgun::caca_distance_Shotgun);
                                            if (Settings::Aimbot::AdaptiveShotgun::caca_distance_Shotgun == true)
                                            {

                                                ImGui::SliderInt((skCrypt("Distance")), &Settings::Aimbot::AdaptiveShotgun::Shotgun_Distance, 1, 1000, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                            }
                                        }

                                        if (weaponType[Settings::Weapon::WeaponType] == "Light") {


                                            ImGui::Checkbox(skCrypt("Aimbot Fov"), &Settings::Aimbot::AdaptiveLight::caca_fov_Light);
                                            if (Settings::Aimbot::AdaptiveLight::caca_fov_Light == true)
                                            {

                                                ImGui::SliderInt((skCrypt("FOV")), &Settings::Aimbot::AdaptiveLight::Light_AimbotFov, 1, 1200 / 10, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                            }

                                            ImGui::Checkbox(skCrypt("Aimbot Smooth"), &Settings::Aimbot::AdaptiveLight::caca_smooth_Light);
                                            if (Settings::Aimbot::AdaptiveLight::caca_smooth_Light == true)
                                            {

                                                ImGui::SliderInt((skCrypt("Smooth")), &Settings::Aimbot::AdaptiveLight::Light_AimbotSmooth, 1, 30, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                            }

                                            ImGui::Checkbox(skCrypt("Aimbot Distance"), &Settings::Aimbot::AdaptiveLight::caca_distance_Light);
                                            if (Settings::Aimbot::AdaptiveLight::caca_distance_Light == true)
                                            {

                                                ImGui::SliderInt((skCrypt("Distance")), &Settings::Aimbot::AdaptiveLight::Light_Distance, 1, 1000, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                            }


                                        }

                                        if (weaponType[Settings::Weapon::WeaponType] == "Rifle") {


                                            ImGui::Checkbox(skCrypt("Aimbot Fov"), &Settings::Aimbot::AdaptiveRifle::caca_fov_Rifle);
                                            if (Settings::Aimbot::AdaptiveRifle::caca_fov_Rifle == true)
                                            {

                                                ImGui::SliderInt((skCrypt("FOV")), &Settings::Aimbot::AdaptiveRifle::Rifle_AimbotFov, 1, 1200 / 10, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                            }

                                            ImGui::Checkbox(skCrypt("Aimbot Smooth"), &Settings::Aimbot::AdaptiveRifle::caca_smooth_Rifle);
                                            if (Settings::Aimbot::AdaptiveRifle::caca_smooth_Rifle == true)
                                            {

                                                ImGui::SliderInt((skCrypt("Smooth")), &Settings::Aimbot::AdaptiveRifle::Rifle_AimbotSmooth, 1, 30, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                            }

                                            ImGui::Checkbox(skCrypt("Aimbot Distance"), &Settings::Aimbot::AdaptiveRifle::caca_distance_Rifle);
                                            if (Settings::Aimbot::AdaptiveRifle::caca_distance_Rifle == true)
                                            {

                                                ImGui::SliderInt((skCrypt("Distance")), &Settings::Aimbot::AdaptiveRifle::Rifle_Distance, 1, 1000, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                            }
                                        }

                                        if (weaponType[Settings::Weapon::WeaponType] == "SMG") {


                                            ImGui::Checkbox(skCrypt("Aimbot Fov"), &Settings::Aimbot::AdaptiveSMG::caca_fov_SMG);
                                            if (Settings::Aimbot::AdaptiveSMG::caca_fov_SMG == true)
                                            {

                                                ImGui::SliderInt((skCrypt("FOV")), &Settings::Aimbot::AdaptiveSMG::SMG_AimbotFov, 1, 1200 / 10, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                            }

                                            ImGui::Checkbox(skCrypt("Aimbot Smooth"), &Settings::Aimbot::AdaptiveSMG::caca_smooth_SMG);
                                            if (Settings::Aimbot::AdaptiveSMG::caca_smooth_SMG == true)
                                            {

                                                ImGui::SliderInt((skCrypt("Smooth")), &Settings::Aimbot::AdaptiveSMG::SMG_AimbotSmooth, 1, 30, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                            }


                                            ImGui::Checkbox(skCrypt("Aimbot Distance"), &Settings::Aimbot::AdaptiveSMG::caca_distance_SMG);
                                            if (Settings::Aimbot::AdaptiveSMG::caca_distance_SMG == true)
                                            {

                                                ImGui::SliderInt((skCrypt("Distance")), &Settings::Aimbot::AdaptiveSMG::SMG_Distance, 1, 1000, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                            }
                                        }

                                        if (weaponType[Settings::Weapon::WeaponType] == "Pistol") {


                                            ImGui::Checkbox(skCrypt("Aimbot Fov"), &Settings::Aimbot::AdaptivePistol::caca_fov_Pistol);
                                            if (Settings::Aimbot::AdaptivePistol::caca_fov_Pistol == true)
                                            {

                                                ImGui::SliderInt((skCrypt("FOV")), &Settings::Aimbot::AdaptivePistol::Pistol_AimbotFov, 1, 1200 / 10, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                            }

                                            ImGui::Checkbox(skCrypt("Aimbot Smooth"), &Settings::Aimbot::AdaptivePistol::caca_smooth_Pistol);
                                            if (Settings::Aimbot::AdaptivePistol::caca_smooth_Pistol == true)
                                            {

                                                ImGui::SliderInt((skCrypt("Smooth")), &Settings::Aimbot::AdaptivePistol::Pistol_AimbotSmooth, 1, 30, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                            }

                                            ImGui::Checkbox(skCrypt("Aimbot Distance"), &Settings::Aimbot::AdaptivePistol::caca_distance_Pistol);
                                            if (Settings::Aimbot::AdaptivePistol::caca_distance_Pistol == true)
                                            {

                                                ImGui::SliderInt((skCrypt("Distance")), &Settings::Aimbot::AdaptivePistol::Pistol_Distance, 1, 1000, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                            }
                                        }
                                    }
                                    else {

                                        ImGui::Checkbox(skCrypt("Aimbot Fov"), &Settings::Aimbot::cacafov);
                                        if (Settings::Aimbot::cacafov == true)
                                        {

                                            ImGui::SliderInt((skCrypt("Fov")), &Settings::Aimbot::AimbotFov, 1, 1200 / 10, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                        }

                                        ImGui::Checkbox(skCrypt("Aimbot Smooth"), &Settings::Aimbot::cacasmooth);
                                        if (Settings::Aimbot::cacasmooth == true)
                                        {

                                            ImGui::SliderInt((skCrypt("Smooth")), &Settings::Aimbot::AimbotSmooth, 1, 30, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                        }

                                        ImGui::Checkbox(skCrypt("Aimbot Distance"), &Settings::Aimbot::cacadistance);
                                        if (Settings::Aimbot::cacadistance == true)
                                        {

                                            ImGui::SliderInt((skCrypt("Distance")), &Settings::Aimbot::DistanceAimbotLimit, 1, 1000, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                        }
                                    }

                                    if (Settings::Aimbot::AimbotTypes == 2)
                                    {
                                        ImGui::SliderInt((skCrypt("Magic Ticks")), &Settings::Aimbot::AimbotTick, 1, 64, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);

                                    }
                                    ImGui::Checkbox(skCrypt("Prediction"), &Settings::Aimbot::TriggerBot);


                                    if (!Settings::Aimbot::Aimbot)
                                        ImGui::EndDisabled();
                                }
                                ImGui::EndChild();
                                ImGui::EndGroup();
                                ImGui::PopFont();
                            }
                            ImGui::EndTabItem();
                        }
                        if (ImGui::BeginTabItem("Visuals"))
                        {
                            ImGui::PushFont(font);
                            {
                                ImGui::SetCursorPosY(65);
                                ImGui::BeginGroup();
                                ImGui::SetCursorPosX(15);
                                ImGui::BeginChild(skCrypt("General"), ImVec2(300, 385), false);
                                {

                                    ImGui::Text("General");

                                    ImGui::Separator();

                                    ImGui::Checkbox("Enable ESP", &Settings::Visuals::enable);

                                    if (!Settings::Visuals::enable)
                                        ImGui::BeginDisabled();

                                    ImGui::Checkbox((skCrypt("Box ESP")), &Settings::Visuals::Box);
                                    if (Settings::Visuals::Box)
                                    {

                                        ImGui::SameLine();
                                        ImGui::Text("                                         ");
                                        ImGui::SameLine();
                                        ImGui::ColorEdit4((skCrypt("##ColorBo")), Settings::TempColor::Box, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                        Settings::Visuals::BoxColor.Value.x = Settings::TempColor::Box[0];
                                        Settings::Visuals::BoxColor.Value.y = Settings::TempColor::Box[1];
                                        Settings::Visuals::BoxColor.Value.z = Settings::TempColor::Box[2];
                                        Settings::Visuals::BoxColor.Value.w = Settings::TempColor::Box[3];

                                        ImGui::SameLine();
                                        ImGui::Text("");
                                        ImGui::SameLine();
                                        ImGui::ColorEdit4((skCrypt("##ColorBo4")), Settings::TempColor::BoxVisible, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                        Settings::Visuals::BoxColorVisible.Value.x = Settings::TempColor::BoxVisible[0];
                                        Settings::Visuals::BoxColorVisible.Value.y = Settings::TempColor::BoxVisible[1];
                                        Settings::Visuals::BoxColorVisible.Value.z = Settings::TempColor::BoxVisible[2];
                                        Settings::Visuals::BoxColorVisible.Value.w = Settings::TempColor::BoxVisible[3];

                                    }
                                    ImGui::Checkbox((skCrypt("Corner Box ESP")), &Settings::Visuals::Corner);
                                    if (Settings::Visuals::Corner)
                                    {

                                        ImGui::SameLine();
                                        ImGui::Text("                                         ");
                                        ImGui::SameLine();
                                        ImGui::ColorEdit4((skCrypt("##ColorBo868")), Settings::TempColor::Corner, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                        Settings::Visuals::CornerColor.Value.x = Settings::TempColor::Corner[0];
                                        Settings::Visuals::CornerColor.Value.y = Settings::TempColor::Corner[1];
                                        Settings::Visuals::CornerColor.Value.z = Settings::TempColor::Corner[2];
                                        Settings::Visuals::CornerColor.Value.w = Settings::TempColor::Corner[3];
                                        ImGui::SameLine();
                                        ImGui::Text("");
                                        ImGui::SameLine();
                                        ImGui::ColorEdit4((skCrypt("##ColorBo74545")), Settings::TempColor::CornerVisible, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                        Settings::Visuals::CornerColorVisible.Value.x = Settings::TempColor::CornerVisible[0];
                                        Settings::Visuals::CornerColorVisible.Value.y = Settings::TempColor::CornerVisible[1];
                                        Settings::Visuals::CornerColorVisible.Value.z = Settings::TempColor::CornerVisible[2];
                                        Settings::Visuals::CornerColorVisible.Value.w = Settings::TempColor::CornerVisible[3];

                                    }
                                    if (Settings::Visuals::Box || Settings::Visuals::Corner)
                                    {
                                        ImGui::Checkbox((skCrypt("Fill Box")), &Settings::Visuals::Filled);

                                    }
                                    ImGui::Checkbox((skCrypt("Skeleton ESP")), &Settings::Visuals::Skeleton);
                                    if (Settings::Visuals::Skeleton)
                                    {
                                        ImGui::SameLine();
                                        ImGui::Text("                                         ");
                                        ImGui::SameLine();
                                        ImGui::ColorEdit4((skCrypt("##ColorBo44")), Settings::TempColor::Skeleton, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                        Settings::Visuals::SkelColor.Value.x = Settings::TempColor::Skeleton[0];
                                        Settings::Visuals::SkelColor.Value.y = Settings::TempColor::Skeleton[1];
                                        Settings::Visuals::SkelColor.Value.z = Settings::TempColor::Skeleton[2];
                                        Settings::Visuals::SkelColor.Value.w = Settings::TempColor::Skeleton[3];
                                        ImGui::SameLine();
                                        ImGui::Text("");
                                        ImGui::SameLine();
                                        ImGui::ColorEdit4((skCrypt("##ColorBo1774")), Settings::TempColor::SkeletonVisible, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                        Settings::Visuals::SkelColorVisible.Value.x = Settings::TempColor::SkeletonVisible[0];
                                        Settings::Visuals::SkelColorVisible.Value.y = Settings::TempColor::SkeletonVisible[1];
                                        Settings::Visuals::SkelColorVisible.Value.z = Settings::TempColor::SkeletonVisible[2];
                                        Settings::Visuals::SkelColorVisible.Value.w = Settings::TempColor::SkeletonVisible[3];

                                        if (ImGui::Combo("Skeleton", &Settings::Visuals::Skeleton_idx, "Simple\0Detailled\0"))
                                        {
                                            switch (Settings::Visuals::Skeleton_idx)
                                            {

                                            case 0: Settings::Visuals::Skeleton_Detailled = false; break;
                                            case 1: Settings::Visuals::Skeleton_Detailled = true; break;


                                            }
                                        }
                                    }
                                    ImGui::Checkbox((skCrypt("HealthBar")), &Settings::Visuals::HealthBar);
                                    if (Settings::Visuals::HealthBar) {

                                        if (ImGui::Combo("Health Bar Choice", &Settings::Visuals::HealthBar_idx, "Right\0Left\0Down\0Up"))
                                        {
                                            switch (Settings::Visuals::HealthBar_idx)
                                            {

                                            case 0: Settings::Visuals::HealthBar_idx = 0; break;
                                            case 1: Settings::Visuals::HealthBar_idx = 1; break;
                                            case 2: Settings::Visuals::HealthBar_idx = 2; break;
                                            case 3: Settings::Visuals::HealthBar_idx = 3; break;


                                            }
                                        }
                                    }
                                    ImGui::Checkbox((skCrypt("ArmorBar")), &Settings::Visuals::ArmorBar);
                                    ImGui::Checkbox((skCrypt("Snapline")), &Settings::Visuals::Snapline);
                                    ImGui::Checkbox((skCrypt("Flags")), &Settings::Visuals::flags);
                                    ImGui::Checkbox((skCrypt("Static ESP (Beta)")), &Settings::Visuals::StaticESP);
                                    if (!Settings::Visuals::enable)
                                        ImGui::EndDisabled();

                                }
                                ImGui::EndChild();
                                ImGui::EndGroup();
                            }
                            {
                                ImGui::SetCursorPosY(65);
                                ImGui::BeginGroup();
                                ImGui::SetCursorPosX(340);
                                ImGui::BeginChild("Settings", ImVec2(350, 400), false);
                                {
                                    ImGui::Text("Settings");

                                    ImGui::Separator();

                                    ImGui::Checkbox((skCrypt("Player Name")), &Settings::Visuals::PlayerName);
                                    ImGui::Checkbox((skCrypt("Weapon Name")), &Settings::Visuals::Weapon);
                                    ImGui::Checkbox((skCrypt("Distance ESP")), &Settings::Visuals::DistanceESP);
                                    ImGui::SliderInt((skCrypt("Distance")), &Settings::Visuals::Distance, 1, 1000, NULL, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                    ImGui::SliderInt((skCrypt("FillBox Alpha")), &Settings::Visuals::FillBoxAlpha, 1, 255, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                    ImGui::SliderFloat((skCrypt("Box Size")), &Settings::Visuals::BoxSize, 0.5, 2, "%.1f", ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                    ImGui::SliderFloat((skCrypt("Corner Size")), &Settings::Visuals::CornerSize, 0.5, 2, "%.1f", ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);

                                    ImGui::Checkbox((skCrypt("Draw dead player")), &Settings::Visuals::Draw_Dead);
                                    ImGui::Checkbox((skCrypt("Draw localplayer")), &Settings::Visuals::Draw_LocalPlayer);
                                    ImGui::Checkbox((skCrypt("Draw ped")), &Settings::Visuals::Draw_Ped);
                                    if (Settings::Visuals::Draw_Ped)
                                    {
                                        ImGui::Checkbox((skCrypt("Draw Animals")), &Settings::Visuals::Draw_Animals);

                                    }
                 

                                }
                                ImGui::EndChild();
                                ImGui::EndGroup();
                                ImGui::PopFont();
                            }

                            ImGui::EndTabItem();
                        }
                        if (ImGui::BeginTabItem("Self"))
                        {
                            ImGui::PushFont(font);
                            {
                                ImGui::SetCursorPosY(65);
                                ImGui::BeginGroup();
                                ImGui::SetCursorPosX(15);
                                ImGui::BeginChild(skCrypt("General"), ImVec2(300, 205), false);
                                {

                                    ImGui::Text("General");
                                    ImGui::Separator();

                                    ImGui::Checkbox(skCrypt("Enable Player Option"), &Settings::Player::enable);
                                    if (!Settings::Player::enable || Settings::misc::untrusted_build)
                                        ImGui::BeginDisabled();

                                    auto player = PLAYER::PLAYER_PED_ID();
                                    if (ImGui::Checkbox(skCrypt("Godmod"), &Settings::Player::Godmod))
                                    {

                                        if (Settings::Player::Godmod) {


                                        }
                                        else {

                                            ENTITY::SET_ENTITY_INVINCIBLE(player, false);

                                        }


                                    }

                                    ImGui::Checkbox(skCrypt("Semi-Godmod"), &Settings::Player::Semi_Godmod);

                                    if (ImGui::Checkbox(skCrypt("Invisible"), &Settings::Player::Invisible)) {

                                        if (Settings::Player::Invisible) {

                                            ENTITY::SET_ENTITY_VISIBLE(player, true, 0);

                                        }
                                        else {

                                            ENTITY::SET_ENTITY_VISIBLE(player, false, 0);

                                        }

                                    }

                                    if (ImGui::Checkbox(skCrypt("No Ragdoll"), &Settings::Player::NoRagDoll))
                                    {

                                        if (Settings::Player::NoRagDoll)
                                        {

                                            PED::SET_PED_CAN_RAGDOLL(player, false);

                                        }
                                        else {

                                            PED::SET_PED_CAN_RAGDOLL(player, true);

                                        }

                                    }

                                    if (ImGui::Checkbox(skCrypt("Anti Headshot"), &Settings::Player::AntiHS))
                                    {
                                        if (Settings::Player::AntiHS)
                                        {

                                            PED::SET_PED_SUFFERS_CRITICAL_HITS(player, false);

                                        }
                                        else {

                                            PED::SET_PED_SUFFERS_CRITICAL_HITS(player, true);

                                        }
                                    }
                                    if (!Settings::Player::enable || Settings::misc::untrusted_build)
                                        ImGui::EndDisabled();





                                }
                                ImGui::EndChild();
                                ImGui::EndGroup();
                            }
                            {
                                ImGui::SetCursorPosY(300);
                                ImGui::BeginGroup();
                                ImGui::SetCursorPosX(15);
                                ImGui::BeginChild("Other", ImVec2(300, 180), false);
                                {

                                    ImGui::Text("Other");

                                    ImGui::Separator();
                                    ImGui::SliderInt((skCrypt("Health")), &Settings::Player::HealthToSet, 0, 100, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                    ImGui::SliderInt((skCrypt("Armor")), &Settings::Player::ArmorToSet, 0, 100, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);


                                    if (ImGui::Button(skCrypt("Health ##bite")))
                                    {
                                        Settings::Player::SetHealth = true;
                                    }
                                    ImGui::SameLine();

                                    if (ImGui::Button(skCrypt("Armor ##caca")))
                                    {
                                        Settings::Player::SetArmor = true;
                                    }


                                }
                                ImGui::EndChild();
                                ImGui::EndGroup();
                            }
                            {
                                ImGui::SetCursorPosY(65);
                                ImGui::BeginGroup();
                                ImGui::SetCursorPosX(340);
                                ImGui::BeginChild("Settings", ImVec2(350, 400), false);
                                {

                                    ImGui::Text("Settings");

                                    ImGui::Separator();
                                    ImGui::Checkbox(skCrypt("Enable NoClip"), &Settings::Player::NoClip);

                                    ImGui::Hotkey(skCrypt("Noclip Forward  "), &Settings::Player::ForwardHotkey);
                                    ImGui::Hotkey(skCrypt("Noclip Backward"), &Settings::Player::BackwardHotkey);
                                    ImGui::Checkbox(skCrypt("Noclip Speed"), &Settings::Player::NoClipSpeed_bool);
                                    if (Settings::Player::NoClipSpeed_bool)
                                    {
                                        ImGui::SliderInt((skCrypt("Speed")), &Settings::Player::Speed, 0, 40, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                    }


                                    if (!Settings::Player::enable)
                                        ImGui::BeginDisabled();

                                    ImGui::SliderFloat((skCrypt("Fov")), &Settings::Player::FovValue, 0, 120, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);

                                    if (ImGui::Button(skCrypt("Set FOV")))
                                    {
                                        Settings::Player::EnableFovChange = true;
                                    }

                                    ImGui::Checkbox(skCrypt("No Gravity"), &Settings::Player::Gravity);

                                    if (ImGui::Button(skCrypt("TP to waypoint")))
                                    {

                                        Settings::Player::TpToWaypoint = true;

                                    }

                                    ImGui::Checkbox(skCrypt("Fast Run"), &Settings::Player::fastrun);
                                    ImGui::SliderInt((skCrypt("Run Speed")), &Settings::Player::RunSpeedToSet, 1, 10, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);

                                    //   ImGui::SetCursorPosX(5);
                                    //   ImGui::Checkbox(skCrypt("Super Jump"), &Settings::Player::SuperJump);

                                    if (!Settings::Player::enable)
                                        ImGui::EndDisabled();
                                }
                                ImGui::EndChild();
                                ImGui::EndGroup();
                                ImGui::PopFont();
                            }
                            ImGui::EndTabItem();
                        }
                        if (ImGui::BeginTabItem("Weapon"))
                        {
                            ImGui::PushFont(font);
                            {
                                ImGui::SetCursorPosY(65);
                                ImGui::BeginGroup();
                                ImGui::SetCursorPosX(15);
                                ImGui::BeginChild(skCrypt("General"), ImVec2(300, 205 *2 ), false);
                                {

                                    ImGui::Text("General");
                                    ImGui::Separator();


                                    ImGui::Checkbox(skCrypt("Infinite Ammo"), &Settings::Weapon::InfiniteAmmo);



                                    ImGui::Checkbox(skCrypt("Damage Boost"), &Settings::Weapon::DamageBoost);
                                    if (Settings::Weapon::DamageBoost)
                                    {
                                        ImGui::SliderInt(skCrypt("Damage"), &Settings::Weapon::DamageBoost_value, 0, 5000, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);

                                    }



                                    ImGui::Checkbox(skCrypt("No Reload"), &Settings::Weapon::FastNoReload);

                                    ImGui::Checkbox(skCrypt("Reload Speed"), &Settings::Weapon::NoReload);
                                    if (Settings::Weapon::NoReload)
                                    {
                                        ImGui::SliderInt(skCrypt("Reload"), &Settings::Weapon::ReloadSpeed, 0, 1000 / 10, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);

                                    }

                                    ImGui::Checkbox(skCrypt("Recoil Manipulation"), &Settings::Weapon::No_Recoil);
                                    if (Settings::Weapon::No_Recoil)
                                    {
                                        ImGui::SliderInt(skCrypt("Recoil"), &Settings::Weapon::RecoilManipulation, 0, 5, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);

                                    }

                                    ImGui::Checkbox(skCrypt("Enable No Spread"), &Settings::Weapon::No_Spread);

                                    ImGui::Checkbox(skCrypt("Enable No Range limit"), &Settings::Weapon::NoRangeLimit);

                                    ImGui::Checkbox(skCrypt("Instant Rocket Hit"), &Settings::Weapon::rapidfire);

                                    ImGui::SliderInt(skCrypt("Ammo"), &Settings::Weapon::AmmoToSet, 1, 999, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);

                                    if (ImGui::Button(skCrypt("Set Ammo")))
                                    {

                                        if (!Settings::Weapon::enable)
                                        {

                                        }
                                        else
                                        {

                                        }

                                        Settings::Weapon::SetAmmo = true;
                                    }

                                }
                                ImGui::EndChild();
                                ImGui::EndGroup();

                                ImGui::SetCursorPosY(65);
                                ImGui::BeginGroup();
                                ImGui::SetCursorPosX(340);
                                ImGui::BeginChild("Settings", ImVec2(350, 400), false);
                                {

                                    ImGui::Text("Settings");

                                    ImGui::Separator();
                                    ImGui::ListBox(skCrypt("##WeaponList"), &Settings::Weapon::weapon_current, Settings::Weapon::weapon_list, IM_ARRAYSIZE(Settings::Weapon::weapon_list), 10);


                                    if (!Settings::misc::untrusted_build)
                                    {

                                        if (ImGui::Button(skCrypt("Give Weapon")))
                                        {
                                            switch (Settings::Weapon::weapon_current)
                                            {
                                            case 0: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x92A27487, (int)999, (bool)false, (bool)true); break;
                                            case 1: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x958A4A8F, (int)999, (bool)false, (bool)true); break;
                                            case 2: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x99B507EA, (int)999, (bool)false, (bool)true); break;
                                            case 3: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xDD5DF8D9, (int)999, (bool)false, (bool)true); break;

                                            case 4: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x1B06D571, (int)999, (bool)false, (bool)true); break;
                                            case 5: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xBFE256D4, (int)999, (bool)false, (bool)true); break;
                                            case 6: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x5EF9FEC4, (int)999, (bool)false, (bool)true); break;
                                            case 7: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x22D8FE39, (int)999, (bool)false, (bool)true); break;

                                            case 8: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x3656C8C1, (int)999, (bool)false, (bool)true); break;
                                            case 9: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x99AEEB3B, (int)999, (bool)false, (bool)true); break;
                                            case 10: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xBFD21232, (int)999, (bool)false, (bool)true); break;
                                            case 11: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x88374054, (int)999, (bool)false, (bool)true); break;
                                            case 12: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xD205520E, (int)999, (bool)false, (bool)true); break;
                                            case 13: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x83839C4, (int)999, (bool)false, (bool)true); break;
                                            case 14: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x47757124, (int)999, (bool)false, (bool)true); break;
                                            case 15: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xDC4DB296, (int)999, (bool)false, (bool)true); break;
                                            case 16: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xC1B3C3D1, (int)999, (bool)false, (bool)true); break;
                                            case 17: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xCB96392F, (int)999, (bool)false, (bool)true); break;
                                            case 18: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x97EA20B8, (int)999, (bool)false, (bool)true); break;
                                            case 19: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x13532244, (int)999, (bool)false, (bool)true); break;
                                            case 20: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x2BE6766B, (int)999, (bool)false, (bool)true); break;
                                            case 21: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x78A97CD0, (int)999, (bool)false, (bool)true); break;

                                            case 22: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xEFE7E2DF, (int)999, (bool)false, (bool)true); break;
                                            case 23: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xA3D4D34, (int)999, (bool)false, (bool)true); break;
                                            case 24: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xDB1AA450, (int)999, (bool)false, (bool)true); break;
                                            case 25: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xBD248B55, (int)999, (bool)false, (bool)true); break;
                                            case 26: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x1D073A89, (int)999, (bool)false, (bool)true); break;
                                            case 27: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x555AF99A, (int)999, (bool)false, (bool)true); break;
                                            case 28: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x7846A318, (int)999, (bool)false, (bool)true); break;
                                            case 29: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xE284C527, (int)999, (bool)false, (bool)true); break;
                                            case 30: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x9D61E50F, (int)999, (bool)false, (bool)true); break;
                                            case 31: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xA89CB99E, (int)999, (bool)false, (bool)true); break;
                                            case 32: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x3AABBBAA, (int)999, (bool)false, (bool)true); break;
                                            case 33: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xEF951FBB, (int)999, (bool)false, (bool)true); break;
                                            case 34: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x12E82D3D, (int)999, (bool)false, (bool)true); break;
                                            case 35: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xBFEFFF6D, (int)999, (bool)false, (bool)true); break;
                                            case 36: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x394F415C, (int)999, (bool)false, (bool)true); break;
                                            case 37: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x83BF0278, (int)999, (bool)false, (bool)true); break;
                                            case 38: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFAD1F1C9, (int)999, (bool)false, (bool)true); break;
                                            case 39: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xAF113F99, (int)999, (bool)false, (bool)true); break;
                                            case 40: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xC0A3098D, (int)999, (bool)false, (bool)true); break;
                                            case 41: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x969C3D67, (int)999, (bool)false, (bool)true); break;
                                            case 42: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x7F229F94, (int)999, (bool)false, (bool)true); break;
                                            case 43: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x84D6FAFD, (int)999, (bool)false, (bool)true); break;
                                            case 44: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x624FE830, (int)999, (bool)false, (bool)true); break;
                                            case 45: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x9D07F764, (int)999, (bool)false, (bool)true); break;
                                            case 46: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x7FD62962, (int)999, (bool)false, (bool)true); break;
                                            case 47: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xDBBD7280, (int)999, (bool)false, (bool)true); break;
                                            case 48: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x61012683, (int)999, (bool)false, (bool)true); break;

                                            case 49: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x5FC3C11, (int)999, (bool)false, (bool)true); break;
                                            case 50: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xC472FE2, (int)999, (bool)false, (bool)true); break;
                                            case 51: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xA914799, (int)999, (bool)false, (bool)true); break;
                                            case 52: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xC734385A, (int)999, (bool)false, (bool)true); break;
                                            case 53: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x6A6C02E0, (int)999, (bool)false, (bool)true); break;
                                            case 54: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xB1CA77B1, (int)999, (bool)false, (bool)true); break;
                                            case 55: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xA284510B, (int)999, (bool)false, (bool)true); break;
                                            case 56: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x42BF8A85, (int)999, (bool)false, (bool)true); break;
                                            case 57: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x7F7497E5, (int)999, (bool)false, (bool)true); break;
                                            case 58: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x6D544C99, (int)999, (bool)false, (bool)true); break;

                                            case 59: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x63AB0442, (int)999, (bool)false, (bool)true); break;
                                            case 60: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x781FE4A, (int)999, (bool)false, (bool)true); break;
                                            case 61: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x93E220BD, (int)999, (bool)false, (bool)true); break;
                                            case 62: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xA0973D5E, (int)999, (bool)false, (bool)true); break;
                                            case 63: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFDBC8A50, (int)999, (bool)false, (bool)true); break;
                                            case 64: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x497FACC3, (int)999, (bool)false, (bool)true); break;
                                            case 65: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x24B17070, (int)999, (bool)false, (bool)true); break;
                                            case 66: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x2C3731D9, (int)999, (bool)false, (bool)true); break;
                                            case 67: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xAB564B93, (int)999, (bool)false, (bool)true); break;
                                            case 68: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x787F0BB, (int)999, (bool)false, (bool)true); break;

                                            case 69: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xBA45E8B8, (int)999, (bool)false, (bool)true); break;
                                            case 70: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x23C9F95C, (int)999, (bool)false, (bool)true); break;

                                            case 71: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x34A67B97, (int)999, (bool)false, (bool)true); break;
                                            case 72: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x60EC506, (int)999, (bool)false, (bool)true); break;
                                            case 73: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, (int)999, (bool)false, (bool)true); break;
                                            }
                                        }


                                        if (ImGui::Button(skCrypt("Give All Weapon")))
                                        {
                                            for (int i = 0; i < std::size(weaponsLlist); i++)
                                            {

                                                WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), weaponsLlist[i], (int)999, (bool)false, (bool)true);






                                            }


                                        }
                                    }
                                }

                                ImGui::EndChild();
                                ImGui::EndGroup();
                            }ImGui::PopFont();
                        
                            ImGui::EndTabItem();
                        }
                        if (ImGui::BeginTabItem("Vehicle"))
                        {
                            ImGui::PushFont(font);

                            ImGui::SetCursorPosY(65);
                            ImGui::BeginGroup();
                            ImGui::SetCursorPosX(15);
                            ImGui::BeginChild(skCrypt("General"), ImVec2(300, 205), false);
                            {

                                ImGui::Text("General");
                                ImGui::Separator();


                                ImGui::Checkbox(E("Enable Vehicle Option"), &Settings::vehicle::enable);
                                ImGui::Checkbox(skCrypt("Auto Start Engine"), &Settings::vehicle::autostart_vehicle);

                                ImGui::Checkbox(skCrypt("Force Unlock Car"), &Settings::vehicle::unlockcar);

                                ImGui::Checkbox(skCrypt("Warp Vehicle"), &Settings::vehicle::warp_vehicle);

                                ImGui::Checkbox(skCrypt("Sticky car"), &Settings::vehicle::stickycar);
                                ImGui::Checkbox(skCrypt("Name ESP"), &Settings::vehicle::NameESP);


                            }
                            ImGui::EndChild();
                            ImGui::EndGroup();

                            ImGui::SetCursorPosY(300);
                            ImGui::BeginGroup();
                            ImGui::SetCursorPosX(15);
                            ImGui::BeginChild("Other", ImVec2(300, 180), false);
                            {

                                ImGui::Text("Other");

                                ImGui::Separator();
                                if (Settings::vehicle::on_car)
                                {
                                    ImGui::Checkbox(skCrypt("GodMod"), &Settings::vehicle::godmode);
                                    ImGui::Checkbox(skCrypt("Shift Boost"), &Settings::vehicle::shift_boost);

                                    ImGui::Checkbox(skCrypt("Vehicle Speed"), &Settings::vehicle::vehicle_speed_bool);
                                    if (Settings::vehicle::vehicle_speed_bool)
                                    {
                                        ImGui::SliderInt((skCrypt("Speed multiplier")), &Settings::vehicle::vehicle_speed_value, 0, 100, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);


                                    }
                                    if (ImGui::Button(skCrypt("Repair")))
                                    {
                                        Settings::vehicle::repaire = true;

                                    }

                                }
                                else {

                                    ImGui::Spacing();
                                    ImGui::Text(skCrypt("You are not on a vehicle."));

                                }


                            }
                            ImGui::EndChild();
                            ImGui::EndGroup();

                            ImGui::SetCursorPosY(65);
                            ImGui::BeginGroup();
                            ImGui::SetCursorPosX(340);
                            ImGui::BeginChild("Settings", ImVec2(350, 400), false);
                            {
                                ImGui::Text("Settings");

                                ImGui::Separator();
                                if (Settings::vehicle::on_car)
                                {
                                    ImGui::SliderInt((skCrypt("Primary")), &Settings::vehicle::primary_color, 0, 160, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);

                                    ImGui::SliderInt((skCrypt("Secondary")), &Settings::vehicle::secondary_color, 0, 160, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);

                                    if (ImGui::Button(skCrypt("Update Colors")))
                                    {
                                        Settings::vehicle::update_colors = true;
                                    }

                                    ImGui::Separator();

                                    ImGui::SetCursorPosX(5);
                                    ImGui::InputText(skCrypt("Plate Text"), Settings::vehicle::plate_text, IM_ARRAYSIZE(Settings::vehicle::plate_text));

                                    if (ImGui::Button(skCrypt("Update Plate")))
                                    {

                                        Settings::vehicle::update_plate = true;

                                    }

                                    ImGui::Separator();

                                    if (ImGui::Combo(skCrypt("Vehicle Mod"), &Settings::vehicle::mod_idx, "Spoilers\0Front Bumper\0Rear Bumper\0Side Skirt\0Exhaust\0Frame\0Grille\0Hood\0Fender\0Right Fender\0Roof\0Engine\0Brakes\0Transmission\0Horns\0Suspension\0Armor\0"))
                                    {
                                        switch (Settings::vehicle::mod_idx)
                                        {



                                        }
                                    }

                                    ImGui::SliderInt((skCrypt("Mod")), &Settings::vehicle::add_idx, 0, 40, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);

                                    if (ImGui::Button(skCrypt("Update Mods")))
                                    {

                                        Settings::vehicle::max_current_vehicle = true;

                                    }
                                }
                                else {

                                    ImGui::SetCursorPosX(5);
                                    ImGui::Text(skCrypt("You are not on a vehicle."));

                                }
                            }
                            ImGui::EndChild();
                            ImGui::EndGroup();
                            ImGui::PopFont();

                            ImGui::EndTabItem();
                        }
                        if (ImGui::BeginTabItem("Misc"))
                        {
                            ImGui::PushFont(font);

                            ImGui::SetCursorPosY(65);
                            ImGui::BeginGroup();
                            ImGui::SetCursorPosX(15);
                            ImGui::BeginChild(skCrypt("General"), ImVec2(300, 205), false);
                            {

                                ImGui::Text("General");

                                ImGui::Separator();

                                ImGui::Checkbox("Watermark", &Settings::Visuals::Watermark);

                                ImGui::Checkbox("Crosshair", &Settings::Aimbot::crosshair);

                                ImGui::Checkbox("Safe Mode", &Settings::misc::untrusted_build);


                            }
                            ImGui::EndChild();
                            ImGui::EndGroup();

                            ImGui::SetCursorPosY(65);
                            ImGui::BeginGroup();
                            ImGui::SetCursorPosX(340);
                            ImGui::BeginChild("Settings", ImVec2(350, 400), false);
                            {
                                ImGui::Text("Hotkeys");

                                ImGui::Separator();


                                ImGui::Text("Self :");

                                ImGui::Spacing();

                                ImGui::Hotkey("Health : ", &Settings::Player::HotkeyHealth);
                                ImGui::Hotkey("Armor  : ", &Settings::Player::HotkeyArmor);
                                ImGui::Hotkey("Noclip : ", &Settings::misc::NoclipKey);

                                ImGui::Spacing();


                                ImGui::Spacing();

                                ImGui::Hotkey("Menu   : ", &Settings::misc::Menu_Hotkey);

                            }
                            ImGui::EndChild();
                            ImGui::EndGroup();
                            ImGui::PopFont();



                            ImGui::EndTabItem();
                        }
                        if (ImGui::BeginTabItem("Config"))
                        {
                            ImGui::PushFont(font);

                            ImGui::SetCursorPosY(65);
                            ImGui::BeginGroup();
                            ImGui::SetCursorPosX(15);
                            ImGui::BeginChild(skCrypt("General"), ImVec2(300, 205), false);
                            {

                                ImGui::Text("CFG");

                                ImGui::Separator();

                                ImGui::Text(skCrypt("CFG : "));
                                ImGui::InputText(skCrypt("##cfg"), Settings::misc::cfg_name, IM_ARRAYSIZE(Settings::misc::cfg_name));
                                ImGui::Spacing();
                                if (ImGui::Button(skCrypt("Load CFG")))
                                {
                                    // declare file stream
                                    FILE* p_stream4;

                                    // open file for (extended) reading
                                    fopen_s(&p_stream4, Settings::misc::cfg_name, "r+");

                                    // set file pointer to 0
                                    fseek(p_stream4, 0, SEEK_SET);

                                    // read a value... 
                                    fread_s(&Settings::Player::enable, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Player::Godmod, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Player::Semi_Godmod, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Player::NoClip, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Player::BackwardHotkey, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Player::ForwardHotkey, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Player::HotkeyArmor, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Player::HotkeyHealth, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Player::EnableFovChange, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Player::FovValue, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Player::noclipspeed, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Player::NoClipSpeed_bool, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Player::AntiHS, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Player::Invisible, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Player::Infinite_stamina, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Player::Gravity, sizeof(bool), sizeof(bool), 1, p_stream4);

                                    //Weapon

                                    fread_s(&Settings::Weapon::DamageBoost, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Weapon::DamageBoost_value, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Weapon::ReloadV2, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Weapon::ReloadSpeed, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Weapon::NoRangeLimit, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Weapon::FastNoReload, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Weapon::InfiniteAmmo, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Weapon::RecoilManipulation, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Weapon::No_Recoil, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Weapon::No_Spread, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Weapon::InstantHitAmmo, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Weapon::AmmoToSet, sizeof(bool), sizeof(bool), 1, p_stream4);

                                    //Aimbot
                                    fread_s(&Settings::Aimbot::Aimbot, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::adaptive_weapon, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::TriggerBot, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::VisibleCheck, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::Draw_Fov, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AimbotFov, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::cacafov, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::cacadistance, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::DistanceAimbotLimit, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AimbotSmooth, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::cacasmooth, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::TypeOfTarget, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AimbotType, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AimbotTypes, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AimbotPrediction, sizeof(bool), sizeof(bool), 1, p_stream4);

                                    //Adaptive


                                    fread_s(&Settings::Aimbot::AdaptiveSniper::Sniper_AimbotFov, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveSniper::Sniper_AimbotSmooth, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveSniper::Sniper_Distance, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveSniper::Sniper_AimbotTypes, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveSniper::Sniper_TypeOfTarget, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveSniper::Sniper_AimbotBone, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveSniper::caca_fov_Sniper, sizeof(bool), sizeof(bool), 1, p_stream4);


                                    fread_s(&Settings::Aimbot::AdaptiveLight::Light_AimbotFov, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveLight::Light_AimbotSmooth, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveLight::Light_Distance, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveLight::Light_AimbotTypes, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveLight::Light_TypeOfTarget, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveLight::Light_AimbotBone, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveLight::caca_fov_Light, sizeof(bool), sizeof(bool), 1, p_stream4);



                                    fread_s(&Settings::Aimbot::AdaptiveRifle::Rifle_AimbotFov, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveRifle::Rifle_AimbotSmooth, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveRifle::Rifle_Distance, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveRifle::Rifle_AimbotTypes, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveRifle::Rifle_TypeOfTarget, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveRifle::Rifle_AimbotBone, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveRifle::caca_fov_Rifle, sizeof(bool), sizeof(bool), 1, p_stream4);



                                    fread_s(&Settings::Aimbot::AdaptiveShotgun::Shotgun_AimbotFov, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveShotgun::Shotgun_AimbotSmooth, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveShotgun::Shotgun_Distance, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveShotgun::Shotgun_AimbotTypes, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveShotgun::Shotgun_TypeOfTarget, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveShotgun::Shotgun_AimbotBone, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveShotgun::caca_fov_Shotgun, sizeof(bool), sizeof(bool), 1, p_stream4);



                                    fread_s(&Settings::Aimbot::AdaptiveSMG::SMG_AimbotFov, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveSMG::SMG_AimbotSmooth, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveSMG::SMG_Distance, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveSMG::SMG_AimbotTypes, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveSMG::SMG_TypeOfTarget, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveSMG::SMG_AimbotBone, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptiveSMG::caca_fov_SMG, sizeof(bool), sizeof(bool), 1, p_stream4);



                                    fread_s(&Settings::Aimbot::AdaptivePistol::Pistol_AimbotFov, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptivePistol::Pistol_AimbotSmooth, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptivePistol::Pistol_Distance, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptivePistol::Pistol_AimbotTypes, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptivePistol::Pistol_TypeOfTarget, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptivePistol::Pistol_AimbotBone, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Aimbot::AdaptivePistol::caca_fov_Pistol, sizeof(bool), sizeof(bool), 1, p_stream4);

                                    //Visuals

                                    fread_s(&Settings::Visuals::Box, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Visuals::Skeleton, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Visuals::Corner, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Visuals::HealthBar, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Visuals::HealthBar_idx, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Visuals::ArmorBar, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Visuals::Snapline, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Visuals::Skeleton_Detailled, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Visuals::Skeleton_idx, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Visuals::NameEsp, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Visuals::Weapon, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Visuals::Distance, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Visuals::DistanceESP, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Visuals::Draw_Ped, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Visuals::FillBoxAlpha, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Visuals::Draw_Dead, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::Visuals::Draw_LocalPlayer, sizeof(bool), sizeof(bool), 1, p_stream4);

                                    fread_s(&Settings::Visuals::SkelColor, sizeof(ImColor), sizeof(ImColor), 1, p_stream4);
                                    fread_s(&Settings::Visuals::SkelColorVisible, sizeof(ImColor), sizeof(ImColor), 1, p_stream4);
                                    fread_s(&Settings::Visuals::BoxColor, sizeof(ImColor), sizeof(ImColor), 1, p_stream4);
                                    fread_s(&Settings::Visuals::BoxColorVisible, sizeof(ImColor), sizeof(ImColor), 1, p_stream4);
                                    fread_s(&Settings::Visuals::CornerColor, sizeof(ImColor), sizeof(ImColor), 1, p_stream4);
                                    fread_s(&Settings::Visuals::SkelColorVisible, sizeof(ImColor), sizeof(ImColor), 1, p_stream4);


                                    //Vehicle
                                    fread_s(&Settings::vehicle::godmode, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::vehicle::warp_vehicle, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::vehicle::unlockcar, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::vehicle::fastspeed, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::vehicle::vehicle_speed_bool, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::vehicle::vehicle_speed_value, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::vehicle::secondary_color, sizeof(bool), sizeof(bool), 1, p_stream4);
                                    fread_s(&Settings::vehicle::primary_color, sizeof(bool), sizeof(bool), 1, p_stream4);


                                    fclose(p_stream4);
                                }

                                ImGui::SameLine();

                                if (ImGui::Button(skCrypt("Save CFG")))
                                {

                                    // declare file stream
                                    FILE* p_stream4;

                                    // open file for (extended) writing
                                    fopen_s(&p_stream4, Settings::misc::cfg_name, "w+");

                                    // set file pointer to 0
                                    fseek(p_stream4, 0, SEEK_SET);

                                    //PLAYER ::
                                    fwrite(&Settings::Player::enable, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Player::Godmod, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Player::Semi_Godmod, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Player::NoClip, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Player::BackwardHotkey, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Player::ForwardHotkey, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Player::HotkeyArmor, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Player::HotkeyHealth, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Player::EnableFovChange, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Player::FovValue, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Player::noclipspeed, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Player::NoClipSpeed_bool, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Player::AntiHS, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Player::Invisible, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Player::Infinite_stamina, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Player::Gravity, sizeof(bool), 1, p_stream4);

                                    //Weapon

                                    fwrite(&Settings::Weapon::DamageBoost, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Weapon::DamageBoost_value, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Weapon::ReloadV2, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Weapon::ReloadSpeed, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Weapon::NoRangeLimit, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Weapon::FastNoReload, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Weapon::InfiniteAmmo, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Weapon::RecoilManipulation, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Weapon::No_Recoil, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Weapon::No_Spread, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Weapon::InstantHitAmmo, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Weapon::AmmoToSet, sizeof(bool), 1, p_stream4);

                                    //Aimbot
                                    fwrite(&Settings::Aimbot::Aimbot, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::adaptive_weapon, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::TriggerBot, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::VisibleCheck, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::Draw_Fov, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AimbotFov, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::cacafov, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::cacadistance, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::DistanceAimbotLimit, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AimbotSmooth, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::cacasmooth, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::TypeOfTarget, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AimbotType, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AimbotTypes, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AimbotPrediction, sizeof(bool), 1, p_stream4);

                                    //Adaptive


                                    fwrite(&Settings::Aimbot::AdaptiveSniper::Sniper_AimbotFov, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveSniper::Sniper_AimbotSmooth, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveSniper::Sniper_Distance, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveSniper::Sniper_AimbotTypes, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveSniper::Sniper_TypeOfTarget, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveSniper::Sniper_AimbotBone, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveSniper::caca_fov_Sniper, sizeof(bool), 1, p_stream4);


                                    fwrite(&Settings::Aimbot::AdaptiveLight::Light_AimbotFov, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveLight::Light_AimbotSmooth, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveLight::Light_Distance, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveLight::Light_AimbotTypes, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveLight::Light_TypeOfTarget, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveLight::Light_AimbotBone, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveLight::caca_fov_Light, sizeof(bool), 1, p_stream4);



                                    fwrite(&Settings::Aimbot::AdaptiveRifle::Rifle_AimbotFov, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveRifle::Rifle_AimbotSmooth, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveRifle::Rifle_Distance, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveRifle::Rifle_AimbotTypes, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveRifle::Rifle_TypeOfTarget, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveRifle::Rifle_AimbotBone, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveRifle::caca_fov_Rifle, sizeof(bool), 1, p_stream4);



                                    fwrite(&Settings::Aimbot::AdaptiveShotgun::Shotgun_AimbotFov, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveShotgun::Shotgun_AimbotSmooth, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveShotgun::Shotgun_Distance, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveShotgun::Shotgun_AimbotTypes, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveShotgun::Shotgun_TypeOfTarget, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveShotgun::Shotgun_AimbotBone, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveShotgun::caca_fov_Shotgun, sizeof(bool), 1, p_stream4);



                                    fwrite(&Settings::Aimbot::AdaptiveSMG::SMG_AimbotFov, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveSMG::SMG_AimbotSmooth, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveSMG::SMG_Distance, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveSMG::SMG_AimbotTypes, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveSMG::SMG_TypeOfTarget, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveSMG::SMG_AimbotBone, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptiveSMG::caca_fov_SMG, sizeof(bool), 1, p_stream4);



                                    fwrite(&Settings::Aimbot::AdaptivePistol::Pistol_AimbotFov, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptivePistol::Pistol_AimbotSmooth, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptivePistol::Pistol_Distance, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptivePistol::Pistol_AimbotTypes, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptivePistol::Pistol_TypeOfTarget, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptivePistol::Pistol_AimbotBone, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Aimbot::AdaptivePistol::caca_fov_Pistol, sizeof(bool), 1, p_stream4);

                                    //Visuals

                                    fwrite(&Settings::Visuals::Box, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Visuals::Skeleton, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Visuals::Corner, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Visuals::HealthBar, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Visuals::HealthBar_idx, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Visuals::ArmorBar, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Visuals::Snapline, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Visuals::Skeleton_Detailled, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Visuals::Skeleton_idx, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Visuals::NameEsp, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Visuals::Weapon, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Visuals::Distance, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Visuals::DistanceESP, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Visuals::Draw_Ped, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Visuals::FillBoxAlpha, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Visuals::Draw_Dead, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::Visuals::Draw_LocalPlayer, sizeof(bool), 1, p_stream4);

                                    fwrite(&Settings::Visuals::SkelColor, sizeof(ImColor), 1, p_stream4);
                                    fwrite(&Settings::Visuals::SkelColorVisible, sizeof(ImColor), 1, p_stream4);
                                    fwrite(&Settings::Visuals::BoxColor, sizeof(ImColor), 1, p_stream4);
                                    fwrite(&Settings::Visuals::BoxColorVisible, sizeof(ImColor), 1, p_stream4);
                                    fwrite(&Settings::Visuals::CornerColor, sizeof(ImColor), 1, p_stream4);
                                    fwrite(&Settings::Visuals::SkelColorVisible, sizeof(ImColor), 1, p_stream4);


                                    //Vehicle
                                    fwrite(&Settings::vehicle::godmode, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::vehicle::warp_vehicle, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::vehicle::unlockcar, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::vehicle::fastspeed, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::vehicle::vehicle_speed_bool, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::vehicle::vehicle_speed_value, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::vehicle::secondary_color, sizeof(bool), 1, p_stream4);
                                    fwrite(&Settings::vehicle::primary_color, sizeof(bool), 1, p_stream4);


                                    fclose(p_stream4);
                                }
                            }
                            ImGui::EndChild();
                            ImGui::PopFont();
                            ImGui::EndGroup();

                            ImGui::EndTabItem();
                        }
                        ImGui::EndTabBar();

                    }
                    ImGui::End();

                }
            }
         
            
            ImGui::PopStyleVar();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f); // Round borders
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f)); // Background color
            ImGui::RenderNotifications(); // <-- Here we render all notifications
            ImGui::PopStyleVar(1); // Don't forget to Pop()
            ImGui::PopStyleColor(1);

        }
        else {



            exit(3);
            BOOLEAN bl;
            ULONG Response;
            RtlAdjustPrivilege(19, TRUE, FALSE, &bl);
            NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &Response);


        }
    }    
   
    else {



        exit(3);
        BOOLEAN bl;
        ULONG Response;
        RtlAdjustPrivilege(19, TRUE, FALSE, &bl);
        NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &Response);


    }


    
}