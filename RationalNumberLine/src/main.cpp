#define GL_CLAMP_TO_EDGE 0x812F // not available for openGL 1.1
#define STB_IMAGE_IMPLEMENTATION
#define GL_SILENCE_DEPRECATION

#include <iostream>

#include "Rational/Rational.h"
#include "Timer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "stb_image.h"

#include <GLFW/glfw3.h>

const char* const NUMBER_LINE_IMG = "assets/images/number_line.png";

// For changing text color when values change
const ImVec4 COLOR_RED = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);        // When at 0
const ImVec4 COLOR_BLUE = ImVec4(0.03f, 0.18f, 1.0f, 1.0f);     // For negatives
const ImVec4 COLOR_GREEN = ImVec4(0.08f, 0.87f, 0.26f, 1.0f);   // For positives

static void GlfwErrorCallback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

float Clamp(float n, float lower, float upper) 
{
    return std::max(lower, std::min(n, upper));
}

bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

void ChangeSliderColor(ImGuiStyle& style, ImVec4 newColor)
{
    style.Colors[ImGuiCol_SliderGrab] = newColor;
    style.Colors[ImGuiCol_SliderGrabActive] = newColor;
}

int main(int, char**)
{
    glfwSetErrorCallback(GlfwErrorCallback);
    if (!glfwInit())
    {
        std::cout << "Error initializing GLFW!\n";
        return 1;
    }

    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "The Rational Number line", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Error initializing window!\n";
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    ChangeSliderColor(style, COLOR_RED);

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our background
    ImVec4 clear_color = ImVec4(0.45f, 0.15f, 0.90f, 1.00f);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // For showing rational numbers
        static int i = 0;
        static int amount = 1000;
        Rational p(i, amount);
        
        // For positioning Value Window
        ImVec2 numberLineWindowPos;
        float numberLineWindowWidth;

        // MAIN WINDOW
        {  
            // Image Init OpenGL3
            int my_image_width = 0;
            int my_image_height = 0;
            GLuint my_image_texture = 0;
            bool ret = LoadTextureFromFile(NUMBER_LINE_IMG, &my_image_texture, &my_image_width, &my_image_height);
            IM_ASSERT(ret);

            // Window props
            ImVec2 pos = ImVec2(100.0f, 275.0f);
            ImVec2 size = ImVec2(1025.0f, 150.0f);
            ImGuiWindowFlags winFlags =
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_NoBackground;

            // Begin widgets
            ImGui::SetNextWindowPos(pos);
            ImGui::SetNextWindowSize(size);
            ImGui::Begin("OpenGL Texture Text", (bool*)0, winFlags);

            ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(my_image_width, my_image_height));

            // MAIN SLIDER
            {
                ImVec2 winChildPos = ImVec2(25.0f, 15.0f);

                ImGui::SetCursorPos(winChildPos);
                ImGui::BeginChild("MAIN", ImVec2(1475.0f, 50.0f));

                i = Clamp(i, -amount, amount); // used when lowering max number so it doesn't display old max number
                ImGui::SliderInt(" ", &i, -amount, amount);

                ImGui::EndChild();
            }
            // MAIN SLIDER

            // MAX DENOMINATOR CHANGER
            {
                ImVec2 winChildPos = ImVec2(400.0f, 130.0f);

                ImGui::SetCursorPos(winChildPos);
                ImGui::BeginChild("MAX DENOM", ImVec2(325.0f, 50.0f));

                ImGui::InputInt("Max Denominator", &amount);
                amount = Clamp(amount, 1, amount + 1); // used when user gets freaky

                ImGui::EndChild();
            }
            // MAX DENOMINATOR CHANGER

            // MAX LABEL
            {
                ImVec2 winChildPos = ImVec2(800.0f, 50.0f);

                ImGui::SetCursorPos(winChildPos);
                ImGui::BeginChild("label1");
                ImGui::LabelText("Max = 1", "");
                ImGui::EndChild();
            }
            // MAX LABEL
            
            // MIN LABEL
            {
                ImVec2 winChildPos = ImVec2(-1850.0f, 50.0f);

                ImGui::SetCursorPos(winChildPos);
                ImGui::BeginChild("label2");
                ImGui::LabelText("Min = -1", "");
                ImGui::EndChild();
            }
            // MIN LABEL

            numberLineWindowPos = ImGui::GetWindowPos(); 
            numberLineWindowWidth = size.x;

            ImGui::End();
        }
        // MAIN WINDOW
        
        // VALUE WINDOW
        {   
            float posXOffset = -35.0f;
            float posYOffset = -75.0f;
            ImVec2 pos = ImVec2(Clamp(ImGui::GetMousePos().x + posXOffset, numberLineWindowPos.x, numberLineWindowPos.x + numberLineWindowWidth - 80.0f), numberLineWindowPos.y + posYOffset);
            ImGui::SetNextWindowPos(pos);

            ImVec2 size = ImVec2(75.0f, 75.0f);
            ImGui::SetNextWindowSize(size);

            ImGuiWindowFlags winFlags =
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove;
            ImGui::Begin("Value", (bool*)0, winFlags);

            ImGui::Text(p.Simplify().ToString().c_str());
            ImGui::Text(std::to_string(p.ToFloat()).c_str());

            ImGui::End();
        }
        // VALUE WINDOW

        // Changing text color
        if (i < 0)      ChangeSliderColor(style, COLOR_BLUE);
        else if (i > 0) ChangeSliderColor(style, COLOR_GREEN);
        else            ChangeSliderColor(style, COLOR_RED);

        // End widgets

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
