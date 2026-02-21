import pytesseract
from PIL import Image, ImageFilter, ImageEnhance
import requests
from io import BytesIO
import os
import cv2
import numpy as np

# Set path to tesseract executable
pytesseract.pytesseract.tesseract_cmd = r'C:/Program Files/Tesseract-OCR/tesseract.exe'

# Set the TESSDATA_PREFIX environment variable to the tessdata directory
os.environ['TESSDATA_PREFIX'] = r'C:/Program Files/Tesseract-OCR/tessdata'

def preprocess_image(image):
    # Convert PIL Image to OpenCV format
    image_cv = np.array(image)
    
    # Convert to grayscale
    gray = cv2.cvtColor(image_cv, cv2.COLOR_RGB2GRAY)
    
    # Apply thresholding to improve contrast
    _, thresh = cv2.threshold(gray, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
    
    # Find the angle of rotation
    coords = np.column_stack(np.where(thresh > 0))
    angle = cv2.minAreaRect(coords)[-1]
    
    # Correct the angle for OpenCV
    if angle < -45:
        angle = -(90 + angle)
    else:
        angle = -angle
    
    # Deskew the image
    (h, w) = thresh.shape[:2]
    center = (w // 2, h // 2)
    M = cv2.getRotationMatrix2D(center, angle, 1.0)
    rotated = cv2.warpAffine(image_cv, M, (w, h), flags=cv2.INTER_CUBIC, borderMode=cv2.BORDER_REPLICATE)
    
    # Convert back to PIL Image
    image = Image.fromarray(rotated)
    
    # Enhance image contrast
    enhancer = ImageEnhance.Contrast(image)
    image = enhancer.enhance(2)  # Increase contrast
    
    # Apply sharpening filter
    image = image.filter(ImageFilter.SHARPEN)
    
    return image

def solve(image_url):
    # Download the image from the URL
    response = requests.get(image_url)
    image = Image.open(BytesIO(response.content))
    
    # Preprocess the image
    image = preprocess_image(image)
    
    # Process the image with pytesseract
    text = pytesseract.image_to_string(image)
    
    # Print the extracted text
    print(text)

print()
solve('https://m.media-amazon.com/images/I/61BZ4zrjZXL.jpg')
print("----------------------------------------------------------")

print()
solve('https://m.media-amazon.com/images/I/91LPf6OjV9L.jpg')
print("----------------------------------------------------------")

print()
solve('https://m.media-amazon.com/images/I/71DiLRHeZdL.jpg')
print("----------------------------------------------------------")
