//
// Created by Laney Moy on 11/17/2020.
//

#include <core/neural_network.h>

#include <numeric>
#include <stdexcept>
#include <utility>

namespace pixel_predictor {

namespace neural_network {

NeuralNetwork::NeuralNetwork() : layer_sizes_(default_layer_sizes_) {
  RandomizeWeights();
}

NeuralNetwork::NeuralNetwork(const vector<int> &layer_sizes)
    : layer_sizes_(layer_sizes) {
  RandomizeWeights();
}

NeuralNetwork::NeuralNetwork(vector<int> layer_sizes,
                                       vector<vector<double>> weights)
    : layer_sizes_(std::move(layer_sizes)), weights_(std::move(weights)) {
  // check that all weight sizes are correct
  if (layer_sizes_.size() != weights_.size() + 1) {
    Reset();
    throw std::invalid_argument("different size layer sizes and weights");
  }

  int layer = 0;
  for (const vector<double> &layer_weights : weights_) {
    if (layer_weights.size() != layer_sizes_[layer] * layer_sizes_[layer + 1]) {
      Reset();
      throw std::invalid_argument("different size layer sizes and weights");
    }
    ++layer;
  }
}

void NeuralNetwork::Reset() {
  weights_.clear();
  RandomizeWeights();
}

void NeuralNetwork::RandomizeWeights() {
  // after each layer except for the last, general a layer of weights
  for (size_t prev_layer = 0; prev_layer < layer_sizes_.size() - 1;
       ++prev_layer) {
    vector<double> layer_weights;

    // generate random weights
    layer_weights.reserve(layer_sizes_[prev_layer] *
                          layer_sizes_[prev_layer + 1]);
    for (int x = 0; x < layer_sizes_[prev_layer] * layer_sizes_[prev_layer + 1];
         ++x) {
      layer_weights.push_back((double)rand() / RAND_MAX * 5 - 2.5);
    }

    weights_.push_back(layer_weights);
  }
}

vector<double> NeuralNetwork::Output(const vector<double> &input) const {
  return FeedForward(input).back();
}

vector<vector<double>>
NeuralNetwork::FeedForward(const vector<double> &input) const {
  if (input.size() != layer_sizes_[0]) {
    throw std::invalid_argument("wrong size");
  }

  vector<vector<double>> all_node_values = {input};

  // for each weight layer, calculate the subsequent node values
  for (size_t weight_level = 0; weight_level < weights_.size();
       ++weight_level) {
    const vector<double> &weights = weights_[weight_level];
    vector<double> next_layer_values;

    // generate next weights
    vector<vector<double>> next_weights;
    for (int i = 0; i < layer_sizes_[weight_level + 1]; ++i) {
      vector<double> node_weights;
      node_weights.reserve(layer_sizes_[weight_level]);
      for (int j = 0; j < layer_sizes_[weight_level]; ++j) {
        node_weights.push_back(weights[j * layer_sizes_[weight_level + 1] + i]);
      }
      next_weights.push_back(node_weights);
    }

    // calculate next nodes
    vector<double> next_nodes;
    next_nodes.reserve(layer_sizes_[weight_level]);
    for (int i = 0; i < layer_sizes_[weight_level + 1]; ++i) {
      next_nodes.push_back(ActivationFunction(std::inner_product(
          all_node_values.back().begin(), all_node_values.back().end(),
          next_weights[i].begin(), 0.0)));
    }

    all_node_values.push_back(next_nodes);
  }

  return all_node_values;
}

double NeuralNetwork::ActivationFunction(const double &input) const {
  return 1 / (1 + exp(-input));
}

void NeuralNetwork::Train(const vector<vector<double>> &training_input,
                               const vector<vector<double>> &training_output) {
  Train(training_input, training_output, 20);
}

void NeuralNetwork::Train(const vector<vector<double>> &training_input,
                               const vector<vector<double>> &training_output,
                               int iterations) {
  for (int iteration = 0; iteration < iterations; ++iteration) {
    for (size_t index = 0; index < training_input.size(); ++index) {
      vector<vector<double>> node_values = FeedForward(training_input[index]);
      Backpropagate(training_output[index], node_values);
    }
  }
}

double NeuralNetwork::CalculateError(vector<double> targets,
                                          vector<double> actuals) {
  double total = 0;
  for (size_t index = 0; index < targets.size(); ++index) {
    total += pow(targets[index] - actuals[index], 2);
  }
  return total;
}

void NeuralNetwork::Backpropagate(vector<double> target,
                                       vector<vector<double>> node_values) {
  vector<vector<double>> error_values;
  vector<vector<double>> delta_weights;

  vector<double> final_errors;
  int index = 0;
  for (const double &actual : node_values.back()) {
    final_errors.push_back((target[index] - actual) *
                           ActivationDerivative(actual));
    ++index;
  }
  error_values.push_back(final_errors);

  for (size_t layer = node_values.size() - 2;
       layer >= 0 && layer <= node_values.size(); --layer) {
    vector<double> layer_errors;
    vector<double> layer_delta_weights;

    vector<vector<double>> next_weights;
    for (int i = 0; i < layer_sizes_[layer]; ++i) {
      vector<double> node_weights;
      node_weights.reserve(layer_sizes_[layer + 1]);
      for (int j = 0; j < layer_sizes_[layer + 1]; ++j) {
        node_weights.push_back(
            weights_[layer][i * layer_sizes_[layer + 1] + j]);
      }
      next_weights.push_back(node_weights);
    }

    for (size_t weight = 0; weight < weights_[layer].size(); ++weight) {
      unsigned int previous_node = weight / layer_sizes_[layer + 1];
      unsigned int next_node = weight % layer_sizes_[layer + 1];
      layer_delta_weights.push_back(alpha_ * error_values.back()[next_node] *
                                    node_values[layer][previous_node]);
    }

    layer_errors.reserve(layer_sizes_[layer]);
    for (int node = 0; node < layer_sizes_[layer]; ++node) {
      layer_errors.push_back(std::inner_product(error_values.back().begin(),
                                                error_values.back().end(),
                                                next_weights[node].begin(),
                                                0.0) *
                             ActivationDerivative(node_values[layer][node]));
    }

    error_values.push_back(layer_errors);
    delta_weights.push_back(layer_delta_weights);
  }

  // update weights
  for (size_t layer = 0; layer < weights_.size(); ++layer) {
    for (size_t weight = 0; weight < weights_[layer].size(); ++weight) {
      weights_[layer][weight] +=
          delta_weights[delta_weights.size() - 1 - layer][weight];
    }
  }
}

double NeuralNetwork::ActivationDerivative(const double &value) const {
  return value * (1 - value);
}

void NeuralNetwork::SetAlpha(double alpha) { alpha_ = alpha; }

} // namespace neural_network

} // namespace pixel_predictor