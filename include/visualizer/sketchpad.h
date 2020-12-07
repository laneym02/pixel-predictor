//
// Created by Laney Moy on 12/4/2020.
//

#ifndef FINAL_PROJECT_INCLUDE_VISUALIZER_SKETCHPAD_H_
#define FINAL_PROJECT_INCLUDE_VISUALIZER_SKETCHPAD_H_

#pragma once

#include "cinder/gl/gl.h"

using glm::vec3;
using std::vector;

namespace pixel_predictor {

namespace visualizer {

/**
 * A sketchpad which will be displayed in the Cinder application and respond to
 * mouse events. Furthermore, the sketchpad can output its current state in the
 * same format as the Naive Bayes image data files.
 */
class Sketchpad {
public:
  /**
   * Creates a sketchpad
   * @param top_left_corner the upper left corner of the sketchpad
   * @param height_pixels the height of the sketchpad in sketchpad pixels
   * @param width_pixels the width of the sketchpad in sketchpad pixels
   * @param sketchpad_width the width of the sketchpad
   * @param brush_radius the radius of the brush
   */
  Sketchpad(const glm::vec2 &top_left_corner, size_t height_pixels,
            size_t width_pixels, double sketchpad_width,
            double brush_radius = 2);

  /**
   * Displays the current state of the sketchpad in the Cinder application
   */
  void Draw() const;

  /**
   * Colors missing pixels in the predicted colors
   * @param predicted_colors the colors predicted by the application
   */
  void DrawPrediction(const vector<vector<double>> &predicted_colors);

  /**
   * Colors in pixels within the radius of the brush
   * @param brush_screen_coords the location of the center of the brush
   */
  void HandleBrush(const glm::vec2 &brush_screen_coords);

  /**
   * Set all of the sketchpad pixels to an unshaded state
   */
  void Clear();

  /**
   * Get the RGB color vector at a pixel
   * @param row the row of the pixel
   * @param col the column of the pixel
   * @return a vector of the RGB values of the color at that pixel
   */
  vector<double> GetColorVector(int row, int col);

  /**
   * Get the current brush color
   * @return the current brush color
   */
  vec3 GetBrushColor();

  /**
   * Switch to the next color;
   */
  void NextColor();

private:
  glm::vec2 top_left_corner_;

  size_t height_pixels_;
  size_t width_pixels_;

  double pixel_side_length_;

  ci::Color8u border_color_ = ci::Color8u(0, 0, 0);

  double brush_radius_;
  vector<vec3> brush_colors_ =
      vector<vec3>({vec3(0, 0, 0), vec3(1, 0, 0), vec3(1, 0.5, 0),
                    vec3(1, 1, 0), vec3(0, 1, 0), vec3(0, 1, 1), vec3(0, 0, 1),
                    vec3(0.5, 0, 1), vec3(1, 0, 1), vec3(1, 1, 1)});
  size_t brush_color_index_ = 0;

  vec3 default_color_ = vec3(-1, -1, -1);

  vector<vector<vec3>> colors_;

  /**
   * Draw an empty sketchpad pixel
   * @param rectangle the pixel bounding box
   */
  void DrawEmpty(ci::Rectf rectangle) const;

  /**
   * Get the bounding box of a sketchpad pixel
   * @param row the pixel row
   * @param col the pixel column
   * @return the bounding box
   */
  ci::Rectf GetPixelBoundingBox(size_t row, size_t col) const;
};

} // namespace visualizer

} // namespace pixel_predictor

#endif // FINAL_PROJECT_INCLUDE_VISUALIZER_SKETCHPAD_H_
