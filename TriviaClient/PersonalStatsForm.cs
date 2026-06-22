namespace TriviaClient;

public partial class PersonalStatsForm : Form
{
    public PersonalStatsForm()
    {
        InitializeComponent();
        LoadStats();
    }

    private void LoadStats()
    {
        try
        {
            ServerResponse response = AppState.Communicator.SendAndReceive(
                Serializer.SerializeGetPersonalStats());

            List<string> stats = Deserializer.GetPersonalStats(response);

            lblAvgTime.Text        = stats.Count > 0 ? stats[0] : "Average answer time: 0";
            lblCorrectAnswers.Text = stats.Count > 1 ? stats[1] : "Correct answers: 0";
            lblTotalAnswers.Text   = stats.Count > 2 ? stats[2] : "Total answers: 0";
            lblGamesPlayed.Text    = stats.Count > 3 ? stats[3] : "Number of games: 0";
            lblScore.Text          = stats.Count > 4 ? stats[4] : "Score: 0";
        }
        catch (Exception ex)
        {
            MessageBox.Show("Could not load statistics:\n" + ex.Message, "Error");
        }
    }

    private void btnBack_Click(object sender, EventArgs e)
    {
        this.Close();
    }
}
