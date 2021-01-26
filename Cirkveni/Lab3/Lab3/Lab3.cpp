#include <iostream>

bool Combine(uint16_t* base, uint16_t addition)
{
    bool res = ((*base & addition) == 0);
    *base = *base | addition;
    return res;
}

uint16_t CountTailOnes(uint16_t n, uint16_t ignoreLast) //It should work
{
    for (uint16_t m = n >> ignoreLast; (m & 1) == 1; m >>= 1) ignoreLast++;
    return ignoreLast;
}

uint16_t GetMin(uint16_t n, uint16_t def = -1) //Tested
{
    uint16_t res = 0;
    if (n == 0) return def;
    for (uint16_t k = 8; k != 0; k--)
    {
        if ((n & ((1 << k) - 1)) == 0)
        {
            res += k;
            n >>= k;
        }
    }
    return res;
}

void SwapEnds(uint16_t a, uint16_t b, uint16_t* c, uint16_t* d) //It should work
{
    *c = a;
    *d = b;
    uint16_t diff = (b % 3) - (a % 3);
    *c += diff;
    *d -= diff;
}

void PreservingDivmod(uint16_t a, uint16_t b, uint16_t* c, uint16_t* d) //It should work
{
    uint16_t e = (a / 3) * 3;
    uint16_t f = (a % 3) * 3;
    e += b / 3;
    f += b % 3;
    *c = e;
    *d = f;
}

class CP_sudoku {
public:
    struct SudokuField {
        int field[9][9];
    };
    SudokuField* insert(SudokuField old, uint16_t row, uint16_t col, int value)
    {
        SudokuField* res = (SudokuField*)malloc(sizeof(SudokuField));
        for (uint16_t i = 0; i < 9; i++)
        {
            for (uint16_t j = 0; j < 9; j++)
            {
                res->field[i][j] = old.field[i][j];
            }
        }
        res->field[row][col] = value;
        return res;
    }
    struct SFlink
    {
        SudokuField* value;
        uint16_t modeindex;
        uint16_t locations;
        uint16_t values;
        SFlink* next;
    };
    template <size_t rows, size_t cols>
    SudokuField* solve(int(&init_values)[rows][cols])
    {
        SudokuField* start = (SudokuField*)malloc(sizeof(SudokuField));
        *start = {};
        for (uint16_t i = 0; i < 9; i++)
        {
            for (uint16_t j = 0; j < 9; j++)
            {
                int a = init_values[i][j];
                if (a == -1) a++;
                start->field[i][j] = a;
            }
        }
        SudokuField* end = solve(start);
        return end;
    }
    std::string printField(SudokuField* source)
    {
        std::string res;
        for (uint16_t i = 0; i < 9; i++)
        {
            for (uint16_t j = 0; j < 9; j++)
            {
                res += source->field[i][j] + 48;
            }
            res += "\n";
        }
        return res;
    }
    SudokuField* solve(SudokuField* start)
    {
        SFlink* first = (SFlink*)malloc(sizeof(SFlink));
        if (!first) return NULL;
        *first = {};
        first->value = start;
        iterations = 0;
        while (first)
        {
            iterations++;
            uint16_t mode = first->modeindex;
            SudokuField* current = first->value;
            first = first->next;
            uint16_t index = mode & 15;
            uint16_t locations = 0;
            uint16_t values = 0;
            mode >>= 4;
            uint16_t count = 10;
            uint16_t count_v[9] = {};
            uint16_t count_h[9] = {};
            uint16_t count_s[9] = {};
            uint16_t id_v[9] = {};
            uint16_t id_h[9] = {};
            uint16_t id_s[9] = {};
            uint16_t pos_v[9] = {};
            uint16_t pos_h[9] = {};
            uint16_t pos_s[9] = {};
            uint16_t remaining = 0;
            bool isValid = true;
            for (uint16_t i = 0; i < 9; i++)
            {
                if (!isValid) {
                    break;
                }
                for (uint16_t j = 0; j < 9; j++)
                {
                    uint16_t k, l;
                    PreservingDivmod(i, j, &k, &l);
                    int v = current->field[i][j];
                    if (v == 0)
                    {
                        count_v[i]++;
                        count_h[j]++;
                        count_s[k]++;
                        remaining++;
                        isValid &= Combine(&pos_v[i], 1 << j);
                        isValid &= Combine(&pos_h[j], 1 << i);
                        isValid &= Combine(&pos_s[k], 1 << l);
                        if (!isValid) break;
                    }
                    else
                    {
                        uint16_t pwr = 1 << (v - 1);
                        isValid &= Combine(&id_v[i], pwr);
                        isValid &= Combine(&id_h[j], pwr);
                        isValid &= Combine(&id_s[k], pwr);
                        if (!isValid) break;
                    }
                }
            }
            if (!isValid) break;
            if (mode == 0)
            {
                //----------------------------------------------------------------------New series
                for (uint16_t i = 0; i < 9; i++)
                {
                    if (count_v[i] < count && count_v[i] != 0)
                    {
                        mode = 1;
                        index = i;
                        count = count_v[i];
                        locations = pos_v[i];
                        values = id_v[i] ^ 511;
                    }
                }
                for (uint16_t i = 0; i < 9; i++)
                {
                    if (count_h[i] < count && count_h[i] != 0)
                    {
                        mode = 2;
                        index = i;
                        count = count_h[i];
                        locations = pos_h[i];
                        values = id_h[i] ^ 511;
                    }
                }
                for (uint16_t i = 0; i < 9; i++)
                {
                    if (count_s[i] < count && count_s[i] != 0)
                    {
                        mode = 3;
                        index = i;
                        count = count_s[i];
                        locations = pos_s[i];
                        values = id_s[i] ^ 511;
                    }
                }
            }
            uint16_t curr = GetMin(locations);
            locations &= locations - 1;
            uint16_t newr = -1, newc = -1;
            switch (mode)
            {
            case 1:
                newr = index;
                newc = curr;
                break;
            case 2:
                newr = curr;
                newc = index;
                break;
            case 3:
                PreservingDivmod(index, curr, &newr, &newc);
                break;
            case 0:
                return current;
            default:
                throw "HOW?!?";
            }
            uint16_t tests = id_v[newr];
            tests = tests | id_h[newc];
            tests = tests | id_s[newr - (newr % 3) + (newc / 3)];
            bool x = false;
            for (int i = 0; i < 9; i++)
            {
                uint16_t power = 1 << i;
                if ((tests & power) == 0)
                {
                    x = true;
                    SudokuField* newField = insert(*current, newr, newc, i + 1);
                    SFlink* newlink = (SFlink*)malloc(sizeof(SFlink));
                    newlink->value = newField;
                    newlink->values = values - power;
                    newlink->modeindex = newlink->values == 0 ? 0 : mode;
                    newlink->locations = locations;
                    newlink->next = first;
                    first = newlink;
                }
            }
        }
        return nullptr;
    }
    int getBacktraceCalls()
    {
        return iterations;
    }
private:
    int iterations;
};

CP_sudoku::SudokuField* convert(std::string data)
{
    CP_sudoku::SudokuField* F = (CP_sudoku::SudokuField*)malloc(sizeof(CP_sudoku::SudokuField));
    if (!F) return NULL;
    int i = 0;
    int j = 0;
    for (char c: data)
    {
        if ((c == '\n') || (j == 9)) { i++; j = 0; continue; }
        if (i >= 9) break;
        F->field[i][j] = ((int)(c)) - 48;
        j++;
    }
    return F;
}

int main()
{
    std::string decide = "";
    //std::cin >> decide;
    std::cout << "Processing..." << std::endl;
    if (decide[0]=='E' || true)
    {
        decide = "000260701\n680070090\n190004500\n820100040\n004602900\n050003028\n009300074\n040050036\n703018000";
    }
    CP_sudoku::SudokuField* start = convert(decide);
    CP_sudoku X = {};
    CP_sudoku::SudokuField* res = X.solve(start);
    if (res == nullptr) return 0;
    else std::cout << X.solve(res) << std::endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
