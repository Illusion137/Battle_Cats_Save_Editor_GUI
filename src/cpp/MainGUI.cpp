#include "../headers/MainGUI.h"

BCats::Main::Screens BCats::Main::currentScreen = BCats::Main::Screen_HOME;

void BCats::Main::on_exit_success(){}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int BCats::Main::opengl_main_loop() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

#if defined(IMGUI_IMPL_OPENGL_ES2)
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    GLFWwindow* window = glfwCreateWindow(1280, 720, "BCSFE_GUI", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigInputTextCursorBlink = true;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    bool showWindow = true;
    ImVec4 clear_color = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

#ifdef __EMSCRIPTEN__
    io.IniFilename = NULL;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        BCats::Main::on_gui_render(&showWindow);

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif
    BCats::Main::on_exit_success();

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void BCats::Main::on_gui_render(bool *open) {
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::Begin("Dockspace", open, window_flags);

    ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("Dockspace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    ImGui::SetNextWindowDockID(ImGui::GetID("Dockspace"));

    switch (currentScreen) {
    case Screen_HOME:
        render_home();
        break;
    }

    ImGui::End();
}

void BCats::Main::render_download_save_modal_popup(bool* open) {
    if (*open) {
        ImGui::OpenPopup("Download Save From Battle Cats");
        ImGui::BeginPopupModal("Download Save From Battle Cats", open);
        {
            ImGui::SetWindowFontScale(1.5);
            static Data::SaveData toDownloadData;
            const char* comboCountryCodes[]{ "SELECT Country Code", "en","jp"};
            if(ImGui::BeginCombo("Country Code", comboCountryCodes[toDownloadData.countryCode]))
            {
                if (ImGui::Selectable(comboCountryCodes[1], toDownloadData.countryCode == Data::CountryCode_EN))
                    toDownloadData.countryCode = Data::CountryCode_EN;
                if (ImGui::Selectable(comboCountryCodes[2], toDownloadData.countryCode == Data::CountryCode_JP))
                    toDownloadData.countryCode = Data::CountryCode_JP;
                ImGui::EndCombo();
            }
            const char* comboVersions[]{ "SELECT Version", "12.1.0","1.0.0" };
            if (ImGui::BeginCombo("Version", comboVersions[toDownloadData.version]))
            {
                if (ImGui::Selectable(comboVersions[1], toDownloadData.version == Data::CountryCode_EN))
                    toDownloadData.version = Data::Version_12_1_0;
                if (ImGui::Selectable(comboVersions[2], toDownloadData.version == Data::CountryCode_JP))
                    toDownloadData.version = Data::Version_1_0_0;
                ImGui::EndCombo();
            }
            static char transferCodeBuffer[10];
            if (ImGui::InputText("Transfer Code", transferCodeBuffer, 10, ImGuiInputTextFlags_CharsDecimal))
                toDownloadData.transferCode = transferCodeBuffer;

            static char confirmationCodeBuffer[5];
            if(ImGui::InputText("Confirmation Code", confirmationCodeBuffer, 5, ImGuiInputTextFlags_CharsDecimal))
                toDownloadData.confirmationCode = confirmationCodeBuffer;

            ImGui::BeginDisabled(
                toDownloadData.countryCode == Data::CountryCode_NONE ||
                toDownloadData.version == Data::Version_NONE ||
                toDownloadData.transferCode.length() != 9 ||
                toDownloadData.confirmationCode.length() != 4
            );
            if (ImGui::Button("Submit")) {
                currentScreen = 
            }
            ImGui::EndDisabled();

            ImGui::EndPopup();
        }
    }
}

void BCats::Main::render_home() {
#define BUTTON_SIZE 300
    static Textures::Texture bgTexture, catTexture, jsonFileTexture, saveFileTexture;
    if (!Textures::loadedTexturesSet.count("res/Home/background.png"))
        bgTexture = Textures::load_texture("res/Home/background.png");
    if(!Textures::loadedTexturesSet.count("res/Home/cat.png"))
        catTexture = Textures::load_texture("res/Home/cat.png");
    if (!Textures::loadedTexturesSet.count("res/Home/jsonFile.png"))
        jsonFileTexture = Textures::load_texture("res/Home/jsonFile.png");
    if (!Textures::loadedTexturesSet.count("res/Home/saveFile.png"))
        saveFileTexture = Textures::load_texture("res/Home/saveFile.png");

    static bool openDownloadDataModelPopup = false;
    render_download_save_modal_popup(&openDownloadDataModelPopup);

    ImVec2 windowSize = ImGui::GetWindowSize();
    ImGuiStyle& style = ImGui::GetStyle();

    ImGui::Begin("Home", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
    ImVec2 cursorPos = ImGui::GetCursorPos();
    ImGui::Image((ImTextureID)(intptr_t)bgTexture.texture, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y),
        ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,0.1));
    
    ImGui::SetWindowFontScale(2);
    ImGui::SetCursorPos(ImVec2(
        (windowSize.x / 2) - (ImGui::CalcTextSize("Battle Cats Save Editor").x / 2),
        50
    ));

    ImGui::TextColored(ImVec4(1,0.1,0.8,1),"Battle Cats Save Editor");

    ImGui::SetWindowFontScale(1.3);
    style.ItemSpacing.x = 60;
    style.FramePadding = ImVec2(0, 0);
    style.WindowPadding = ImVec2(0, 0);
    float buttonYPositions = ImGui::GetCursorPos().y;

    ImGui::SetCursorPos(ImVec2((windowSize.x - (BUTTON_SIZE * 3) - (style.ItemSpacing.x * 2)) / 2, buttonYPositions));
    ImGui::BeginGroup();
    {
        if (ImGui::ImageButton((ImTextureID)(intptr_t)catTexture.texture, ImVec2(BUTTON_SIZE, BUTTON_SIZE)))
            openDownloadDataModelPopup = true;
        ImGui::Text("Download Save From Battle Cats");
        ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
        ImGui::ImageButton((ImTextureID)(intptr_t)saveFileTexture.texture, ImVec2(BUTTON_SIZE, BUTTON_SIZE));
        ImGui::Text("Select Save File");
        ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
        ImGui::ImageButton((ImTextureID)(intptr_t)jsonFileTexture.texture, ImVec2(BUTTON_SIZE, BUTTON_SIZE));
        ImGui::Text("Select Saved Data JSON File");
        ImGui::EndGroup();
    }
    style.ItemSpacing.x = 0;
    ImGui::End();
}