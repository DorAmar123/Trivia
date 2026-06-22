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

        if (!int.TryParse(timeStr, out int answerTimeout) || answerTimeout <= 0)
        {
            MessageBox.Show("Time per question must be a positive number.", "Error");
            return;
        }

        if (!int.TryParse(playersStr, out int maxUsers) || maxUsers < 2)
        {
            MessageBox.Show("Max players must be at least 2.", "Error");
            return;
        }

        try
        {
            ServerResponse response = AppState.Communicator.SendAndReceive(
                Serializer.SerializeCreateRoom(roomName, maxUsers, 10, answerTimeout));

            if (response.Code == ResponseCode.CreateRoom && Deserializer.GetStatus(response) == 1)
            {
                AppState.AnswerTimeoutSeconds = answerTimeout;
                WaitingRoomForm waiting = new WaitingRoomForm(roomName, AppState.CurrentUsername, isAdmin: true);
                waiting.Show();
                this.Hide();
            }
            else
            {
                MessageBox.Show("Failed to create room.", "Error");
            }
        }
        catch (Exception ex)
        {
            MessageBox.Show("Error: " + ex.Message, "Error");
        }
    }

    private void btnBack_Click(object sender, EventArgs e)
    {
        this.Close();
    }
}
