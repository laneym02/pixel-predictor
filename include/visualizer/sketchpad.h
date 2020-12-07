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
   * Creates a sketchpad.
   * (Note that sketchpad pixels are larger than screen pixels.)
   *
   * @param top_left_corner     the screen coordinates of the top left corner of
   *                            the sketchpad
   * @param num_pixels_per_side the number of sketchpad pixels in one row/column
   *                            of the sketchpad
   * @param sketchpad_size      the side length of the sketchpad, measured in
   *                            screen pixels
   * @param brush_radius        the maximum distance (measured in sketchpad
   *                            pixels) from the brush that will be shaded
   */
  Sketchpad(const glm::vec2 &top_left_corner, size_t height_pixels,
            size_t width_pixels, double sketchpad_width,
            double brush_radius = 1.15);

  /**
   * Displays the current state of the sketchpad in the Cinder application.
   */
  void Draw() const;

  /**
   * Shades in the sketchpad pixels whose centers are within brush_radius units
   * of the brush's location. (One unit is equal to the length of one sketchpad
   * pixel.)
   *
   * @param brush_screen_coords the screen coordinates at which the brush is
   *           located
   */
  void HandleBrush(const glm::vec2 &brush_screen_coords);

  /**
   * Set all of the sketchpad pixels to an unshaded state.
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

  /** Number of screen pixels in the width/height of one sketchpad pixel */
  double pixel_side_length_;

  ci::Color8u border_color_ = ci::Color8u(0, 0, 0);

  double brush_radius_;
  vector<vec3> brush_colors_ = vector<vec3>(
      {vec3(0, 0, 0), vec3(255, 0, 0), vec3(255, 127, 0), vec3(255, 255, 0),
       vec3(0, 255, 0), vec3(0, 255, 255), vec3(0, 0, 255), vec3(127, 0, 255),
       vec3(255, 0, 255), vec3(255, 255, 255)});
  size_t brush_color_index_ = 0;

  vec3 default_color_ = vec3(-1, -1, -1);

  vector<vector<vec3>> colors_;

  void drawEmpty(ci::Rectf rectangle) const;
};

} // namespace visualizer

} // namespace pixel_predictor

#endif // FINAL_PROJECT_INCLUDE_VISUALIZER_SKETCHPAD_H_
