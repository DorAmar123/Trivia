namespace TriviaClient;

public partial class HighScoresForm : Form
{
    public HighScoresForm()
    {
        InitializeComponent();
        LoadHighScores();
    }

    private void LoadHighScores()
    {
        try
        {
            ServerResponse response = AppState.Communicator.SendAndReceive(
                Serializer.SerializeGetHighScores());

            List<string> scores = Deserializer.GetHighScores(response);

            lblFirst.Text  = "1. " + (scores.Count > 0 ? scores[0] : "N/A");
            lblSecond.Text = "2. " + (scores.Count > 1 ? scores[1] : "N/A");
            lblThird.Text  = "3. " + (scores.Count > 2 ? scores[2] : "N/A");
        }
        catch (Exception ex)
        {
            MessageBox.Show("Could not load high scores:\n" + ex.Message, "Error");
        }
    }

    private void btnBack_Click(object sender, EventArgs e)
    {
        this.Close();
    }
}
