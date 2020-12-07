//
// Created by Laney Moy on 12/4/2020.
//

#include <visualizer/pixel_predictor_app.h>

namespace pixel_predictor {

namespace visualizer {

PixelPredictorApp::PixelPredictorApp()
    : sketchpad_(
          vec2(kMargin, kMargin + kColorSwatchSize + 2 * kColorSwatchMargin),
          kImageHeight, kImageWidth, kWindowWidth - 2 * kMargin) {
  ci::app::setWindowSize((int)kWindowWidth, (int)(kWindowHeight));
}

void PixelPredictorApp::update() {
  ci::gl::clear(ci::Color("white"));

  sketchpad_.Draw();
  if (!is_in_sketchpad_mode_) {
    sketchpad_.DrawPrediction(predicted_colors_);
  }

  ci::gl::drawStringCentered(welcome_message_,
                             vec2(kWindowWidth / 2, kMargin / 2),
                             ci::Color("black"), ci::Font(font_name_, 30));

  ci::gl::drawStringCentered(
      "Current brush color:", vec2(kWindowWidth / 2, kMargin),
      ci::Color("black"), ci::Font(font_name_, 15));

  DrawColorSwatch((int)(kWindowWidth / 2 - kColorSwatchSize / 2),
                  (int)(kMargin + kColorSwatchMargin));

  string options_message;
  if (is_in_sketchpad_mode_) {
    options_message = options_sketchpad_mode_;
  } else {
    options_message = options_prediction_mode_;
  }

  ci::gl::drawStringCentered(
      options_message, vec2(kWindowWidth / 2, kWindowHeight - kMargin / 2),
      ci::Color("black"), ci::Font(font_name_, 20));
}

void PixelPredictorApp::mouseDown(ci::app::MouseEvent event) {
  if (is_in_sketchpad_mode_) {
    sketchpad_.HandleBrush(event.getPos());
  }
}

void PixelPredictorApp::mouseDrag(ci::app::MouseEvent event) {
  if (is_in_sketchpad_mode_) {
    sketchpad_.HandleBrush(event.getPos());
  }
}

void PixelPredictorApp::keyDown(ci::app::KeyEvent event) {
  switch (event.getCode()) {
  case ci::app::KeyEvent::KEY_RETURN:
    if (is_in_sketchpad_mode_) {
      PredictPixels();
      is_in_sketchpad_mode_ = false;
    } else {
      sketchpad_.Clear();
      predictor_engine_.Reset();
      populated_pixel_colors_.clear();
      populated_pixels_.clear();
      empty_pixels_.clear();
      predicted_colors_.clear();
      is_in_sketchpad_mode_ = true;
    }
    break;
  case ci::app::KeyEvent::KEY_DELETE:
    if (is_in_sketchpad_mode_) {
      sketchpad_.Clear();
    }
    break;
  case ci::app::KeyEvent::KEY_DOWN:
    if (is_in_sketchpad_mode_) {
      sketchpad_.NextColor();
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
  for (size_t col = 0; col < kImageWidth; ++col) {
    for (size_t row = 0; row < kImageHeight; ++row) {
      vector<double> color = sketchpad_.GetColorVector(row, col);
      if (color[0] < 0) {
        empty_pixels_.push_back(vector<double>({(double)row, (double)col}));
      } else {
        populated_pixels_.push_back(vector<double>({(double)row, (double)col}));
        populated_pixel_colors_.push_back(color);
      }
    }
  }

  predictor_engine_.ProcessData(populated_pixels_, populated_pixel_colors_, iterations_);

  for (const vector<double> &location : empty_pixels_) {
    predicted_colors_.push_back(predictor_engine_.Predict(location));
  }
}

} // namespace visualizer

} // namespace pixel_predictor