//
// Created by Laney Moy on 11/17/2020.
//

#ifndef IDEAL_GAS_INCLUDE_CORE_NN_MODEL_H_

#include <cmath>
#include <vector>

using std::vector;

namespace pixel_predictor {

namespace neural_network {

/**
 * Represents a neural network
 */
class NeuralNetwork {
public:
  /**
   * Create a neural network with specified layer sizes
   * @param layer_sizes the vector of layer sizes
   */
  NeuralNetwork(const vector<int> &layer_sizes = {2, 3, 4, 3});

  /**
   * Create a neural network with specified layer sizes and weights
   * @param layer_sizes the vector of layer sizes
   * @param weights the vector of vectors of weights
   */
  NeuralNetwork(vector<int> layer_sizes, vector<vector<double>> weights);

  /**
   * Iteratively train a neural network
   * @param training_input training data input
   * @param training_output training data output
   * @param iterations number of iterations
   */
  void Train(const vector<vector<double>> &training_input,
             const vector<vector<double>> &training_output,
             int iterations = 20);

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

  /**
   * Set alpha
   * @param alpha
   */
  void SetAlpha(double alpha);

private:
  vector<int> layer_sizes_;
  vector<vector<double>> weights_;
  double alpha_ = 0.5;

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
   * Backpropagate on a data point
   * @param target the target final values
   * @param node_values the values at each node
   */
  void Backpropagate(vector<double> target, vector<vector<double>> node_values);

  /**
   * Calculate the total error of a point (for future use in threshold-based
   * training)
   * @param target the target final values
   * @param actual the actual final values
   * @return the total error
   */
  double CalculateError(vector<double> target, vector<double> actual);

  /**
   * The activation function
   * @param input the input value
   * @return the output of the activation function
   */
  double ActivationFunction(const double &input) const;

  /**
   * The derivative of the activation function
   * @param value the value of the activation function
   * @return the derivative at the point
   */
  double ActivationDerivative(const double &value) const;
};

} // namespace neural_network

} // namespace pixel_predictor

#define IDEAL_GAS_INCLUDE_CORE_NN_MODEL_H_

#endif // IDEAL_GAS_INCLUDE_CORE_NN_MODEL_H_
