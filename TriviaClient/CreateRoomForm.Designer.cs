namespace TriviaClient;

partial class CreateRoomForm
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
        lblRoomName = new Label();
        txtRoomName = new TextBox();
        lblTimePerQuestion = new Label();
        txtTimePerQuestion = new TextBox();
        lblSeconds = new Label();
        lblQuestionCount = new Label();
        txtQuestionCount = new TextBox();
        lblMaxPlayers = new Label();
        txtMaxPlayers = new TextBox();
        btnCreate = new Button();
        btnBack = new Button();

        SuspendLayout();

        // Form
        this.Text = "Trivia - Create Room";
        this.ClientSize = new Size(310, 316);
        this.StartPosition = FormStartPosition.CenterScreen;
        this.FormBorderStyle = FormBorderStyle.FixedSingle;
        this.MaximizeBox = false;

        // lblTitle
        lblTitle.Text = "Create a Room";
        lblTitle.Font = new Font("Microsoft Sans Serif", 12F, FontStyle.Bold);
        lblTitle.Location = new Point(80, 15);
        lblTitle.Size = new Size(160, 25);
        lblTitle.TextAlign = ContentAlignment.MiddleCenter;

        // lblRoomName
        lblRoomName.Text = "Room Name:";
        lblRoomName.Location = new Point(30, 60);
        lblRoomName.Size = new Size(90, 20);

        // txtRoomName
        txtRoomName.Location = new Point(130, 57);
        txtRoomName.Size = new Size(150, 23);

        // lblTimePerQuestion
        lblTimePerQuestion.Text = "Time (sec):";
        lblTimePerQuestion.Location = new Point(30, 100);
        lblTimePerQuestion.Size = new Size(90, 20);

        // txtTimePerQuestion
        txtTimePerQuestion.Location = new Point(130, 97);
        txtTimePerQuestion.Size = new Size(80, 23);
        txtTimePerQuestion.Text = "30";

        // lblSeconds
        lblSeconds.Text = "seconds";
        lblSeconds.Location = new Point(218, 100);
        lblSeconds.Size = new Size(60, 20);

        // lblQuestionCount
        lblQuestionCount.Text = "Questions:";
        lblQuestionCount.Location = new Point(30, 140);
        lblQuestionCount.Size = new Size(90, 20);

        // txtQuestionCount
        txtQuestionCount.Location = new Point(130, 137);
        txtQuestionCount.Size = new Size(80, 23);
        txtQuestionCount.Text = "10";

        // lblMaxPlayers
        lblMaxPlayers.Text = "Max Players:";
        lblMaxPlayers.Location = new Point(30, 180);
        lblMaxPlayers.Size = new Size(90, 20);

        // txtMaxPlayers
        txtMaxPlayers.Location = new Point(130, 177);
        txtMaxPlayers.Size = new Size(80, 23);
        txtMaxPlayers.Text = "4";

        // btnCreate
        btnCreate.Text = "Create Room";
        btnCreate.Location = new Point(85, 225);
        btnCreate.Size = new Size(130, 35);
        btnCreate.Click += btnCreate_Click;

        // btnBack
        btnBack.Text = "Back";
        btnBack.Location = new Point(110, 270);
        btnBack.Size = new Size(80, 26);
        btnBack.Click += btnBack_Click;

        this.Controls.Add(lblTitle);
        this.Controls.Add(lblRoomName);
        this.Controls.Add(txtRoomName);
        this.Controls.Add(lblTimePerQuestion);
        this.Controls.Add(txtTimePerQuestion);
        this.Controls.Add(lblSeconds);
        this.Controls.Add(lblQuestionCount);
        this.Controls.Add(txtQuestionCount);
        this.Controls.Add(lblMaxPlayers);
        this.Controls.Add(txtMaxPlayers);
        this.Controls.Add(btnCreate);
        this.Controls.Add(btnBack);

        ResumeLayout(false);
    }

    private Label lblTitle;
    private Label lblRoomName;
    private TextBox txtRoomName;
    private Label lblTimePerQuestion;
    private TextBox txtTimePerQuestion;
    private Label lblSeconds;
    private Label lblQuestionCount;
    private TextBox txtQuestionCount;
    private Label lblMaxPlayers;
    private TextBox txtMaxPlayers;
    private Button btnCreate;
    private Button btnBack;
}
