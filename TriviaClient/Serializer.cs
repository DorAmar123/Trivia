using System.Text;
using System.Text.Json;

namespace TriviaClient;

public enum RequestCode
{
    Login = 1,
    Signup = 2,
    Logout = 3,
    GetRooms = 4,
    GetPlayersInRoom = 5,
    JoinRoom = 6,
    CreateRoom = 7,
    GetPersonalStats = 8,
    GetHighScores = 9,
    CloseRoom = 11,
    StartGame = 12,
    LeaveRoom = 13,
    GetRoomState = 14,
    LeaveGame = 15,
    GetQuestion = 16,
    SubmitAnswer = 17,
    GetGameResults = 18
}

public static class Serializer
{
    public static byte[] Serialize(RequestCode code, object data)
    {
        string json = JsonSerializer.Serialize(data);
        byte[] jsonBytes = Encoding.UTF8.GetBytes(json);
        int size = jsonBytes.Length;

        byte[] packet = new byte[5 + size];
        packet[0] = (byte)code;
        packet[1] = (byte)((size >> 24) & 0xFF);
        packet[2] = (byte)((size >> 16) & 0xFF);
        packet[3] = (byte)((size >> 8) & 0xFF);
        packet[4] = (byte)(size & 0xFF);
        Array.Copy(jsonBytes, 0, packet, 5, size);

        return packet;
    }

    public static byte[] SerializeLogin(string username, string password)
    {
        return Serialize(RequestCode.Login, new { username, password });
    }

    public static byte[] SerializeSignup(string username, string password, string email)
    {
        return Serialize(RequestCode.Signup, new { username, password, email });
    }

    public static byte[] SerializeLogout()
    {
        return Serialize(RequestCode.Logout, new { });
    }

    public static byte[] SerializeGetRooms()
    {
        return Serialize(RequestCode.GetRooms, new { });
    }

    public static byte[] SerializeGetPlayersInRoom(int roomId)
    {
        return Serialize(RequestCode.GetPlayersInRoom, new { roomId });
    }

    public static byte[] SerializeJoinRoom(int roomId)
    {
        return Serialize(RequestCode.JoinRoom, new { roomId });
    }

    public static byte[] SerializeCreateRoom(string roomName, int maxUsers, int questionCount, int answerTimeout)
    {
        return Serialize(RequestCode.CreateRoom, new { roomName, maxUsers, questionCount, answerTimeout });
    }

    public static byte[] SerializeGetPersonalStats()
    {
        return Serialize(RequestCode.GetPersonalStats, new { });
    }

    public static byte[] SerializeGetHighScores()
    {
        return Serialize(RequestCode.GetHighScores, new { });
    }

    public static byte[] SerializeCloseRoom()
    {
        return Serialize(RequestCode.CloseRoom, new { });
    }

    public static byte[] SerializeStartGame()
    {
        return Serialize(RequestCode.StartGame, new { });
    }

    public static byte[] SerializeLeaveRoom()
    {
        return Serialize(RequestCode.LeaveRoom, new { });
    }

    public static byte[] SerializeGetRoomState()
    {
        return Serialize(RequestCode.GetRoomState, new { });
    }

    public static byte[] SerializeLeaveGame()
    {
        return Serialize(RequestCode.LeaveGame, new { });
    }

    public static byte[] SerializeGetQuestion()
    {
        return Serialize(RequestCode.GetQuestion, new { });
    }

    public static byte[] SerializeSubmitAnswer(int answerId)
    {
        return Serialize(RequestCode.SubmitAnswer, new { answerId });
    }

    public static byte[] SerializeGetGameResults()
    {
        return Serialize(RequestCode.GetGameResults, new { });
    }
}
