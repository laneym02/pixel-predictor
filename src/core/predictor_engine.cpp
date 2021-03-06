//
// Created by Laney Moy on 11/30/2020.
//

#include <core/predictor_engine.h>
#include <stdexcept>
#include <utility>

namespace pixel_predictor {

PredictorEngine::PredictorEngine(const vector<double> &max_inputs,
                                 const vector<double> &max_outputs,
                                 Method method)
    : method_(method) {
  if (max_inputs.empty() || max_outputs.empty()) {
    throw std::invalid_argument("Must have non-empty input and output");
  }
  max_inputs_ = max_inputs;
  max_outputs_ = max_outputs;
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
  case BasicNeuralNetwork:
    vector<vector<double>> adjusted_inputs;
    adjusted_inputs.reserve(inputs.size());
    for (const vector<double> &input : inputs) {
      if (input.size() != max_inputs_.size()) {
        throw std::invalid_argument("Incorrect input size");
      }
      adjusted_inputs.push_back(AdjustInput(input));
    }

    vector<vector<double>> adjusted_outputs;
    adjusted_outputs.reserve(outputs.size());
    for (const vector<double> &output : outputs) {
      if (output.size() != max_outputs_.size()) {
        throw std::invalid_argument("Incorrect output size");
      }
      adjusted_outputs.push_back(AdjustTrainingOutput(output));
    }

    network_.Train(adjusted_inputs, adjusted_outputs, iterations);
  }
}

vector<double> PredictorEngine::Predict(const vector<double> &input) {
  switch (method_) {
  case BasicNeuralNetwork:
    if (input.size() != max_inputs_.size()) {
      throw std::invalid_argument("Incorrect input size");
    }
    return AdjustOutput(network_.Output(AdjustInput(input)));
  }
  return vector<double>();
}

vector<double> PredictorEngine::AdjustInput(const vector<double> &input) {
  vector<double> adjusted;
  adjusted.reserve(input.size());
  for (size_t index = 0; index < input.size(); ++index) {
    adjusted.push_back(input.at(index) / max_inputs_.at(index) * 2 - 1);
  }
  return adjusted;
}

vector<double>
PredictorEngine::AdjustTrainingOutput(const vector<double> &output) {
  vector<double> adjusted;
  adjusted.reserve(output.size());
  for (size_t index = 0; index < output.size(); ++index) {
    adjusted.push_back(output.at(index) / max_outputs_.at(index));
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
  case BasicNeuralNetwork:
    network_.Reset();
  }
}

void PredictorEngine::Instantiate() {
  switch (method_) {
  case BasicNeuralNetwork:
    network_ = NeuralNetwork(
        {(int)max_inputs_.size(), 4, 4, 4, (int)max_outputs_.size()});
  }
}
} // namespace pixel_predictor
