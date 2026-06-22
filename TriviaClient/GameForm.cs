namespace TriviaClient;

public partial class GameForm : Form
{
    private int _correctAnswers = 0;
    private int _totalQuestions = 0;
    private int _timeLeft = 0;
    private System.Windows.Forms.Timer _timer = new();
    private bool _answerSubmitted = false;

    public GameForm()
    {
        InitializeComponent();
        _timer.Interval = 1000;
        _timer.Tick += Timer_Tick;
    }

    protected override void OnLoad(EventArgs e)
    {
        base.OnLoad(e);
        FetchAndDisplayQuestion();
    }

    private void FetchAndDisplayQuestion()
    {
        _timer.Stop();
        _answerSubmitted = false;
        SetAnswerButtonsEnabled(false);

        try
        {
            ServerResponse response = AppState.Communicator.SendAndReceive(Serializer.SerializeGetQuestion());

            if (Deserializer.GetStatus(response) == 0)
            {
                OpenResultsScreen();
                return;
            }

            QuestionData qd = Deserializer.GetQuestion(response);

            if (qd.Question == "" || qd.Answers.Count == 0)
            {
                OpenResultsScreen();
                return;
            }

            _totalQuestions++;
            lblQuestion.Text = qd.Question;

            var sortedAnswers = qd.Answers.OrderBy(kv => kv.Key).ToList();
            Button[] btns = { btnAnswer1, btnAnswer2, btnAnswer3, btnAnswer4 };

            for (int i = 0; i < btns.Length; i++)
            {
                if (i < sortedAnswers.Count)
                {
                    btns[i].Text = sortedAnswers[i].Value;
                    btns[i].Tag = sortedAnswers[i].Key;
                    btns[i].Visible = true;
                    btns[i].BackColor = SystemColors.Control;
                }
                else
                {
                    btns[i].Visible = false;
                }
            }

            UpdateScoreLabel();
            SetAnswerButtonsEnabled(true);

            _timeLeft = AppState.AnswerTimeoutSeconds;
            lblTimer.Text = $"Time: {_timeLeft}s";
            _timer.Start();
        }
        catch (Exception ex)
        {
            MessageBox.Show("Error fetching question: " + ex.Message, "Error");
        }
    }

    private void Timer_Tick(object? sender, EventArgs e)
    {
        _timeLeft--;
        lblTimer.Text = $"Time: {_timeLeft}s";

        if (_timeLeft <= 0)
        {
            _timer.Stop();
            if (!_answerSubmitted)
            {
                SubmitAnswer(0);
            }
        }
    }

    private void btnAnswer1_Click(object sender, EventArgs e) => HandleAnswerClick(btnAnswer1);
    private void btnAnswer2_Click(object sender, EventArgs e) => HandleAnswerClick(btnAnswer2);
    private void btnAnswer3_Click(object sender, EventArgs e) => HandleAnswerClick(btnAnswer3);
    private void btnAnswer4_Click(object sender, EventArgs e) => HandleAnswerClick(btnAnswer4);

    private void HandleAnswerClick(Button btn)
    {
        if (_answerSubmitted) return;
        _timer.Stop();
        int answerId = btn.Tag is int id ? id : 0;
        SubmitAnswer(answerId);
    }

    private void SubmitAnswer(int answerId)
    {
        _answerSubmitted = true;
        SetAnswerButtonsEnabled(false);

        try
        {
            ServerResponse response = AppState.Communicator.SendAndReceive(Serializer.SerializeSubmitAnswer(answerId));
            if (Deserializer.GetStatus(response) == 1)
            {
                _correctAnswers++;
                UpdateScoreLabel();
            }
        }
        catch (Exception ex)
        {
            MessageBox.Show("Error submitting answer: " + ex.Message, "Error");
        }

        Task.Delay(800).ContinueWith(_ =>
        {
            this.Invoke((MethodInvoker)FetchAndDisplayQuestion);
        });
    }

    private void UpdateScoreLabel()
    {
        lblScore.Text = $"Correct: {_correctAnswers} / {_totalQuestions}";
    }

    private void SetAnswerButtonsEnabled(bool enabled)
    {
        btnAnswer1.Enabled = enabled;
        btnAnswer2.Enabled = enabled;
        btnAnswer3.Enabled = enabled;
        btnAnswer4.Enabled = enabled;
    }

    private void OpenResultsScreen()
    {
        try
        {
            ServerResponse response = AppState.Communicator.SendAndReceive(Serializer.SerializeGetGameResults());
            List<PlayerResult> results = Deserializer.GetGameResults(response);
            ResultsForm resultsForm = new ResultsForm(results);
            resultsForm.Show();
        }
        catch
        {
            ResultsForm resultsForm = new ResultsForm(new List<PlayerResult>());
            resultsForm.Show();
        }
        this.Close();
    }

    private void btnLeaveGame_Click(object sender, EventArgs e)
    {
        _timer.Stop();
        try
        {
            AppState.Communicator.SendAndReceive(Serializer.SerializeLeaveGame());
        }
        catch { }

        MainMenuForm menu = new MainMenuForm();
        menu.Show();
        this.Close();
    }

    protected override void OnFormClosing(FormClosingEventArgs e)
    {
        _timer.Stop();
        base.OnFormClosing(e);
    }
}
