#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>
#include <thread>

#include "auxil_vector.h"
#include "sorts_vector.h"

bool DisplayAsButtonRow(const std::string &name, const std::vector <int> &vector, uint pointer, PTR_STATE state);

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 800), "Sorts");
    window.setVerticalSyncEnabled(true);

    ImGui::SFML::Init(window, false);
    ImGuiIO &io = ImGui::GetIO();
    //io.Fonts->AddFontDefault();
    ImFont* font = io.Fonts->AddFontFromFileTTF("..\\res\\PureProg 12.ttf", 24.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    font->DisplayOffset.y = -1;
    font->DisplayOffset.x = 1;
    ImGui::SFML::UpdateFontTexture();

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    sf::Clock deltaClock;

    bool paused;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());


        static std::vector <int> vector = Shuffle(20);
        static uint pointer = 0;
        static PTR_STATE state = IDLE;
        paused = !DisplayAsButtonRow("Vector", vector, pointer, state);
        static std::thread sortThread(SelectionSort, std::ref(vector), std::ref(pointer), std::ref(state), std::ref(paused));
        if (sortThread.joinable()) {
            sortThread.detach();
        }

        window.clear(clear_color);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}

bool DisplayAsButtonRow(const std::string &name, const std::vector <int> &vector, uint pointer, PTR_STATE state) {
    static bool fastForward = false;
    ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_HorizontalScrollbar);
    for (uint i = 0; i < vector.size(); i++) {
        ImGui::PushID(i);
        if (i == pointer) {
            ImGui::PushStyleColor(ImGuiCol_Button, StateColor(state));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, StateColor(state));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, StateColor(state));
        } else {
            ImGui::PushStyleColor(ImGuiCol_Button, StateColor(IDLE));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, StateColor(IDLE));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, StateColor(IDLE));
        }
        ImGui::Button(std::to_string(vector[i]).c_str());
        ImGui::PopStyleColor(3);
        ImGui::PopID();
        ImGui::SameLine();
    }
    bool advance = false;
    if (ImGui::Button(">"))
        advance = true;
    ImGui::SameLine();
    if (ImGui::Button(">>"))
        fastForward = !fastForward;
    ImGui::End();
    return fastForward || advance;
}