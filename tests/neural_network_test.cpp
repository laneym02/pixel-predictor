//
// Created by Laney Moy on 11/17/2020.
//

#include <core/neural_network.h>

#include <catch2/catch.hpp>
#include <iostream>

using pixel_predictor::neural_network::NeuralNetwork;

TEST_CASE("Get neural network output") {

  SECTION("One layer") {
    vector<int> layer_sizes({2, 3});
    vector<vector<double>> weights({{1, 0.5, 0.5, 1, 1, 0.25}});

    NeuralNetwork network(layer_sizes, weights);

    vector<double> output = network.Output(vector<double>({1, 2}));

    REQUIRE(output.at(0) == Approx(0.9525741268));
    REQUIRE(output.at(1) == Approx(0.92414182));
    REQUIRE(output.at(2) == Approx(0.7310585786));
  }

  SECTION("Three layers of different sizes") {
    vector<int> layer_sizes({2, 3, 2});
    vector<vector<double>> weights(
        {{1, 0.5, 0.5, 1, 1, 0.25}, {1, 0.5, 0.25, 0.5, 1, 0.5}});

    NeuralNetwork network(layer_sizes, weights);

    vector<double> output = network.Output(vector<double>({1, 2}));

    REQUIRE(output.at(0) == Approx(0.8715426836));
    REQUIRE(output.at(1) == Approx(0.7864884761));
  }

  SECTION("More layers") {
    srand(0);
    vector<int> layer_sizes({2, 3, 4, 3});

    NeuralNetwork network(layer_sizes);

    vector<double> output = network.Output(vector<double>({1, 2}));

    REQUIRE(output.at(0) == Approx(0.2483233651));
    REQUIRE(output.at(1) == Approx(0.2225897386));
    REQUIRE(output.at(2) == Approx(0.9658595848));
  }
}

TEST_CASE("Invalid arguments for training and output") {
  SECTION("Empty vector") {
    NeuralNetwork network;
    REQUIRE_THROWS_AS(network.Output(vector<double>()), std::invalid_argument);
  }

  SECTION("Incorrect size") {
    NeuralNetwork network;
    REQUIRE_THROWS_AS(network.Output(vector<double>({1})),
                      std::invalid_argument);
  }

  SECTION("Different number of inputs and outputs") {
    NeuralNetwork network;
    REQUIRE_THROWS_AS(
        network.Train(vector<vector<double>>({{1, .5}, {0.5, .25}}),
                      vector<vector<double>>({{1, 0, .5}})),
        std::invalid_argument);
  }

  SECTION("Incorrect input size") {
    NeuralNetwork network;
    REQUIRE_THROWS_AS(
        network.Train(vector<vector<double>>({{1, .5}, {0.5}}),
                      vector<vector<double>>({{1, 0, .5}, {0, .2, .5}})),
        std::invalid_argument);
  }

  SECTION("Incorrect output size") {
    NeuralNetwork network;
    REQUIRE_THROWS_AS(
        network.Train(vector<vector<double>>({{1, .5}, {0.5, 0.2}}),
                      vector<vector<double>>({{1, 0, .5}, {0, .2}})),
        std::invalid_argument);
  }
}

TEST_CASE("Train neural network") {
  SECTION("Train iteratively") {
    SECTION("Once on one data point") {
      vector<int> layer_sizes({2, 3, 2});
      vector<vector<double>> weights(
          {{1, 0.5, 0.5, 1, 1, 0.25}, {1, 0.5, 0.25, 0.5, 1, 0.5}});

      NeuralNetwork network(layer_sizes, weights);
      network.SetAlpha(1);

      network.Train(vector<vector<double>>({{1, 2}}),
                    vector<vector<double>>({{1, 0}}), 1);

      vector<double> output = network.Output(vector<double>({1, 2}));

      REQUIRE(output.at(0) == Approx(0.8739710314));
      REQUIRE(output.at(1) == Approx(0.7302340494));
    }

    SECTION("Multiple iterations on one data point") {
      vector<int> layer_sizes({2, 3, 2});
      vector<vector<double>> weights(
          {{1, 0.5, 0.5, 1, 1, 0.25}, {1, 0.5, 0.25, 0.5, 1, 0.5}});

      NeuralNetwork network(layer_sizes, weights);
      network.SetAlpha(1);

      network.Train(vector<vector<double>>({{1, 2}}),
                    vector<vector<double>>({{1, 0}}), 3);

      vector<double> output = network.Output(vector<double>({1, 2}));

      REQUIRE(output.at(0) == Approx(0.8788204623));
      REQUIRE(output.at(1) == Approx(0.5814688036));
    }

    SECTION("One iteration on multiple data points") {
      vector<int> layer_sizes({2, 3, 2});
      vector<vector<double>> weights(
          {{1, 0.5, 0.5, 1, 1, 0.25}, {1, 0.5, 0.25, 0.5, 1, 0.5}});

      NeuralNetwork network(layer_sizes, weights);
      network.SetAlpha(1);

      network.Train(vector<vector<double>>({{1, 2}, {0.5, 2.5}}),
                    vector<vector<double>>({{1, 0}, {0.5, 0.2}}), 1);

      vector<double> output = network.Output(vector<double>({1, 2}));

      REQUIRE(output.at(0) == Approx(0.8606789496));
      REQUIRE(output.at(1) == Approx(0.6794101896));
    }

    SECTION("Multiple iterations on multiple data points") {
      vector<int> layer_sizes({2, 3, 2});
      vector<vector<double>> weights(
          {{1, 0.5, 0.5, 1, 1, 0.25}, {1, 0.5, 0.25, 0.5, 1, 0.5}});

      NeuralNetwork network(layer_sizes, weights);
      network.SetAlpha(1);

      network.Train(vector<vector<double>>({{1, 2}, {0.5, 2.5}}),
                    vector<vector<double>>({{1, 0}, {0.5, 0.2}}), 10);

      vector<double> output = network.Output(vector<double>({1, 2}));

      REQUIRE(output.at(0) == Approx(0.7927611541));
      REQUIRE(output.at(1) == Approx(0.1984379229));
    }

    SECTION("Multiple iterations on multiple data points with more layers") {
      srand(0);
      vector<int> layer_sizes({2, 3, 4, 3});

      NeuralNetwork network(layer_sizes);
      network.SetAlpha(1);

      network.Train(vector<vector<double>>({{1, .5}, {0.5, .25}}),
                    vector<vector<double>>({{1, 0, .5}, {0.5, 0.2, .7}}), 20);

      vector<double> output = network.Output(vector<double>({1, .5}));

      REQUIRE(output.at(0) == Approx(0.7067299923));
      REQUIRE(output.at(1) == Approx(0.1649370727));
      REQUIRE(output.at(2) == Approx(0.9063482244));
    }
  }
}