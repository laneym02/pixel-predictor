//
// Created by Laney Moy on 12/4/2020.
//

#ifndef FINAL_PROJECT_INCLUDE_VISUALIZER_PIXEL_PREDICTOR_APP_H_
#define FINAL_PROJECT_INCLUDE_VISUALIZER_PIXEL_PREDICTOR_APP_H_

#pragma once

#include <core/predictor_engine.h>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "sketchpad.h"

using glm::vec2;
using std::string;
using pixel_predictor::PredictorEngine;

namespace pixel_predictor {

namespace visualizer {

/**
 * Allows a user to draw a digit on a sketchpad and uses Naive Bayes to
 * classify it.
 */
class PixelPredictorApp : public ci::app::App {
public:
  PixelPredictorApp();

  void update() override;
  void mouseDown(ci::app::MouseEvent event) override;
  void mouseDrag(ci::app::MouseEvent event) override;
  void keyDown(ci::app::KeyEvent event) override;

  const double kMargin = 100;
  const size_t kImageHeight = 25;
  const size_t kImageWidth = 50;

  const double kColorSwatchSize = 70;
  const double kColorSwatchMargin = 30;

  const double kWindowWidth = 1000;
  const double kWindowHeight =
      kMargin * 2 + kColorSwatchMargin * 2 + kColorSwatchSize +
      (kWindowWidth - 2 * kMargin) / kImageWidth * kImageHeight;

private:
  bool is_in_sketchpad_mode_ = true;

  string font_name_ = "Calibri";
  string welcome_message_ =
      "Welcome to Pixel Predictor! Draw part of an image and watch the "
      "predictor fill in the rest.";
  string options_sketchpad_mode_ =
      "Press the Down arrow to change color. Press Delete to clear the "
      "sketchpad. Press P to predict the remaining pixels.";
  string options_prediction_mode_ = "Press Enter to draw another image.";

  Sketchpad sketchpad_;

  PredictorEngine predictor_engine_ = PredictorEngine(
      vector<double>({(double)kImageHeight, (double)kImageWidth}),
      vector<double>({1, 1, 1}));

  vector<vector<double>> populated_pixels_;
  vector<vector<double>> populated_pixel_colors_;
  vector<vector<double>> empty_pixels_;
  vector<vector<double>> predicted_colors_;

  void DrawColorSwatch(int x1, int y1);

  void PredictPixels();
};

} // namespace visualizer

} // namespace pixel_predictor

#endif // FINAL_PROJECT_INCLUDE_VISUALIZER_PIXEL_PREDICTOR_APP_H_
