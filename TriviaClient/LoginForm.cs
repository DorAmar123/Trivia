namespace TriviaClient;

public partial class LoginForm : Form
{
    public LoginForm()
    {
        InitializeComponent();
    }

    private void btnLogin_Click(object sender, EventArgs e)
    {
        string username = txtUsername.Text.Trim();
        string password = txtPassword.Text;

        if (username == "" || password == "")
        {
            MessageBox.Show("Please fill in all fields.", "Error");
            return;
        }

        try
        {
            if (!AppState.Communicator.IsConnected())
                AppState.Communicator.Connect();

            ServerResponse response = AppState.Communicator.SendAndReceive(
                Serializer.SerializeLogin(username, password));

            if (response.Code == ResponseCode.Login && Deserializer.GetStatus(response) == 1)
            {
                AppState.CurrentUsername = username;
                MainMenuForm menu = new MainMenuForm();
                menu.Show();
                this.Hide();
            }
            else
            {
                MessageBox.Show("Login failed. Wrong username or password.", "Error");
            }
        }
        catch (Exception ex)
        {
            MessageBox.Show("Could not connect to server:\n" + ex.Message, "Error");
        }
    }

    private void btnGoToSignup_Click(object sender, EventArgs e)
    {
        SignupForm signup = new SignupForm();
        signup.Show();
        this.Hide();
    }
}
