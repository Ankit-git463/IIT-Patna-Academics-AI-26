import pytesseract
from PIL import Image, ImageFilter, ImageEnhance
import requests
from io import BytesIO
import os

# Set path to tesseract executable
pytesseract.pytesseract.tesseract_cmd = r'C:/Program Files/Tesseract-OCR/tesseract.exe'

# Set the TESSDATA_PREFIX environment variable to the tessdata directory
os.environ['TESSDATA_PREFIX'] = r'C:/Program Files/Tesseract-OCR/tessdata'

def preprocess_image(image):
    # Convert to grayscale
    image = image.convert('L')
    
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
