//
// Created by Laney Moy on 11/17/2020.
//

#ifndef IDEAL_GAS_INCLUDE_CORE_NN_MODEL_H_

#include <vector>

using std::vector;

namespace pixel_predictor {

namespace neural_network {

class NeuralNetworkModel {
public:
  NeuralNetworkModel();
  explicit NeuralNetworkModel(const vector<int> &layer_sizes);
  NeuralNetworkModel(const vector<int> &layer_sizes,
                     const vector<vector<double>> &weights);

  vector<double> Output(const vector<double> &input) const;

  void Reset();

private:
  vector<int> default_layer_sizes_{2, 3, 4, 3};
  vector<int> layer_sizes_ = default_layer_sizes_;
  vector<vector<double>> weights_;

  void RandomizeWeights();

  vector<vector<double>> FeedForward(const vector<double> &input) const;

  double ActivationFunction(const double &input) const;
};

} // namespace neural_network

} // namespace pixel_predictor

#define IDEAL_GAS_INCLUDE_CORE_NN_MODEL_H_

#endif // IDEAL_GAS_INCLUDE_CORE_NN_MODEL_H_
