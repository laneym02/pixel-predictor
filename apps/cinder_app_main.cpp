//
// Created by Laney Moy on 11/17/2020.
//
#include <visualizer/pixel_predictor_app.h>

using pixel_predictor::visualizer::PixelPredictorApp;

void prepareSettings(PixelPredictorApp::Settings* settings) {
  settings->setResizable(false);
}

// This line is a macro that expands into an "int main()" function.
CINDER_APP(PixelPredictorApp, ci::app::RendererGl, prepareSettings);
