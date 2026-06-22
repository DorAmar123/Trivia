namespace TriviaClient;

partial class PersonalStatsForm
{
    private System.ComponentModel.IContainer components = null;

    protected override void Dispose(bool disposing)
    {
        if (disposing && (components != null))
            components.Dispose();
        base.Dispose(disposing);
    }

    private void InitializeComponent()
    {
        lblTitle = new Label();
        lblCorrectAnswers = new Label();
        lblTotalAnswers = new Label();
        lblAvgTime = new Label();
        lblGamesPlayed = new Label();
        lblScore = new Label();
        btnBack = new Button();

        SuspendLayout();

        // Form
        this.Text = "Trivia - Personal Statistics";
        this.ClientSize = new Size(300, 278);
        this.StartPosition = FormStartPosition.CenterScreen;
        this.FormBorderStyle = FormBorderStyle.FixedSingle;
        this.MaximizeBox = false;

        // lblTitle
        lblTitle.Text = "My Statistics";
        lblTitle.Font = new Font("Microsoft Sans Serif", 12F, FontStyle.Bold);
        lblTitle.Location = new Point(80, 15);
        lblTitle.Size = new Size(140, 25);
        lblTitle.TextAlign = ContentAlignment.MiddleCenter;

        // lblCorrectAnswers
        lblCorrectAnswers.Text = "Correct Answers: ";
        lblCorrectAnswers.Location = new Point(40, 60);
        lblCorrectAnswers.Size = new Size(220, 22);

        // lblTotalAnswers
        lblTotalAnswers.Text = "Total Answers: ";
        lblTotalAnswers.Location = new Point(40, 92);
        lblTotalAnswers.Size = new Size(220, 22);

        // lblAvgTime
        lblAvgTime.Text = "Avg Answer Time: ";
        lblAvgTime.Location = new Point(40, 124);
        lblAvgTime.Size = new Size(220, 22);

        // lblGamesPlayed
        lblGamesPlayed.Text = "Games Played: ";
        lblGamesPlayed.Location = new Point(40, 156);
        lblGamesPlayed.Size = new Size(220, 22);

        // lblScore
        lblScore.Text = "Score: ";
        lblScore.Font = new Font("Microsoft Sans Serif", 9F, FontStyle.Bold);
        lblScore.Location = new Point(40, 188);
        lblScore.Size = new Size(220, 22);

        // btnBack
        btnBack.Text = "Back";
        btnBack.Location = new Point(100, 228);
        btnBack.Size = new Size(90, 30);
        btnBack.Click += btnBack_Click;

        this.Controls.Add(lblTitle);
        this.Controls.Add(lblCorrectAnswers);
        this.Controls.Add(lblTotalAnswers);
        this.Controls.Add(lblAvgTime);
        this.Controls.Add(lblGamesPlayed);
        this.Controls.Add(lblScore);
        this.Controls.Add(btnBack);

        ResumeLayout(false);
    }

    private Label lblTitle;
    private Label lblCorrectAnswers;
    private Label lblTotalAnswers;
    private Label lblAvgTime;
    private Label lblGamesPlayed;
    private Label lblScore;
    private Button btnBack;
}
