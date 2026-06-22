namespace TriviaClient;

public partial class CreateRoomForm : Form
{
    private readonly MainMenuForm? _parentMenu;

    public CreateRoomForm(MainMenuForm? parentMenu = null)
    {
        InitializeComponent();
        _parentMenu = parentMenu;
    }

    private void btnCreate_Click(object sender, EventArgs e)
    {
        string roomName = txtRoomName.Text.Trim();
        string timeStr = txtTimePerQuestion.Text.Trim();
        string questionsStr = txtQuestionCount.Text.Trim();
        string playersStr = txtMaxPlayers.Text.Trim();

        if (roomName == "" || timeStr == "" || questionsStr == "" || playersStr == "")
        {
            MessageBox.Show("Please fill in all fields.", "Error");
            return;
        }

        if (!int.TryParse(timeStr, out int answerTimeout) || answerTimeout <= 0)
        {
            MessageBox.Show("Time per question must be a positive number.", "Error");
            return;
        }

        if (!int.TryParse(questionsStr, out int questionCount) || questionCount < 1)
        {
            MessageBox.Show("Number of questions must be at least 1.", "Error");
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
                Serializer.SerializeCreateRoom(roomName, maxUsers, questionCount, answerTimeout));

            if (response.Code == ResponseCode.CreateRoom && Deserializer.GetStatus(response) == 1)
            {
                AppState.AnswerTimeoutSeconds = answerTimeout;
                WaitingRoomForm waiting = new WaitingRoomForm(roomName, AppState.CurrentUsername, isAdmin: true);
                waiting.Show();
                _parentMenu?.Close();
                this.Close();
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
        if (_parentMenu != null && !_parentMenu.IsDisposed)
            _parentMenu.Show();
        else
            new MainMenuForm().Show();
        this.Close();
    }
}
