//
// Created by Laney Moy on 12/4/2020.
//

#include <visualizer/sketchpad.h>

using glm::vec2;

namespace pixel_predictor {

namespace visualizer {

Sketchpad::Sketchpad(const glm::vec2 &top_left_corner, size_t height_pixels,
                     size_t width_pixels, double sketchpad_width,
                     double brush_radius)
    : top_left_corner_(top_left_corner), height_pixels_(height_pixels),
      width_pixels_(width_pixels),
      pixel_side_length_(sketchpad_width / width_pixels),
      brush_radius_(brush_radius) {
  for (size_t row = 0; row < height_pixels_; ++row) {
    vector<vec3> row_colors;
    row_colors.reserve(width_pixels_);
    for (size_t col = 0; col < width_pixels_; ++col) {
      row_colors.push_back(default_color_);
    }
    colors_.push_back(row_colors);
  }
}

void Sketchpad::Draw() const {
  for (size_t row = 0; row < height_pixels_; ++row) {
    for (size_t col = 0; col < width_pixels_; ++col) {
      ci::Rectf pixel_bounding_box = GetPixelBoundingBox(row, col);

      if (colors_[row][col] == default_color_) {
        DrawEmpty(pixel_bounding_box);
      } else {
        ci::gl::color(ci::Color(colors_[row][col][0], colors_[row][col][1],
                                colors_[row][col][2]));
        ci::gl::drawSolidRect(pixel_bounding_box);
      }

      ci::gl::color(border_color_);
      ci::gl::drawStrokedRect(pixel_bounding_box);
    }
  }
}

void Sketchpad::HandleBrush(const glm::vec2 &brush_screen_coords) {
  vec2 brush_sketchpad_coords =
      (brush_screen_coords - top_left_corner_) / (float)pixel_side_length_;

  for (size_t row = 0; row < height_pixels_; ++row) {
    for (size_t col = 0; col < width_pixels_; ++col) {
      vec2 pixel_center = {col + 0.5, row + 0.5};

      if (glm::distance(brush_sketchpad_coords, pixel_center) <=
          brush_radius_) {
        colors_[row][col] = GetBrushColor();
      }
    }
  }
}

void Sketchpad::Clear() {
  for (size_t row = 0; row < height_pixels_; ++row) {
    for (size_t col = 0; col < width_pixels_; ++col) {
      colors_[row][col] = default_color_;
    }
  }
}

vector<double> Sketchpad::GetColorVector(int row, int col) {
  return vector<double>(
      {colors_[row][col][0], colors_[row][col][1], colors_[row][col][2]});
}

vec3 Sketchpad::GetBrushColor() { return brush_colors_[brush_color_index_]; }

void Sketchpad::NextColor() {
  ++brush_color_index_;
  if (brush_color_index_ >= brush_colors_.size()) {
    brush_color_index_ = 0;
  }
}

void Sketchpad::DrawEmpty(ci::Rectf rectangle) const {
  ci::gl::color(cinder::Color("white"));
  ci::gl::drawSolidRect(rectangle);
  ci::gl::color(cinder::Color("red"));
  ci::gl::drawLine(vec2(rectangle.x1, rectangle.y2),
                   vec2(rectangle.x2, rectangle.y1));
}

vector<double> Sketchpad::GetDefaultColorVector() {
  return {default_color_[0], default_color_[1], default_color_[2]};
}

void Sketchpad::DrawPrediction(const vector<vector<double>> &predicted_colors) {
  auto iterator = predicted_colors.begin();

  for (size_t col = 0; col < width_pixels_; ++col) {
    for (size_t row = 0; row < height_pixels_; ++row) {
      if (colors_[row][col] == default_color_) {
        ci::Rectf pixel_bounding_box = GetPixelBoundingBox(row, col);

        vector<double> next_color = *iterator;
        ci::gl::color((float)next_color[0], (float)next_color[1],
                      (float)next_color[2]);
        ci::gl::drawSolidRect(pixel_bounding_box);

        ci::gl::color(border_color_);
        ci::gl::drawStrokedRect(pixel_bounding_box);

        ++iterator;
      }
    }
  }
}

ci::Rectf Sketchpad::GetPixelBoundingBox(size_t row, size_t col) const {
  vec2 pixel_top_left = top_left_corner_ + vec2(col * pixel_side_length_,
                                                row * pixel_side_length_);
  vec2 pixel_bottom_right =
      pixel_top_left + vec2(pixel_side_length_, pixel_side_length_);
  return {pixel_top_left, pixel_bottom_right};
}

} // namespace visualizer

} // namespace pixel_predictor