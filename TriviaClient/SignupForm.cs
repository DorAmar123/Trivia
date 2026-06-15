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

        MessageBox.Show("Signup clicked! (not connected yet)", "Info");
    }

    private void btnBack_Click(object sender, EventArgs e)
    {
        LoginForm login = new LoginForm();
        login.Show();
        this.Close();
    }
}
