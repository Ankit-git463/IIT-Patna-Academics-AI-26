import socket
import os

# Client configuration
HOST = '127.0.0.1'  
PORT = 65432        
DIRECTORY = "./client_files/"  

# Function to request the list 
def request_files():
    # TCP socket for communication
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        # Connect to the server
        s.connect((HOST, PORT))
        
        # Send request to list all files
        s.sendall(b"listall")
        
        # Receive and decode the response from the server
        data = s.recv(1024).decode()
        
        # Print the list of available files
        print(f"Available files:\n{data}")

#request and download from the server
def download_file(filename):

    # Create a TCP socket for communication
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        # Connect to the server
        s.connect((HOST, PORT))
        
        # Send the request for the specific file
        s.sendall(f"send {filename}".encode())
        
        # Receive the file 
        data = s.recv(1024)
        
        # Check if the server sent an error message (e.g., file not found)
        if b"ERROR" in data:

            # Print an error message if the file is not found
            print("File not found on the server.")
        else:
            # If the file is found, construct the path to save it locally
            filepath = DIRECTORY + filename
            
            # Open the file in write-binary mode and save the received data
            with open(filepath, 'wb') as file:
                file.write(data)
            
            # Print a message indicating successful download
            print(f"File '{filename}' downloaded successfully.")

# Main execution block
if __name__ == "__main__":
    # Ensure the directory to save files exists; if not, create it
    if not os.path.exists(DIRECTORY):
        os.makedirs(DIRECTORY)

    # user input and send commands to the server
    while True:
        # Wait for user input (e.g., "listall" or "send filename")
        command = input(">> ")
        
        # Handle the "listall" command to get a list of files from the server
        if command == "listall":
            request_files()
        
        # Handle the "send" command to download a specific file
        elif command.startswith("send"):
            # Extract the filename from the command
            _, filename = command.split()
            
            # Call the function to download the file
            download_file(filename)
        
        # Handle unrecognized commands
        else:
            print("Unknown command")
