#pragma once

#include <cmath>
#include <imgui.h>

struct HeronSteps {
  float a, b, c;      
  float semiPerimeter;
  float area;         
  bool valid;         

  void calculate() {
    valid = (a + b > c) && (a + c > b) && (b + c > a);
    if (!valid) {
      semiPerimeter = 0.0f;
      area = 0.0f;
      return;
    }

    semiPerimeter = (a + b + c) / 2.0f;

    area = sqrt(semiPerimeter * (semiPerimeter - a) * (semiPerimeter - b) * (semiPerimeter - c));
  }
};


void render_heron_steps_panel(HeronSteps& heron_steps) {
  ImGui::Begin("Kroki liczenia pola - Wzor Herona", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

  ImGui::DragFloat("Bok A", &heron_steps.a, 0.1f, -50.0f, 50.0f);
  ImGui::DragFloat("Bok B", &heron_steps.b, 0.1f, -50.0f, 50.0f);
  ImGui::DragFloat("Bok C", &heron_steps.c, 0.1f, -50.0f, 50.0f);

  if (ImGui::Button("Oblicz")) {
    heron_steps.calculate();
  }

  if (heron_steps.valid) {
    ImGui::Text("1. Sprawdzenie, czy mozna zbudowac trojkat:");
    ImGui::Text("   %s", "Mozna zbudowac trojkat");

    ImGui::Separator();
    ImGui::Text("2. Obliczenie polobwodu (s):");
    ImGui::Text("   s = (a + b + c) / 2");
    ImGui::Text("   s = (%.2f + %.2f + %.2f) / 2 = %.2f", heron_steps.a, heron_steps.b, heron_steps.c, heron_steps.semiPerimeter);

    ImGui::Separator();
    ImGui::Text("3. Obliczenie skladnikow wzoru Herona:");
    ImGui::Text("   s - a = %.2f", heron_steps.semiPerimeter - heron_steps.a);
    ImGui::Text("   s - b = %.2f", heron_steps.semiPerimeter - heron_steps.b);
    ImGui::Text("   s - c = %.2f", heron_steps.semiPerimeter - heron_steps.c);

    ImGui::Separator();
    ImGui::Text("4. Obliczenie pola:");
    ImGui::Text("   Pole = sqrt(s * (s - a) * (s - b) * (s - c))");
    ImGui::Text("   Pole = sqrt(%.2f * %.2f * %.2f * %.2f) = %.2f",
                heron_steps.semiPerimeter,
                heron_steps.semiPerimeter - heron_steps.a,
                heron_steps.semiPerimeter - heron_steps.b,
                heron_steps.semiPerimeter - heron_steps.c,
                heron_steps.area);
  } else {
    ImGui::Text("Blad: Nie mozna zbudowac trojkata z podanych bokow!");
  }

  ImGui::End();
}
