import requests
import cv2
import numpy as np
from PIL import Image, ImageEnhance
import pytesseract
from io import BytesIO


# Function to download, process, and extract text from the image
def solve(url):

    # Download the image
    response = requests.get(url)
    img = Image.open(BytesIO(response.content))

    # Convert the image to an OpenCV format
    img_cv = np.array(img)

    # Preprocess the image: Convert to grayscale
    gray = cv2.cvtColor(img_cv, cv2.COLOR_BGR2GRAY)

    # Apply thresholding to the grayscale image
    _, threshold_img = cv2.threshold(gray, 128, 255, cv2.THRESH_BINARY)

    # Convert the processed image back to PIL format
    processed_img = Image.fromarray(threshold_img)

    # Perform OCR using Tesseract and extract all the text
    text = pytesseract.image_to_string(processed_img)

    # Print the extracted text
    print("Extracted text:\n", text)

    # Further preprocessing for "Net Weight" region:
    # Enhance contrast and sharpen the image
    enhancer = ImageEnhance.Contrast(processed_img)
    enhanced_img = enhancer.enhance(2.0)  # Increasing contrast

    # Crop the specific region where the "Net Weight" is expected
    # Adjust the coordinates to match the region of "Net Weight"
    net_weight_region = enhanced_img.crop((800, 1100, 1100, 1200))  # Update these coordinates as needed

    # Extract text from the cropped area for "Net Weight"
    net_weight_text = pytesseract.image_to_string(net_weight_region)
    
    # Print the extracted "Net Weight" text
    print("Extracted 'Net Weight' text:\n", net_weight_text)


# Test the function with different image URLs
solve('https://m.media-amazon.com/images/I/61BZ4zrjZXL.jpg')
print("----------------------------------------------------------")
solve('https://m.media-amazon.com/images/I/91LPf6OjV9L.jpg')
print("----------------------------------------------------------")
solve('https://m.media-amazon.com/images/I/71DiLRHeZdL.jpg')
print("----------------------------------------------------------")
