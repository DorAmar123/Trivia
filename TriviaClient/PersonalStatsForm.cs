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

            // server sends: [correctAnswers, totalAnswers, avgTime, numGames, score]
            lblCorrectAnswers.Text = "Correct Answers: " + (stats.Count > 0 ? stats[0] : "0");
            lblTotalAnswers.Text   = "Total Answers: "   + (stats.Count > 1 ? stats[1] : "0");
            lblAvgTime.Text        = "Avg Answer Time: " + (stats.Count > 2 ? stats[2] : "0") + " sec";
            lblGamesPlayed.Text    = "Games Played: "    + (stats.Count > 3 ? stats[3] : "0");
            lblScore.Text          = "Score: "           + (stats.Count > 4 ? stats[4] : "0");
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
