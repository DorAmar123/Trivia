import socket

def main():
    port_str = input("Enter port: ")
    
    try:
        port = int(port_str)
        if port < 1024 or port > 65535:
            print("Error")
            return
    except ValueError:
        print("Error")
        return

    try:
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.connect(('127.0.0.1', port))

        # Login Send
        login_json = '{"username": "user1", "password": "1234"}'
        login_size = len(login_json)
        # Message code 1 (for login), 4 bytes size, then json data
        login_msg = bytes([1]) + login_size.to_bytes(4, byteorder='big') + login_json.encode('utf-8')
        client_socket.sendall(login_msg)

        # Receive Response
        resp_code = client_socket.recv(1)
        resp_size_bytes = client_socket.recv(4)
        resp_size = int.from_bytes(resp_size_bytes, byteorder='big')
        resp_data = client_socket.recv(resp_size).decode('utf-8')
        print(f"Login Response - Code: {int.from_bytes(resp_code, 'big')}, Data: {resp_data}")

        # Signup Send
        signup_json = '{"username": "user1", "password": "1234", "email": "user1@gmail.com"}'
        signup_size = len(signup_json)
        # Message code 2 (for signup), 4 bytes size, then json data
        signup_msg = bytes([2]) + signup_size.to_bytes(4, byteorder='big') + signup_json.encode('utf-8')
        client_socket.sendall(signup_msg)

        # Receive Response
        resp_code2 = client_socket.recv(1)
        resp_size_bytes2 = client_socket.recv(4)
        resp_size2 = int.from_bytes(resp_size_bytes2, byteorder='big')
        resp_data2 = client_socket.recv(resp_size2).decode('utf-8')
        print(f"Signup Response - Code: {int.from_bytes(resp_code2, 'big')}, Data: {resp_data2}")

        client_socket.close()
    except Exception:
        print("Error")

if __name__ == "__main__":
    main()