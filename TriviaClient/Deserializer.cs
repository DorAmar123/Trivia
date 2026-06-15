using System.Text.Json;

namespace TriviaClient;

public enum ResponseCode
{
    LoginSuccess = 1,
    LoginFailed = 2,
    SignupSuccess = 3,
    SignupFailed = 4,
    LogoutSuccess = 5,
    GetRoomsSuccess = 6,
    GetPlayersInRoomSuccess = 7,
    JoinRoomSuccess = 8,
    JoinRoomFailed = 9,
    CreateRoomSuccess = 10,
    CreateRoomFailed = 11,
    GetPersonalStatsSuccess = 12,
    GetPersonalStatsFailed = 13,
    GetHighScoresSuccess = 14,
    GetHighScoresFailed = 15,
    Error = 99
}

public class ServerResponse
{
    public ResponseCode Code { get; set; }
    public JsonElement Data { get; set; }
}

public static class Deserializer
{
    public static ServerResponse Deserialize(byte[] packet)
    {
        if (packet.Length < 5)
            throw new Exception("Packet too short");

        ResponseCode code = (ResponseCode)packet[0];
        int size = (packet[1] << 24) | (packet[2] << 16) | (packet[3] << 8) | packet[4];

        string json = "{}";
        if (size > 0 && packet.Length >= 5 + size)
            json = System.Text.Encoding.UTF8.GetString(packet, 5, size);

        JsonElement data = JsonDocument.Parse(json).RootElement;

        return new ServerResponse { Code = code, Data = data };
    }

    public static string GetMessage(ServerResponse response)
    {
        if (response.Data.TryGetProperty("message", out JsonElement msg))
            return msg.GetString() ?? "";
        return "";
    }

    public static List<string> GetRooms(ServerResponse response)
    {
        var rooms = new List<string>();
        if (response.Data.TryGetProperty("rooms", out JsonElement arr))
            foreach (var item in arr.EnumerateArray())
                rooms.Add(item.GetString() ?? "");
        return rooms;
    }

    public static List<string> GetPlayers(ServerResponse response)
    {
        var players = new List<string>();
        if (response.Data.TryGetProperty("players", out JsonElement arr))
            foreach (var item in arr.EnumerateArray())
                players.Add(item.GetString() ?? "");
        return players;
    }

    public static List<string> GetHighScores(ServerResponse response)
    {
        var scores = new List<string>();
        if (response.Data.TryGetProperty("highScores", out JsonElement arr))
            foreach (var item in arr.EnumerateArray())
                scores.Add(item.GetString() ?? "");
        return scores;
    }
}
