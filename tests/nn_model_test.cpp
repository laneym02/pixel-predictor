//
// Created by Laney Moy on 11/17/2020.
//

#include <core/nn-model.h>

#include <catch2/catch.hpp>
#include <iostream>

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

  SECTION("More layers") {
    srand(0);
    vector<int> layer_sizes({2, 3, 4, 3});

    NeuralNetworkModel model(layer_sizes);

    vector<double> output = model.Output(vector<double>({1, 2}));

    REQUIRE(output.at(0) == Approx(0.7772072585));
    REQUIRE(output.at(1) == Approx(0.7494078919));
    REQUIRE(output.at(2) == Approx(0.9138059277));
  }
}

TEST_CASE("Train neural network") {
  SECTION("Train iteratively") {
    SECTION("Once on one data point") {
      vector<int> layer_sizes({2, 3, 2});
      vector<vector<double>> weights(
          {{1, 0.5, 0.5, 1, 1, 0.25}, {1, 0.5, 0.25, 0.5, 1, 0.5}});

      NeuralNetworkModel model(layer_sizes, weights);
      model.SetAlpha(1);

      model.Train(vector<vector<double>>({{1, 2}}),
                  vector<vector<double>>({{1, 0}}), 1);

      vector<double> output = model.Output(vector<double>({1, 2}));

      REQUIRE(output.at(0) == Approx(0.8739710314));
      REQUIRE(output.at(1) == Approx(0.7302340494));
    }

    SECTION("Multiple iterations on one data point") {
      vector<int> layer_sizes({2, 3, 2});
      vector<vector<double>> weights(
          {{1, 0.5, 0.5, 1, 1, 0.25}, {1, 0.5, 0.25, 0.5, 1, 0.5}});

      NeuralNetworkModel model(layer_sizes, weights);
      model.SetAlpha(1);

      model.Train(vector<vector<double>>({{1, 2}}),
                  vector<vector<double>>({{1, 0}}), 3);

      vector<double> output = model.Output(vector<double>({1, 2}));

      REQUIRE(output.at(0) == Approx(0.8788204623));
      REQUIRE(output.at(1) == Approx(0.5814688036));
    }

    SECTION("One iteration on multiple data points") {
      vector<int> layer_sizes({2, 3, 2});
      vector<vector<double>> weights(
          {{1, 0.5, 0.5, 1, 1, 0.25}, {1, 0.5, 0.25, 0.5, 1, 0.5}});

      NeuralNetworkModel model(layer_sizes, weights);
      model.SetAlpha(1);

      model.Train(vector<vector<double>>({{1, 2}, {0.5, 2.5}}),
                  vector<vector<double>>({{1, 0}, {0.5, 0.2}}), 1);

      vector<double> output = model.Output(vector<double>({1, 2}));

      REQUIRE(output.at(0) == Approx(0.8606789496));
      REQUIRE(output.at(1) == Approx(0.6794101896));
    }

    SECTION("Multiple iterations on multiple data points") {
      vector<int> layer_sizes({2, 3, 2});
      vector<vector<double>> weights(
          {{1, 0.5, 0.5, 1, 1, 0.25}, {1, 0.5, 0.25, 0.5, 1, 0.5}});

      NeuralNetworkModel model(layer_sizes, weights);
      model.SetAlpha(1);

      model.Train(vector<vector<double>>({{1, 2}, {0.5, 2.5}}),
                  vector<vector<double>>({{1, 0}, {0.5, 0.2}}), 10);

      vector<double> output = model.Output(vector<double>({1, 2}));

      REQUIRE(output.at(0) == Approx(0.7927611541));
      REQUIRE(output.at(1) == Approx(0.1984379229));
    }

    SECTION("Multiple iterations on multiple data points with more layers") {
      srand(0);
      vector<int> layer_sizes({2, 3, 4, 3});

      NeuralNetworkModel model(layer_sizes);
      model.SetAlpha(1);

      model.Train(vector<vector<double>>({{1, .5}, {0.5, .25}}),
                  vector<vector<double>>({{1, 0, .5}, {0.5, 0.2, .7}}), 20);

      vector<double> output = model.Output(vector<double>({1, .5}));

      REQUIRE(output.at(0) == Approx(0.7517122526));
      REQUIRE(output.at(1) == Approx(0.1490035724));
      REQUIRE(output.at(2) == Approx(0.6305182434));
    }
  }
}