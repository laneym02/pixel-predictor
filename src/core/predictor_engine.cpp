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
                                  const vector<vector<double>> &outputs) {
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

    network_.Train(adjusted_inputs, adjusted_outputs);
  }
}

vector<double> PredictorEngine::Predict(vector<double> input) {
  switch (method_) {
  case NeuralNetwork:
    return AdjustOutput(network_.Output(AdjustInput(std::move(input))));
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

void PredictorEngine::Reset() {
  switch (method_) {
  case NeuralNetwork:
    network_.Reset();
  }
}

void PredictorEngine::CreateNetwork() {}
void PredictorEngine::Instantiate() {
  switch (method_) {
  case NeuralNetwork:
    CreateNetwork();
  }
}
} // namespace pixel_predictor
