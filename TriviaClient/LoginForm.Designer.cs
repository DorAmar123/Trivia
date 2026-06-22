namespace TriviaClient;

partial class LoginForm
{
    private System.ComponentModel.IContainer components = null;

    protected override void Dispose(bool disposing)
    {
        if (disposing && (components != null))
            components.Dispose();
        base.Dispose(disposing);
    }

    private void InitializeComponent()
    {
        lblTitle = new Label();
        lblUsername = new Label();
        txtUsername = new TextBox();
        lblPassword = new Label();
        txtPassword = new TextBox();
        btnLogin = new Button();
        lblNoAccount = new Label();
        btnGoToSignup = new Button();

        SuspendLayout();

        // Form
        this.Text = "Trivia - Login";
        this.ClientSize = new Size(320, 232);
        this.StartPosition = FormStartPosition.CenterScreen;
        this.FormBorderStyle = FormBorderStyle.FixedSingle;
        this.MaximizeBox = false;

        // lblTitle
        lblTitle.Text = "Trivia Game - Login";
        lblTitle.Font = new Font("Microsoft Sans Serif", 12F, FontStyle.Bold);
        lblTitle.Location = new Point(60, 20);
        lblTitle.Size = new Size(200, 25);
        lblTitle.TextAlign = ContentAlignment.MiddleCenter;

        // lblUsername
        lblUsername.Text = "Username:";
        lblUsername.Location = new Point(40, 65);
        lblUsername.Size = new Size(80, 20);

        // txtUsername
        txtUsername.Location = new Point(130, 62);
        txtUsername.Size = new Size(140, 23);

        // lblPassword
        lblPassword.Text = "Password:";
        lblPassword.Location = new Point(40, 100);
        lblPassword.Size = new Size(80, 20);

        // txtPassword
        txtPassword.Location = new Point(130, 97);
        txtPassword.Size = new Size(140, 23);
        txtPassword.PasswordChar = '*';

        // btnLogin
        btnLogin.Text = "Login";
        btnLogin.Location = new Point(110, 140);
        btnLogin.Size = new Size(90, 30);
        btnLogin.Click += btnLogin_Click;

        // lblNoAccount
        lblNoAccount.Text = "Oh your new here?";
        lblNoAccount.Location = new Point(60, 190);
        lblNoAccount.Size = new Size(150, 20);

        // btnGoToSignup
        btnGoToSignup.Text = "Sign Up";
        btnGoToSignup.Location = new Point(210, 186);
        btnGoToSignup.Size = new Size(70, 26);
        btnGoToSignup.Click += btnGoToSignup_Click;

        this.Controls.Add(lblTitle);
        this.Controls.Add(lblUsername);
        this.Controls.Add(txtUsername);
        this.Controls.Add(lblPassword);
        this.Controls.Add(txtPassword);
        this.Controls.Add(btnLogin);
        this.Controls.Add(lblNoAccount);
        this.Controls.Add(btnGoToSignup);

        ResumeLayout(false);
    }

    private Label lblTitle;
    private Label lblUsername;
    private TextBox txtUsername;
    private Label lblPassword;
    private TextBox txtPassword;
    private Button btnLogin;
    private Label lblNoAccount;
    private Button btnGoToSignup;
}
