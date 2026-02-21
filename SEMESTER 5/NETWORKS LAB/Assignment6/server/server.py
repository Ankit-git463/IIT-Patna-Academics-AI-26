import socket
import os

# Server configuration
# IP address for the server 
HOST = '127.0.0.1'  
# Port number on which the server will listen for connections
PORT = 65432       
# Folder containing the files to share with clients
DIRECTORY = "./server_files/"  

# Function to list all files
def list_files():
    return os.listdir(DIRECTORY)  # Returns a list of all files in the specified directory

# Function to send a specific file to the client
def send_file(conn, filename):
    # Constructs the full path of the file requested
    filepath = DIRECTORY + filename  
    
    # Checks if the file exists in the directory
    if os.path.exists(filepath): 

        # Opens the file in binary read mode and sends its content to the client
        with open(filepath, 'rb') as file:
            #Sends the entire content of the file to the client
            conn.sendall(file.read())  
    else:
        # If the file is not found, an error message is sent back to the client
        conn.sendall(b"ERROR: File not found")

# Function to start the server
def start_server():
    # Create a TCP/IP socket
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        
        # Binds the socket to the specified address (HOST, PORT)
        s.bind((HOST, PORT))
        # Server starts listening for incoming connections (backlog set to default)
        s.listen()
        print(f"Server started. Listening on {HOST}:{PORT}")

        # loop to handle multiple clients one by one
        while True:

            # Accept a new connection from a client
            conn, addr = s.accept()
            # 'conn' is a new socket object used to communicate with the client
            # 'addr' is the address of the client (IP, Port)
            with conn:
                # Prints the client's address that connected
    
                print("Connection made Successful !!")
                print(f"Connected by {addr}")  
                print()

                # Receives data from the client, 1024 bytes at a time
                data = conn.recv(1024).decode()  # Decodes the data from bytes to string

                # If the client requests the list of all files
                if data == "listall":
                    # Calls the function to get a list of files
                    files = list_files()  
                    # Sends the list as a string
                    conn.sendall("\n".join(files).encode())  


                # If the client requests a specific file using the "send" command
                elif data.startswith("send"):
                    # Extracts the filename from the client's request
                    _, filename = data.split()
                    # Calls the function to send the requested file
                    send_file(conn, filename)  


# Main entry point for the server script
if __name__ == "__main__":
    # Check if the directory exists; if not, create it
    if not os.path.exists(DIRECTORY):
        os.makedirs(DIRECTORY)
    # Start the server
    start_server()
