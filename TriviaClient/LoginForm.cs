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

        // TODO: send login request via Communicator
        MessageBox.Show("Login clicked! (not connected yet)", "Info");
    }

    private void btnGoToSignup_Click(object sender, EventArgs e)
    {
        SignupForm signup = new SignupForm();
        signup.Show();
        this.Hide();
    }
}
