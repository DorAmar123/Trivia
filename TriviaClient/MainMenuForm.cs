namespace TriviaClient;

public partial class MainMenuForm : Form
{
    public MainMenuForm()
    {
        InitializeComponent();
        lblWelcome.Text = "Welcome, " + AppState.CurrentUsername + "!";
    }

    private void btnCreateRoom_Click(object sender, EventArgs e)
    {
        CreateRoomForm createRoom = new CreateRoomForm();
        createRoom.Show();
        this.Hide();
    }

    private void btnJoinRoom_Click(object sender, EventArgs e)
    {
        JoinRoomForm joinRoom = new JoinRoomForm();
        joinRoom.Show();
        this.Hide();
    }

    private void btnStatistics_Click(object sender, EventArgs e)
    {
        StatisticsMenuForm stats = new StatisticsMenuForm();
        stats.ShowDialog();
    }

    private void btnLogout_Click(object sender, EventArgs e)
    {
        try
        {
            AppState.Communicator.SendAndReceive(Serializer.SerializeLogout());
        }
        catch { }

        AppState.Communicator.Disconnect();
        AppState.CurrentUsername = "";

        LoginForm login = new LoginForm();
        login.Show();
        this.Close();
    }
}
