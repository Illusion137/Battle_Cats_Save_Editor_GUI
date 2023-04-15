#include "Widgets.h"

bool ImGui::GradientButton(const char* label, const ImVec2& size_arg, ImU32 text_color, ImU32 bg_color_1, ImU32 bg_color_2, BCats::Widgets::Animation::ButtonAnimation& animation)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    PushFont(BCats::Widgets::outlinedFont);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    //const ImRect bb(pos, pos + size);
    ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id)) {
        ImGui::PopFont();
        return false;
    }

    ImGuiButtonFlags flags = ImGuiButtonFlags_None;
    if (g.LastItemData.InFlags & ImGuiItemFlags_ButtonRepeat)
        flags |= ImGuiButtonFlags_Repeat;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

    // Render
    const bool is_gradient = bg_color_1 != bg_color_2;
    if (held || hovered)
    {
        // Modify colors (ultimately this can be prebaked in the style)
        float h_increase = (held && hovered) ? 0.02f : 0.02f;
        float v_increase = (held && hovered) ? 0.20f : 0.07f;

        ImVec4 bg1f = ColorConvertU32ToFloat4(bg_color_1);
        ColorConvertRGBtoHSV(bg1f.x, bg1f.y, bg1f.z, bg1f.x, bg1f.y, bg1f.z);
        bg1f.x = ImMin(bg1f.x + h_increase, 1.0f);
        bg1f.z = ImMin(bg1f.z + v_increase, 1.0f);
        ColorConvertHSVtoRGB(bg1f.x, bg1f.y, bg1f.z, bg1f.x, bg1f.y, bg1f.z);
        bg_color_1 = GetColorU32(bg1f);
        if (is_gradient)
        {
            ImVec4 bg2f = ColorConvertU32ToFloat4(bg_color_2);
            ColorConvertRGBtoHSV(bg2f.x, bg2f.y, bg2f.z, bg2f.x, bg2f.y, bg2f.z);
            bg2f.z = ImMin(bg2f.z + h_increase, 1.0f);
            bg2f.z = ImMin(bg2f.z + v_increase, 1.0f);
            ColorConvertHSVtoRGB(bg2f.x, bg2f.y, bg2f.z, bg2f.x, bg2f.y, bg2f.z);
            bg_color_2 = GetColorU32(bg2f);
        }
        else
        {
            bg_color_2 = bg_color_1;
        }
    }
    RenderNavHighlight(bb, id);

#if 0
    // V1 : faster but prevents rounding
    window->DrawList->AddRectFilledMultiColor(bb.Min, bb.Max, bg_color_1, bg_color_1, bg_color_2, bg_color_2);
    if (g.Style.FrameBorderSize > 0.0f)
        window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(ImGuiCol_Border), 0.0f, 0, g.Style.FrameBorderSize);
#endif
    if (held || hovered) {
#define HOVER_SPEED 2
        PushStyleColor(ImGuiCol_Border, animation.isPurpleCycle ? ImVec4(0.83, 0.11, 0.76, 1.0) : ImVec4(0.93, 0.87, 0.16, 1.0));
        if (animation.hoverTick >= HOVER_SPEED) {
            animation.isPurpleCycle ^= 1;
            animation.hoverTick = 0;
        }
        else
            animation.hoverTick++;
    }
    else
        PushStyleColor(ImGuiCol_Border, ImVec4(0.0, 0.0, 0.0, 1.0));
    // V2
    if (pressed || !animation.isPressedAnimationFinished) { 
#define HALF_POINT 4
#define END_POINT 8
        animation.isPressedAnimationFinished = false;
        if (animation.pressedTick >= END_POINT) {
            animation.pressedTick = 0;
            animation.isPressedAnimationFinished = true;
        }
        else
            animation.pressedTick++;
        int calcPressedTick = animation.pressedTick > HALF_POINT ? std::abs(animation.pressedTick - END_POINT) : animation.pressedTick;
        bb.Min = ImVec2(bb.Min.x - calcPressedTick, bb.Min.y - calcPressedTick );
        bb.Max = ImVec2(bb.Max.x + calcPressedTick, bb.Max.y + calcPressedTick );
    }
    //

    int vert_start_idx = window->DrawList->VtxBuffer.Size;
    window->DrawList->AddRectFilled(bb.Min, bb.Max, bg_color_1, g.Style.FrameRounding);
    int vert_end_idx = window->DrawList->VtxBuffer.Size;
    if (is_gradient)
        ShadeVertsLinearColorGradientKeepAlpha(window->DrawList, vert_start_idx, vert_end_idx, bb.Min, bb.GetBL(), bg_color_1, bg_color_2);
    if (g.Style.FrameBorderSize > 0.0f)
        window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(ImGuiCol_Border), g.Style.FrameRounding, 0, g.Style.FrameBorderSize);

    if (g.LogEnabled)
        LogSetNextTextDecoration("[", "]");

    //ImGui::PushFontShadow(0xFF000000);
    PushStyleColor(ImGuiCol_Text, text_color);

    RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

    PopFont();
    PopStyleColor(2);
    //ImGui::PopFontShadow();

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

bool BCats::Widgets::render_button(std::string title, Animation::ButtonAnimation& animation, ImVec2 size, ImU32 textColor) {
    
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.5f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 3.5f);
	
	ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.98, 0.74, 0.1, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.73, 0.55, 0.11, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.73, 0.55, 0.11, 1));

    ImGui::PushFontShadow(0xFF000000);

	bool buttonPressed = ImGui::GradientButton(title.c_str(), size, textColor, IM_COL32(0xff, 0xc1, 0x00, 0xff), IM_COL32(0xbb, 0x8c, 0x1a, 0xff), 
        animation);

    ImGui::PopFontShadow();

    ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(3);
	return buttonPressed;
}

void BCats::Widgets::render_text(std::string title, ImU32 textColor) {
    ImGui::PushStyleColor(ImGuiCol_Text, textColor);
    ImGui::PushFont(defaultFont);

    ImGui::TextUnformatted(title.c_str());

    ImGui::PopFont();
    ImGui::PopStyleColor();
}

void BCats::Widgets::render_outlined_text(std::string title, ImU32 textColor) {
    ImGui::PushFontShadow(0xff000000);
    ImGui::PushStyleColor(ImGuiCol_Text, textColor);
    ImGui::PushFont(outlinedFont);

    ImGui::TextUnformatted(title.c_str());
    
    ImGui::PopFont();
    ImGui::PopStyleColor();
    ImGui::PopFontShadow();
}

bool BCats::Widgets::render_checkbox(std::string label, bool* checkBoxActive) {
    ImGui::PushStyleColor(ImGuiCol_CheckMark,     ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg,        ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive,  ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border,        ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 3.0f);
    //ImGui::PushFontShadow(0xff000000);
    ImGui::PushFont(outlinedFont);
    ImGui::SetWindowFontScale(0.7);

    ImGui::Checkbox(label.c_str(), checkBoxActive);

    ImGui::SetWindowFontScale(1);
    ImGui::PopFont();
    //ImGui::PopFontShadow();
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(5);
    return *checkBoxActive;
}

bool BCats::Widgets::render_toggle_button_text(std::string title, bool& value, Animation::ButtonAnimation& anim, ImU32 toggledOnColor, ImVec2 size, ImU32 toggledOnColorGradient, ImU32 textColor, ImU32 toggledOffColor) {
    
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.5f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.5f);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.98, 0.74, 0.1, 1));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.73, 0.55, 0.11, 1));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.73, 0.55, 0.11, 1));

    //ImGui::PushFontShadow(0xFF000000);
    
    bool buttonPressed = ImGui::GradientButton(title.c_str(), size, textColor,
        value ? toggledOnColor : toggledOffColor,
        value ? toggledOnColorGradient : toggledOffColor - 0x001f1f1f,
        anim);
    if (buttonPressed)
        value = !value;
    
    //ImGui::PopFontShadow();

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);
    return buttonPressed;
}
bool BCats::Widgets::render_toggle_button_icon(const char* strId, ImTextureID iconTexture, ImVec2 size, bool& value, Animation::ImageButtonAnimation& anim, ImVec4 toggledOnColor, ImVec4 toggledOffColor) {

    bool buttonPressed = render_image_button(strId, iconTexture, anim, size, ImVec2(0,0), ImVec2(1,1), ImVec4(0,0,0,0),
        value ? toggledOnColor : toggledOffColor);

    if (buttonPressed)
        value = !value;

    return buttonPressed;
    return false;
}

bool BCats::Widgets::render_drag_int_range(const char* label, int* data, const int min, const int max, ImU32 textColor)
{
    using namespace ImGui;

    const ImGuiDataType data_type = ImGuiDataType_S32;
    const char* format = "%d";
    const ImGuiSliderFlags flags = 0;
    const float speed = 0.1F;
    const float DRAG_MOUSE_THRESHOLD_FACTOR = 0.5F;

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    
    ///CHANGE WIDTH OF BOX
    const float w = 40;

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));
    const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

    const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id, &frame_bb, temp_input_allowed ? ImGuiItemFlags_Inputable : 0))
        return false;

    // Default format string when passing NULL
    if (format == NULL)
        format = DataTypeGetInfo(data_type)->PrintFmt;

    const bool hovered = ItemHoverable(frame_bb, id);
    bool temp_input_is_active = temp_input_allowed && TempInputIsActive(id);
    if (!temp_input_is_active)
    {
        // Tabbing or CTRL-clicking on Drag turns it into an InputText
        const bool input_requested_by_tabbing = temp_input_allowed && (g.LastItemData.StatusFlags & ImGuiItemStatusFlags_FocusedByTabbing) != 0;
        const bool clicked = hovered && IsMouseClicked(0, id);
        const bool double_clicked = (hovered && g.IO.MouseClickedCount[0] == 2 && TestKeyOwner(ImGuiKey_MouseLeft, id));
        const bool make_active = (input_requested_by_tabbing || clicked || double_clicked || g.NavActivateId == id);
        if (make_active && (clicked || double_clicked))
            SetKeyOwner(ImGuiKey_MouseLeft, id);
        if (make_active && temp_input_allowed)
            if (input_requested_by_tabbing || (clicked && g.IO.KeyCtrl) || double_clicked || (g.NavActivateId == id && (g.NavActivateFlags & ImGuiActivateFlags_PreferInput)))
                temp_input_is_active = true;

        // (Optional) simple click (without moving) turns Drag into an InputText
        if (g.IO.ConfigDragClickToInputText && temp_input_allowed && !temp_input_is_active)
            if (g.ActiveId == id && hovered && g.IO.MouseReleased[0] && !IsMouseDragPastThreshold(0, g.IO.MouseDragThreshold * DRAG_MOUSE_THRESHOLD_FACTOR))
            {
                g.NavActivateId = id;
                g.NavActivateFlags = ImGuiActivateFlags_PreferInput;
                temp_input_is_active = true;
            }

        if (make_active && !temp_input_is_active)
        {
            SetActiveID(id, window);
            SetFocusID(id, window);
            FocusWindow(window);
            g.ActiveIdUsingNavDirMask = (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
        }
    }

    if (temp_input_is_active)
    {
        // Only clamp CTRL+Click input when ImGuiSliderFlags_AlwaysClamp is set
        const bool is_clamp_input = (flags & ImGuiSliderFlags_AlwaysClamp) != 0 && (&min == NULL || &max == NULL || DataTypeCompare(data_type, &min, &max) < 0);
        return TempInputScalar(frame_bb, id, label, data_type, data, format, is_clamp_input ? &min : NULL, is_clamp_input ? &max : NULL);
    }

    // Draw frame
    //const ImU32 frame_col = GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
    const ImU32 frame_col = IM_COL32(0, 0, 0, 0);
    RenderNavHighlight(frame_bb, id);
    RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, style.FrameRounding);

    // Drag behavior
    const bool value_changed = DragBehavior(id, data_type, data, speed, &min, &max, format, flags);
    if (value_changed)
        MarkItemEdited(id);

    // Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
    char value_buf[64];
    const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, data, format);
    ImGui::SetCursorScreenPos(frame_bb.Min);
    render_outlined_text(std::to_string(*data), textColor);
    //RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, value_buf_end, NULL, ImVec2(0.5f, 0.5f));

    //if (label_size.x > 0.0f)
        //RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | (temp_input_allowed ? ImGuiItemStatusFlags_Inputable : 0));
    return value_changed;
}

void BCats::Widgets::render_tooltip(const char* message, float delaySeconds) {
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && GImGui->HoveredIdTimer > delaySeconds) {

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 6));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
        
        ImGui::SetTooltip(message);

        ImGui::PopStyleVar(2);
    }
}

bool BCats::Widgets::render_image_button(const char* str_id, ImTextureID user_texture_id, Animation::ImageButtonAnimation& animation,
    const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& bg_col, ImVec4& tint_col, ImGuiButtonFlags flags)
{
    using namespace ImGui;

    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiID id = window->GetID(str_id);

    const ImVec2 padding = g.Style.FramePadding;
    ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2.0f);
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

    // Render
    //const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
    //RenderNavHighlight(bb, id);
    //RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, g.Style.FrameRounding));
    //if (bg_col.w > 0.0f)
        //window->DrawList->AddRectFilled(bb.Min + padding, bb.Max - padding, GetColorU32(bg_col));
    if (pressed || !animation.isPressedAnimationFinished) {
#define HALF_POINT 4
#define END_POINT 8
        animation.isPressedAnimationFinished = false;
        if (animation.pressedTick >= END_POINT) {
            animation.pressedTick = 0;
            animation.isPressedAnimationFinished = true;
        }
        else
            animation.pressedTick++;
        int calcPressedTick = animation.pressedTick > HALF_POINT ? std::abs(animation.pressedTick - END_POINT) : animation.pressedTick;
        bb.Min = ImVec2(bb.Min.x - calcPressedTick, bb.Min.y - calcPressedTick);
        bb.Max = ImVec2(bb.Max.x + calcPressedTick, bb.Max.y + calcPressedTick);
    }
    if (hovered)
        tint_col = ImVec4(tint_col.x - 0.1, tint_col.y - 0.1, tint_col.z - 0.1, tint_col.w);

    window->DrawList->AddImage(user_texture_id, bb.Min + padding, bb.Max - padding, uv0, uv1, GetColorU32(tint_col));

    return pressed;
}
