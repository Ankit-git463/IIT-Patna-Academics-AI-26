import numpy as np
import matplotlib.pyplot as plt
from sklearn.datasets import make_classification

# Sigmoid Activation Function
def sigmoid(x):
    return 1 / (1 + np.exp(-x))

# Derivative of Sigmoid
def sigmoid_derivative(x):
    return x * (1 - x)

# Generate the dataset (XOR-like)
X = np.array([[0, 0], [0, 1], [1, 0], [1, 1]])  # Inputs (x1, x2)
y = np.array([[0], [1], [1], [1]])  # Target outputs (y)

# Initialize weights and bias
np.random.seed(1)  # For reproducibility
w1 = np.random.rand(2, 3)  # Weights for the hidden layer (2 inputs, 3 neurons in the hidden layer)
w2 = np.random.rand(3, 1)  # Weights for the output layer (3 neurons, 1 output)
b1 = np.zeros((1, 3))  # Bias for hidden layer
b2 = np.zeros((1, 1))  # Bias for output layer

# Learning rate
learning_rate = 1

# Training process
epochs = 1
losses = []

for epoch in range(epochs):
    # Forward Propagation
    hidden_input = np.dot(X, w1) + b1  # Input to the hidden layer
    hidden_output = sigmoid(hidden_input)  # Output from hidden layer
    final_input = np.dot(hidden_output, w2) + b2  # Input to the output layer
    final_output = sigmoid(final_input)  # Final output of the network

    # Calculate the error
    error = y - final_output
    losses.append(np.mean(np.abs(error)))  # Store the loss (mean absolute error)

    # Backpropagation
    # Output layer gradients
    d_output = error * sigmoid_derivative(final_output)
    d_hidden = d_output.dot(w2.T) * sigmoid_derivative(hidden_output)

    # Update weights and biases
    w2 += hidden_output.T.dot(d_output) * learning_rate
    b2 += np.sum(d_output, axis=0, keepdims=True) * learning_rate
    w1 += X.T.dot(d_hidden) * learning_rate
    b1 += np.sum(d_hidden, axis=0, keepdims=True) * learning_rate

    if epoch % 1 == 0:
        print(f"Epoch {epoch}, Loss: {losses[-1]}")

# After training, print the final weights and bias
print("Final weights (w1):", w1)
print("Final weights (w2):", w2)
print("Final biases (b1):", b1)
print("Final biases (b2):", b2)

# Plotting the loss over epochs
plt.plot(losses)
plt.title('Loss over Epochs')
plt.xlabel('Epochs')
plt.ylabel('Loss')
plt.show()

# Test the network (after training)
hidden_input = np.dot(X, w1) + b1
hidden_output = sigmoid(hidden_input)
final_input = np.dot(hidden_output, w2) + b2
final_output = sigmoid(final_input)

print("Predictions after training:")
print(final_output)
