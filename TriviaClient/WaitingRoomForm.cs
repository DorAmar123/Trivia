namespace TriviaClient;

public partial class WaitingRoomForm : Form
{
    private Thread _refreshThread;
    private bool _isWindowActive = true;
    private readonly bool _isAdmin;

    public WaitingRoomForm(string roomName, string adminName, bool isAdmin = false)
    {
        InitializeComponent();
        _isAdmin = isAdmin;
        lblRoomName.Text = "Room: " + roomName;
        lblAdmin.Text = "Admin: " + adminName;
        lstPlayers.Items.Add(adminName);

        if (_isAdmin)
        {
            btnStartGame.Visible = true;
            btnCloseRoom.Visible = true;
            lblWaiting.Text = "You are the admin. Start the game when ready.";
        }

        _refreshThread = new Thread(RefreshLoop);
        _refreshThread.IsBackground = true;
        _refreshThread.Start();
    }

    private void btnLeave_Click(object sender, EventArgs e)
    {
        _isWindowActive = false;

        try
        {
            AppState.Communicator.SendAndReceive(Serializer.SerializeLeaveRoom());
        }
        catch { }

        BackToMenu();
    }

    private void btnStartGame_Click(object sender, EventArgs e)
    {
        btnStartGame.Enabled = false;

        try
        {
            ServerResponse response = AppState.Communicator.SendAndReceive(Serializer.SerializeStartGame());

            if (Deserializer.GetStatus(response) == 1)
            {
                _isWindowActive = false;
                OpenGameScreen();
            }
            else
            {
                btnStartGame.Enabled = true;
                MessageBox.Show("Failed to start the game.", "Error");
            }
        }
        catch (Exception ex)
        {
            btnStartGame.Enabled = true;
            MessageBox.Show("Error: " + ex.Message, "Error");
        }
    }

    private void btnCloseRoom_Click(object sender, EventArgs e)
    {
        _isWindowActive = false;

        try
        {
            AppState.Communicator.SendAndReceive(Serializer.SerializeCloseRoom());
        }
        catch { }

        BackToMenu();
    }

    private void RefreshLoop()
    {
        while (_isWindowActive)
        {
            try
            {
                ServerResponse response = AppState.Communicator.SendAndReceive(Serializer.SerializeGetRoomState());

                if (Deserializer.GetStatus(response) == 0)
                {
                    _isWindowActive = false;
                    this.Invoke((MethodInvoker)delegate {
                        MessageBox.Show("The admin closed the room.", "Room Closed", MessageBoxButtons.OK, MessageBoxIcon.Information);
                        BackToMenu();
                    });
                    break;
                }

                if (Deserializer.GetHasGameBegun(response))
                {
                    _isWindowActive = false;
                    this.Invoke((MethodInvoker)delegate {
                        OpenGameScreen();
                    });
                    break;
                }

                List<string> currentPlayers = Deserializer.GetRoomStatePlayers(response);
                this.Invoke((MethodInvoker)delegate {
                    UpdatePlayersList(currentPlayers);
                });
            }
            catch (Exception ex)
            {
                Console.WriteLine("Refresh error: " + ex.Message);
            }

            Thread.Sleep(3000);
        }
    }

    private void UpdatePlayersList(List<string> players)
    {
        lstPlayers.Items.Clear();
        foreach (var player in players)
        {
            lstPlayers.Items.Add(player);
        }
    }

    private void OpenGameScreen()
    {
        GameForm game = new GameForm();
        game.Show();
        this.Close();
    }

    private void BackToMenu()
    {
        MainMenuForm menu = new MainMenuForm();
        menu.Show();
        this.Close();
    }

    protected override void OnFormClosing(FormClosingEventArgs e)
    {
        _isWindowActive = false;
        base.OnFormClosing(e);
    }
}
