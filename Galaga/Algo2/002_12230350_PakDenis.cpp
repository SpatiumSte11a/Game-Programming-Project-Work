#include <iostream>
#include <vector>

using namespace std;

static void printOptimalParens(const vector<vector<int>>& splitMatrix, int startIndex, int lastIndex);

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    if (T < 1 || T > 100)
    {
        return 0;
    }


    while (T--)
    {
        int n;
        cin >> n;

        if (n < 2 || n > 1000)
        {
            return 0;
        }

        vector<long long> sizes(n + 1);

        for (int i = 0; i <= n; i++)
        {
            cin >> sizes[i];
        }

        vector<vector<long long>> costMatrix(n + 1, vector<long long>(n + 1, 0));
        vector<vector<int>> splitMatrix(n + 1, vector<int>(n + 1, 0));

        const long long INF = 4'000'000'000'000'000'000LL;

        for (int chainLength = 2; chainLength <= n; chainLength++)
        {
            for (int startIndex = 1; startIndex <= n - chainLength + 1; startIndex++)
            {
                int lastIndex = startIndex + chainLength - 1;
                costMatrix[startIndex][lastIndex] = INF;

                for (int splitPoint = startIndex; splitPoint < lastIndex; splitPoint++)
                {
                    long long currentCost =
                        costMatrix[startIndex][splitPoint]
                        + costMatrix[splitPoint + 1][lastIndex]
                        + sizes[startIndex - 1] * sizes[splitPoint] * sizes[lastIndex];

                    if (currentCost < costMatrix[startIndex][lastIndex])
                    {
                        costMatrix[startIndex][lastIndex] = currentCost;
                        splitMatrix[startIndex][lastIndex] = splitPoint;
                    }
                }
            }
        }

        cout << costMatrix[1][n] << '\n';
        printOptimalParens(splitMatrix, 1, n);
        cout << '\n';
    }

    return 0;
}

static void printOptimalParens(const vector<vector<int>>& splitMatrix, int startIndex, int lastIndex)
{
    if (startIndex == lastIndex)
    {
        cout << "W" << startIndex;
        return;
    }

    int splitPoint = splitMatrix[startIndex][lastIndex];

    cout << "(";
    printOptimalParens(splitMatrix, startIndex, splitPoint);
    printOptimalParens(splitMatrix, splitPoint + 1, lastIndex);
    cout << ")";
}