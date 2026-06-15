namespace TriviaClient;

public partial class StatisticsMenuForm : Form
{
    public StatisticsMenuForm()
    {
        InitializeComponent();
    }

    private void btnPersonalStats_Click(object sender, EventArgs e)
    {
        PersonalStatsForm stats = new PersonalStatsForm();
        stats.ShowDialog();
    }

    private void btnHighScores_Click(object sender, EventArgs e)
    {
        HighScoresForm scores = new HighScoresForm();
        scores.ShowDialog();
    }

    private void btnBack_Click(object sender, EventArgs e)
    {
        this.Close();
    }
}
