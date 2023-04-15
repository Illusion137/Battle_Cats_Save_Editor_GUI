#include "Error.h"

void BCats::Error::render_error() {
	if (openError) {
		ImGui::OpenPopup("ERROR");
		ImGui::SetNextWindowSize(ImVec2(300, 100));
		if (ImGui::BeginPopupModal("ERROR", &openError, ImGuiWindowFlags_NoResize)) {
			ImGui::TextWrapped(exceptionError.c_str());
			ImGui::EndPopup();
		}
	}
}