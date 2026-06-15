namespace TriviaClient;

partial class JoinRoomForm
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
        lblAvailable = new Label();
        lstRooms = new ListBox();
        btnJoin = new Button();
        btnRefresh = new Button();
        btnBack = new Button();

        SuspendLayout();

        // Form
        this.Text = "Trivia - Join Room";
        this.Size = new Size(300, 340);
        this.StartPosition = FormStartPosition.CenterScreen;
        this.FormBorderStyle = FormBorderStyle.FixedSingle;
        this.MaximizeBox = false;

        // lblTitle
        lblTitle.Text = "Join a Room";
        lblTitle.Font = new Font("Microsoft Sans Serif", 12F, FontStyle.Bold);
        lblTitle.Location = new Point(85, 15);
        lblTitle.Size = new Size(130, 25);
        lblTitle.TextAlign = ContentAlignment.MiddleCenter;

        // lblAvailable
        lblAvailable.Text = "Available rooms:";
        lblAvailable.Location = new Point(20, 55);
        lblAvailable.Size = new Size(120, 20);

        // lstRooms
        lstRooms.Location = new Point(20, 78);
        lstRooms.Size = new Size(250, 130);

        // btnJoin
        btnJoin.Text = "Join Room";
        btnJoin.Location = new Point(85, 225);
        btnJoin.Size = new Size(120, 32);
        btnJoin.Click += btnJoin_Click;

        // btnRefresh
        btnRefresh.Text = "Refresh";
        btnRefresh.Location = new Point(85, 267);
        btnRefresh.Size = new Size(120, 28);
        btnRefresh.Click += btnRefresh_Click;

        // btnBack
        btnBack.Text = "Back";
        btnBack.Location = new Point(105, 302);
        btnBack.Size = new Size(80, 26);
        btnBack.Click += btnBack_Click;

        this.Controls.Add(lblTitle);
        this.Controls.Add(lblAvailable);
        this.Controls.Add(lstRooms);
        this.Controls.Add(btnJoin);
        this.Controls.Add(btnRefresh);
        this.Controls.Add(btnBack);

        ResumeLayout(false);
    }

    private Label lblTitle;
    private Label lblAvailable;
    private ListBox lstRooms;
    private Button btnJoin;
    private Button btnRefresh;
    private Button btnBack;
}
