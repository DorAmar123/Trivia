namespace TriviaClient;

partial class SignupForm
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
        lblConfirm = new Label();
        txtConfirm = new TextBox();
        lblEmail = new Label();
        txtEmail = new TextBox();
        btnSignup = new Button();
        btnBack = new Button();

        SuspendLayout();

        // Form
        this.Text = "Trivia - Sign Up";
        this.ClientSize = new Size(310, 308);
        this.StartPosition = FormStartPosition.CenterScreen;
        this.FormBorderStyle = FormBorderStyle.FixedSingle;
        this.MaximizeBox = false;

        // lblTitle
        lblTitle.Text = "Trivia Game - Sign Up";
        lblTitle.Font = new Font("Microsoft Sans Serif", 12F, FontStyle.Bold);
        lblTitle.Location = new Point(55, 20);
        lblTitle.Size = new Size(210, 25);
        lblTitle.TextAlign = ContentAlignment.MiddleCenter;

        // lblUsername
        lblUsername.Text = "Username:";
        lblUsername.Location = new Point(30, 65);
        lblUsername.Size = new Size(90, 20);

        // txtUsername
        txtUsername.Location = new Point(130, 62);
        txtUsername.Size = new Size(150, 23);

        // lblPassword
        lblPassword.Text = "Password:";
        lblPassword.Location = new Point(30, 100);
        lblPassword.Size = new Size(90, 20);

        // txtPassword
        txtPassword.Location = new Point(130, 97);
        txtPassword.Size = new Size(150, 23);
        txtPassword.PasswordChar = '*';

        // lblConfirm
        lblConfirm.Text = "Confirm:";
        lblConfirm.Location = new Point(30, 135);
        lblConfirm.Size = new Size(90, 20);

        // txtConfirm
        txtConfirm.Location = new Point(130, 132);
        txtConfirm.Size = new Size(150, 23);
        txtConfirm.PasswordChar = '*';

        // lblEmail
        lblEmail.Text = "Email:";
        lblEmail.Location = new Point(30, 170);
        lblEmail.Size = new Size(90, 20);

        // txtEmail
        txtEmail.Location = new Point(130, 167);
        txtEmail.Size = new Size(150, 23);

        // btnSignup
        btnSignup.Text = "Sign Up";
        btnSignup.Location = new Point(100, 215);
        btnSignup.Size = new Size(90, 30);
        btnSignup.Click += btnSignup_Click;

        // btnBack
        btnBack.Text = "Back to Login";
        btnBack.Location = new Point(75, 260);
        btnBack.Size = new Size(160, 28);
        btnBack.Click += btnBack_Click;

        this.Controls.Add(lblTitle);
        this.Controls.Add(lblUsername);
        this.Controls.Add(txtUsername);
        this.Controls.Add(lblPassword);
        this.Controls.Add(txtPassword);
        this.Controls.Add(lblConfirm);
        this.Controls.Add(txtConfirm);
        this.Controls.Add(lblEmail);
        this.Controls.Add(txtEmail);
        this.Controls.Add(btnSignup);
        this.Controls.Add(btnBack);

        ResumeLayout(false);
    }

    private Label lblTitle;
    private Label lblUsername;
    private TextBox txtUsername;
    private Label lblPassword;
    private TextBox txtPassword;
    private Label lblConfirm;
    private TextBox txtConfirm;
    private Label lblEmail;
    private TextBox txtEmail;
    private Button btnSignup;
    private Button btnBack;
}
