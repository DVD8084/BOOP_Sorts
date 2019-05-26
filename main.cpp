#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui-SFML.h>

#include <json.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <fstream>
#include <iostream>
#include <thread>

#include "Vector.h"
#include "SortVector.h"

#define DEFAULT_ROW_WIDTH 20
#define MIN_VECTOR_SIZE 2
#define MAX_VECTOR_SIZE 500
#define MIN_FRAMESKIP -100
#define MAX_FRAMESKIP 1000

using json = nlohmann::json;

bool Display(const std::string &name, SortVector &vector, int color, uint buttonLength);

bool DisplayInfo(const std::string &name);

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 800), "Sorts", sf::Style::Default);
    window.setVerticalSyncEnabled(true);

    ImGui::SFML::Init(window, false);
    ImGuiIO &io = ImGui::GetIO();
    //io.Fonts->AddFontDefault();

    ImVector<ImWchar> ranges;
    ImFontGlyphRangesBuilder builder;
    builder.AddChar(0x0398); // THETA
    builder.AddChar(0x03a9); // OMEGA
    builder.AddRanges(io.Fonts->GetGlyphRangesCyrillic());
    builder.BuildRanges(&ranges);

    ImFont *font = io.Fonts->AddFontFromFileTTF("res\\PureProg 12.ttf", 24.0f, nullptr, ranges.Data);
    font->DisplayOffset.y = -1;
    font->DisplayOffset.x = 1;
    ImFont *smol = io.Fonts->AddFontFromFileTTF("res\\PureProg 12.ttf", 16.0f, nullptr, ranges.Data);
    ImFont *big = io.Fonts->AddFontFromFileTTF("res\\PureProg 12.ttf", 32.0f, nullptr, ranges.Data);

    ImGui::SFML::UpdateFontTexture();

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    sf::Clock deltaClock;

    while (window.isOpen()) {

        static std::thread sortThread;

        static SortVector vector;
        static ALGORITHM algorithm;

        static bool fullscreenMode = false;

        sf::Event event;

        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                vector.Deactivate();
                if (sortThread.joinable())
                    sortThread.join();
                algorithm = NONE;
                window.close();
            }

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)) {
                vector.Deactivate();
                if (sortThread.joinable())
                    sortThread.join();
                algorithm = NONE;
                window.close();
            }

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::F11)) {
                if (fullscreenMode) {
                    window.create(sf::VideoMode(1280, 800), "Sorts", sf::Style::Default);
                } else {
                    window.create(sf::VideoMode::getFullscreenModes()[0], "Sorts", sf::Style::Fullscreen);
                }
                window.setVerticalSyncEnabled(true);

                ImGui::SFML::Init(window, false);
                ImGui::SFML::UpdateFontTexture();

                fullscreenMode = !fullscreenMode;
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        static std::string algName;
        static int size = 100;
        static bool setElements = false;
        static std::string elements;
        static bool shuffle = true;
        static int color = 2;
        static int base = 10;
        static bool radix = false;
        static bool reload = false;

        static int maxLength = 0;

        ImGui::Begin("Sorting Algorithms", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        if (vector.IsActive()) {
            if (ImGui::Button("Stop the simulation")) {
                vector.Deactivate();
                if (sortThread.joinable())
                    sortThread.join();
                vector.Clear();
                algorithm = NONE;
            }
            if (ImGui::Button("Restart the simulation")) {
                vector.Deactivate();
                if (sortThread.joinable())
                    sortThread.join();
                vector.Reload();
                reload = true;
            }
            if (shuffle || color)
                if (ImGui::Button("Reshuffle and restart")) {
                    vector.Deactivate();
                    if (sortThread.joinable())
                        sortThread.join();
                    vector.Reload();
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
            ImGui::SameLine();
            ImGui::RadioButton("Both", &color, 2);
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
            if (ImGui::Button("Cocktail Sort"))
                algorithm = COCKTAIL;
            if (ImGui::Button("Comb Sort"))
                algorithm = COMB;
            if (ImGui::Button("ShellSort"))
                algorithm = SHELLSORT;
            if (ImGui::Button("QuickSort"))
                algorithm = QUICKSORT;
            if (ImGui::Button("Merge Sort"))
                algorithm = MERGESORT;
            if (ImGui::Button("Counting Sort"))
                algorithm = COUNTING;
            if (ImGui::Button("Radix LSD Sort")) {
                if (radix) {
                    algorithm = RADIXLSD;
                    radix = false;
                } else {
                    radix = true;
                }
            }
            if (ImGui::Button("Radix MSD Sort")) {
                if (radix) {
                    algorithm = RADIXMSD;
                    radix = false;
                } else {
                    radix = true;
                }
            }
            if (radix) {
                ImGui::InputInt("Radix", &base);
                if (base < 2)
                    base = 2;
            }
            if (ImGui::Button("Stooge Sort"))
                algorithm = STOOGE;
            if (ImGui::Button("BogoSort"))
                algorithm = BOGOSORT;
            if (ImGui::Button("Exit")) {
                window.close();
            }
        }

        ImGui::End();

        if (reload) {
            reload = false;
        } else {
            if ((algorithm != NONE) && !vector.IsActive()) {
                if (setElements && !color) {
                    vector.Shuffle(static_cast<const uint &>(size), elements, shuffle);
                } else {
                    vector.Shuffle(static_cast<const uint &>(size));
                }
            }
        }

        if (!sortThread.joinable())
            maxLength = vector.GetMaxLength();

        if (!vector.IsEmpty()) {
            if (!vector.IsActive()) {
                vector.Activate();
                switch (algorithm) {
                    case SELECTION:
                        sortThread = std::thread(&SortVector::SelectionSort, &vector);
                        algName = "Selection Sort";
                        break;
                    case INSERTION:
                        sortThread = std::thread(&SortVector::InsertionSort, &vector);
                        algName = "Insertion Sort";
                        break;
                    case BUBBLE:
                        sortThread = std::thread(&SortVector::BubbleSort, &vector);
                        algName = "Bubble Sort";
                        break;
                    case COCKTAIL:
                        sortThread = std::thread(&SortVector::CocktailSort, &vector);
                        algName = "Cocktail Sort";
                        break;
                    case COMB:
                        sortThread = std::thread(&SortVector::CombSort, &vector);
                        algName = "Comb Sort";
                        break;
                    case SHELLSORT:
                        sortThread = std::thread(&SortVector::ShellSort, &vector);
                        algName = "ShellSort";
                        break;
                    case QUICKSORT:
                        sortThread = std::thread(&SortVector::QuickSort, &vector, 0, vector.GetSize());
                        algName = "QuickSort";
                        break;
                    case MERGESORT:
                        sortThread = std::thread(&SortVector::MergeSort, &vector, 0, vector.GetSize());
                        algName = "Merge Sort";
                        break;
                    case COUNTING:
                        sortThread = std::thread(&SortVector::CountingSort, &vector);
                        algName = "Counting Sort";
                        break;
                    case RADIXLSD:
                        sortThread = std::thread(&SortVector::RadixLSDSort, &vector, static_cast<uint>(base));
                        algName = "Radix LSD Sort (base " + std::to_string(base) + ")";
                        break;
                    case RADIXMSD:
                        sortThread = std::thread(&SortVector::RadixMSDSort, &vector, static_cast<uint>(base), 0,
                                                 vector.GetSize());
                        algName = "Radix MSD Sort (base " + std::to_string(base) + ")";
                        break;
                    case STOOGE:
                        sortThread = std::thread(&SortVector::StoogeSort, &vector, 0, vector.GetSize());
                        algName = "Stooge Sort";
                        break;
                    case BOGOSORT:
                        sortThread = std::thread(&SortVector::BogoSort, &vector);
                        algName = "BogoSort";
                        break;
                    default:
                        algorithm = NONE;
                        vector.Deactivate();
                        vector.Clear();
                        break;
                }
            } else {
                Display(algName, vector, color, maxLength);
            }
        } else {
            algorithm = NONE;
        }

        window.clear(clear_color);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}

bool Display(const std::string &name, SortVector &vector, int color, uint buttonLength) {

    static bool fastForward = false;

    static bool infoOpened = false;

    static int rowWidth = DEFAULT_ROW_WIDTH;

    static char buffer[12];

    ImGuiIO io = ImGui::GetIO();

    ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::PushFont(io.Fonts->Fonts[1]);
    ImGui::Text("Reads: %-10d  Writes: %-10d  Compares: %-10d  Swaps: %-10d",
                vector.GetRead(), vector.GetWrite(), vector.GetCompare(), vector.GetSwap());
    ImGui::PopFont();

    for (uint i = 0; i < vector.GetSize(); i++) {

        ImGui::PushID(i);

        if (color) {
            if (vector.GetIterators().count(i)) {
                ImGui::PushStyleColor(ImGuiCol_Button,
                                      (ImVec4) ImColor::HSV((float) vector.GetElement(i) / vector.GetSize(),
                                                            1.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                                      (ImVec4) ImColor::HSV((float) vector.GetElement(i) / vector.GetSize(),
                                                            1.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                                      (ImVec4) ImColor::HSV((float) vector.GetElement(i) / vector.GetSize(),
                                                            1.0f, 1.0f));
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button,
                                      (ImVec4) ImColor::HSV((float) vector.GetElement(i) / vector.GetSize(),
                                                            0.7f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                                      (ImVec4) ImColor::HSV((float) vector.GetElement(i) / vector.GetSize(),
                                                            0.7f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                                      (ImVec4) ImColor::HSV((float) vector.GetElement(i) / vector.GetSize(),
                                                            0.7f, 0.7f));
            }
        } else {
            if (vector.GetIterators().count(i)) {
                auto p = vector.GetIterators().find(i);
                ImGui::PushStyleColor(ImGuiCol_Button, StateColor(p->second));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, StateColor(p->second));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, StateColor(p->second));
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button, StateColor(IDLE));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, StateColor(IDLE));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, StateColor(IDLE));
            }
        }
        if (color == 2)
            ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4) ImColor::HSV((float) vector.GetElement(i) / vector.GetSize(),
                                                                       0.50f, 1.00f));
        if (color == 1) {
            ImGui::Button("  ");
        } else {
            sprintf(buffer, "%*d", buttonLength, vector.GetElement(i));
            ImGui::Button(buffer);
        }

        ImGui::PopStyleColor(3);
        if (color == 2)
            ImGui::PopStyleColor();

        ImGui::PopID();

        if ((i + 1) % rowWidth && i + 1 != vector.GetSize())
            ImGui::SameLine();
    }

    static int frameskip = 0;

    ImGui::InputInt("Frameskip      ", &frameskip);
    if (frameskip < MIN_FRAMESKIP)
        frameskip = MIN_FRAMESKIP;
    if (frameskip > MAX_FRAMESKIP)
        frameskip = MAX_FRAMESKIP;

    vector.SetFrameskip(frameskip);

    ImGui::SameLine();

    if (ImGui::Button(">")) {
        if (frameskip < 0)
            while (vector.StateHasNotChanged())
                vector.Resume();
        vector.Resume();
    }

    ImGui::SameLine();

    if (fastForward)
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
    else
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Button));

    if (vector.GetIterators().empty())
        fastForward = false;

    if (ImGui::Button(">>"))
        fastForward = !fastForward;

    ImGui::PopStyleColor();

    if (!infoOpened) {
        ImGui::SameLine();
        if (ImGui::Button("Info")) {
            infoOpened = true;
        }
    } else {
        if (!DisplayInfo(name)) {
            infoOpened = false;
        }
    }

    if (fastForward)
        vector.Resume();


    ImGui::InputInt("Width", &rowWidth);
    if (rowWidth < 1)
        rowWidth = 1;

    ImGui::End();

}

bool DisplayInfo(const std::string &name) {
    std::string path = name;
    if (path.substr(7, 7) == "SD Sort") { // Radix LSD and MSD sorts have the "base X" label. We need to get rid of that
        path = path.substr(0, 14);
    }

    bool headerMode = false;

    bool closed = false;

    ImGuiIO io = ImGui::GetIO();

    ImGui::Begin((path + " Info").c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    try {
        std::ifstream infoFile("json/" + path + ".json");
        json info;
        infoFile >> info;

        ImGui::PushFont(io.Fonts->Fonts[2]);
        ImGui::Text("%s", info["name"].get<std::string>().c_str());
        ImGui::PopFont();

        ImGui::PushFont(io.Fonts->Fonts[1]);
        ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4) ImColor(0.9f, 0.9f, 0.9f));
        ImGui::Text(u8"Time complexity: Ω(%s) (best), Θ(%s) (average), O(%s) (worst)",
                    info["best-case"].get<std::string>().c_str(),
                    info["average-case"].get<std::string>().c_str(),
                    info["worst-case"].get<std::string>().c_str());

        ImGui::Text("");

        json text = info["text"];

        for (const auto &line : text.items()) {
            if (line.value() == "{/header}" && headerMode) {
                ImGui::PushFont(io.Fonts->Fonts[1]);
                ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4) ImColor(0.9f, 0.9f, 0.9f));
                headerMode = false;
            } else if (line.value() == "{header}" && !headerMode) {
                ImGui::PopStyleColor();
                ImGui::PopFont();
                headerMode = true;
            } else {
                ImGui::Text("%s", line.value().get<std::string>().c_str());
            }
        }

        infoFile.close();

        ImGui::Text("");

        if (!headerMode) {
            ImGui::PopStyleColor();
            ImGui::PopFont();
        }
    }
    catch (const nlohmann::detail::parse_error &error) {
        ImGui::Text("%s", error.what());
    }
    catch (const nlohmann::detail::type_error &error) {
        ImGui::Text("%s", error.what());
    }

    if (ImGui::Button("Close")) {
        closed = true;
    }

    ImGui::End();

    return !closed;
}