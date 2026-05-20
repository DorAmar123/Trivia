import socket
import json

# Constants
LOGIN_CODE = 1
SIGNUP_CODE = 2
PORT_MIN = 1024
PORT_MAX = 65535

def is_username_valid(username):
    """Validates username: 1-20 chars, alphanumeric and underscores only."""
    if not (0 < len(username) <= 20):
        return False
    for char in username:
        if not (char.isalnum() or char == '_'):
            return False
    return True

def send_packet(sock, code, data_dict):
    """Serializes and sends a packet according to the protocol."""
    json_str = json.dumps(data_dict)
    data_bytes = json_str.encode('utf-8')

    # Building the header: 1 byte Code + 4 bytes Size (Big Endian)
    header = bytes([code]) + len(data_bytes).to_bytes(4, byteorder='big')
    packet = header + data_bytes

    sock.sendall(packet)

def recv_packet(sock):
    """Receives and parses a packet from the server."""
    try:
        res_code = int.from_bytes(sock.recv(1), byteorder='big')
        if res_code == 0:  # Server closed connection
            return None, None

        res_size = int.from_bytes(sock.recv(4), byteorder='big')
        res_data = sock.recv(res_size).decode('utf-8')

        return res_code, res_data
    except Exception as e:
        print(f"\n[!] Error receiving data: {e}")
        return None, None

def get_valid_port():
    """Prompts for a valid port between 1024-65535."""
    while True:
        port_input = input(f"Enter server port ({PORT_MIN}-{PORT_MAX}): ")
        if port_input.isdigit():
            port = int(port_input)
            if PORT_MIN <= port <= PORT_MAX:
                return port
        print(f"Invalid port. Please enter a number between {PORT_MIN} and {PORT_MAX}.")

def signup(sock):
    print("\n--- Signup ---")
    username = input("Username: ")
    if not is_username_valid(username):
        print("Invalid username! (1-20 chars, letters/numbers/_ only)")
        return

    password = input("Password: ")
    email = input("Email: ")

    payload = {"username": username, "password": password, "email": email}
    send_packet(sock, SIGNUP_CODE, payload)

    code, response = recv_packet(sock)
    print(f"\nServer Response Code: {code}")
    print(f"Content: {response}")

def login(sock):
    print("\n--- Login ---")
    username = input("Username: ")
    if not is_username_valid(username):
        print("Invalid username!")
        return

    password = input("Password: ")

    payload = {"username": username, "password": password}
    send_packet(sock, LOGIN_CODE, payload)

    code, response = recv_packet(sock)
    print(f"\nServer Response Code: {code}")
    print(f"Content: {response}")

def main():
    print("Welcome to Trivia Tester 1.0.3")
    port = get_valid_port()

    try:
        # Create socket and connect
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
            sock.connect(('127.0.0.1', port))
            print(f"Connected to 127.0.0.1:{port}")

            while True:
                print("\nMain Menu:")
                print("1) Signup")
                print("2) Login")
                print("3) Exit")

                choice = input("Select an option: ")

                if choice == '1':
                    signup(sock)
                elif choice == '2':
                    login(sock)
                elif choice == '3':
                    print("Exiting...")
                    break
                else:
                    print("Invalid choice, try again.")

    except ConnectionRefusedError:
        print("\n[!] Connection failed. Is the server running?")
    except Exception as e:
        print(f"\n[!] An error occurred: {e}")

if __name__ == "__main__":
    main()
