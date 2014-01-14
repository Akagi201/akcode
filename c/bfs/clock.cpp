/*
ID: akagi201
LANG: C++
TASK: clocks
*/

#include <fstream>

using namespace std;

const int with[10][5] = {{}, {1, 2, 4, 5}, {1, 2, 3}, {2, 3, 5, 6}, {1, 4, 7}, {2, 4, 5, 6, 8}, {3, 6, 9}, {4, 5, 7, 8}, {7, 8, 9}, {5, 6, 8, 9}};
char t[270000][10], way[270000] = {1}, use[270000][10];
int pre[270000];
string ans;

void bfs();
bool check(int);
void work(int, int, int);

int main()
{
    ifstream fin ("clocks.in");
    ofstream fout ("clocks.out");

    for (int i = 1, tem; i <= 9; i++)
    {
        fin >> tem;
        t[0][i] = tem % 12;
    }

    bfs();
    fout << ans << endl;
    fin.close();
    fout.close();
    return 0;
}

void bfs()
{
    int tail = 0, head = -1;

    while (tail > head)
    {
        head++;

        for (int i = way[head]; i <= 9; i++)
        {
            if (use[head][i] < 3)
            {
                work(i, head, ++tail);
                way[tail] = i;
                pre[tail] = head;

                for (int j = 1; j <= 9; j++)
                {
                    use[tail][j] = use[head][j];
                }

                use[tail][i]++;

                if (check(tail))
                {
                    ans = char(way[tail] + '0');

                    for (int j = pre[tail]; j != 0; j = pre[j])
                    {
                        ans = ' ' + ans;
                        ans = char(way[j] + '0') + ans;
                    }

                    return;
                }
            }
        }
    }
}

bool check(int k)
{
    for (int i = 1; i <= 9; i++)
    {
        if (t[k][i] != 0)
        {
            return false;
        }
    }

    return true;
}

void work(int k, int from, int cur)
{
    for (int i = 1; i <= 9; i++)
    {
        t[cur][i] = t[from][i];
    }

    for (int i = 0; i <= 4 && with[k][i] != 0; i++)
    {
        t[cur][with[k][i]] = (t[from][with[k][i]] + 3) % 12;
    }
}


