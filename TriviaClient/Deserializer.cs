using System.Text.Json;

namespace TriviaClient;

public enum ResponseCode
{
    Login = 1,
    Signup = 2,
    Error = 3,
    Logout = 4,
    GetRooms = 5,
    GetPlayersInRoom = 6,
    JoinRoom = 7,
    CreateRoom = 8,
    GetHighScore = 9,
    GetPersonalStats = 10,
    CloseRoom = 11,
    StartGame = 12,
    LeaveRoom = 13,
    GetRoomState = 14,
    LeaveGame = 15,
    GetQuestion = 16,
    SubmitAnswer = 17,
    GetGameResults = 18
}

public class ServerResponse
{
    public ResponseCode Code { get; set; }
    public JsonElement Data { get; set; }
}

public class RoomInfo
{
    public int Id { get; set; }
    public string Name { get; set; } = "";
    public int MaxPlayers { get; set; }
}

public class QuestionData
{
    public string Question { get; set; } = "";
    public Dictionary<int, string> Answers { get; set; } = new();
}

public class PlayerResult
{
    public string Username { get; set; } = "";
    public int CorrectAnswerCount { get; set; }
    public int WrongAnswerCount { get; set; }
    public float AverageAnswerTime { get; set; }
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

        JsonElement data;
        try
        {
            using JsonDocument doc = JsonDocument.Parse(json);
            data = doc.RootElement.Clone();
        }
        catch
        {
            using JsonDocument doc = JsonDocument.Parse("{}");
            data = doc.RootElement.Clone();
        }
        return new ServerResponse { Code = code, Data = data };
    }

    public static int GetStatus(ServerResponse response)
    {
        if (response.Data.TryGetProperty("status", out JsonElement status))
            return status.GetInt32();
        return 0;
    }

    public static string GetMessage(ServerResponse response)
    {
        if (response.Data.TryGetProperty("message", out JsonElement msg))
            return msg.GetString() ?? "";
        return "";
    }

    public static List<RoomInfo> GetRooms(ServerResponse response)
    {
        var rooms = new List<RoomInfo>();
        if (response.Data.TryGetProperty("Rooms", out JsonElement arr))
            foreach (var item in arr.EnumerateArray())
                rooms.Add(new RoomInfo
                {
                    Id = item.TryGetProperty("id", out var id) ? id.GetInt32() : 0,
                    Name = item.TryGetProperty("name", out var name) ? name.GetString() ?? "" : "",
                    MaxPlayers = item.TryGetProperty("maxPlayers", out var mp) ? mp.GetInt32() : 0
                });
        return rooms;
    }

    public static List<string> GetPlayers(ServerResponse response)
    {
        var players = new List<string>();
        if (response.Data.TryGetProperty("PlayersInRoom", out JsonElement arr))
            foreach (var item in arr.EnumerateArray())
                players.Add(item.GetString() ?? "");
        return players;
    }

    public static List<string> GetHighScores(ServerResponse response)
    {
        var scores = new List<string>();
        if (response.Data.TryGetProperty("HighScores", out JsonElement arr))
            foreach (var item in arr.EnumerateArray())
                scores.Add(item.GetString() ?? "");
        return scores;
    }

    public static List<string> GetPersonalStats(ServerResponse response)
    {
        var stats = new List<string>();
        if (response.Data.TryGetProperty("UserStatistics", out JsonElement arr))
            foreach (var item in arr.EnumerateArray())
                stats.Add(item.GetString() ?? "");
        return stats;
    }

    public static bool GetHasGameBegun(ServerResponse response)
    {
        if (response.Data.TryGetProperty("hasGameBegun", out JsonElement hasBegun))
            return hasBegun.GetBoolean();
        return false;
    }

    public static int GetCorrectAnswerId(ServerResponse response)
    {
        if (response.Data.TryGetProperty("correctAnswerId", out JsonElement id))
            return id.GetInt32();
        return 0;
    }

    public static int GetAnswerTimeout(ServerResponse response)
    {
        if (response.Data.TryGetProperty("answerTimeout", out JsonElement t))
            return t.GetInt32();
        return 0;
    }

    public static List<string> GetRoomStatePlayers(ServerResponse response)
    {
        var players = new List<string>();
        if (response.Data.TryGetProperty("players", out JsonElement arr))
            foreach (var item in arr.EnumerateArray())
                players.Add(item.GetString() ?? "");
        return players;
    }

    public static QuestionData GetQuestion(ServerResponse response)
    {
        var qd = new QuestionData();
        if (response.Data.TryGetProperty("question", out JsonElement q))
            qd.Question = q.GetString() ?? "";
        if (response.Data.TryGetProperty("answers", out JsonElement answers))
            foreach (var prop in answers.EnumerateObject())
                if (int.TryParse(prop.Name, out int key))
                    qd.Answers[key] = prop.Value.GetString() ?? "";
        return qd;
    }

    public static List<PlayerResult> GetGameResults(ServerResponse response)
    {
        var results = new List<PlayerResult>();
        if (response.Data.TryGetProperty("results", out JsonElement arr))
            foreach (var item in arr.EnumerateArray())
                results.Add(new PlayerResult
                {
                    Username = item.TryGetProperty("username", out var u) ? u.GetString() ?? "" : "",
                    CorrectAnswerCount = item.TryGetProperty("correctAnswerCount", out var c) ? c.GetInt32() : 0,
                    WrongAnswerCount = item.TryGetProperty("wrongAnswerCount", out var w) ? w.GetInt32() : 0,
                    AverageAnswerTime = item.TryGetProperty("averageAnswerTime", out var t) ? t.GetSingle() : 0f
                });
        return results;
    }
}
