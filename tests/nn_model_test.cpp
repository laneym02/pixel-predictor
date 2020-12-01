//
// Created by Laney Moy on 11/17/2020.
//

#include <core/nn-model.h>

#include <catch2/catch.hpp>

using pixel_predictor::neural_network::NeuralNetworkModel;

TEST_CASE("Get neural network output") {

  SECTION("One layer") {
    vector<int> layer_sizes({2, 3});
    vector<vector<double>> weights({{1, 0.5, 0.5, 1, 1, 0.25}});

    NeuralNetworkModel model(layer_sizes, weights);

    vector<double> output = model.Output(vector<double>({1, 2}));

    REQUIRE(output.at(0) == Approx(0.9525741268));
    REQUIRE(output.at(1) == Approx(0.92414182));
    REQUIRE(output.at(2) == Approx(0.7310585786));
  }

  SECTION("Three layers of different sizes") {
    vector<int> layer_sizes({2, 3, 2});
    vector<vector<double>> weights(
        {{1, 0.5, 0.5, 1, 1, 0.25}, {1, 0.5, 0.25, 0.5, 1, 0.5}});

    NeuralNetworkModel model(layer_sizes, weights);

    vector<double> output = model.Output(vector<double>({1, 2}));

    REQUIRE(output.at(0) == Approx(0.8715426836));
    REQUIRE(output.at(1) == Approx(0.7864884761));
  }
}

TEST_CASE("Train neural network") {
  SECTION("Iteratively train once on one data point") {
    vector<int> layer_sizes({2, 3, 2});
    vector<vector<double>> weights(
        {{1, 0.5, 0.5, 1, 1, 0.25}, {1, 0.5, 0.25, 0.5, 1, 0.5}});

    NeuralNetworkModel model(layer_sizes, weights);

    model.Train(
        vector<vector<double>>({{1, 2}}), vector<vector<double>>({{1, 0}}),
        vector<vector<double>>({{1, 2}}), vector<vector<double>>({{1, 0}}), 1);

    vector<double> output = model.Output(vector<double>({1,2}));

    REQUIRE(output.at(0) == Approx(0.8739710314));
    REQUIRE(output.at(1) == Approx(0.7302340494));
  }
}