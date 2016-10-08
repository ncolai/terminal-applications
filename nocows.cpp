/*
ID: reginig1
PROG: nocows
LANG: C++
*/

/* These USACO problems are such a struggle. I don't know how many hours I've burned at this point
* trying to solve these.
* Well, anyway, here is the typical analysis:
*/

#include <iostream>
#include <stdio.h>
#include <assert.h>
#define MOD_CONST 9901
//If I put in 205 or greater here, the program doesn't work. I suspect it's because extra
//solutions are added. Sean's advice apparently is not 100% foolproof.
#define MAX_NODES 202
#define MAX_HEIGHT 102
using namespace std;

/*
int exp(int base, int power)
{
	if (power == 0) return 1;
	else return base * exp(base, power - 1);
}

long long choose(int top, int bottom)
{
	if (bottom == 0) return 1;
	if (top < bottom * 2) bottom = top - bottom;
	long long topprod = 1;
	long long bottomprod = 1;
	for (long long i = 1; i <= bottom; i++) {
		topprod *= top - (bottom - i);
		bottomprod *= i;
		//cout << topprod << " " << bottomprod << "\n";
		if (topprod % bottomprod == 0) {
			topprod /= bottomprod;
			bottomprod = 1;
		}
	}
	return topprod;
}

long long totaltrees;
int maxnodes;
int maxheight;

void inputs()
{
	FILE *fin;
	fin = fopen("nocowsin.txt", "r");
	assert(fin != NULL);
	fscanf(fin, "%d %d", &maxnodes, &maxheight);
	fclose(fin);
}

long long choosearray[300][300];
//nodes at level is available nodes rather than all nodes. (one node preselected to provide path.)
void expandtree(int nodesatlevel, int totalnodes, int height, long long paths)
{
	//printf("%d %d %d %d\n", nodesatlevel, totalnodes, height, paths);
	//printf("%d %d\n", maxnodes, maxheight);

	if (height == maxheight && totalnodes == maxnodes) {
		totaltrees += paths;
		return;
	}
	if (totalnodes > maxnodes)
		return;
	if (height > maxheight)
		return;

    if (totaltrees >= MOD_CONST) {
        totaltrees %= MOD_CONST;
    }
	//int newheight = height + 1;

    //for (int i = 0; i <= nodesatlevel; i++) {
	for (int i = nodesatlevel; i >= 0; i--) {
        //if (totalnodes + 2*i > maxnodes) break;
        //expandtree(2 * i, totalnodes + (2*i), newheight, paths * choosearray[nodesatlevel][i] % MOD_CONST);
        expandtree(2 * i + 1, totalnodes + (2*i), height + 1, paths * choosearray[nodesatlevel+1][i+1] % MOD_CONST);
	}
}

int main() {
	FILE *fout;
	fout = fopen("nocowsout.txt", "w");

	choosearray[1][1] = 1;
	for (int i = 1; i <= 60; i++) {
        for (int j = 0; j <= i; j++) {
            choosearray[i][j] = choose(i, j) % MOD_CONST;
            //cout << choosearray[2*i][j] << " ";
        }
        //cout << "\n";
	}

	inputs();
	//expandtree(1,1,1,1);
	expandtree(1, 2*maxheight-1, 2, 1);
	fprintf(fout, "%d\n", totaltrees % MOD_CONST);
	fclose(fout);
	return 0;
}
*/

//modified to fit problem. (Return number mod 9901.)
int PascalsTri[MAX_NODES][MAX_NODES];

//Initially I calculated the choose function individually, then I threw up my hands
//and calculated the whole stupid triangle up to that solution (which is faster.)
void fillTri(int sz)
{
    for (int i = 0; i <= sz; i++)
    for (int j = 0; j <= i; j++)
        if (j == 0 || j == i) {
            PascalsTri[i][j] = 1;
        }
        else {
            PascalsTri[i][j] = (PascalsTri[i-1][j-1] + PascalsTri[i-1][j]) % MOD_CONST;
        }
}

//I had to be careful with this, since too high of limits could easily make the stack size too big.
int numTrees[MAX_NODES][MAX_NODES/2][MAX_HEIGHT];

//You can see my hours of struggle above. I finally thought of a dynamic solution, and this is what
//I came up with. A lot of runtime was cut by generating Pascal's triangle.
void findTrees(int curHeight)
{
    for (int i = 1; i <= 200; i++)
    for (int j = 1; j <= 100; j++)
    if (numTrees[i][j][curHeight]) {
        for (int k = 1; k <= j; k++) {
            if (i + 2*k <= MAX_NODES) {
                numTrees[i + 2*k][2*k][curHeight + 1] +=
                numTrees[i][j][curHeight] * PascalsTri[j][k];
                numTrees[i + 2*k][2*k][curHeight + 1] %= MOD_CONST;
            }
        }
    }

    /*
    for (int i = 1; i <= 100; i++) {
        for (int j = 1; j <= 30; j++) {
            cout << numTrees[i][j][curHeight + 1] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
    */

}

int main()
{
    FILE *fin, *fout;
    fin = fopen("nocows.in", "r");
    fout = fopen("nocows.out", "w");
    assert(fin != NULL && fout != NULL);
    int totalnodes, height;
    fscanf(fin, "%d %d", &totalnodes, &height);

    fillTri(203);
    /*
    for (int i = 0; i <= 200; i++) {
        for (int counter = 0; PascalsTri[i][counter] != 0; counter++)
            cout << PascalsTri[i][counter] << "\t";
        cout << "\n";
    }
    */

    //need to have an initial node to start.
    numTrees[1][1][1] = 1;

    //do DP over each succesive height.
    for (int i = 1; i < height; i++) {
        findTrees(i);
    }

    //find the total number of trees with given height and total number of nodes.
    //(mod 9901.)
    int totalTreesMod9901 = 0;
    for (int i = 1; i <= 100; i++) {
        totalTreesMod9901 += numTrees[totalnodes][i][height];
        totalTreesMod9901 %= MOD_CONST;
    }

    fprintf(fout, "%d\n", totalTreesMod9901);
    fclose(fin);
    fclose(fout);

    return 0;
}
