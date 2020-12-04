//
// Created by Laney Moy on 12/4/2020.
//

#include <visualizer/pixel_predictor_app.h>

namespace pixel_predictor {

namespace visualizer {

PixelPredictorApp::PixelPredictorApp() {}

void PixelPredictorApp::draw() { AppBase::draw(); }

void PixelPredictorApp::mouseDown(ci::app::MouseEvent event) {
  AppBase::mouseDown(event);
}

void PixelPredictorApp::mouseDrag(ci::app::MouseEvent event) {
  AppBase::mouseDrag(event);
}

void PixelPredictorApp::keyDown(ci::app::KeyEvent event) {
  AppBase::keyDown(event);
}

} // namespace visualizer

} // namespace pixel_predictor