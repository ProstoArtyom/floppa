#include <bits/stdc++.h>

using namespace std;

int const N = 7005, M = 70005, H = M * 2;

int Color[N];
vector<int> G[N];
int Weight[N][N];
int TotalWeight, TotalScore;

pair<int, int> edges[H];
int perm[H];


bool const DEBUG = false, TEST = false, ADVANCED = false;
bool const UseTestSolution = true;

int const ExecTime = (DEBUG)? (1950) : (950);

std::chrono::steady_clock::time_point ExecTimePoint;

bool IsFindAnswer = false;

int MnColor[N];
long long MnWeight[N], ColorWeight[N][4];
bool Used[N];

int n, m, h;
int ans[N];

void CheckTime();

long double F(int x)
{
    return log2(x + 1000);
}

int CalcScore(int WConf)
{
    //return 1e9 - WConf;
    return (int)(log2(n)) * 100000 * (F(TotalWeight) - F(WConf)) / (F(TotalWeight) - F(0));
}

int CalcScore(int *Colors)
{
    int WConf = 0;
    for (int i = 0; i < n; ++i)
    {
        for (int v : G[i])
        {
            if (i < v && Colors[i] == Colors[v])
            {
                WConf += Weight[i][v];
            }
        }
    }

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
    h = 2 * m;
    for (int i = 0; i < m; ++i)
    {
        int v1, v2, w;
        cin >> v1 >> v2 >> w;
        G[--v1].push_back(--v2);
        G[v2].push_back(v1);
        Weight[v1][v2] = w;
        Weight[v2][v1] = w;
        edges[i] = {v1, v2};
        edges[m + i] = {v2, v1};
        TotalWeight += w;
    }
}

void Output()
{
    for (int i = 0; i < n; ++i)
    {
        cout << ans[i] << ' ';
    }
    cout << '\n';

    if (DEBUG)
    {
        cout << "time: " << chrono::duration_cast<chrono::milliseconds>(std::chrono::steady_clock::now() - ExecTimePoint + std::chrono::milliseconds(ExecTime)).count() << endl;
        if (!IsFindAnswer)
        {
            cout << "WARNING! Answer was not found!" << endl;
        }
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
        IsFindAnswer = true;
        TotalScore = score;
        for (int i = 0; i < n; ++i)
        {
            ans[i] = Colors[i];
        }
        return true;
    }

    return false;
}

int SetPrColor(int v1, int v2, int v3)
{
    for (int i = 1; i < 4; i++)
    {
        ColorWeight[v1][i] = 0;
        ColorWeight[v2][i] = 0;
        ColorWeight[v3][i] = 0;
    }

    for (int v : G[v1])
    {
        if (v != v2 && v != v3)
        {
            ColorWeight[v1][Color[v]] += Weight[v1][v];
        }
    }

    for (int v : G[v2])
    {
        if (v != v1 && v != v3)
        {
            ColorWeight[v2][Color[v]] += Weight[v2][v];
        }
    }

    for (int v : G[v3])
    {
        if (v != v1 && v != v2)
        {
            ColorWeight[v3][Color[v]] += Weight[v3][v];
        }
    }

    int cv1, cv2, cv3;
    long long mn = 1e18;

    for (int i = 1; i < 4; ++i)
    {
        for (int j = 1; j < 4; ++j)
        {
            for (int k = 1; k < 4; ++k)
            {
                int cur = ColorWeight[v1][i] + ColorWeight[v2][j] + ColorWeight[v3][k] + ((i == j)? (Weight[v1][v2]) : (0)) + ((i == k)? (Weight[v1][v3]) : (0)) + ((k == j)? (Weight[v2][v3]) : (0));
                if (cur < mn)
                {
                    mn = cur;
                    cv1 = i;
                    cv2 = j;
                    cv3 = k;
                }
            }
        }
    }

    ColorWeight[v1][0] = 0;
    ColorWeight[v2][0] = 0;
    ColorWeight[v3][0] = 0;
    int dif = mn - ColorWeight[v1][Color[v1]] - ColorWeight[v2][Color[v2]] - ColorWeight[v3][Color[v3]] - ((Color[v1] == Color[v2] && Color[v1] != 0)? (Weight[v1][v2]) : (0)) - ((Color[v1] == Color[v3] && Color[v3] != 0)? (Weight[v1][v3]) : (0)) - ((Color[v3] == Color[v2] && Color[v3] != 0)? (Weight[v3][v2]) : (0));

    MnWeight[v1]= ColorWeight[v1][cv1];
    MnWeight[v2]= ColorWeight[v2][cv2];
    MnWeight[v3]= ColorWeight[v3][cv3];

    Color[v1] = cv1;
    Color[v2] = cv2;
    Color[v3] = cv3;

    return dif;
}

int SetPrColor(int v1, int v2)
{
    for (int i = 1; i < 4; i++)
    {
        ColorWeight[v1][i] = 0;
        ColorWeight[v2][i] = 0;
    }

    for (int v : G[v1])
    {
        if (v != v2)
        {
            ColorWeight[v1][Color[v]] += Weight[v1][v];
        }
    }

    for (int v : G[v2])
    {
        if (v != v1)
        {
            ColorWeight[v2][Color[v]] += Weight[v2][v];
        }
    }

    int cv1, cv2;
    long long mn = 1e18;

    for (int i = 1; i < 4; ++i)
    {
        for (int j = 1; j < 4; ++j)
        {
            int cur = ColorWeight[v1][i] + ColorWeight[v2][j] + ((i == j)? (Weight[v1][v2]) : (0));
            if (cur < mn)
            {
                mn = cur;
                cv1 = i;
                cv2 = j;
            }
        }
    }

    ColorWeight[v1][0] = 0;
    ColorWeight[v2][0] = 0;
    int dif = ColorWeight[v1][cv1] + ColorWeight[v2][cv2] + ((cv1 == cv2)? (Weight[v1][v2]) : (0)) - ColorWeight[v1][Color[v1]] - ColorWeight[v2][Color[v2]] - ((Color[v1] == Color[v2] && Color[v1] != 0)? (Weight[v1][v2]) : (0));

    MnWeight[v1]= ColorWeight[v1][cv1];
    MnWeight[v2]= ColorWeight[v2][cv2];

    Color[v1] = cv1;
    Color[v2] = cv2;

    return dif;
}

#define MRAND_MAX 32767 * 2

static unsigned long int mnext = 1;

int mrand()
{
  mnext = mnext * 1103515245 + 12345;
  return (unsigned int)(mnext/65536) % (MRAND_MAX + 1);
}

int permSize;

void resizePerm(int sz)
{
    permSize = sz;
    for (int i = 0; i < sz; i++)
    {
        perm[i] = i;
    }
}

void generatePerm()
{
    for (int i = permSize - 1; i >= 0; i--)
    {
        int j = mrand() % (i + 1);

        swap(perm[i], perm[j]);
    }
}

long long getCostValue(int v)
{
    //return mrand();
    return (MnWeight[v] * 10000 + G[v].size()) * mrand();
}

long long getCost(int v)
{
    //return G[v].size();

    if (Color[v])
    {
        return ColorWeight[v][Color[v]] - MnWeight[v];
    }
    else
    {
        return 1e18 - MnWeight[v];
    }
}

long long getCost(int v1, int v2)
{
    return (getCost(v1) + getCost(v2)) * mrand();
    //return (getCost(v1) + getCost(v2)) * mrand();
}

long long getCost(int v1, int v2, int v3)
{
    return (getCost(v1) + getCost(v2) + getCost(v3)) * mrand();
    //return (getCost(v1) + getCost(v2)) * mrand();
}

    priority_queue<array<long long, 4>> pr; // max
priority_queue<array<int, 2>> mpr;

void TryPush(int v1, long long cost)
{
    if (!Used[v1])
    {
        resizePerm(G[v1].size());
        generatePerm();

        if (G[v1].size() == 1)
        {
            pr.push({getCost(v1, G[v1][0]), v1, G[v1][0], -1});
        }
        else
        {
            for (int i = 1; i < G[v1].size(); i++)
            {
                pr.push({getCost(v1, G[v1][perm[i - 1]], G[v1][perm[i]]), v1, G[v1][perm[i - 1]], G[v1][perm[i]]});
            }
        }
    }
    //cout << v1 << '\n';
}

void TestSolution()
{
    //priority_queue<array<long long, 3>, vector<array<long long, 3>>, greater<array<long long, 3>>> pr; // min
    //priority_queue<array<long long, 2>> mpr; // max

    for (int i = 0; i < n; ++i)
    {
        Color[i] = 0;
        Used[i] = false;

        //for (int j = 1; j < 4; ++j)
        //{
        //    ColorWeight[i][j] = 0;
        //}
        MnWeight[i] = 0;
    }

    int s1 = mrand() % n, s2 = G[s1][mrand() % G[s1].size()];
    if (G[s1].size() == 1)
    {
        pr.push({0, s1, s2, -1});
    }
    else
    {
        int s3;
        do
        {
            s3 = G[s1][mrand() % G[s1].size()];
        }
        while (s3 == s2);

        pr.push({0, s1, s2, s3});
    }
    int WConf = 0;

    while (!pr.empty())
    {
        //cout << "ok" << '\n';
        long long cost = pr.top()[0];
        int v1 = pr.top()[1];
        int v2 = pr.top()[2];
        int v3 = pr.top()[3];
        pr.pop();
        if (v3 == -1)
        {
            WConf += SetPrColor(v1, v2);
        }
        else
        {
            WConf += SetPrColor(v1, v2, v3);
            //cout << v3 << ' ' << cost << " v3Cost" << '\n';
            TryPush(v3, cost);
            Used[v3] = true;
        }

        //cout << v1 << ' ' << cost << " Cost" << '\n';
        //cout << "fun" << '\n';
        TryPush(v1, cost);
        //cout << "fun2" << '\n';
        TryPush(v2, cost);
        Used[v1] = true;
        Used[v2] = true;
        //cout << 2 << '\n';


        CheckTime();
        //cout << "ok2" << '\n';
    }

    //cout << WConf << '\n';
    TrySetAns(CalcScore(WConf));
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    if (DEBUG || TEST)
    {
        freopen("../tests/20.in", "r", stdin);
    }

    ExecTimePoint = std::chrono::steady_clock::now() + std::chrono::milliseconds(ExecTime); // setting a timer

    Input();

    for (int i = 0; i < n; ++i)
    {
        ans[i] = mrand() % 3 + 1;
    }

    if (UseTestSolution)
    {
        while (true)
        {
            TestSolution();
        }
    }

    Output();

    return 0;
}
