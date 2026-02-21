import numpy as np

# define the sigmoid function
def sigmoid(x, derivative=False):
    if derivative:
        return sigmoid(x) * (1 - sigmoid(x))
    else:
        return 1 / (1 + np.exp(-x))

# choose a random seed for reproducible results
np.random.seed(1)

# learning rate
alpha = 0.1

# number of nodes in the hidden layer
num_hidden = 3

# inputs
X = np.array([
    [0, 0, 1],
    [0, 1, 1],
    [1, 0, 0],
    [1, 1, 0],
    [1, 0, 1],
    [1, 1, 1],
])

# outputs
y = np.array([[0, 1, 0, 1, 1, 0]]).T

# initialize weights randomly with mean 0 and range [-1, 1]
# +1 for the bias weight
w1 = 2 * np.random.random((X.shape[1] + 1, num_hidden)) - 1  # Input to hidden layer weights
w2 = 2 * np.random.random((num_hidden + 1, y.shape[1])) - 1  # Hidden to output layer weights

# number of iterations of gradient descent
num_iterations = 10000

# for each iteration of gradient descent
for i in range(num_iterations):
    # forward phase
    input_layer_outputs = np.hstack((np.ones((X.shape[0], 1)), X))  # add bias
    hidden_layer_outputs = np.hstack((np.ones((X.shape[0], 1)), sigmoid(np.dot(input_layer_outputs, w1))))  # add bias to hidden layer
    w3 = sigmoid(np.dot(hidden_layer_outputs, w2))  # final outputs

    # backward phase
    output_error = w3 - y
    hidden_error = hidden_layer_outputs[:, 1:] * (1 - hidden_layer_outputs[:, 1:]) * np.dot(output_error, w2.T[:, 1:])

    # partial derivatives
    hidden_pd = input_layer_outputs[:, :, np.newaxis] * hidden_error[:, np.newaxis, :]
    output_pd = hidden_layer_outputs[:, :, np.newaxis] * output_error[:, np.newaxis, :]

    # average for total gradients
    total_hidden_gradient = np.average(hidden_pd, axis=0)
    total_output_gradient = np.average(output_pd, axis=0)

    # update weights
    w1 += -alpha * total_hidden_gradient
    w2 += -alpha * total_output_gradient

# print the final outputs of the neural network on the inputs X
print("Final Output After Training:\n", w3)

# print the structure and the weights of the neural network
print("\nNeural Network Structure:\n")
print("Input Layer (with Bias):", input_layer_outputs.shape)
print("Hidden Layer Weights (w1 - including Bias):\n", w1)
print("Hidden Layer Outputs (after Activation):", hidden_layer_outputs.shape)
print("Output Layer Weights (w2 - including Bias):\n", w2)
print("Final Output Layer Outputs (w3 - after Activation):", w3.shape)
