import socket

LOGIN_CODE  = 1
SIGNUP_CODE = 2
PORT        = 8826


def is_username_valid(username):
    if len(username) == 0 or len(username) > 20:
        return False
    for c in username:
        if not (c.isalnum() or c == '_'):
            return False
    return True


def send_recv(sock, code, json_str):
    data = json_str.encode('utf-8')
    msg  = bytes([code]) + len(data).to_bytes(4, byteorder='big') + data
    sock.sendall(msg)

    resp_code = sock.recv(1)
    resp_size = int.from_bytes(sock.recv(4), byteorder='big')
    resp_data = sock.recv(resp_size).decode('utf-8')
    return int.from_bytes(resp_code, 'big'), resp_data


def try_login(sock, username, password):
    if is_username_valid(username) == False:
        print("bad username")
        return None, None
    body = '{"username": "' + username + '", "password": "' + password + '"}'
    code, data = send_recv(sock, LOGIN_CODE, body)
    print("got:", code, data)
    return code, data


def try_signup(sock, username, password, mail):
    if is_username_valid(username) == False:
        print("bad username")
        return None, None
    body = '{"username": "' + username + '", "password": "' + password + '", "mail": "' + mail + '"}'
    code, data = send_recv(sock, SIGNUP_CODE, body)
    print("got:", code, data)
    return code, data


def open_connection(port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(('127.0.0.1', port))
    return s


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
        # try to login before the user exists - should fail
        print("login without signup")
        s = open_connection(port)
        try_login(s, "user1", "1234")
        s.close()

        # signup a new user - should work
        print()
        print("signup")
        s = open_connection(port)
        try_signup(s, "user1", "1234", "user1@gmail.com")
        s.close()

        # signup with the same name again - should fail
        print()
        print("signup again same name")
        s = open_connection(port)
        try_signup(s, "user1", "1234", "user1@gmail.com")
        s.close()

        # login with the user we just created - should work
        print()
        print("login")
        s1 = open_connection(port)
        try_login(s1, "user1", "1234")
 
        # second login while the first is still open - should fail
        print()
        print("login twice")
        s2 = open_connection(port)
        try_login(s2, "user1", "1234")
        s2.close()
        s1.close()

        # empty name and a name with a space + ! - client should reject before sending
        print()
        print("bad usernames")
        s = open_connection(port)
        try_login(s, "", "1234")
        try_login(s, "bad name!", "1234")
        s.close()

    except Exception as e:
        print("Error:", e)


if __name__ == "__main__":
    main()
