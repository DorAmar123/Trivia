namespace TriviaClient;

public partial class JoinRoomForm : Form
{
    private List<RoomInfo> m_rooms = new List<RoomInfo>();

    private Thread _refreshThread;
    private bool _isWindowActive = true;
    private readonly MainMenuForm? _parentMenu;
    public JoinRoomForm(MainMenuForm? parentMenu = null)
    {
        InitializeComponent();
        _parentMenu = parentMenu;
        LoadRooms();

        _refreshThread = new Thread(RefreshLoop);
        _refreshThread.IsBackground = true;
        _refreshThread.Start();
    }

    private void RefreshLoop()
    {
        while (_isWindowActive)
        {
            Thread.Sleep(3000);

            if (!_isWindowActive) break;

            try
            {
                this.Invoke((MethodInvoker)delegate {
                    LoadRooms();
                });
            }
            catch (Exception ex)
            {
                Console.WriteLine("Rooms refresh error: " + ex.Message);
            }
        }
    }
    private void LoadRooms()
    {
        int previouslySelectedId = -1;
        if (lstRooms.SelectedIndex >= 0 && lstRooms.SelectedIndex < m_rooms.Count)
            previouslySelectedId = m_rooms[lstRooms.SelectedIndex].Id;

        try
        {
            ServerResponse response = AppState.Communicator.SendAndReceive(
                Serializer.SerializeGetRooms());

            List<RoomInfo> newRooms = Deserializer.GetRooms(response);

            lstRooms.BeginUpdate();
            lstRooms.Items.Clear();
            m_rooms = newRooms;
            foreach (var room in m_rooms)
                lstRooms.Items.Add($"{room.Name} (max {room.MaxPlayers} players)");

            if (previouslySelectedId != -1)
            {
                int idx = m_rooms.FindIndex(r => r.Id == previouslySelectedId);
                if (idx >= 0)
                    lstRooms.SelectedIndex = idx;
            }
            lstRooms.EndUpdate();
        }
        catch (Exception ex)
        {
            Console.WriteLine("Could not load rooms: " + ex.Message);
        }
    }

    private void btnJoin_Click(object sender, EventArgs e)
    {
        if (lstRooms.SelectedIndex < 0)
        {
            MessageBox.Show("Please select a room to join.", "Error");
            return;
        }

        RoomInfo selected = m_rooms[lstRooms.SelectedIndex];

        try
        {
            ServerResponse response = AppState.Communicator.SendAndReceive(
                Serializer.SerializeJoinRoom(selected.Id));

            if (response.Code == ResponseCode.JoinRoom && Deserializer.GetStatus(response) == 1)
            {
                _isWindowActive = false;

                WaitingRoomForm waiting = new WaitingRoomForm(selected.Name, "", isAdmin: false);
                waiting.Show();
                _parentMenu?.Close();
                this.Close();
            }
            else
            {
                MessageBox.Show("Failed to join room.", "Error");
            }
        }
        catch (Exception ex)
        {
            MessageBox.Show("Error: " + ex.Message, "Error");
        }
    }

    private void btnRefresh_Click(object sender, EventArgs e)
    {
        LoadRooms();
    }

    private void btnBack_Click(object sender, EventArgs e)
    {
        _isWindowActive = false;
        if (_parentMenu != null && !_parentMenu.IsDisposed)
            _parentMenu.Show();
        else
            new MainMenuForm().Show();
        this.Close();
    }
    protected override void OnFormClosing(FormClosingEventArgs e)
    {
        _isWindowActive = false;
        base.OnFormClosing(e);
    }
}
