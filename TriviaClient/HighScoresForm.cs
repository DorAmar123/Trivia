namespace TriviaClient;

public partial class HighScoresForm : Form
{
    public HighScoresForm()
    {
        InitializeComponent();

        // placeholder scores
        lblFirst.Text = "1. player1 - 1500 pts";
        lblSecond.Text = "2. player2 - 1200 pts";
        lblThird.Text = "3. player3 - 900 pts";
    }

    private void btnBack_Click(object sender, EventArgs e)
    {
        this.Close();
    }
}
