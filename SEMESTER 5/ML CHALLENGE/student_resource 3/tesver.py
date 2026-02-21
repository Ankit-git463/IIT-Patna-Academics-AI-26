import subprocess

# Run the tesseract --version command
tesseract_version = subprocess.run(['tesseract', '--version'], stdout=subprocess.PIPE)

# Print the output
print(tesseract_version.stdout.decode('utf-8'))
