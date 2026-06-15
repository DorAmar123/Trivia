namespace TriviaClient;

public partial class WaitingRoomForm : Form
{
    public WaitingRoomForm(string roomName, string adminName)
    {
        InitializeComponent();
        lblRoomName.Text = "Room: " + roomName;
        lblAdmin.Text = "Admin: " + adminName;
        lstPlayers.Items.Add(adminName);
    }

    private void btnLeave_Click(object sender, EventArgs e)
    {
        try
        {
            AppState.Communicator.SendAndReceive(Serializer.SerializeLogout());
        }
        catch { }

        MainMenuForm menu = new MainMenuForm();
        menu.Show();
        this.Close();
    }
}
