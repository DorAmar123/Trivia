namespace TriviaClient;

partial class HighScoresForm
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
        lblSubtitle = new Label();
        lblFirst = new Label();
        lblSecond = new Label();
        lblThird = new Label();
        btnBack = new Button();

        SuspendLayout();

        // Form
        this.Text = "Trivia - High Scores";
        this.Size = new Size(300, 270);
        this.StartPosition = FormStartPosition.CenterScreen;
        this.FormBorderStyle = FormBorderStyle.FixedSingle;
        this.MaximizeBox = false;

        // lblTitle
        lblTitle.Text = "High Scores";
        lblTitle.Font = new Font("Microsoft Sans Serif", 12F, FontStyle.Bold);
        lblTitle.Location = new Point(85, 15);
        lblTitle.Size = new Size(130, 25);
        lblTitle.TextAlign = ContentAlignment.MiddleCenter;

        // lblSubtitle
        lblSubtitle.Text = "Top 3 Players:";
        lblSubtitle.Location = new Point(95, 50);
        lblSubtitle.Size = new Size(110, 20);

        // lblFirst
        lblFirst.Text = "1. ";
        lblFirst.Font = new Font("Microsoft Sans Serif", 10F, FontStyle.Bold);
        lblFirst.Location = new Point(40, 85);
        lblFirst.Size = new Size(220, 25);

        // lblSecond
        lblSecond.Text = "2. ";
        lblSecond.Font = new Font("Microsoft Sans Serif", 10F);
        lblSecond.Location = new Point(40, 120);
        lblSecond.Size = new Size(220, 25);

        // lblThird
        lblThird.Text = "3. ";
        lblThird.Font = new Font("Microsoft Sans Serif", 10F);
        lblThird.Location = new Point(40, 155);
        lblThird.Size = new Size(220, 25);

        // btnBack
        btnBack.Text = "Back";
        btnBack.Location = new Point(100, 195);
        btnBack.Size = new Size(90, 30);
        btnBack.Click += btnBack_Click;

        this.Controls.Add(lblTitle);
        this.Controls.Add(lblSubtitle);
        this.Controls.Add(lblFirst);
        this.Controls.Add(lblSecond);
        this.Controls.Add(lblThird);
        this.Controls.Add(btnBack);

        ResumeLayout(false);
    }

    private Label lblTitle;
    private Label lblSubtitle;
    private Label lblFirst;
    private Label lblSecond;
    private Label lblThird;
    private Button btnBack;
}
