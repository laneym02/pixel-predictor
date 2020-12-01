//
// Created by Laney Moy on 11/30/2020.
//

#include <core/predictor_engine.h>

namespace pixel_predictor {

PredictorEngine::PredictorEngine(vector<double> max_inputs,
                                 vector<double> max_outputs)
    : max_inputs_(max_inputs), max_outputs_(max_outputs) {}

PredictorEngine::PredictorEngine(vector<double> max_inputs,
                                 vector<double> max_outputs, Method method)
    : max_inputs_(max_inputs), max_outputs_(max_outputs), method_(method) {}

void PredictorEngine::SetMethod(Method method) { method_ = method; }

void PredictorEngine::AddData(vector<vector<double>> inputs_,
                              vector<vector<double>> outputs_) {}

vector<double> PredictorEngine::Predict(vector<double> input) {
  switch (method_) {
  case pixel_predictor::NeuralNetwork:
    return AdjustOutput(network_.Output(AdjustInput(input)));
  }
}

vector<double> PredictorEngine::AdjustInput(vector<double> input) {
  for (size_t index = 0; index < input.size(); ++index) {
    input.at(index) /= max_inputs_.at(index);
  }
  return input;
}

vector<double> PredictorEngine::AdjustOutput(vector<double> output) {
  for (size_t index = 0; index < output.size(); ++index) {
    output.at(index) *= max_outputs_.at(index);
  }
  return output;
}
} // namespace pixel_predictor
