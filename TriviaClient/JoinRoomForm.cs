namespace TriviaClient;

public partial class JoinRoomForm : Form
{
    private List<RoomInfo> m_rooms = new List<RoomInfo>();

    public JoinRoomForm()
    {
        InitializeComponent();
        LoadRooms();
    }

    private void LoadRooms()
    {
        lstRooms.Items.Clear();
        m_rooms.Clear();

        try
        {
            ServerResponse response = AppState.Communicator.SendAndReceive(
                Serializer.SerializeGetRooms());

            m_rooms = Deserializer.GetRooms(response);
            foreach (var room in m_rooms)
                lstRooms.Items.Add($"{room.Name} (max {room.MaxPlayers} players)");
        }
        catch (Exception ex)
        {
            MessageBox.Show("Could not load rooms:\n" + ex.Message, "Error");
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
                WaitingRoomForm waiting = new WaitingRoomForm(selected.Name, "Admin");
                waiting.Show();
                this.Hide();
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
        this.Close();
    }
}
