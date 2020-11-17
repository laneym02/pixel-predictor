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
  NeuralNetworkModel(const vector<int>& layer_sizes);
  NeuralNetworkModel(const vector<vector<float>>& weights);

  vector<float> FeedForward(const vector<float>& input) const;

private:
  vector<int> default_layer_sizes_ = {2, 3, 4, 3};
  vector<vector<float>> weights_;

  void RandomizeWeights(const vector<int>& layer_sizes);

};

}

}

#define IDEAL_GAS_INCLUDE_CORE_NN_MODEL_H_

#endif // IDEAL_GAS_INCLUDE_CORE_NN_MODEL_H_
