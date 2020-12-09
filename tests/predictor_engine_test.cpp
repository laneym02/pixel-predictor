//
// Created by Laney Moy on 12/3/2020.
//
#include <core/predictor_engine.h>

#include <catch2/catch.hpp>

using pixel_predictor::PredictorEngine;

TEST_CASE("Invalid constructor arguments") {
  SECTION("Max input vector empty") {
    REQUIRE_THROWS_AS(PredictorEngine(vector<double>(), vector<double>({1, 1})),
                      std::invalid_argument);
  }

  SECTION("Max output vector empty") {
    REQUIRE_THROWS_AS(PredictorEngine(vector<double>({1, 1}), vector<double>()),
                      std::invalid_argument);
  }

  SECTION("Both max vectors empty") {
    REQUIRE_THROWS_AS(PredictorEngine(vector<double>(), vector<double>()),
                      std::invalid_argument);
  }
}

TEST_CASE("Invalid processing arguments") {
  SECTION("Different number of inputs and outputs") {
    srand(0);
    PredictorEngine predictor_engine(vector<double>({1, 1}),
                                     vector<double>({1, 1, 1}));

    REQUIRE_THROWS_AS(predictor_engine.ProcessData(
                          vector<vector<double>>({{1, .5}, {0.5, .25}}),
                          vector<vector<double>>({{1, 0, .5}})),
                      std::invalid_argument);
  }

  SECTION("Incorrect input size") {
    srand(0);
    PredictorEngine predictor_engine(vector<double>({1, 1}),
                                     vector<double>({1, 1, 1}));

    REQUIRE_THROWS_AS(predictor_engine.ProcessData(
                          vector<vector<double>>({{1, .5}, {0.5}}),
                          vector<vector<double>>({{1, 0, .5}, {0, .2, .5}})),
                      std::invalid_argument);
  }

  SECTION("Incorrect output size") {
    srand(0);
    PredictorEngine predictor_engine(vector<double>({1, 1}),
                                     vector<double>({1, 1, 1}));

    REQUIRE_THROWS_AS(predictor_engine.ProcessData(
                          vector<vector<double>>({{1, .5}, {0.5, .25}}),
                          vector<vector<double>>({{1, 0, .5}, {0, 0.3}})),
                      std::invalid_argument);
  }
}

TEST_CASE("Invalid prediction arguments") {
  SECTION("Empty vector") {
    srand(0);
    PredictorEngine predictor_engine(vector<double>({1, 1}),
                                     vector<double>({1}));
    REQUIRE_THROWS_AS(predictor_engine.Predict(vector<double>()),
                      std::invalid_argument);
  }
  SECTION("Incorrect size") {
    srand(0);
    PredictorEngine predictor_engine(vector<double>({1, 1}),
                                     vector<double>({1}));
    REQUIRE_THROWS_AS(predictor_engine.Predict(vector<double>({1})),
                      std::invalid_argument);
  }
}

TEST_CASE("Process data and make predictions") {
  SECTION("Max of 1") {
    srand(0);
    PredictorEngine predictor_engine(vector<double>({1, 1}),
                                     vector<double>({1, 1, 1}));

    predictor_engine.ProcessData(
        vector<vector<double>>({{1, .5}, {0.5, .25}}),
        vector<vector<double>>({{1, 0, .5}, {0.5, 0.2, .7}}), 20);

    vector<double> output = predictor_engine.Predict(vector<double>({1, .5}));

    REQUIRE(output.at(0) == Approx(0.5871288934));
    REQUIRE(output.at(1) == Approx(0.2842217277));
    REQUIRE(output.at(2) == Approx(0.4417590979));
  }

  SECTION("Input max greater than 1") {
    srand(0);
    PredictorEngine predictor_engine(vector<double>({10, 10}),
                                     vector<double>({1, 1, 1}));

    predictor_engine.ProcessData(
        vector<vector<double>>({{5, 7}, {0.5, 8.1}}),
        vector<vector<double>>({{0.1, 0, .35}, {0.5, .2, .7}}), 20);

    vector<double> output = predictor_engine.Predict(vector<double>({1, 5}));

    REQUIRE(output.at(0) == Approx(0.2571575368));
    REQUIRE(output.at(1) == Approx(0.2876872274));
    REQUIRE(output.at(2) == Approx(0.4562224389));
  }

  SECTION("Output max greater than 1") {
    srand(0);
    PredictorEngine predictor_engine(vector<double>({1, 1}),
                                     vector<double>({10, 10, 10}));

    predictor_engine.ProcessData(
        vector<vector<double>>({{0.5, 0.7}, {0.5, 0.81}}),
        vector<vector<double>>({{1, 0, 3.5}, {5, .2, 7}}), 20);

    vector<double> output = predictor_engine.Predict(vector<double>({1, 0.5}));

    REQUIRE(output.at(0) == Approx(2.9547138933));
    REQUIRE(output.at(1) == Approx(2.6363956761));
    REQUIRE(output.at(2) == Approx(4.3019408417));
  }

  SECTION("Different max values among inputs") {
    srand(0);
    PredictorEngine predictor_engine(vector<double>({10, 5}),
                                     vector<double>({1, 1, 1}));

    predictor_engine.ProcessData(
        vector<vector<double>>({{5, 2}, {0.5, 4.1}}),
        vector<vector<double>>({{0.1, 0, .35}, {0.5, .2, .7}}), 20);

    vector<double> output = predictor_engine.Predict(vector<double>({1, 5}));

    REQUIRE(output.at(0) == Approx(0.2631903741));
    REQUIRE(output.at(1) == Approx(0.2774133469));
    REQUIRE(output.at(2) == Approx(0.4416153803));
  }

  SECTION("Different max values among outputs") {
    srand(0);
    PredictorEngine predictor_engine(vector<double>({1, 1}),
                                     vector<double>({10, 5, 7}));

    predictor_engine.ProcessData(
        vector<vector<double>>({{5, 2}, {0.5, 4.1}}),
        vector<vector<double>>({{0.1, 0, 3.5}, {5, 2, 6.7}}), 20);

    vector<double> output = predictor_engine.Predict(vector<double>({1, 0.5}));

    REQUIRE(output.at(0) == Approx(2.1872291336));
    REQUIRE(output.at(1) == Approx(1.9174608538));
    REQUIRE(output.at(2) == Approx(4.4213548474));
  }

  SECTION("Inputs and outputs of other sizes") {
    srand(0);
    PredictorEngine predictor_engine(vector<double>({1, 1, 1}),
                                     vector<double>({1, 1, 1, 1}));

    predictor_engine.ProcessData(
        vector<vector<double>>({{1, .5, .3}, {0.5, .25, .2}}),
        vector<vector<double>>({{1, 0, .5, .3}, {0.5, 0.2, .7, .2}}), 20);

    vector<double> output =
        predictor_engine.Predict(vector<double>({1, 0.5, .1}));

    REQUIRE(output.at(0) == Approx(0.72319726));
    REQUIRE(output.at(1) == Approx(0.4050941157));
    REQUIRE(output.at(2) == Approx(0.276713194));
  }
}