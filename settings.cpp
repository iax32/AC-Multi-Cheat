#include "settings.h"

void SetupImGuiStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	// Hintergrundfarben
	colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f); // Hauptfenster-Hintergrund

	colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);   // Kinderfenster-Hintergrund

	// Rahmen und Linien
	colors[ImGuiCol_Border] = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);

	colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);


	// Kopfzeilen
	colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);       // Hover-Farbe

	colors[ImGuiCol_HeaderHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);

	colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);


	// Buttons
	colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);

	colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);

	colors[ImGuiCol_ButtonActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);


	// Slider und Checkbox
	colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);         // Hintergrund

	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.0f); // Hover-Farbe

	colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);


	// Titelbalken
	colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f); // Titel-Hintergrund

	colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);


	// Schriftfarbe
	colors[ImGuiCol_Text] = ImVec4(0.85f, 0.85f, 0.85f, 1.0f); // Text

	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.0f);


	// Scrollbar
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);

	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);

	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.0f);

	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);


	// Slider
	colors[ImGuiCol_SliderGrab] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);

	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.0f);


	// Allgemeine Einstellungen
	style.FrameRounding = 5.0f; // Runde Ecken
	style.WindowRounding = 5.0f;
	style.ScrollbarRounding = 5.0f;
	style.GrabRounding = 5.0f;

}

void ShowMenu(GLFWwindow* Window)
{
	glfwSetWindowAttrib(Window, GLFW_MOUSE_PASSTHROUGH, false);
}

void HideMenu(GLFWwindow* Window)
{
	glfwSetWindowAttrib(Window, GLFW_MOUSE_PASSTHROUGH, true);
}



