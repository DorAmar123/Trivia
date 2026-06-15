namespace TriviaClient;

public partial class WaitingRoomForm : Form
{
    public WaitingRoomForm(string roomName, string adminName)
    {
        InitializeComponent();
        lblRoomName.Text = "Room: " + roomName;
        lblAdmin.Text = "Admin: " + adminName;

        // placeholder players list
        lstPlayers.Items.Add(adminName);
    }

    private void btnLeave_Click(object sender, EventArgs e)
    {
        // TODO: send leave room request via Communicator
        MainMenuForm menu = new MainMenuForm();
        menu.Show();
        this.Close();
    }
}
