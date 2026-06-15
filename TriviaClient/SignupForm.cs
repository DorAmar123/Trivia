namespace TriviaClient;

public partial class SignupForm : Form
{
    public SignupForm()
    {
        InitializeComponent();
    }

    private void btnSignup_Click(object sender, EventArgs e)
    {
        string username = txtUsername.Text.Trim();
        string password = txtPassword.Text;
        string confirm = txtConfirm.Text;
        string email = txtEmail.Text.Trim();

        if (username == "" || password == "" || email == "")
        {
            MessageBox.Show("Please fill in all fields.", "Error");
            return;
        }

        if (password != confirm)
        {
            MessageBox.Show("Passwords do not match.", "Error");
            return;
        }

        try
        {
            if (!AppState.Communicator.IsConnected())
                AppState.Communicator.Connect();

            ServerResponse response = AppState.Communicator.SendAndReceive(
                Serializer.SerializeSignup(username, password, email));

            if (response.Code == ResponseCode.Signup && Deserializer.GetStatus(response) == 1)
            {
                MessageBox.Show("Account created! Please log in.", "Success");
                LoginForm login = new LoginForm();
                login.Show();
                this.Close();
            }
            else
            {
                MessageBox.Show("Signup failed. Username may already exist.", "Error");
            }
        }
        catch (Exception ex)
        {
            MessageBox.Show("Could not connect to server:\n" + ex.Message, "Error");
        }
    }

    private void btnBack_Click(object sender, EventArgs e)
    {
        LoginForm login = new LoginForm();
        login.Show();
        this.Close();
    }
}
