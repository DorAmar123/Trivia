namespace TriviaClient;

public partial class ResultsForm : Form
{
    private readonly List<PlayerResult> _results;
    private bool _leftGame = false;

    public ResultsForm(List<PlayerResult> results)
    {
        InitializeComponent();
        _results = results;
    }

    private void SendLeaveGameOnce()
    {
        if (_leftGame) return;
        _leftGame = true;
        try { AppState.Communicator.SendAndReceive(Serializer.SerializeLeaveGame()); } catch { }
    }

    protected override void OnLoad(EventArgs e)
    {
        base.OnLoad(e);
        DisplayResults();
    }

    private void DisplayResults()
    {
        lvResults.Items.Clear();

        if (_results.Count == 0)
        {
            lblWinner.Text = "No results available.";
            return;
        }

        var sorted = _results.OrderByDescending(r => r.CorrectAnswerCount)
                             .ThenBy(r => r.AverageAnswerTime)
                             .ToList();

        foreach (var result in sorted)
        {
            var item = new ListViewItem(result.Username);
            item.SubItems.Add(result.CorrectAnswerCount.ToString());
            item.SubItems.Add(result.WrongAnswerCount.ToString());
            item.SubItems.Add(result.AverageAnswerTime.ToString("F2") + "s");
            lvResults.Items.Add(item);
        }

        if (lvResults.Items.Count > 0)
        {
            lvResults.Items[0].BackColor = Color.Gold;
            lblWinner.Text = "Winner: " + sorted[0].Username + "!";
        }

        string myName = AppState.CurrentUsername;
        for (int i = 0; i < sorted.Count; i++)
        {
            if (sorted[i].Username == myName)
            {
                lvResults.Items[i].BackColor = i == 0 ? Color.Gold : Color.LightGreen;
                break;
            }
        }
    }

    private void btnBackToMenu_Click(object sender, EventArgs e)
    {
        SendLeaveGameOnce();
        MainMenuForm menu = new MainMenuForm();
        menu.Show();
        this.Close();
    }

    protected override void OnFormClosing(FormClosingEventArgs e)
    {
        SendLeaveGameOnce();
        base.OnFormClosing(e);
    }
}
