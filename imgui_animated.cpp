#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_animated.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include <map>
using namespace ImGui;


bool ImGui::Toggle( const char* label, bool* v )
{
	ImGuiWindow* window = GetCurrentWindow( );
	if ( window->SkipItems )
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID( label );
	const ImVec2 label_size = CalcTextSize( label, NULL, true );

	float height = ImGui::GetFrameHeight( );
	const ImVec2 pos = window->DC.CursorPos;

	float width = height * 2.f;
	float radius = height * 0.50f;

	const ImRect total_bb( pos, pos + ImVec2( width + ( label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f ), label_size.y + style.FramePadding.y * 2.0f ) );

	ItemSize( total_bb, style.FramePadding.y );
	if ( !ItemAdd( total_bb, id ) )
		return false;

	float last_active_id_timer = g.LastActiveIdTimer;

	bool hovered, held;
	bool pressed = ButtonBehavior( total_bb, id, &hovered, &held );
	if ( pressed )
	{
		*v = !( *v );
		MarkItemEdited( id );
		g.LastActiveIdTimer = 0.f;
	}

	if ( g.LastActiveIdTimer == 0.f && g.LastActiveId == id && !pressed )
		g.LastActiveIdTimer = last_active_id_timer;

	float t = *v ? 1.0f : 0.0f;

	if ( g.LastActiveId == id )
	{
		float t_anim = ImSaturate( g.LastActiveIdTimer / 0.16f );
		t = *v ? ( t_anim ) : ( 1.0f - t_anim );
	}

//	ImU32 col_bg = GetColorU32( ( held && hovered ) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg );
	ImColor col_bg = ImColor(255, 255 , 255 , 0);
	if (hovered)
	{
		col_bg = ImColor(255,255,255,100);
	}

	if (*v == true)
	{
			col_bg = ImColor(47, 74, 105, 240);
	}
	

	
	const ImRect frame_bb( pos, pos + ImVec2( width, height ) );

	RenderFrame( frame_bb.Min, frame_bb.Max, col_bg, true, height * 0.5f );
	RenderNavHighlight( total_bb, id );

	ImVec2 label_pos = ImVec2( frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y );
	RenderText( label_pos, label );
	window->DrawList->AddCircleFilled( ImVec2( pos.x + radius + t * ( width - radius * 2.0f ), pos.y + radius ), radius - 1.5f, ImGui::GetColorU32( ImGuiCol_CheckMark ), 36 );

	return pressed;
}

