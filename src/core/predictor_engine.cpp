//
// Created by Laney Moy on 11/30/2020.
//

#include <core/predictor_engine.h>
#include <stdexcept>
#include <utility>

namespace pixel_predictor {

PredictorEngine::PredictorEngine(vector<double> max_inputs,
                                 vector<double> max_outputs)
    : max_inputs_(std::move(max_inputs)), max_outputs_(std::move(max_outputs)) {
  Instantiate();
}

PredictorEngine::PredictorEngine(vector<double> max_inputs,
                                 vector<double> max_outputs, Method method)
    : max_inputs_(std::move(max_inputs)), max_outputs_(std::move(max_outputs)),
      method_(method) {
  Instantiate();
}

void PredictorEngine::SetMethod(Method method) {
  Reset();
  method_ = method;
  Instantiate();
}

void PredictorEngine::ProcessData(const vector<vector<double>> &inputs,
                                  const vector<vector<double>> &outputs,
                                  int iterations) {
  if (inputs.size() != outputs.size()) {
    throw std::invalid_argument("Inputs and outputs differ in size");
  }
  switch (method_) {
  case NeuralNetwork:
    vector<vector<double>> adjusted_inputs;
    adjusted_inputs.reserve(inputs.size());
    for (const vector<double> &input : inputs) {
      adjusted_inputs.push_back(AdjustInput(input));
    }

    vector<vector<double>> adjusted_outputs;
    adjusted_outputs.reserve(outputs.size());
    for (const vector<double> &output : outputs) {
      adjusted_outputs.push_back(AdjustOutput(output));
    }

    network_.Train(adjusted_inputs, adjusted_outputs, iterations);
  }
}

vector<double> PredictorEngine::Predict(const vector<double> &input) {
  switch (method_) {
  case NeuralNetwork:
    return AdjustOutput(network_.Output(AdjustInput(input)));
  }
  return vector<double>();
}

vector<double> PredictorEngine::AdjustInput(const vector<double> &input) {
  vector<double> adjusted;
  adjusted.reserve(input.size());
  for (size_t index = 0; index < input.size(); ++index) {
    adjusted.push_back(input.at(index) / max_inputs_.at(index));
  }
  return adjusted;
}

vector<double> PredictorEngine::AdjustOutput(const vector<double> &output) {
  vector<double> adjusted;
  adjusted.reserve(output.size());
  for (size_t index = 0; index < output.size(); ++index) {
    adjusted.push_back(output.at(index) * max_outputs_.at(index));
  }
  return adjusted;
}

void PredictorEngine::Reset() {
  switch (method_) {
  case NeuralNetwork:
    network_.Reset();
  }
}

void PredictorEngine::Instantiate() {
  switch (method_) {
  case NeuralNetwork:
    network_ = NeuralNetworkModel(
        {(int)max_inputs_.size(), 3, 4, (int)max_outputs_.size()});
  }
}
} // namespace pixel_predictor
