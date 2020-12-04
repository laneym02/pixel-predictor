//
// Created by Laney Moy on 12/4/2020.
//

#include <visualizer/sketchpad.h>

namespace pixel_predictor {

namespace visualizer {

Sketchpad::Sketchpad(const glm::vec2 &top_left_corner,
                     size_t num_pixels_per_side, double sketchpad_size,
                     double brush_radius) {}

void Sketchpad::Draw() const {}

void Sketchpad::HandleBrush(const glm::vec2 &brush_screen_coords) {}

void Sketchpad::Clear() {}

}

} // namespace pixel_predictor