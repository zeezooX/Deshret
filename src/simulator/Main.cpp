#include <iostream>
#include <string>
#include <cstring>
#define String std::to_string
#include "API.h"

// variable initialization
int unvisited[324] = {0};
int unvisited_i = 0;
bool maze[324][4] = {false};
bool visited[324] = {false};
int moves[324] = {0};
int moves_i = 0;
int path[324] = {0};
int path_i = 0;
int path_j = 0;
int current = 19;
int orient = 0;
char directions[] = {'n', 'e', 's', 'w'};
int dir[] = {18, 1, -18, -1};

int p[324] = {0};

// function initialization
void bfs(int s)
{
    int q[324] = {0};
    int q_i = 0;
    int q_j = 0;
    bool used[324] = {false};
    int d[324] = {0};
    q[q_i] = s;
    q_i++;
    used[s] = true;
    p[s] = -1;
    while (q_i > q_j)
    {
        int v = q[q_j];
        q_j++;
        for (int i = 0; i < 4; i++)
        {
            if (!maze[v][i])
                continue;
            int u = v + dir[i];
            if (!used[u])
            {
                used[u] = true;
                q[q_i] = u;
                q_i++;
                d[u] = d[v];
                p[u] = v;
            }
        }
    }
}

void wall(int shift)
{
    int x = (4 + orient + shift) % 4;
    int y = current + (x < 2 ? 1 : -1) * (1 + ((x + 1) % 2) * 17);
    if (!visited[y])
    {
        unvisited[unvisited_i] = y;
        unvisited_i++;
    }
    maze[current][x] = true;
}

void go(int a, int l)
{
    if ((4 + orient - 1) % 4 == a)
    {
        API::turnLeft();
        orient = a;
    }
    while (orient != a)
    {
        API::turnRight();
        orient = (4 + orient + 1) % 4;
    }
    API::moveForward(l);
}

void sprint(int arr[], int start, int end)
{
    while (end > start)
    {
        for (int i = 0; i < 4; i++)
        {
            if (arr[end - 1] - current == dir[i])
            {
                int n = arr[end - 1];
                end--;
                int c = 1;
                while (end - start > 1 && arr[end - 1] - n == dir[i])
                {
                    n = arr[end - 1];
                    end--;
                    c++;
                }
                go(i, c);
                current = n;
                break;
            }
        }
    }
}

void log(const std::string &text)
{
    std::cerr << text << std::endl;
}

int main(int argc, char *argv[])
{
    log("Running...");
    API::setWall(0, 0, 's');

    // marking off-border cells as visited
    for (int i = 0; i < 18; i++)
    {
        visited[i] = true;
        visited[i * 18] = true;
        visited[i + 306] = true;
        visited[i * 18 + 17] = true;
    }

    // numbering cells
    for (int i = 0; i < 18; i++)
        for (int j = 0; j < 18; j++)
            API::setText(j - 1, i - 1, String(j + i * 18));

    while (true)
    {
        // coloring visited cell yellow
        API::setColor(current % 18 - 1, current / 18 - 1, 'y');
        // marking discovered walls
        if (API::wallLeft())
        {
            int w = (4 + orient - 1) % 4;
            API::setWall(current % 18 - 1, current / 18 - 1, directions[w]);
        }
        if (API::wallRight())
        {
            int w = (4 + orient + 1) % 4;
            API::setWall(current % 18 - 1, current / 18 - 1, directions[w]);
        }
        if (API::wallFront())
        {
            int w = orient;
            API::setWall(current % 18 - 1, current / 18 - 1, directions[w]);
        }

        // marking cell as visited
        visited[current] = true;

        // storing discovered walls
        if (!API::wallFront())
            wall(0);
        if (!API::wallRight())
            wall(1);
        if (!API::wallLeft())
            wall(-1);

        if (!unvisited_i)
            break;

        // getting nearest unvisited cell
        int z = unvisited[unvisited_i - 1];
        unvisited_i--;
        while (true)
        {
            if (visited[z])
            {
            }
            else if (visited[z + 1] && visited[z - 1] && visited[z + 18] && visited[z - 18])
            {
                visited[z] = true;
                API::setColor(z % 18 - 1, z / 18 - 1, 'o');
                for (int i = 0; i < 4; i++)
                    maze[z][i] = maze[z + dir[i]][(i + 2) % 4];
            }
            else
                break;
            if (unvisited_i)
            {
                z = unvisited[unvisited_i - 1];
                unvisited_i--;
            }
            else
            {
                z = -1;
                break;
            }
        }

        if (z == -1)
            break;

        int c = 0;
        int tmp = current;
        while (true)
        {
            // log("Unvisited Stack: " + str(unvisited));
            // log("Moves Stack: " + str(moves));
            log(String(current) + " -> " + String(z));

            bool b = false;
            for (int j = 0; j < 4; j++)
            {
                if (tmp + dir[j] == z && maze[tmp][j])
                {
                    log("sprint " + String(c));
                    sprint(moves, moves_i - c, moves_i);
                    moves_i -= c;

                    go(j, 1);
                    moves[moves_i] = current;
                    moves_i++;
                    current = z;
                    int x = (orient + 2) % 4;
                    maze[current][x] = true;
                    b = true;
                    break;
                }
            }

            if (b)
                break;

            c++;
            tmp = moves[moves_i - c];
        }
    }

    // calculate best path
    bfs(19);

    int v = 170;
    while (v != -1)
    {
        path[path_i] = v;
        path_i++;
        v = p[v];
    }
    while ((path[path_j] == 170 || path[path_j] == 171 || path[path_j] == 152 || path[path_j] == 153) &&
           ((path[path_j + 1] == 170 || path[path_j + 1] == 171 || path[path_j + 1] == 152 || path[path_j + 1] == 153)))
    {
        path_j++;
    }
    for(int i = path_j; i < path_i; i++)
        API::setColor(path[i] % 18 - 1, path[i] / 18 - 1, 'g');

    int tmp = current;
    int i = 0;
    while(p[tmp] != -1)
    {
        i++;
        tmp = p[tmp];
    }
    moves_i = i;
    tmp = current;
    while(p[tmp] != -1)
    {
        moves[i - 1] = p[tmp];
        i--;
        tmp = p[tmp];
    }
    sprint(moves, 0, moves_i);
    
    path_i--;
    sprint(path, path_j, path_i);

    log("Simulation Complete!");
    log("Yellow: Cells Mapped By Discovery");
    log("Orange: Cells Mapped By Deduction");
    log("Green: Shortest Path To Center");

    // API::setColor(0, 0, 'G');
    // API::setText(0, 0, "abc");
    // while (true) {
    //     if (!API::wallLeft()) {
    //         API::turnLeft();
    //     }
    //     while (API::wallFront()) {
    //         API::turnRight();
    //     }
    //     API::moveForward();
    // }
}
