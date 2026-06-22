namespace TriviaClient;

partial class GameForm
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
        lblTimer = new Label();
        lblScore = new Label();
        lblQuestion = new Label();
        btnAnswer1 = new Button();
        btnAnswer2 = new Button();
        btnAnswer3 = new Button();
        btnAnswer4 = new Button();
        btnLeaveGame = new Button();

        SuspendLayout();

        // Form
        this.Text = "Trivia - Game";
        this.ClientSize = new Size(485, 372);
        this.StartPosition = FormStartPosition.CenterScreen;
        this.FormBorderStyle = FormBorderStyle.FixedSingle;
        this.MaximizeBox = false;

        // lblTitle
        lblTitle.Text = "Trivia Game";
        lblTitle.Font = new Font("Microsoft Sans Serif", 13F, FontStyle.Bold);
        lblTitle.Location = new Point(195, 12);
        lblTitle.Size = new Size(150, 26);
        lblTitle.TextAlign = ContentAlignment.MiddleCenter;

        // lblTimer
        lblTimer.Text = "Time: 20s";
        lblTimer.Font = new Font("Microsoft Sans Serif", 10F, FontStyle.Bold);
        lblTimer.ForeColor = Color.DarkRed;
        lblTimer.Location = new Point(360, 15);
        lblTimer.Size = new Size(110, 22);
        lblTimer.TextAlign = ContentAlignment.MiddleRight;

        // lblScore
        lblScore.Text = "Correct: 0   Wrong: 0";
        lblScore.Font = new Font("Microsoft Sans Serif", 9F);
        lblScore.Location = new Point(15, 15);
        lblScore.Size = new Size(170, 22);
        lblScore.TextAlign = ContentAlignment.MiddleLeft;

        // lblQuestion
        lblQuestion.Text = "Loading question...";
        lblQuestion.Font = new Font("Microsoft Sans Serif", 10F);
        lblQuestion.Location = new Point(20, 55);
        lblQuestion.Size = new Size(450, 80);
        lblQuestion.TextAlign = ContentAlignment.MiddleCenter;
        lblQuestion.BorderStyle = BorderStyle.FixedSingle;
        lblQuestion.BackColor = Color.AliceBlue;

        // btnAnswer1
        btnAnswer1.Text = "Answer 1";
        btnAnswer1.Font = new Font("Microsoft Sans Serif", 9F);
        btnAnswer1.Location = new Point(20, 155);
        btnAnswer1.Size = new Size(215, 60);
        btnAnswer1.Click += btnAnswer1_Click;

        // btnAnswer2
        btnAnswer2.Text = "Answer 2";
        btnAnswer2.Font = new Font("Microsoft Sans Serif", 9F);
        btnAnswer2.Location = new Point(255, 155);
        btnAnswer2.Size = new Size(215, 60);
        btnAnswer2.Click += btnAnswer2_Click;

        // btnAnswer3
        btnAnswer3.Text = "Answer 3";
        btnAnswer3.Font = new Font("Microsoft Sans Serif", 9F);
        btnAnswer3.Location = new Point(20, 230);
        btnAnswer3.Size = new Size(215, 60);
        btnAnswer3.Click += btnAnswer3_Click;

        // btnAnswer4
        btnAnswer4.Text = "Answer 4";
        btnAnswer4.Font = new Font("Microsoft Sans Serif", 9F);
        btnAnswer4.Location = new Point(255, 230);
        btnAnswer4.Size = new Size(215, 60);
        btnAnswer4.Click += btnAnswer4_Click;

        // btnLeaveGame
        btnLeaveGame.Text = "Leave Game";
        btnLeaveGame.Font = new Font("Microsoft Sans Serif", 9F);
        btnLeaveGame.Location = new Point(175, 320);
        btnLeaveGame.Size = new Size(140, 32);
        btnLeaveGame.BackColor = Color.LightCoral;
        btnLeaveGame.Click += btnLeaveGame_Click;

        this.Controls.Add(lblTitle);
        this.Controls.Add(lblTimer);
        this.Controls.Add(lblScore);
        this.Controls.Add(lblQuestion);
        this.Controls.Add(btnAnswer1);
        this.Controls.Add(btnAnswer2);
        this.Controls.Add(btnAnswer3);
        this.Controls.Add(btnAnswer4);
        this.Controls.Add(btnLeaveGame);

        ResumeLayout(false);
    }

    private Label lblTitle;
    private Label lblTimer;
    private Label lblScore;
    private Label lblQuestion;
    private Button btnAnswer1;
    private Button btnAnswer2;
    private Button btnAnswer3;
    private Button btnAnswer4;
    private Button btnLeaveGame;
}
