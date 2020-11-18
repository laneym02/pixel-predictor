//
// Created by Laney Moy on 11/17/2020.
//

#include <core/nn-model.h>

#include <numeric>
#include <stdexcept>

namespace pixel_predictor {

namespace neural_network {

NeuralNetworkModel::NeuralNetworkModel() { RandomizeWeights(); }

NeuralNetworkModel::NeuralNetworkModel(const vector<int> &layer_sizes)
    : layer_sizes_(layer_sizes) {
  RandomizeWeights();
}

NeuralNetworkModel::NeuralNetworkModel(const vector<int> &layer_sizes,
                                       const vector<vector<double>> &weights)
    : layer_sizes_(layer_sizes), weights_(weights) {
  if (layer_sizes_.size() != weights_.size()) {
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
  for (size_t prev_layer = 0; prev_layer < layer_sizes_.size() - 1;
       ++prev_layer) {
    vector<double> layer_weights;

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

  for (size_t weight_level = 0; weight_level < weights_.size();
       ++weight_level) {
    const vector<double> &weights = weights_.at(weight_level);
    vector<double> next_layer_values;

    vector<vector<double>> next_weights;
    for (int i = 0; i < layer_sizes_.at(weight_level); ++i) {
      vector<double> node_weights;
      node_weights.reserve(layer_sizes_.at(weight_level + 1));
      for (int j = 0; j < layer_sizes_.at(weight_level + 1); ++j) {
        node_weights.push_back(
            weights.at(i * layer_sizes_.at(weight_level) + j));
      }
      next_weights.push_back(node_weights);
    }

    vector<double> next_cells;
    next_cells.reserve(layer_sizes_.at(weight_level));
    for (int i = 0; i < layer_sizes_.at(weight_level); ++i) {
      next_cells.push_back(ActivationFunction(std::inner_product(
          all_node_values.back().begin(), all_node_values.back().end(),
          next_weights.at(i).begin(), 0.0)));
    }

    all_node_values.push_back(next_cells);
  }

  return all_node_values;
}

double NeuralNetworkModel::ActivationFunction(const double &input) const {
  return 1 / (1 + exp(-input));
}

} // namespace neural_network

} // namespace pixel_predictor