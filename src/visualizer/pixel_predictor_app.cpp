//
// Created by Laney Moy on 12/4/2020.
//

#include <visualizer/pixel_predictor_app.h>

namespace pixel_predictor {

namespace visualizer {

PixelPredictorApp::PixelPredictorApp()
    : sketchpad_(glm::vec2(kMargin, kMargin), kImageDimension,
                 kWindowSize - 2 * kMargin) {
  ci::app::setWindowSize((int)kWindowSize, (int)kWindowSize);
}

void PixelPredictorApp::draw() {
  ci::Color8u background_color(255, 246, 148); // light yellow
  ci::gl::clear(background_color);

  sketchpad_.Draw();

  ci::gl::drawStringCentered(
      "Press Delete to clear the sketchpad. Press Enter to make a prediction.",
      glm::vec2(kWindowSize / 2, kMargin / 2), ci::Color("black"));

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
}

} // namespace visualizer

} // namespace pixel_predictor