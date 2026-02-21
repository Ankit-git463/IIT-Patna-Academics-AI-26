# Install necessary libraries
# !pip install easyocr opencv-python requests

import easyocr
import cv2
import matplotlib.pyplot as plt
import numpy as np
import requests
from io import BytesIO

# URL of the image
image_url = 'https://m.media-amazon.com/images/I/41-NCxNuBxL.jpg'

# Download the image from the URL
response = requests.get(image_url)
image = np.array(bytearray(response.content), dtype=np.uint8)
image = cv2.imdecode(image, cv2.IMREAD_COLOR)

# Initialize the EasyOCR Reader
reader = easyocr.Reader(['en'])  # You can specify other languages as needed

# Perform OCR on the image
results = reader.readtext(image)

# Draw bounding boxes around the detected text and display the text
for (bbox, text, prob) in results:
    # Unpack the bounding box
    (top_left, top_right, bottom_right, bottom_left) = bbox
    top_left = tuple(map(int, top_left))
    bottom_right = tuple(map(int, bottom_right))

    # Draw the rectangle around the text
    cv2.rectangle(image, top_left, bottom_right, (0, 255, 0), 2)
    
    # Put the detected text on the image
    cv2.putText(image, text, (top_left[0], top_left[1] - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 0, 0), 2)

# Display the image with bounding boxes and text
plt.imshow(cv2.cvtColor(image, cv2.COLOR_BGR2RGB))
plt.axis('off')
plt.show()

# Print the extracted text
for (bbox, text, prob) in results:
    print(f"Detected Text: {text} (Confidence: {prob:.2f})")
