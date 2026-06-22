namespace TriviaClient;

static class Program
{
    [STAThread]
    static void Main()
    {
        ApplicationConfiguration.Initialize();
        Application.ApplicationExit += (s, e) => AppState.Communicator.Disconnect();
        Application.Run(new LoginForm());
    }
}
