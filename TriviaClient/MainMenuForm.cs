namespace TriviaClient;

public partial class MainMenuForm : Form
{
    public MainMenuForm()
    {
        InitializeComponent();
    }

    private void btnCreateRoom_Click(object sender, EventArgs e)
    {
        // TODO: open CreateRoomForm
        MessageBox.Show("Create Room clicked! (not connected yet)", "Info");
    }

    private void btnJoinRoom_Click(object sender, EventArgs e)
    {
        // TODO: open JoinRoomForm
        MessageBox.Show("Join Room clicked! (not connected yet)", "Info");
    }

    private void btnStatistics_Click(object sender, EventArgs e)
    {
        // TODO: open StatisticsForm
        MessageBox.Show("Statistics clicked! (not connected yet)", "Info");
    }

    private void btnLogout_Click(object sender, EventArgs e)
    {
        // TODO: send logout request via Communicator
        LoginForm login = new LoginForm();
        login.Show();
        this.Close();
    }
}
