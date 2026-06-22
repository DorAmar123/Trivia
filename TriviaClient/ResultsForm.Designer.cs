namespace TriviaClient;

partial class ResultsForm
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
        lblWinner = new Label();
        lvResults = new ListView();
        colName = new ColumnHeader();
        colCorrect = new ColumnHeader();
        colWrong = new ColumnHeader();
        colAvgTime = new ColumnHeader();
        btnBackToMenu = new Button();

        SuspendLayout();

        // Form
        this.Text = "Trivia - Game Results";
        this.ClientSize = new Size(490, 372);
        this.StartPosition = FormStartPosition.CenterScreen;
        this.FormBorderStyle = FormBorderStyle.FixedSingle;
        this.MaximizeBox = false;

        // lblTitle
        lblTitle.Text = "Game Results";
        lblTitle.Font = new Font("Microsoft Sans Serif", 13F, FontStyle.Bold);
        lblTitle.Location = new Point(150, 12);
        lblTitle.Size = new Size(200, 26);
        lblTitle.TextAlign = ContentAlignment.MiddleCenter;

        // lblWinner
        lblWinner.Text = "Winner: ";
        lblWinner.Font = new Font("Microsoft Sans Serif", 11F, FontStyle.Bold);
        lblWinner.ForeColor = Color.DarkGoldenrod;
        lblWinner.Location = new Point(20, 48);
        lblWinner.Size = new Size(450, 24);
        lblWinner.TextAlign = ContentAlignment.MiddleCenter;

        // lvResults
        colName.Text = "Player";
        colName.Width = 140;
        colCorrect.Text = "Correct";
        colCorrect.Width = 80;
        colWrong.Text = "Wrong";
        colWrong.Width = 80;
        colAvgTime.Text = "Avg Time";
        colAvgTime.Width = 90;

        lvResults.Columns.AddRange(new ColumnHeader[] { colName, colCorrect, colWrong, colAvgTime });
        lvResults.FullRowSelect = true;
        lvResults.GridLines = true;
        lvResults.View = View.Details;
        lvResults.Location = new Point(20, 82);
        lvResults.Size = new Size(450, 220);

        // btnBackToMenu
        btnBackToMenu.Text = "Back to Menu";
        btnBackToMenu.Font = new Font("Microsoft Sans Serif", 9F);
        btnBackToMenu.Location = new Point(170, 320);
        btnBackToMenu.Size = new Size(140, 32);
        btnBackToMenu.Click += btnBackToMenu_Click;

        this.Controls.Add(lblTitle);
        this.Controls.Add(lblWinner);
        this.Controls.Add(lvResults);
        this.Controls.Add(btnBackToMenu);

        ResumeLayout(false);
    }

    private Label lblTitle;
    private Label lblWinner;
    private ListView lvResults;
    private ColumnHeader colName;
    private ColumnHeader colCorrect;
    private ColumnHeader colWrong;
    private ColumnHeader colAvgTime;
    private Button btnBackToMenu;
}
