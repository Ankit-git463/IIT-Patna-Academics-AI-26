import requests
import cv2
import numpy as np
from PIL import Image
import pytesseract
from io import BytesIO

# URL of the image
url = 'https://m.media-amazon.com/images/I/213wY3gUsmL.jpg'
url = 'https://m.media-amazon.com/images/I/41-NCxNuBxL.jpg'


def solve(url):

    # Download the image
    response = requests.get(url)
    img = Image.open(BytesIO(response.content))

    # Convert the image to an OpenCV format
    img_cv = np.array(img)

    # Preprocess the image: Convert to grayscale and apply thresholding
    gray = cv2.cvtColor(img_cv, cv2.COLOR_BGR2GRAY)
    _, threshold_img = cv2.threshold(gray, 128, 255, cv2.THRESH_BINARY)

    # Convert the processed image back to PIL format
    processed_img = Image.fromarray(threshold_img)

    # Perform OCR using Tesseract 3.02 and extract all the text
    text = pytesseract.image_to_string(processed_img)

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

