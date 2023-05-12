#include <bits/stdc++.h>

using namespace std;

int const N = 7005, M = 70005, H = M * 2;
int const ExecTime = 950;

int Color[N];
vector<int> G[N];
int Weight[N][N];
int TotalWeight, TotalScore;

pair<int, int> edges[H];
int perm[H];

std::chrono::steady_clock::time_point ExecTimePoint;


bool const DEBUG = false, TEST = false, ADVANCED = false;
bool const UseEdgesDefaultSolution = true, UseEdgesHighDegreeSolution = true;


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
        for (int i = 0; i < n; ++i)
        {
            ans[i] = Colors[i];
        }
        return true;
    }

    return false;
}

int SetSColor(int v1, int v2)
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

    int cv1 = rand() % 3 + 1;
    int cv2 = cv1;

    CurColorWeight[0][0] = 0;
    CurColorWeight[1][0] = 0;
    int dif = CurColorWeight[0][cv1] + CurColorWeight[1][cv2] + ((cv1 == cv2 && cv1 != 0)? (Weight[v1][v2]) : (0)) - CurColorWeight[0][Color[v1]] - CurColorWeight[1][Color[v2]] - ((Color[v1] == Color[v2] && Color[v1] != 0)? (Weight[v1][v2]) : (0));

    Color[v1] = cv1;
    Color[v2] = cv2;

    if (DEBUG && ADVANCED)
    {
        cout << "COLOR SET: " << endl;
        cout << v1 << ' ' << v2 << ' ' << Color[v1] << ' ' << Color[v2] << endl;
    }

    return dif;
}

int SetRColor(int v1, int v2)
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

    int cv1 = (Color[v1])? (Color[v1]) : (rand() % 3 + 1);
    int cv2 = (Color[v2])? (Color[v2]) : ((cv1 + rand() % 2) % 3 + 1);

    CurColorWeight[0][0] = 0;
    CurColorWeight[1][0] = 0;
    int dif = CurColorWeight[0][cv1] + CurColorWeight[1][cv2] + ((cv1 == cv2 && cv1 != 0)? (Weight[v1][v2]) : (0)) - CurColorWeight[0][Color[v1]] - CurColorWeight[1][Color[v2]] - ((Color[v1] == Color[v2] && Color[v1] != 0)? (Weight[v1][v2]) : (0));

    Color[v1] = cv1;
    Color[v2] = cv2;

    if (DEBUG && ADVANCED)
    {
        cout << "COLOR SET: " << endl;
        cout << v1 << ' ' << v2 << ' ' << Color[v1] << ' ' << Color[v2] << endl;
    }

    return dif;
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

    for (int i = 1; i < 4; ++i)
    {
        for (int j = 1; j < 4; ++j)
        {
            int cur = CurColorWeight[0][i] + CurColorWeight[1][j] + ((i == j)? (Weight[v1][v2]) : (0));
            if (cur < mn)
            {
                mn = cur;
                cv1 = i;
                cv2 = j;
            }
        }
    }

    CurColorWeight[0][0] = 0;
    CurColorWeight[1][0] = 0;
    int dif = CurColorWeight[0][cv1] + CurColorWeight[1][cv2] + ((cv1 == cv2)? (Weight[v1][v2]) : (0)) - CurColorWeight[0][Color[v1]] - CurColorWeight[1][Color[v2]] - ((Color[v1] == Color[v2] && Color[v1] != 0)? (Weight[v1][v2]) : (0));

    Color[v1] = cv1;
    Color[v2] = cv2;

    return dif;
}

int SetColor(int v1, int cv1 = 0)
{
    int CurColorWeight[4] = {};
    for (int v : G[v1])
    {
        CurColorWeight[Color[v]] += Weight[v1][v];
    }

    CurColorWeight[0] = 0;
    int dif = CurColorWeight[cv1] - CurColorWeight[Color[v1]];

    Color[v1] = cv1;

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

int getCostValue(int w)
{
    //return ((long double)w / TotalWeight) * rand();
    return rand();
}

void EdgesHighDegreeSolution(int len)
{
    memset(Color, 0, sizeof(int) * n);

    int WConf = 0;

    for (int l = 0; l < len; ++l)
    {
        //generatePerm();
        priority_queue<pair<int, int>> pr;

        for (int i = 0; i < h; ++i)
        {
            pr.push({G[edges[i].first].size() + G[edges[i].second].size(), i});
        }

        for (int i = 0; i < h; ++i)
        {
            WConf += SetPrColor(edges[pr.top().second].first, edges[pr.top().second].second);
            CheckTime();
            pr.pop();
        }

        TrySetAns(CalcScore(WConf));

        for (int i = n / 2; i >= 0; i--)
        {
            WConf += SetColor(rand() % n);
        }
    }
}

void EdgesDefaultSolution()
{
    memset(Color, 0, sizeof(int) * n);

    int WConf = 0;

    for (int l = 0; l < 2; ++l)
    {
        generatePerm();

        for (int i = 0; i < h; ++i)
        {
            WConf += SetPrColor(edges[perm[i]].first, edges[perm[i]].second);
            CheckTime();
        }

        TrySetAns(CalcScore(WConf));
    }
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
        ans[i] = rand() % 3 + 1;
    }

    if (UseEdgesHighDegreeSolution)
    {
        EdgesHighDegreeSolution(20);
    }
    if (UseEdgesDefaultSolution)
    {
        while (true)
        {
            resizePerm(h);
            EdgesDefaultSolution();
        }
    }

    Output();

    return 0;
}
