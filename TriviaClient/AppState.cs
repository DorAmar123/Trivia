namespace TriviaClient;

public static class AppState
{
    public static Communicator Communicator { get; } = new Communicator();
    public static string CurrentUsername { get; set; } = "";
    public static int AnswerTimeoutSeconds { get; set; } = 20;
}
