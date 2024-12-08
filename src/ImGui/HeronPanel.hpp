#ifndef HERON_PANEL_HPP_INCLUDED
#define HERON_PANEL_HPP_INCLUDED 1

#include <math.h>
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
  ImGui::Begin("Steps for calculating - Heron's formula", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

  ImGui::DragFloat("Side A", &heron_steps.a, 0.1f, -50.0f, 50.0f);
  ImGui::DragFloat("Side B", &heron_steps.b, 0.1f, -50.0f, 50.0f);
  ImGui::DragFloat("Side C", &heron_steps.c, 0.1f, -50.0f, 50.0f);

  if (ImGui::Button("Calculate")) {
    heron_steps.calculate();
  }

  if (heron_steps.valid) {
    ImGui::Text("1. Checking whether a triangle can be constructed:");
    ImGui::Text("   %s", "You can build a triangle");

    ImGui::Separator();
    ImGui::Text("2. Half circumference calculation (s):");
    ImGui::Text("   s = (a + b + c) / 2");
    ImGui::Text("   s = (%.2f + %.2f + %.2f) / 2 = %.2f", heron_steps.a, heron_steps.b, heron_steps.c, heron_steps.semiPerimeter);

    ImGui::Separator();
    ImGui::Text("3. Calculation of the components of Heron's formula:");
    ImGui::Text("   s - a = %.2f", heron_steps.semiPerimeter - heron_steps.a);
    ImGui::Text("   s - b = %.2f", heron_steps.semiPerimeter - heron_steps.b);
    ImGui::Text("   s - c = %.2f", heron_steps.semiPerimeter - heron_steps.c);

    ImGui::Separator();
    ImGui::Text("4. Area calculation:");
    ImGui::Text("   Area = sqrt(s * (s - a) * (s - b) * (s - c))");
    ImGui::Text("   Area = sqrt(%.2f * %.2f * %.2f * %.2f) = %.2f",
                heron_steps.semiPerimeter,
                heron_steps.semiPerimeter - heron_steps.a,
                heron_steps.semiPerimeter - heron_steps.b,
                heron_steps.semiPerimeter - heron_steps.c,
                heron_steps.area);
  } else {
    ImGui::Text("ERROR: A triangle cannot be formed with the given sides!");
  }

  ImGui::End();
}

#endif // HERON_PANEL_HPP_INCLUDED