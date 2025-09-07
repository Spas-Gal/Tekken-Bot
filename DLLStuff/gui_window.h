#pragma once
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx12.h"
#include "ImGui/imgui_impl_win32.h"

namespace ImGui {

	//shows the main window (pass a pointer to a bool that signals the function call to close the window)
	static void ShowMainWindow(bool* shouldClosePointer) {
		ImGui::Begin("Main Menu", shouldClosePointer);





		ImGui::End();
	}
}