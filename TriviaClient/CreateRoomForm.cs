namespace TriviaClient;

public partial class CreateRoomForm : Form
{
    public CreateRoomForm()
    {
        InitializeComponent();
    }

    private void btnCreate_Click(object sender, EventArgs e)
    {
        string roomName = txtRoomName.Text.Trim();
        string timeStr = txtTimePerQuestion.Text.Trim();
        string playersStr = txtMaxPlayers.Text.Trim();

        if (roomName == "" || timeStr == "" || playersStr == "")
        {
            MessageBox.Show("Please fill in all fields.", "Error");
            return;
        }

        if (!int.TryParse(timeStr, out int timePerQuestion) || timePerQuestion <= 0)
        {
            MessageBox.Show("Time per question must be a positive number.", "Error");
            return;
        }

        if (!int.TryParse(playersStr, out int maxPlayers) || maxPlayers < 2)
        {
            MessageBox.Show("Max players must be at least 2.", "Error");
            return;
        }

        // TODO: send create room request via Communicator
        WaitingRoomForm waiting = new WaitingRoomForm(roomName, "You (Admin)");
        waiting.Show();
        this.Hide();
    }

    private void btnBack_Click(object sender, EventArgs e)
    {
        this.Close();
    }
}
