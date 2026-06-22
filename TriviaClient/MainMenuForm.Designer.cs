namespace TriviaClient;

partial class MainMenuForm
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
        lblWelcome = new Label();
        btnCreateRoom = new Button();
        btnJoinRoom = new Button();
        btnStatistics = new Button();
        btnLogout = new Button();

        SuspendLayout();

        // Form
        this.Text = "Trivia - Main Menu";
        this.ClientSize = new Size(300, 295);
        this.StartPosition = FormStartPosition.CenterScreen;
        this.FormBorderStyle = FormBorderStyle.FixedSingle;
        this.MaximizeBox = false;

        // lblTitle
        lblTitle.Text = "Trivia Game";
        lblTitle.Font = new Font("Microsoft Sans Serif", 14F, FontStyle.Bold);
        lblTitle.Location = new Point(80, 20);
        lblTitle.Size = new Size(140, 28);
        lblTitle.TextAlign = ContentAlignment.MiddleCenter;

        // lblWelcome
        lblWelcome.Text = "Welcome!";
        lblWelcome.Font = new Font("Microsoft Sans Serif", 9.75F, FontStyle.Bold);
        lblWelcome.Location = new Point(10, 55);
        lblWelcome.Size = new Size(274, 22);
        lblWelcome.TextAlign = ContentAlignment.MiddleCenter;

        // btnCreateRoom
        btnCreateRoom.Text = "Create Room";
        btnCreateRoom.Location = new Point(85, 90);
        btnCreateRoom.Size = new Size(130, 35);
        btnCreateRoom.Click += btnCreateRoom_Click;

        // btnJoinRoom
        btnJoinRoom.Text = "Join Room";
        btnJoinRoom.Location = new Point(85, 140);
        btnJoinRoom.Size = new Size(130, 35);
        btnJoinRoom.Click += btnJoinRoom_Click;

        // btnStatistics
        btnStatistics.Text = "Statistics";
        btnStatistics.Location = new Point(85, 190);
        btnStatistics.Size = new Size(130, 35);
        btnStatistics.Click += btnStatistics_Click;

        // btnLogout
        btnLogout.Text = "Logout";
        btnLogout.Location = new Point(85, 240);
        btnLogout.Size = new Size(130, 35);
        btnLogout.Click += btnLogout_Click;

        this.Controls.Add(lblTitle);
        this.Controls.Add(lblWelcome);
        this.Controls.Add(btnCreateRoom);
        this.Controls.Add(btnJoinRoom);
        this.Controls.Add(btnStatistics);
        this.Controls.Add(btnLogout);

        ResumeLayout(false);
    }

    private Label lblTitle;
    private Label lblWelcome;
    private Button btnCreateRoom;
    private Button btnJoinRoom;
    private Button btnStatistics;
    private Button btnLogout;
}
