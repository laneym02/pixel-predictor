//
// Created by Laney Moy on 11/30/2020.
//

#ifndef FINAL_PROJECT_INCLUDE_CORE_PREDICTOR_ENGINE_H_
#define FINAL_PROJECT_INCLUDE_CORE_PREDICTOR_ENGINE_H_

#include <vector>
#include <core/nn-model.h>

using std::vector;
using pixel_predictor::neural_network::NeuralNetworkModel;

namespace pixel_predictor {

/**
 * Prediction methods
 */
enum Method { NeuralNetwork };

/**
 * The engine for the pixel predictor
 */
class PredictorEngine {
public:
  /**
   * Create a PredictorEngine
   * @param max_inputs the maximum input values
   * @param max_outputs the maximum output values
   */
  PredictorEngine(vector<double> max_inputs, vector<double> max_outputs);

  /**
   * Create a PredictorEngine
   * @param max_inputs the maximum input values
   * @param max_outputs the maximum output values
   * @param method the prediction method
   */
  PredictorEngine(vector<double> max_inputs, vector<double> max_outputs,
                  Method method);

  /**
   * Set the prediction method
   * @param method the prediction method
   */
  void SetMethod(Method method);

  /**
   * Add data to the engine
   * @param inputs the inputs of the data
   * @param outputs the outputs of the data
   */
  void ProcessData(const vector<vector<double>>& inputs, const vector<vector<double>>& outputs);

  /**
   * Predict the output of a point
   * @param input the input vector
   * @return the output vector
   */
  vector<double> Predict(vector<double> input);

  /**
   * Reset the engine
   */
  void Reset();

private:
  Method method_ = NeuralNetwork;
  vector<double> max_inputs_;
  vector<double> max_outputs_;

  NeuralNetworkModel network_;

  /**
   * Adjust the inputs based on the max input values
   * @param input the input vector
   * @return the adjusted input
   */
  vector<double> AdjustInput(vector<double> input);

  /**
   * Adjust the outputs based on the max output values
   * @param output the output vector
   * @return the adjusted output
   */
  vector<double> AdjustOutput(vector<double> output);

  /**
   * Create neural network
   */
   void CreateNetwork();

   /**
    * Instantiate PredictorEngine
    */
    void Instantiate();
};
} // namespace pixel_predictor

#endif // FINAL_PROJECT_INCLUDE_CORE_PREDICTOR_ENGINE_H_
