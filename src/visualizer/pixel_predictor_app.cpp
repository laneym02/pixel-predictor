//
// Created by Laney Moy on 12/4/2020.
//

#include <visualizer/pixel_predictor_app.h>

namespace pixel_predictor {

namespace visualizer {

PixelPredictorApp::PixelPredictorApp()
    : sketchpad_(glm::vec2(kMargin, kMargin), kImageHeight, kImageWidth,
                 kWindowWidth - 2 * kMargin) {
  ci::app::setWindowSize((int)kWindowWidth, (int)(kWindowHeight));
}

void PixelPredictorApp::draw() {
  ci::gl::clear(background_color_);

  sketchpad_.Draw();

  ci::gl::drawStringCentered("Welcome to Pixel Predictor! Draw part of an "
                             "image and watch the predictor fill in the rest.",
                             glm::vec2(kWindowWidth / 2, kMargin / 2),
                             ci::Color("black"));

  ci::gl::drawStringCentered(
      "Press the Down arrow to change color. Press Delete to clear the "
      "sketchpad. Press Enter to predict the remaining pixels.",
      glm::vec2(kWindowWidth / 2, kWindowHeight - kMargin / 2),
      ci::Color("black"));
}

void PixelPredictorApp::mouseDown(ci::app::MouseEvent event) {
  sketchpad_.HandleBrush(event.getPos());
}

void PixelPredictorApp::mouseDrag(ci::app::MouseEvent event) {
  sketchpad_.HandleBrush(event.getPos());
}

void PixelPredictorApp::keyDown(ci::app::KeyEvent event) {
  switch (event.getCode()) {
  case ci::app::KeyEvent::KEY_RETURN:
    break;
  case ci::app::KeyEvent::KEY_DELETE:
    sketchpad_.Clear();
    break;
  case ci::app::KeyEvent::KEY_DOWN:
    sketchpad_.NextColor();
    break;
  }
}

} // namespace visualizer

} // namespace pixel_predictor