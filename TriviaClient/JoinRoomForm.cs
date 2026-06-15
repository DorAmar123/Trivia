namespace TriviaClient;

public partial class JoinRoomForm : Form
{
    public JoinRoomForm()
    {
        InitializeComponent();

        // placeholder rooms
        lstRooms.Items.Add("Room 1 (2/4 players)");
        lstRooms.Items.Add("Room 2 (1/4 players)");
        lstRooms.Items.Add("Cool Room (3/4 players)");
    }

    private void btnJoin_Click(object sender, EventArgs e)
    {
        if (lstRooms.SelectedItem == null)
        {
            MessageBox.Show("Please select a room to join.", "Error");
            return;
        }

        string selectedRoom = lstRooms.SelectedItem.ToString()!;

        // TODO: send join room request via Communicator
        WaitingRoomForm waiting = new WaitingRoomForm(selectedRoom, "Admin");
        waiting.Show();
        this.Hide();
    }

    private void btnRefresh_Click(object sender, EventArgs e)
    {
        // TODO: fetch room list via Communicator
        MessageBox.Show("Refresh clicked! (not connected yet)", "Info");
    }

    private void btnBack_Click(object sender, EventArgs e)
    {
        this.Close();
    }
}
