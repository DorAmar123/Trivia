namespace TriviaClient;

partial class StatisticsMenuForm
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
        btnPersonalStats = new Button();
        btnHighScores = new Button();
        btnBack = new Button();

        SuspendLayout();

        this.Text = "Trivia - Statistics";
        this.ClientSize = new Size(280, 216);
        this.StartPosition = FormStartPosition.CenterScreen;
        this.FormBorderStyle = FormBorderStyle.FixedSingle;
        this.MaximizeBox = false;

        lblTitle.Text = "Statistics";
        lblTitle.Font = new Font("Microsoft Sans Serif", 12F, FontStyle.Bold);
        lblTitle.Location = new Point(80, 20);
        lblTitle.Size = new Size(120, 25);
        lblTitle.TextAlign = ContentAlignment.MiddleCenter;

        btnPersonalStats.Text = "Personal Statistics";
        btnPersonalStats.Location = new Point(65, 70);
        btnPersonalStats.Size = new Size(150, 35);
        btnPersonalStats.Click += btnPersonalStats_Click;

        btnHighScores.Text = "High Scores";
        btnHighScores.Location = new Point(65, 120);
        btnHighScores.Size = new Size(150, 35);
        btnHighScores.Click += btnHighScores_Click;

        btnBack.Text = "Back";
        btnBack.Location = new Point(90, 168);
        btnBack.Size = new Size(100, 28);
        btnBack.Click += btnBack_Click;

        this.Controls.Add(lblTitle);
        this.Controls.Add(btnPersonalStats);
        this.Controls.Add(btnHighScores);
        this.Controls.Add(btnBack);

        ResumeLayout(false);
    }

    private Label lblTitle;
    private Button btnPersonalStats;
    private Button btnHighScores;
    private Button btnBack;
}
