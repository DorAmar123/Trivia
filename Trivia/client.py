import socket

LOGIN_CODE  = 1
SIGNUP_CODE = 2
PORT        = 8826


def send_recv(sock, code, json_str):
    data = json_str.encode('utf-8')
    msg  = bytes([code]) + len(data).to_bytes(4, byteorder='big') + data
    sock.sendall(msg)

    resp_code = sock.recv(1)
    resp_size = int.from_bytes(sock.recv(4), byteorder='big')
    resp_data = sock.recv(resp_size).decode('utf-8')
    return int.from_bytes(resp_code, 'big'), resp_data


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

        # Login
        login_json = '{"username": "user1", "password": "1234"}'
        code, data = send_recv(client_socket, LOGIN_CODE, login_json)
        print(f"Login Response - Code: {code}, Data: {data}")

        # Signup
        signup_json = '{"username": "user1", "password": "1234", "email": "user1@gmail.com"}'
        code, data = send_recv(client_socket, SIGNUP_CODE, signup_json)
        print(f"Signup Response - Code: {code}, Data: {data}")

        client_socket.close()
    except Exception:
        print("Error")


if __name__ == "__main__":
    main()
