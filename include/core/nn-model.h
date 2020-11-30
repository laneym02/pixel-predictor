//
// Created by Laney Moy on 11/17/2020.
//

#ifndef IDEAL_GAS_INCLUDE_CORE_NN_MODEL_H_

#include <vector>
#include <cmath>

using std::vector;

namespace pixel_predictor {

namespace neural_network {

/**
 * Represents a neural network
 */
class NeuralNetworkModel {
public:
  /**
   * Create a network
   */
  NeuralNetworkModel();

  /**
   * Create a neural network with specified layer sizes
   * @param layer_sizes the vector of layer sizes
   */
  explicit NeuralNetworkModel(const vector<int> &layer_sizes);

  /**
   * Create a neural network with specified layer sizes and weights
   * @param layer_sizes the vector of layer sizes
   * @param weights the vector of vectors of weights
   */
  NeuralNetworkModel(vector<int> layer_sizes, vector<vector<double>> weights);

  /**
   *
   */
  void Train(const vector<vector<double>> &training_input,
             const vector<vector<double>> &training_output,
             const vector<vector<double>> &test_input,
             const vector<vector<double>> &test_output);

  /**
   *
   */
  void Train(const vector<vector<double>> &training_input,
             const vector<vector<double>> &training_output,
             const vector<vector<double>> &test_input,
             const vector<vector<double>> &test_output, int iterations);

  /**
   * Pass an input vector through the output vector
   * @param input the input vector
   * @return the output vector
   */
  vector<double> Output(const vector<double> &input) const;

  /**
   * Reset the network
   */
  void Reset();

private:
  vector<int> default_layer_sizes_{2, 3, 4, 3};
  vector<int> layer_sizes_;
  vector<vector<double>> weights_;
  double alpha_ = 1;

  /**
   * Populate weights_ with random weights
   */
  void RandomizeWeights();

  /**
   * Calculate the node values when feeding an input vector through the network
   * @param input the input vector
   * @return the vector of all node values
   */
  vector<vector<double>> FeedForward(const vector<double> &input) const;

  /**
   *
   */
  void Backpropagate(vector<double> target, vector<vector<double>> node_values);

  /**
   *
   */
  double CalculateError(vector<double> target, vector<double> actual);

  /**
   * The activation function
   * @param input the input value
   * @return the output of the activation function
   */
  double ActivationFunction(const double &input) const;

  double ActivationDerivative(const double &value) const;
};

} // namespace neural_network

} // namespace pixel_predictor

#define IDEAL_GAS_INCLUDE_CORE_NN_MODEL_H_

#endif // IDEAL_GAS_INCLUDE_CORE_NN_MODEL_H_
