#include "MainGUI.h"

inline static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

inline void BCats::Main::on_enter() {
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
    style.WindowPadding = ImVec2(0, 0);
    
    Widgets::defaultFont = io.Fonts->AddFontFromFileTTF("res/Fonts/HalenoirCompact-DemiBold.otf", 24); //Default Font
    Widgets::outlinedFont = io.Fonts->AddFontFromFileTTF("res/Fonts/Rouben ExtraBold.ttf", 33);
}

inline void BCats::Main::load_window_icon(GLFWwindow* window)
{
    GLFWimage images[1];
    images[0].pixels = stbi_load(WINDOW_ICON, &images[0].width, &images[0].height, 0, 4);
    glfwSetWindowIcon(window, 1, images);
    stbi_image_free(images[0].pixels);
}

int BCats::Main::opengl_main_loop() {
    //StandardError any GLFW errors
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y, WINDOW_NAME, NULL, NULL);
    if (window == NULL)
        return 1;

    Main::load_window_icon(window);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    Main::on_enter();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    bool showWindow = true;
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    while (!glfwWindowShouldClose(window))
    {
        //Get all inputs
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        Main::on_gui_render(&showWindow);

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
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
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void BCats::Main::on_gui_render(bool *open) {
    static bool useWorkArea = true;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(useWorkArea ? viewport->WorkPos : viewport->Pos);
    ImGui::SetNextWindowSize(useWorkArea ? viewport->WorkSize : viewport->Size);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0xff, 0xe3, 0x93, 0xff));

    if (ImGui::Begin("BCSFE_GUI_Fullscreen", open, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        if (isHomeScreen)
            Main::render_home_screen();
        else
            Editor::on_gui_render();
        ImGui::End();

        #ifdef _DEBUG
            {
                static bool deopen = true;
                ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0x25, 0x25, 0x25, 0xaa));
                if (ImGui::Begin("Debug Window", &deopen, ImGuiWindowFlags_NoCollapse)) {
                    ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
                    ImGui::Text("DeltaTime: %f", ImGui::GetIO().DeltaTime);
                    ImGui::End();
                }
                ImGui::PopStyleColor();
            }
        #endif // _DEBUG
    }


    ImGui::PopStyleColor();
}

void BCats::Main::render_download_save_modal_popup(bool* open) {
    if (*open) {        
        static bool throwError = false;
        //Error::render_error(&throwError, "An Error Occurred While Downloading Save Data");
        ImGui::OpenPopup("Download Save From Battle Cats");
        if(ImGui::BeginPopupModal("Download Save From Battle Cats", open))
        {
            ImGui::SetWindowFontScale(1.5);
            static int countryCodeIndex = 0;
            static CatInfo::SaveData toDownloadData;
            const char* comboCountryCodes[]{ "SELECT Country Code", "en","jp"};
            if(ImGui::BeginCombo("Country Code", comboCountryCodes[countryCodeIndex]))
            {
                if (ImGui::Selectable(comboCountryCodes[1], toDownloadData.countryCode == "en"))
                    toDownloadData.countryCode = "en";
                if (ImGui::Selectable(comboCountryCodes[2], toDownloadData.countryCode == "jp"))
                    toDownloadData.countryCode = "jp";
                ImGui::EndCombo();
            }
            static char versionCodeBuffer[12];
            if (ImGui::InputText("Version", versionCodeBuffer, 12, ImGuiInputTextFlags_CharsDecimal))
                toDownloadData.version = versionCodeBuffer;
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                ImGui::SetTooltip("Version found at the top right of Battle Cats when you load it up eg. '12.1.0'");

            static char transferCodeBuffer[10];
            if (ImGui::InputText("Transfer Code", transferCodeBuffer, 10, ImGuiInputTextFlags_CharsHexadecimal))
                toDownloadData.transferCode = transferCodeBuffer;
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                ImGui::SetTooltip("9 Digit Hex String in lowercase found after you begin Data Transfer eg. '013afe468'");

            static char confirmationCodeBuffer[5];
            if(ImGui::InputText("Confirmation Code", confirmationCodeBuffer, 5, ImGuiInputTextFlags_CharsDecimal))
                toDownloadData.confirmationCode = confirmationCodeBuffer;
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                ImGui::SetTooltip("4 Digit Code found after you begin Data Transfer eg. '3014'");

            bool notValidCountryCode = toDownloadData.countryCode == "",
                notValidVersion = toDownloadData.version.length() < 5,
                notValidTransferCode = toDownloadData.transferCode.length() != 9,
                notValidConfirmationCode = toDownloadData.confirmationCode.length() != 4;
                    //!BCSFE::check_dec(toDownloadData.confirmationCode);

            ImGui::BeginDisabled(
                notValidCountryCode ||
                notValidVersion ||
                notValidTransferCode ||
                notValidConfirmationCode
            );
            
            if (ImGui::Button("Submit")) {
                *open = false;
                //currentScreen = 
                //toDownloadData.version = BCSFE::str_to_game_version(toDownloadData.version);
                std::string response;
                //ServerHelper::download_saved_data(response, toDownloadData);
                std::string retPath;
                //if (Helper::is_post_good(response) &&
                    //Helper::save_to_file_to_saves((void*)response.c_str(), retPath)) {
                    //currentSavePath = retPath;
                //}
                //else {
                    
                //}
            }
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
            {
                std::string toolTipStr = "";
                toolTipStr += notValidCountryCode ? "Country Code Not Selected (en/jp)\n" : "";
                toolTipStr += notValidVersion ? "Not Valid Version ([0-9.] {3})\n" : "";
                toolTipStr += notValidTransferCode ? "Not Valid Transfer Code ([a-f,0-9] {10})\n" : "";
                toolTipStr += notValidConfirmationCode ? "Not Valid Confirmation Code ([0-9] {4})\n" : "";
                ImGui::SetTooltip(toolTipStr.c_str());
            }
            ImGui::EndDisabled();

            ImGui::EndPopup();
        }
    }
}

void BCats::Main::render_loading_popup(bool* open, const std::string threadMessage) {
    if (*open) {
        ImGui::OpenPopup("Loading");
        ImGui::SetNextWindowSize(ImVec2(550,100));
        if(ImGui::BeginPopupModal("Loading", open))
        {
            static int tick = 1;

            std::string loadingText;
            loadingText = threadMessage;
            loadingText.insert(loadingText.length() , tick / 40, '.');

            Widgets::render_text(loadingText);

            tick++;
            if (tick == 159)
                tick = 1;
            
            ImGui::EndPopup();
        }
    }
}

void BCats::Main::thread_load_file(ThreadLocalFileArgs& args) {
    if (args.isJson) {
        args.returnThreadMessage = "Copying file to and Creating Backup at: " + FileManager::get_save_folder_path();

        FileManager::saveFilePath = FileManager::find_unused_file_name_in_saves_folder();;
        FileManager::saveFileBackupPath = boost::regex_replace(FileManager::saveFilePath, boost::regex("SAVE-"), "SAVE_BACKUP-");

        try {
            FileManager::copy_file_to(args.filePath, FileManager::saveFilePath);
        }
        catch (const std::exception&){
            //Just ingnore ig
        }

        args.returnThreadMessage = "Attempting to Parse JSON";
        std::ifstream file(FileManager::saveFilePath, std::ios::in | std::ios::binary);
        if (file.good()) {
            try {
                FileManager::saveData = nlohmann::json::parse(file);
            }
            catch (const std::exception& e) {
                file.close();
                args.returnThreadMessage = "Trying to fix JSON file";

                FileManager::fix_json_save_file(FileManager::saveFilePath);
                file.open(FileManager::saveFilePath, std::ios::in | std::ios::binary);
                args.returnThreadMessage = "Attempting to Parse";
                try {
                    FileManager::saveData = nlohmann::json::parse(file);
                }
                catch (const std::exception& e) {
                    file.close();
                    std::filesystem::remove(FileManager::saveFilePath);
                    args.isGood = false;
                    args.isFinished = true;
                    return;
                }
            }
        }
        try {
            FileManager::copy_file_to(FileManager::saveFilePath, FileManager::saveFileBackupPath);
        }
        catch (const std::exception& e) {
            //Just ignore this exception as well
        }
        
        file.close();
        args.isGood = true;
        args.isFinished = true;
    }
    else { //Is Battle Cats Raw Save Data
        ///IMPLEMENT THIS DONT FORGET
        std::cout << "what doing :|\n";
    }
}

void BCats::Main::render_home_screen() {
    static Textures::Texture titleTexture, bgTexture, blackTexture, fieryhenryGithubTexture, illusionGithubTexture;
    Textures::load_texture(titleTexture, "res/Images/Home/title.png");
    Textures::load_texture(bgTexture,    "res/Images/Home/background.jpg");
    Textures::load_texture(blackTexture, "res/Images/Home/black.png");
    Textures::load_texture(fieryhenryGithubTexture, "res/Images/Home/fieryhenryPfp.png");
    Textures::load_texture(illusionGithubTexture, "res/Images/Home/kirby.png");

    Error::render_error();

    static std::thread loadSaveFileThread;
    static bool saveFileLoading = false, 
                saveFileFinished = false,
                saveFileIsGood = false;
    static std::string threadStatusMessage = "Loading ";
    static ThreadLocalFileArgs loadSaveFileThreadArgs = { "SAVE_DATA.json", saveFileFinished, saveFileIsGood, false, threadStatusMessage };

    if (saveFileLoading && !loadSaveFileThreadArgs.isFinished)
        render_loading_popup(&saveFileLoading, loadSaveFileThreadArgs.returnThreadMessage);
    if (loadSaveFileThreadArgs.isFinished && loadSaveFileThreadArgs.isGood) {
        loadSaveFileThread.detach();
        saveFileLoading = false;
        loadSaveFileThreadArgs.isFinished = false;
        isHomeScreen = false;
    } else if (loadSaveFileThreadArgs.isFinished && !loadSaveFileThreadArgs.isGood) {
        loadSaveFileThread.detach();
        Error::openError = true;
        Error::exceptionError = "Failed to Load Save File";
        saveFileLoading = false;
        saveFileFinished = false;
        saveFileIsGood = false;
    }

    const ImVec2 windowSize = ImGui::GetWindowSize();

    const ImVec2 cursorPos = ImGui::GetCursorPos();
    ImGui::Image((ImTextureID)(intptr_t)bgTexture.texture, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y),
        ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,0.9));
    
    //Center Next Item
    ImGui::SetCursorPos(ImVec2(((int)windowSize.x >> 1) - (titleTexture.width >> 2), 30));
    ImGui::Image((ImTextureID)(intptr_t)titleTexture.texture, ImVec2(titleTexture.width >> 1, titleTexture.height >> 1),
        ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1));

    const int buttonXSizeIncrease = 10;
    const ImVec2 buttonSize(BUTTON_SIZE_X + 10, BUTTON_SIZE_Y);

    const float buttonYPositions = 325;
    const float buttonMidXPosition = ((int)windowSize.x >> 1) - ((int)buttonSize.x >> 1);

    ImGui::SetCursorPos(ImVec2(0, buttonYPositions + 15));
    ImGui::Image((ImTextureID)(intptr_t)blackTexture.texture, ImVec2(ImGui::GetContentRegionAvail().x, 150),
        ImVec2(0,0), ImVec2(1,1), ImVec4(1, 1, 1, 0.4));

    ImGui::BeginDisabled(true);
    ImGui::SetCursorPos(ImVec2(buttonMidXPosition, buttonYPositions + 35));
    static Widgets::Animation::ButtonAnimation downloadSaveAnim { false, 0, true, 0 };
    if (Widgets::render_button("Download Save File", downloadSaveAnim, buttonSize)) {

    }
    Widgets::render_tooltip("Select a Save File from Battle Cats Data Transfer");
    ImGui::EndDisabled();

    ImGui::SetCursorPos(ImVec2(buttonMidXPosition, buttonYPositions + 95));
    static Widgets::Animation::ButtonAnimation saveFileAnim { false, 0, true, 0 };
    if (Widgets::render_button("Select Save File", saveFileAnim, buttonSize) ) {
        ImGuiFileDialog::Instance()->OpenDialog("OpenSaveFile", "Open Save File", ".json", ".", 1, nullptr, ImGuiFileDialogFlags_Modal);
    }
    Widgets::render_tooltip("Select a JSON Save File for Battle Cats");

    if (ImGuiFileDialog::Instance()->Display("OpenSaveFile")) {
        //On Select Save File
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
            loadSaveFileThreadArgs.filePath = filePath;
            loadSaveFileThreadArgs.isJson = Helper::is_json_file(filePath);
            saveFileLoading = true;
            loadSaveFileThread = std::thread(thread_load_file, loadSaveFileThreadArgs);
        }
        ImGuiFileDialog::Instance()->Close();
    }

    const int githubLinkImageBtnSize = 100;

    ImGui::SetCursorPos(ImVec2(5, 10));
    Widgets::Animation::ImageButtonAnimation illusionGithubImageBtnAnimation{ true, 0 };
    if (Widgets::render_image_button("Illusion137_Github", (ImTextureID)(intptr_t)illusionGithubTexture.texture, illusionGithubImageBtnAnimation, ImVec2(githubLinkImageBtnSize + 21, githubLinkImageBtnSize - 14))) {
        Helper::open_href(ILLUSION137_GITHUB_URL);
    }
    Widgets::render_tooltip(ILLUSION137_GITHUB_URL, 0.2);

    ImGui::SetCursorPos(ImVec2(120, 10));
    Widgets::Animation::ImageButtonAnimation fieryhenryGithubImageBtnAnimation { true, 0 };
    if (Widgets::render_image_button("fieryhenry_Github", (ImTextureID)(intptr_t)fieryhenryGithubTexture.texture, fieryhenryGithubImageBtnAnimation, ImVec2(githubLinkImageBtnSize, githubLinkImageBtnSize))) {
        Helper::open_href(FIERYHENRY_GITHUB_URL);
    }
    Widgets::render_tooltip(FIERYHENRY_GITHUB_URL, 0.2);

    ImGui::SetCursorPos(ImVec2(windowSize.x - 300, windowSize.y - 30));
    ImGui::SetWindowFontScale(0.7);
    Widgets::render_text("Made By Illusion137; Credits to fieryhenry", IM_COL32(0xff, 0x00, 0xaa, 0xff));
    ImGui::SetWindowFontScale(1);
}