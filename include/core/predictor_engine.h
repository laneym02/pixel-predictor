//
// Created by Laney Moy on 11/30/2020.
//

#ifndef FINAL_PROJECT_INCLUDE_CORE_PREDICTOR_ENGINE_H_
#define FINAL_PROJECT_INCLUDE_CORE_PREDICTOR_ENGINE_H_

#include <vector>
#include <core/nn-model.h>

using std::vector;

namespace pixel_predictor {
enum Method { NeuralNetwork };

class PredictorEngine {
public:
  PredictorEngine(vector<double> max_inputs, vector<double> max_outputs);
  PredictorEngine(vector<double> max_inputs, vector<double> max_outputs,
                  Method method);
  void SetMethod(Method method);
  void AddData(vector<vector<double>> inputs_, vector<vector<double>> outputs_);
  vector<double> Predict(vector<double>);

private:
  Method method_ = NeuralNetwork;
  vector<double> max_inputs_;
  vector<double> max_outputs_;

  neural_network::NeuralNetworkModel network_;

  vector<double> AdjustInput(vector<double> input);
  vector<double> AdjustOutput(vector<double> output);
};
} // namespace pixel_predictor

#endif // FINAL_PROJECT_INCLUDE_CORE_PREDICTOR_ENGINE_H_
