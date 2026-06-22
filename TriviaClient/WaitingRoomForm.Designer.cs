namespace TriviaClient;

partial class WaitingRoomForm
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
        lblAdmin = new Label();
        lblPlayersHeader = new Label();
        lstPlayers = new ListBox();
        lblWaiting = new Label();
        btnLeave = new Button();
        btnStartGame = new Button();
        btnCloseRoom = new Button();

        SuspendLayout();

        // Form
        this.Text = "Trivia - Waiting Room";
        this.Size = new Size(300, 400);
        this.StartPosition = FormStartPosition.CenterScreen;
        this.FormBorderStyle = FormBorderStyle.FixedSingle;
        this.MaximizeBox = false;

        // lblTitle
        lblTitle.Text = "Waiting Room";
        lblTitle.Font = new Font("Microsoft Sans Serif", 12F, FontStyle.Bold);
        lblTitle.Location = new Point(80, 15);
        lblTitle.Size = new Size(140, 25);
        lblTitle.TextAlign = ContentAlignment.MiddleCenter;

        // lblRoomName
        lblRoomName.Text = "Room: ";
        lblRoomName.Font = new Font("Microsoft Sans Serif", 9F, FontStyle.Bold);
        lblRoomName.Location = new Point(20, 55);
        lblRoomName.Size = new Size(260, 20);

        // lblAdmin
        lblAdmin.Text = "Admin: ";
        lblAdmin.Location = new Point(20, 80);
        lblAdmin.Size = new Size(260, 20);

        // lblPlayersHeader
        lblPlayersHeader.Text = "Players in room:";
        lblPlayersHeader.Location = new Point(20, 115);
        lblPlayersHeader.Size = new Size(120, 20);

        // lstPlayers
        lstPlayers.Location = new Point(20, 138);
        lstPlayers.Size = new Size(250, 120);

        // lblWaiting
        lblWaiting.Text = "Waiting for the game to start...";
        lblWaiting.Location = new Point(30, 270);
        lblWaiting.Size = new Size(240, 20);
        lblWaiting.TextAlign = ContentAlignment.MiddleCenter;

        // btnLeave
        btnLeave.Text = "Leave Room";
        btnLeave.Location = new Point(85, 295);
        btnLeave.Size = new Size(120, 30);
        btnLeave.Click += btnLeave_Click;

        // btnStartGame (admin only)
        btnStartGame.Text = "Start Game";
        btnStartGame.Font = new Font("Microsoft Sans Serif", 9F, FontStyle.Bold);
        btnStartGame.BackColor = Color.LightGreen;
        btnStartGame.Location = new Point(20, 335);
        btnStartGame.Size = new Size(120, 30);
        btnStartGame.Visible = false;
        btnStartGame.Click += btnStartGame_Click;

        // btnCloseRoom (admin only)
        btnCloseRoom.Text = "Close Room";
        btnCloseRoom.BackColor = Color.LightCoral;
        btnCloseRoom.Location = new Point(155, 335);
        btnCloseRoom.Size = new Size(115, 30);
        btnCloseRoom.Visible = false;
        btnCloseRoom.Click += btnCloseRoom_Click;

        this.Controls.Add(lblTitle);
        this.Controls.Add(lblRoomName);
        this.Controls.Add(lblAdmin);
        this.Controls.Add(lblPlayersHeader);
        this.Controls.Add(lstPlayers);
        this.Controls.Add(lblWaiting);
        this.Controls.Add(btnLeave);
        this.Controls.Add(btnStartGame);
        this.Controls.Add(btnCloseRoom);

        ResumeLayout(false);
    }

    private Label lblTitle;
    private Label lblRoomName;
    private Label lblAdmin;
    private Label lblPlayersHeader;
    private ListBox lstPlayers;
    private Label lblWaiting;
    private Button btnLeave;
    private Button btnStartGame;
    private Button btnCloseRoom;
}
