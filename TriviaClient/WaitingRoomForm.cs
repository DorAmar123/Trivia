namespace TriviaClient;

public partial class WaitingRoomForm : Form
{
    private Thread _refreshThread;
    private bool _isWindowActive = true;
    public WaitingRoomForm(string roomName, string adminName)
    {
        InitializeComponent();
        lblRoomName.Text = "Room: " + roomName;
        lblAdmin.Text = "Admin: " + adminName;
        lstPlayers.Items.Add(adminName);

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
                        MessageBox.Show("Game is starting!", "Start Game");
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
