//
// Created by Laney Moy on 12/4/2020.
//

#include <visualizer/pixel_predictor_app.h>

namespace pixel_predictor {

namespace visualizer {

PixelPredictorApp::PixelPredictorApp()
    : sketchpad_(sketchpad_top_left_, kImageHeight, kImageWidth,
                 kWindowWidth - 2 * kMargin) {
  ci::app::setWindowSize((int)kWindowWidth, (int)(kWindowHeight));
}

void PixelPredictorApp::draw() {
  ci::gl::clear(background_color_);

  sketchpad_.Draw();

  ci::gl::drawStringCentered("Welcome to Pixel Predictor! Draw part of an "
                             "image and watch the predictor fill in the rest.",
                             vec2(kWindowWidth / 2, kMargin / 2),
                             ci::Color("black"));

  ci::gl::drawStringCentered(
      "Current brush color:", vec2(kWindowWidth / 2, kMargin),
      ci::Color("black"));

  drawColorSwatch((int)(kWindowWidth / 2 - kColorSwatchSize / 2),
                  (int)(kMargin + kColorSwatchMargin));

  ci::gl::drawStringCentered(
      "Press the Down arrow to change color. Press Delete to clear the "
      "sketchpad. Press Enter to predict the remaining pixels.",
      vec2(kWindowWidth / 2, kWindowHeight - kMargin / 2), ci::Color("black"));
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

void PixelPredictorApp::drawColorSwatch(int x1, int y1) {
  ci::Rectf bounding_box(
      vec2(x1, y1), vec2(x1, y1) + vec2(kColorSwatchSize, kColorSwatchSize));
  vec3 color_vector = sketchpad_.GetBrushColor();
  ci::gl::color(color_vector[0], color_vector[1], color_vector[2]);
  ci::gl::drawSolidRect(bounding_box);
  ci::gl::color(cinder::Color("black"));
  ci::gl::drawStrokedRect(bounding_box);
}

} // namespace visualizer

} // namespace pixel_predictor