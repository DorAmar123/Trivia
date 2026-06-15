namespace TriviaClient;

public partial class PersonalStatsForm : Form
{
    public PersonalStatsForm()
    {
        InitializeComponent();

        // placeholder stats
        lblCorrectAnswers.Text = "Correct Answers: 0";
        lblTotalAnswers.Text = "Total Answers: 0";
        lblAvgTime.Text = "Avg Answer Time: 0.0 sec";
        lblGamesPlayed.Text = "Games Played: 0";
        lblScore.Text = "Score: 0";
    }

    private void btnBack_Click(object sender, EventArgs e)
    {
        this.Close();
    }
}
