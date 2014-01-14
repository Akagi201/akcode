
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

using namespace std;

int main(void)
{
   int m = 0;
   int n = 0;
   int i = 0;
   int j = 0;
   int dra[2004] = {0};
   int loo[2004] = {0};
   int money = 0;

#ifndef ONLINE_JUDGE
   freopen("input.txt", "rt", stdin);
   freopen("output.txt", "wt", stdout);
#endif

   while ((EOF != scanf("%d%d", &n, &m)) && (n && m))
   {
      money = 0;
      memset(dra, 0, sizeof(dra)); 
      memset(loo, 0, sizeof(loo));
      for (i = 0; i < n; ++i)
      {
         scanf("%d", dra + i);
      }
      for (j = 0; j < m; ++j)
      {
         scanf("%d", loo + j);
      }
      if (n > m)
      {
         printf("Loowater is doomed!\n");
         continue;
      }
      sort(dra, dra + n);
      sort(loo, loo + m);
      for (i = 0, j = 0; (i < n) && (j < m); ++j)
      {
         if (loo[j] >= dra[i])
         {
            money += loo[j];
            ++i;
         }
      }
      if (i != n)
      {
         printf("Loowater is doomed!\n");
      }
      else
      {
         printf("%d\n", money);
      }
   }
   return 0; 
}

