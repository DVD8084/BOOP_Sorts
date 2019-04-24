#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>
#include <thread>

#include "Vector.h"
#include "SortVector.h"

#define MAX_ROW_WIDTH 20
#define MIN_VECTOR_SIZE 2
#define MAX_VECTOR_SIZE 100

bool DisplayAsButtonRow(const std::string &name, const SortVector &vector, int color);

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 800), "Sorts");
    window.setVerticalSyncEnabled(true);

    ImGui::SFML::Init(window, false);
    ImGuiIO &io = ImGui::GetIO();
    //io.Fonts->AddFontDefault();
    ImFont *font = io.Fonts->AddFontFromFileTTF("..\\res\\PureProg 12.ttf", 24.0f, nullptr,
                                                io.Fonts->GetGlyphRangesCyrillic());
    font->DisplayOffset.y = -1;
    font->DisplayOffset.x = 1;
    ImGui::SFML::UpdateFontTexture();

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    sf::Clock deltaClock;

    while (window.isOpen()) {

        static std::thread sortThread;
        static bool simActive = false;

        sf::Event event;

        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                simActive = false;
                sortThread.join();
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        static SortVector vector;

        static ALGORITHM algorithm;
        static int size = 10;
        static bool setElements = false;
        static std::string elements;
        static bool shuffle = true;
        static int color = 0;

        ImGui::Begin("Sorting Algorithms", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        if (simActive) {
            if (ImGui::Button("Stop the simulation")) {
                simActive = false;
                sortThread.join();
                vector.Clear();
            }
        } else {
            ImGui::InputInt("Size", &size);
            if (size < MIN_VECTOR_SIZE)
                size = MIN_VECTOR_SIZE;
            if (size > MAX_VECTOR_SIZE)
                size = MAX_VECTOR_SIZE;
            ImGui::RadioButton("Numbers", &color, 0);
            ImGui::SameLine();
            ImGui::RadioButton("Colors", &color, 1);
            if (!color) {
                ImGui::Checkbox("Set Elements", &setElements);
                if (setElements) {
                    ImGui::InputText("Elements", &elements);
                    ImGui::Checkbox("Shuffle", &shuffle);
                }
            }
            if (ImGui::Button("Selection Sort"))
                algorithm = SELECTION;
            if (ImGui::Button("Insertion Sort"))
                algorithm = INSERTION;
            if (ImGui::Button("Bubble Sort"))
                algorithm = BUBBLE;
        }

        ImGui::End();

        if (algorithm != NONE) {
            simActive = true;
            if (setElements) {
                vector.Shuffle(size, elements, shuffle);
            } else {
                vector.Shuffle(size);
            }
        }

        if (!vector.IsEmpty()) {
            switch (algorithm) {
                case SELECTION:
                    sortThread = std::thread(&SortVector::SelectionSort, &vector, std::ref(simActive));
                    break;
                case INSERTION:
                    sortThread = std::thread(&SortVector::InsertionSort, &vector, std::ref(simActive));
                    break;
                case BUBBLE:
                    sortThread = std::thread(&SortVector::BubbleSort, &vector, std::ref(simActive));
                    break;
                default:
                    break;
            }
            if (simActive) {
                vector.paused = !DisplayAsButtonRow("Vector", vector, color);
            }
        }

        algorithm = NONE;

        window.clear(clear_color);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}

bool
DisplayAsButtonRow(const std::string &name, const SortVector &vector, int color) {
    static bool fastForward = false;
    ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_HorizontalScrollbar);
    for (uint i = 0; i < vector.GetSize(); i++) {
        ImGui::PushID(i);
        if (color) {
            if (i == vector.GetPosition()) {
                ImGui::PushStyleColor(ImGuiCol_Button,
                                      (ImVec4) ImColor::HSV((float) vector.Get(i) / vector.GetSize(), 0.95f, 0.95f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                                      (ImVec4) ImColor::HSV((float) vector.Get(i) / vector.GetSize(), 0.95f, 0.95f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                                      (ImVec4) ImColor::HSV((float) vector.Get(i) / vector.GetSize(), 0.95f, 0.95f));
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button,
                                      (ImVec4) ImColor::HSV((float) vector.Get(i) / vector.GetSize(), 0.75f, 0.75f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                                      (ImVec4) ImColor::HSV((float) vector.Get(i) / vector.GetSize(), 0.75f, 0.75f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                                      (ImVec4) ImColor::HSV((float) vector.Get(i) / vector.GetSize(), 0.75f, 0.75f));
            }
            ImGui::Button("  ");
        } else {
            if (i == vector.GetPosition()) {
                ImGui::PushStyleColor(ImGuiCol_Button, StateColor(vector.GetState()));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, StateColor(vector.GetState()));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, StateColor(vector.GetState()));
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button, StateColor(IDLE));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, StateColor(IDLE));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, StateColor(IDLE));
            }
            std::string buttonText = std::to_string(vector.Get(i));
            uint max = vector.GetMaxLength();
            while (buttonText.length() < max) {
                buttonText = " " + buttonText;
            }
            ImGui::Button(buttonText.c_str());
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();
        if ((i + 1) % MAX_ROW_WIDTH && i + 1 != vector.GetSize())
            ImGui::SameLine();
    }
    bool advance = false;
    if (ImGui::Button("> "))
        advance = true;
    ImGui::SameLine();
    if (fastForward)
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
    else
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Button));
    if (vector.GetPosition() == 0 && vector.GetState() == IDLE)
        fastForward = false;
    if (ImGui::Button(">>"))
        fastForward = !fastForward;
    ImGui::PopStyleColor();
    ImGui::End();
    return fastForward || advance;
}