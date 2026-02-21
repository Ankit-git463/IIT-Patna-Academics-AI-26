import numpy as np 
import pandas as pd 
import matplotlib.pyplot as plt 
from sklearn.metrics import confusion_matrix, classification_report
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import LogisticRegression
from sklearn.decomposition import PCA

# Load the dataset
df = pd.read_csv("heart.csv")
x = df.drop("output", axis=1)
y = df["output"]

# Splitting the data set
x_train, x_test, y_train, y_test = train_test_split(x, y, test_size=0.2, random_state=47)

# Scaling the data
scaler = StandardScaler()
scaled_x_train = scaler.fit_transform(x_train)
scaled_x_test = scaler.transform(x_test)

# Train the classifier
classifier = LogisticRegression(random_state=0)
classifier.fit(scaled_x_train, y_train) 
y_pred = classifier.predict(scaled_x_test)

# Confusion matrix and classification report
cm = confusion_matrix(y_test, y_pred)
print("Confusion Matrix:")
print(cm)

print("\nClassification Report:")
print(classification_report(y_test, y_pred))

# Visualizing the classes using PCA
pca = PCA(n_components=2)
x_pca = pca.fit_transform(scaled_x_test)

plt.figure(figsize=(8, 6))
plt.scatter(x_pca[:,0], x_pca[:,1], c=y_pred, cmap='viridis', alpha=0.5)
plt.title('Scatter Plot of Classes (Predicted)')
plt.xlabel('Principal Component 1')
plt.ylabel('Principal Component 2')
plt.colorbar(label='Class')
plt.grid(True)
plt.show()
