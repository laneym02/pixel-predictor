//
// Created by Laney Moy on 12/3/2020.
//
#include <core/predictor_engine.h>

#include <catch2/catch.hpp>

using pixel_predictor::PredictorEngine;

TEST_CASE("test") {
  SECTION("testing") {
    PredictorEngine predictor_engine(vector<double>({1, 1}),
                                     vector<double>({1, 1, 1}));
    REQUIRE(1 == predictor_engine.AdjustOutput(vector<double>({1,.5}))[0]);
    REQUIRE(.5 == predictor_engine.AdjustOutput(vector<double>({1,.5}))[1]);
  }
}

TEST_CASE("Process data") {
  SECTION("Max of 1") {
    srand(0);
    PredictorEngine predictor_engine(vector<double>({1, 1}),
                                     vector<double>({1, 1, 1}));

    predictor_engine.ProcessData(
        vector<vector<double>>({{1, .5}, {0.5, .25}}),
        vector<vector<double>>({{1, 0, .5}, {0.5, 0.2, .7}}), 20);

    vector<double> output = predictor_engine.Predict(vector<double>({1, .5}));

    REQUIRE(output.at(0) == Approx(0.7556591268));
    REQUIRE(output.at(1) == Approx(0.1408790987));
    REQUIRE(output.at(2) == Approx(0.6088205007));
  }
}