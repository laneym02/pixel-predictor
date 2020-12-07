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

namespace pixel_predictor {

namespace visualizer {

/**
 * Allows a user to draw a digit on a sketchpad and uses Naive Bayes to
 * classify it.
 */
class PixelPredictorApp : public ci::app::App {
public:
  PixelPredictorApp();

  void draw() override;
  void mouseDown(ci::app::MouseEvent event) override;
  void mouseDrag(ci::app::MouseEvent event) override;
  void keyDown(ci::app::KeyEvent event) override;

  const double kWindowWidth = 1000;
  const double kMargin = 100;
  const size_t kImageHeight = 25;
  const size_t kImageWidth = 50;
  const double kWindowHeight =
      kMargin * 2 + (kWindowWidth - 2 * kMargin) / kImageWidth * kImageHeight;

private:
  Sketchpad sketchpad_;

  ci::Color8u background_color_ = ci::Color8u(255, 246, 148); // light yellow
};

} // namespace visualizer

} // namespace pixel_predictor

#endif // FINAL_PROJECT_INCLUDE_VISUALIZER_PIXEL_PREDICTOR_APP_H_
