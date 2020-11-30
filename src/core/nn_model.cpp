//
// Created by Laney Moy on 11/17/2020.
//

#include <core/nn-model.h>

#include <numeric>
#include <stdexcept>
#include <utility>

namespace pixel_predictor {

namespace neural_network {

NeuralNetworkModel::NeuralNetworkModel() : layer_sizes_(default_layer_sizes_) {
  RandomizeWeights();
}

NeuralNetworkModel::NeuralNetworkModel(const vector<int> &layer_sizes)
    : layer_sizes_(layer_sizes) {
  RandomizeWeights();
}

NeuralNetworkModel::NeuralNetworkModel(vector<int> layer_sizes,
                                       vector<vector<double>> weights)
    : layer_sizes_(std::move(layer_sizes)), weights_(std::move(weights)) {
  // check that all weight sizes are correct
  if (layer_sizes_.size() != weights_.size() + 1) {
    Reset();
    throw std::invalid_argument("different size layer sizes and weights");
  }

  int layer = 0;
  for (const vector<double> &layer_weights : weights_) {
    if (layer_weights.size() !=
        layer_sizes_.at(layer) * layer_sizes_.at(layer + 1)) {
      Reset();
      throw std::invalid_argument("different size layer sizes and weights");
    }
    ++layer;
  }
}

void NeuralNetworkModel::Reset() {
  layer_sizes_ = default_layer_sizes_;
  weights_.clear();
}

void NeuralNetworkModel::RandomizeWeights() {
  // after each layer except for the last, general a layer of weights
  for (size_t prev_layer = 0; prev_layer < layer_sizes_.size() - 1;
       ++prev_layer) {
    vector<double> layer_weights;

    // generate random weights
    for (int x = 0;
         x < layer_sizes_.at(prev_layer) * layer_sizes_.at(prev_layer + 1);
         ++x) {
      layer_weights.push_back(rand() / RAND_MAX);
    }

    weights_.push_back(layer_weights);
  }
}

vector<double> NeuralNetworkModel::Output(const vector<double> &input) const {
  return FeedForward(input).back();
}

vector<vector<double>>
NeuralNetworkModel::FeedForward(const vector<double> &input) const {
  if (input.size() != layer_sizes_.at(0)) {
    throw std::invalid_argument("wrong size");
  }

  vector<vector<double>> all_node_values = {input};

  // for each weight layer, calculate the subsequent node values
  for (size_t weight_level = 0; weight_level < weights_.size();
       ++weight_level) {
    const vector<double> &weights = weights_.at(weight_level);
    vector<double> next_layer_values;

    // generate next weights
    vector<vector<double>> next_weights;
    for (int i = 0; i < layer_sizes_.at(weight_level + 1); ++i) {
      vector<double> node_weights;
      node_weights.reserve(layer_sizes_.at(weight_level));
      for (int j = 0; j < layer_sizes_.at(weight_level); ++j) {
        node_weights.push_back(
            weights.at(j * layer_sizes_.at(weight_level + 1) + i));
      }
      next_weights.push_back(node_weights);
    }

    // calculate next nodes
    vector<double> next_nodes;
    next_nodes.reserve(layer_sizes_.at(weight_level));
    for (int i = 0; i < layer_sizes_.at(weight_level + 1); ++i) {
      next_nodes.push_back(ActivationFunction(std::inner_product(
          all_node_values.back().begin(), all_node_values.back().end(),
          next_weights.at(i).begin(), 0.0)));
    }

    all_node_values.push_back(next_nodes);
  }

  return all_node_values;
}

double NeuralNetworkModel::ActivationFunction(const double &input) const {
  return 1 / (1 + exp(-input));
}

void NeuralNetworkModel::Train(vector<vector<double>> training_data,
                               vector<vector<double>> test_data) {}

void NeuralNetworkModel::Backpropagate(vector<double> target,
                                       vector<vector<double>> node_values) {
  vector<vector<double>> error_values;
  vector<vector<double>> delta_weights;

  vector<double> final_errors;
  int index = 0;
  for (const double &actual : node_values.back()) {
    final_errors.push_back((target.at(index) - actual) *
                           ActivationDerivative(actual));
    ++index;
  }
  error_values.push_back(final_errors);

  for (size_t layer = node_values.size() - 2; layer > 0; --layer) {
    vector<double> layer_errors;
    vector<double> layer_delta_weights;

    vector<vector<double>> next_weights;
    for (size_t i = 0; i < layer_sizes_.at(layer); ++i) {
      vector<double> node_weights;
      for (size_t j = 0; j < layer_sizes_.at(layer + 1); ++j) {
        node_weights.push_back(
            weights_.at(layer).at(i * layer_sizes_.at(layer) + j));
      }
      next_weights.push_back(node_weights);
    }

    for (size_t weight = 0; weight < weights_.at(layer).size(); ++weight) {
      int previous_node = floor(weight / layer_sizes_.at(layer));
      int next_node = weight % layer_sizes_.at(layer + 1);
      layer_delta_weights.push_back(alpha_ * error_values.back().at(next_node) *
                                    node_values.at(layer).at(previous_node));
    }

    for (size_t node = 0; node < layer_sizes_.at(layer); ++node) {
      layer_errors.push_back(
          std::inner_product(error_values.back().begin(),
                             error_values.back().end(),
                             next_weights.at(node).begin(), 0.0) *
          ActivationDerivative(node_values.at(layer).at(node)));
    }

    error_values.push_back(layer_errors);
    delta_weights.push_back(layer_delta_weights);
  }

  // update weights
  for (size_t layer = 0; layer < weights_.size(); ++layer) {
    for (size_t weight = 0; weight < weights_.at(layer).size(); ++weight) {
      weights_.at(layer).at(weight) +=
          delta_weights.at(delta_weights.size() - 1 - layer).at(weight);
    }
  }
}

double NeuralNetworkModel::ActivationDerivative(const double &value) const {
  return value * (1 - value);
}

} // namespace neural_network

} // namespace pixel_predictor