//
// Created by Laney Moy on 12/4/2020.
//

#include <visualizer/pixel_predictor_app.h>

namespace pixel_predictor {

namespace visualizer {

PixelPredictorApp::PixelPredictorApp()
    : sketchpad_(vec2(kMargin, kMargin + kColorSwatchSize + 2 * kColorSwatchMargin), kImageHeight, kImageWidth,
                 kWindowWidth - 2 * kMargin) {
  ci::app::setWindowSize((int)kWindowWidth, (int)(kWindowHeight));
}

void PixelPredictorApp::update() {
  ci::gl::clear(ci::Color("white"));

  sketchpad_.Draw();
  if (!isInSketchpadMode) {
    sketchpad_.DrawPrediction(predicted_colors_);
  }

  ci::gl::drawStringCentered("Welcome to Pixel Predictor! Draw part of an "
                             "image and watch the predictor fill in the rest.",
                             vec2(kWindowWidth / 2, kMargin / 2),
                             ci::Color("black"), ci::Font("Calibri", 20));

  ci::gl::drawStringCentered(
      "Current brush color:", vec2(kWindowWidth / 2, kMargin),
      ci::Color("black"));

  DrawColorSwatch((int)(kWindowWidth / 2 - kColorSwatchSize / 2),
                  (int)(kMargin + kColorSwatchMargin));

  ci::gl::drawStringCentered(
      "Press the Down arrow to change color. Press Delete to clear the "
      "sketchpad. Press Enter to predict the remaining pixels.",
      vec2(kWindowWidth / 2, kWindowHeight - kMargin / 2), ci::Color("black"));
}

void PixelPredictorApp::mouseDown(ci::app::MouseEvent event) {
  if (isInSketchpadMode) {
    sketchpad_.HandleBrush(event.getPos());
  }
}

void PixelPredictorApp::mouseDrag(ci::app::MouseEvent event) {
  if (isInSketchpadMode) {
    sketchpad_.HandleBrush(event.getPos());
  }
}

void PixelPredictorApp::keyDown(ci::app::KeyEvent event) {
  switch (event.getCode()) {
  case ci::app::KeyEvent::KEY_RETURN:
    sketchpad_.Clear();
    predictor_engine_.Reset();
    populated_pixel_colors_.clear();
    populated_pixels_.clear();
    empty_pixels_.clear();
    predicted_colors_.clear();
    isInSketchpadMode = true;
    break;
  case ci::app::KeyEvent::KEY_DELETE:
    if (isInSketchpadMode) {
      sketchpad_.Clear();
    }
    break;
  case ci::app::KeyEvent::KEY_DOWN:
    if (isInSketchpadMode) {
      sketchpad_.NextColor();
    }
    break;
  case ci::app::KeyEvent::KEY_p:
    if (isInSketchpadMode) {
      isInSketchpadMode = false;
      PredictPixels();
    }
    break;
  }
}

void PixelPredictorApp::DrawColorSwatch(int x1, int y1) {
  ci::Rectf bounding_box(
      vec2(x1, y1), vec2(x1, y1) + vec2(kColorSwatchSize, kColorSwatchSize));
  vec3 color_vector = sketchpad_.GetBrushColor();
  ci::gl::color(color_vector[0], color_vector[1], color_vector[2]);
  ci::gl::drawSolidRect(bounding_box);
  ci::gl::color(cinder::Color("black"));
  ci::gl::drawStrokedRect(bounding_box);
}
void PixelPredictorApp::PredictPixels() {
  for (size_t row = 0; row < kImageHeight; ++row) {
    for (size_t col = 0; col < kImageWidth; ++col) {
      vector<double> color = sketchpad_.GetColorVector(row, col);
      if (color[0] < 0) {
        empty_pixels_.push_back(vector<double>({(double)row, (double)col}));
      } else {
        populated_pixels_.push_back(vector<double>({(double)row, (double)col}));
        populated_pixel_colors_.push_back(color);
      }
    }
  }

  predictor_engine_.ProcessData(populated_pixels_, populated_pixel_colors_);

  for (const vector<double> &location : empty_pixels_) {
    predicted_colors_.push_back(predictor_engine_.Predict(location));
  }
}

} // namespace visualizer

} // namespace pixel_predictor