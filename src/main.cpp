#include <bits/stdc++.h>
//#pragma GCC optimization("Ofast")

using namespace std;

int const N = 7005, M = 70005;
int const ExecTime = 950;

int Color[N];
vector<int> G[N];
int Weight[N][N];
int TotalWeight, TotalScore;

std::chrono::steady_clock::time_point ExecTimePoint;


bool const DEBUG = false, TEST = false, ADVANCED = false;
bool const UseEdgesSolution = true, UseRandSolution = false;


int n, m;
int ans[N];

void CheckTime();

long double F(int x)
{
    return log2(x + 1000);
}

int CalcScore(int WConf)
{
    return (int)(log2(n)) * 100000 * (F(TotalWeight) - F(WConf)) / (F(TotalWeight) - F(0));
}

int CalcScore(int *Colors)
{
    int WConf = 0;
    for (int i = 0; i < n; i++)
    {
        for (int v : G[i])
        {
            if (Colors[i] == Colors[v])
            {
                WConf += Weight[i][v];
            }
        }
    }
    WConf /= 2;

    int score = CalcScore(WConf);

    if (DEBUG)
    {
        cout << "Total Weight = " << TotalWeight << ", Conflict Weight = " << WConf << endl;
        cout << "Result Score = " << score << endl;
    }

    return score;
}

void Input()
{
    cin >> n >> m;
    for (int i = 0; i < m; i++)
    {
        int v1, v2, w;
        cin >> v1 >> v2 >> w;
        G[--v1].push_back(--v2);
        G[v2].push_back(v1);
        Weight[v1][v2] = w;
        Weight[v2][v1] = w;
        TotalWeight += w;
    }
}

void Output()
{
    for (int i = 0; i < n; i++)
    {
        cout << ans[i] << ' ';
    }
    cout << '\n';
    if (DEBUG)
    {
        cout << "ANSWER RESULT: " << endl;
        CalcScore(ans);
    }
    exit(0);
}

void CheckTime() // ends the program if time runs out
{
    if (std::chrono::steady_clock::now() > ExecTimePoint)
    {
        Output();
        exit(0);
    }
}

bool TrySetAns(int score, int *Colors = Color)
{
    if (score > TotalScore)
    {
        TotalScore = score;
        for (int i = 0; i < n; i++)
        {
            ans[i] = Colors[i];
        }
        return true;
    }

    return false;
}

void SetRColor(int v1, int v2)
{
    if (!Color[v1])
    {
        Color[v1] = rand() % 3 + 1;
    }

    if (!Color[v2])
    {
        Color[v2] = (Color[v1] + rand() % 2) % 3 + 1;
    }

    if (DEBUG && ADVANCED)
    {
        cout << "COLOR SET: " << endl;
        cout << v1 << ' ' << v2 << ' ' << Color[v1] << ' ' << Color[v2] << endl;
    }
}

int SetPrColor(int v1, int v2)
{
    int CurColorWeight[2][4] = {};
    for (int v : G[v1])
    {
        if (v != v2)
        {
            CurColorWeight[0][Color[v]] += Weight[v1][v];
        }
    }
    for (int v : G[v2])
    {
        if (v != v1)
        {
            CurColorWeight[1][Color[v]] += Weight[v2][v];
        }
    }

    int cv1, cv2, mn = 1e9;

    for (int i = 1; i < 4; i++)
    {
        for (int j = 1; j < 4; j++)
        {
            if ((i != j && CurColorWeight[0][i] + CurColorWeight[1][j] < mn) || (i == j && CurColorWeight[0][i] + CurColorWeight[1][j] + Weight[v1][v2] < mn))
            {
                cv1 = i;
                cv2 = j;
                mn = (i == j)? (CurColorWeight[0][i] + CurColorWeight[1][j] + Weight[v1][v2]) : (CurColorWeight[0][i] + CurColorWeight[1][j]);
            }
        }
    }

    CurColorWeight[0][0] = 0;
    CurColorWeight[1][0] = 0;
    int cur = (Color[v1] == Color[v2] && Color[v1] != 0)? (CurColorWeight[0][Color[v1]] + CurColorWeight[1][Color[v2]] + Weight[v1][v2]) : (CurColorWeight[0][Color[v1]] + CurColorWeight[1][Color[v2]]);
    int dif = mn - cur;

    Color[v1] = cv1;
    Color[v2] = cv2;

    return dif;
}

int ImproveVertix(int v1)
{
    int CurColorWeight[4] = {};
    for (int v : G[v1])
    {
        CurColorWeight[Color[v]] += Weight[v1][v];
    }

    int cv1, mn = 1e9;

    cout << "For " << Color[v1] << endl;

    for (int i = 1; i < 4; i++)
    {
        if (CurColorWeight[i] < mn)
        {
            cv1 = i;
            mn = CurColorWeight[i];
        }
        cout << i << " Color " << CurColorWeight[i] << endl;
    }

    CurColorWeight[0] = 0;
    int dif = mn - CurColorWeight[Color[v1]];

    Color[v1] = cv1;

    return dif;
}

void ImproveSolution(int WConf, int *Colors = Color) // No sence
{
    cout << "BEFORE " << WConf << endl;
    for (int i = 0; i < n; i++)
    {
        WConf += ImproveVertix(i);
    }
    cout << "AFTER " << WConf << endl;

    TrySetAns(CalcScore(WConf));
}

int EdgesSolution()
{
    if (DEBUG)
    {
        cout << "EdgesSolution: " << endl;
    }

    for (int i = 0; i < n; i++)
    {
        Color[i] = 0;
    }

    int WConf = 0;
    priority_queue<array<long double, 3>> pr;

    for (int i = 0; i < n; i++)
    {
        for (int v : G[i])
        {
            //pr.push({Weight[i][v], i, v});
            pr.push({((long double)Weight[i][v] / TotalWeight) * rand(), i, v});
        }
    }

    while (!pr.empty())
    {
        if (DEBUG && ADVANCED)
        {
            cout << "priority cur: " << endl;
            cout << pr.top()[0] << ' ' << pr.top()[1] << ' ' << pr.top()[2] << endl;
        }
        WConf += SetPrColor(pr.top()[1], pr.top()[2]);
        CheckTime(); // 70+ ms
        pr.pop();
    }

    int score = CalcScore(WConf);
    TrySetAns(score);

    if (DEBUG)
    {
        cout << "Solution score: " << score << endl;
        cout << "Total Weight = " << TotalWeight << ", Conflict Weight = " << WConf << endl;
    }

    return score;
}

void RandSolution()
{
    for (int i = 0; i < n; i++)
    {
        Color[i] = rand() % 3 + 1;
    }

    TrySetAns(CalcScore(Color));
    CheckTime();
}

int main()
{
    if (DEBUG || TEST)
    {
        freopen("../tests/20.in", "r", stdin);
    }

    ExecTimePoint = std::chrono::steady_clock::now() + std::chrono::milliseconds(ExecTime); // setting a timer

    Input();
    // TODO add start ans values


    if (UseEdgesSolution)
    {
        while (true)
        {
            EdgesSolution();
        }
    }

    if (UseRandSolution)
    {
        while (true)
        {
            RandSolution();
        }
    }
    Output();

    return 0;
}
