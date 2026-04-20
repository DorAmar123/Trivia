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

        data = client_socket.recv(5).decode('utf-8')
        print(data)

        if data == "Hello":
            client_socket.sendall("Hello".encode('utf-8'))

        client_socket.close()
    except Exception:
        print("Error")

if __name__ == "__main__":
    main()