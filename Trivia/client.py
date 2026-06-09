import socket
import json

PORT = 8826

# Request codes
LOGIN_CODE              = 1
SIGNUP_CODE             = 2
LOGOUT_CODE             = 3
GET_ROOMS_CODE          = 4
GET_PLAYERS_IN_ROOM_CODE = 5
JOIN_ROOM_CODE          = 6
CREATE_ROOM_CODE        = 7
GET_PERSONAL_STATS_CODE = 8
GET_HIGH_SCORE_CODE     = 9


def send_request(sock, code, data: dict = {}):
    body = json.dumps(data).encode('utf-8')
    msg = bytes([code]) + len(body).to_bytes(4, byteorder='big') + body
    sock.sendall(msg)


def recv_response(sock):
    code = int.from_bytes(sock.recv(1), 'big')
    size = int.from_bytes(sock.recv(4), 'big')
    data = json.loads(sock.recv(size).decode('utf-8')) if size > 0 else {}
    return code, data


def login_menu(sock):
    while True:
        print("\n--- Login / Signup ---")
        print("1. Login")
        print("2. Signup")
        print("0. Exit")
        choice = input("> ").strip()

        if choice == "1":
            username = input("Username: ")
            password = input("Password: ")
            send_request(sock, LOGIN_CODE, {"username": username, "password": password})
            code, data = recv_response(sock)
            print(f"Response ({code}): {data}")
            if data.get("status") == 1:
                print(f"Logged in as {username}")
                return username

        elif choice == "2":
            username = input("Username: ")
            password = input("Password: ")
            email    = input("Email: ")
            send_request(sock, SIGNUP_CODE, {"username": username, "password": password, "email": email})
            code, data = recv_response(sock)
            print(f"Response ({code}): {data}")

        elif choice == "0":
            return None


def main_menu(sock, username):
    while True:
        print(f"\n--- Menu ({username}) ---")
        print("1. Get Rooms")
        print("2. Get Players In Room")
        print("3. Join Room")
        print("4. Create Room")
        print("5. Get Personal Stats")
        print("6. Get High Score")
        print("7. Logout")
        print("0. Exit")
        choice = input("> ").strip()

        if choice == "1":
            send_request(sock, GET_ROOMS_CODE)
            code, data = recv_response(sock)
            rooms = data.get("Rooms", [])
            if rooms:
                for r in rooms:
                    print(f"  [{r['id']}] {r['name']} - {r['maxPlayers']} players, {r['numOfQuestionsInGame']} questions")
            else:
                print("  No rooms available.")

        elif choice == "2":
            room_id = int(input("Room ID: "))
            send_request(sock, GET_PLAYERS_IN_ROOM_CODE, {"roomId": room_id})
            code, data = recv_response(sock)
            print(f"Players: {data.get('PlayersInRoom', [])}")

        elif choice == "3":
            room_id = int(input("Room ID: "))
            send_request(sock, JOIN_ROOM_CODE, {"roomId": room_id})
            code, data = recv_response(sock)
            print(f"Response ({code}): {data}")

        elif choice == "4":
            name      = input("Room name: ")
            max_users = int(input("Max players: "))
            questions = int(input("Number of questions: "))
            timeout   = int(input("Answer timeout (seconds): "))
            send_request(sock, CREATE_ROOM_CODE, {
                "roomName": name,
                "maxUsers": max_users,
                "questionCount": questions,
                "answerTimeout": timeout
            })
            code, data = recv_response(sock)
            print(f"Response ({code}): {data}")

        elif choice == "5":
            send_request(sock, GET_PERSONAL_STATS_CODE)
            code, data = recv_response(sock)
            for stat in data.get("UserStatistics", []):
                print(f"  {stat}")

        elif choice == "6":
            send_request(sock, GET_HIGH_SCORE_CODE)
            code, data = recv_response(sock)
            for entry in data.get("HighScores", []):
                print(f"  {entry}")

        elif choice == "7":
            send_request(sock, LOGOUT_CODE)
            code, data = recv_response(sock)
            print(f"Logged out (status: {data.get('status')})")
            return

        elif choice == "0":
            return


def main():
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect(('127.0.0.1', PORT))
        print(f"Connected to server on port {PORT}")
    except Exception as e:
        print(f"Could not connect: {e}")
        return

    try:
        while True:
            username = login_menu(sock)
            if username is None:
                break
            main_menu(sock, username)
    except Exception as e:
        print(f"Error: {e}")
    finally:
        sock.close()


if __name__ == "__main__":
    main()
