// NASP_lab2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

typedef unsigned int uint;
constexpr uint MAX = -1;

void indentedOut(int indent,uint data) {
    while (indent--) std::cout << "|";
    std::cout << data << std::endl;
}


class Data
{
public:
    uint row, column;
    uint16_t value;
    uint last_row, last_column;
    Data()
    {
        this->row = -1;
        this->column = -1;
        this->value = -1;
        this->last_row = -1;
        this->last_column = -1;
    }
    Data(uint row, uint column, uint16_t value, Data* last=nullptr)
    {
        this->row = row;
        this->column = column;
        this->value = value;
        if (last == nullptr)
        {
            this->last_row = -1;
            this->last_column = -1;
        }
        else
        {
            this->last_row = last->row;
            this->last_column = last->column;
        }
    }
    void track(vector<pair<uint, uint>>& solution, Data** databank)
    {
        Data* current = this;
        vector<pair<uint, uint>>& negative = *new vector<pair<uint, uint>>();
        solution = *new vector<pair<uint, uint>>();
        while (current->last_row!=MAX)
        {
            negative.push_back(*new pair<uint,uint>(current->row, current->column));
            current = &databank[current->last_row][current->last_column];
        }
        negative.push_back(*new pair<uint, uint>(current->row, current->column));
        while (!negative.empty())
        {
            solution.push_back(negative.back());
            negative.pop_back();
        }
        free(&negative);
    }
};

class AVL_element{
public:
    uint value;
    Data data;
    int depth, diff;
    AVL_element* left_child;
    AVL_element* right_child;
    AVL_element* parent;
    AVL_element(uint assigned, Data adata) {
        value = assigned;
        data = adata;
        depth = 0;
        diff = 0;
        left_child = NULL;
        right_child = NULL;
        parent = NULL;
    }
    AVL_element* firstChild() {
        return left_child ? left_child : right_child;
    }
    //Izmjene
    void substituteChild(AVL_element* old, AVL_element* new1) {
        if (left_child == old) left_child = new1;
        if (right_child == old) right_child = new1;
    }
    int recalculateValues() {
        int a = left_child != NULL ? left_child->depth : -1;
        int b = right_child != NULL ? right_child->depth : -1;
        int c = std::max(a, b) + 1;
        int d = a - b;
        depth = c;
        diff = d;
        return diff;
    }
};

class AVLTree {
private:
    AVL_element* root=NULL;
public:
    AVLTree() {
        return;
    }
    AVL_element* getRoot() { return root; }
    uint* getRootNode() { return &(root->value); }
    std::pair<AVL_element*, AVL_element*> getChildrenNodes(AVL_element* key)
    {
        return std::pair<AVL_element*, AVL_element*>(key->left_child, key->right_child);
    }
    std::pair<uint, uint> getChildrenNodesValues(uint* key = NULL)
    {
        AVL_element* el = key ? FindElement(*key) : root;
        std::pair<AVL_element*, AVL_element*> res;
        res = getChildrenNodes(el);
        std::pair<uint, uint> values;
        values = std::pair<uint, uint>(res.first->value,res.second->value);
    }

    AVL_element* GetFirst()
    {
        AVL_element* current = this->root;
        while (current->left_child)
        {
            current = current->left_child;
        }
        return current;
    }



    AVL_element* FindElement(uint value) {
        AVL_element* current = root;
        while (current && current->value!=value)
        {
            current = current->value > value ? current->left_child : current->right_child;
        }
        return current;
    }
    AVL_element* FindReplacementOf(AVL_element* element) {
        if (!element) return NULL;
        uint value = element->value;
        AVL_element* current = element;
        AVL_element* last = NULL;
        while (current)
        {
            last = current;
            current = current->value > value ? current->left_child : current->right_child;
        }
        return current ? current : last;
    }
    //-------------------------------------------------[Funkcije koje mijenjaju stablo]
    AVL_element* Rotate(AVL_element* base, bool rightRotate) {
        AVL_element* archbase;
        if (base == root) {
            AVL_element* temp = new AVL_element(0,*new Data());
            temp->left_child = base;
            base->parent = temp;
        }
        archbase = base->parent;
        AVL_element* substitute;
        if (rightRotate) {
            substitute = base->left_child;
            archbase->substituteChild(base, substitute);
            substitute->parent = archbase;

            base->left_child = substitute->right_child;
            if (base->left_child) base->left_child->parent = base;

            substitute->right_child = base;
            base->parent = substitute;
        }
        else
        {
            substitute = base->right_child;
            archbase->substituteChild(base, substitute);
            substitute->parent = archbase;

            base->right_child = substitute->left_child;
            if (base->right_child) base->right_child->parent = base;

            substitute->left_child = base;
            base->parent = substitute;
        }
        if (substitute->left_child) substitute->left_child->recalculateValues();
        if (substitute->right_child) substitute->right_child->recalculateValues();
        substitute->recalculateValues();
        if (base == root) {
            root = substitute;
            free(archbase);
            root->parent = NULL;
        } 
        return substitute;
    }
    void ReAdjust(AVL_element* last) {
        for (AVL_element* back = last; back != NULL; back = back->parent) {
            int size = back->depth;
            int d = back->recalculateValues();
            switch (d)
            {
            case 0:
            case 1:
            case -1:
                break;
            case 2:
                if (back->left_child && (back->left_child->diff < 0)) {
                    Rotate(back->left_child, false);
                }
                back=Rotate(back, true);
            case -2:
                if (back->right_child && (back->right_child->diff > 0)) {
                    Rotate(back->right_child, true);
                }
                back=Rotate(back, false);
            default:
                break;
            }
            if (size==back->depth) break;
        }
    }
    void InsertElement(uint key, Data data) {
        AVL_element* newElement = new AVL_element(key, data);
        if (root == NULL) {
            root = newElement;
            return;
        }
        AVL_element* curr=root;
        AVL_element* last = NULL;
        while (curr) {
            last = curr;
            curr = key < last->value ? last->left_child : last->right_child;
        }
        newElement->parent = last;
        if (key < last->value) {
            last->left_child = newElement;
        } else {
            last->right_child = newElement;
        }
        ReAdjust(last);
    }
    void RemoveElement(uint value, bool printResult = true) {
        AVL_element* el = FindElement(value);
        if (el) {
            RemoveElement(el);
        }
    }
    void RemoveElement(AVL_element* el) {
        int check = 0;
        if (el->left_child) check++;
        if (el->right_child) check++;
        if (check != 2 && el==root) {
            root = root->firstChild();
        }
        else {
            AVL_element* el2 = NULL;
            while (el->left_child || el->right_child) {
                el2 = FindReplacementOf(el);
                el->value = el2->value;
                el->data = el2->data;
                el = el2;
            }
            el2 = el->firstChild();
            el->parent->substituteChild(el, el2);
            if (el2) { el2->parent = el->parent; }
            ReAdjust(el);
        }
    }
};

class Maze
{
public:
    uint h, w;
    uint16_t** costs;
    Data** data;
    Maze(uint16_t** costs, uint h, uint w)
    {
        this->h = h;
        this->w = w;
        this->costs = costs;
        data = new Data * [h];
        for (uint i = 0; i < h; i++)
        {
            data[i] = new Data[w];
            for (uint j = 0; j < w; j++)
            {
                data[i][j] = Data(i,j,-1,nullptr);
            }
        }
    }
    void ApplyFor(uint r, uint c, uint val, AVL_element* X, AVLTree* Q)
    {
        //cout << "Zapis" << r << "," << c << endl;
        uint16_t v_n = val;
        v_n += costs[r][c];
        Data* N = new Data(r, c, v_n, &(X->data));
        Q->InsertElement(v_n,*N);
    }
    uint Run(uint src_r, uint src_c, uint dest_r, uint dest_c, vector<pair<uint, uint>>& solution)
    {
        Data firstInsert = Data(0, 0, costs[0][0]);
        AVLTree* Q = new AVLTree();
        Q->InsertElement(firstInsert.value, firstInsert);
        while (Q->getRoot() != NULL)
        {
            AVL_element* X = Q->GetFirst();
            uint r_c = X->data.row;
            uint c_c = X->data.column;
            uint16_t v_c = X->value;
            //cout << "Provjeravam " << r_c << "," << c_c << " za " << v_c << endl;
            Data dat = data[r_c][c_c];
            uint16_t datval = data[r_c][c_c].value;
            if (X->value < datval)
            {
                data[r_c][c_c] = X->data;
                if ((r_c == dest_r) && (c_c == dest_c))
                {
                    break;
                }
                if (r_c != 0) ApplyFor(r_c - 1, c_c, v_c, X, Q);
                if (c_c != 0) ApplyFor(r_c, c_c - 1, v_c, X, Q);
                if (r_c + 1 != this->h) ApplyFor(r_c + 1, c_c, v_c, X, Q);
                if (c_c + 1 != this->w) ApplyFor(r_c, c_c + 1, v_c, X, Q);
            }
            else
            {
                //cout << r_c << "," << c_c << "je manji:" << datval << endl;
            }
            Q->RemoveElement(X);
        }
        data[dest_r][dest_c].track(solution, data);
        return data[dest_r][dest_c].value;
    }
};

uint solve(const uint n, const uint m, uint16_t** const problem, vector<pair<uint, uint>>& solution)
{
    Maze *M = new Maze(problem, n, m);
    return M->Run(0, 0, n - 1, m - 1,solution);
}

//--------------------------------------------------------------------------------------------------------
//IO & testing

int intput(string purpose)
{
    cout << purpose << " ";
    uint data;
    cin >> data;
    return data;
}

pair<uint16_t**,pair<uint,uint>> ConsoleTestInput()
{
    uint n = intput("Broj redaka:");
    uint m = intput("Broj stupaca:");
    cout << "__Matrica cvijeca__" << endl;
    uint16_t** data;
    data = new uint16_t *[n];
    for (uint i = 0; i < n; i++)
    {
        data[i] = new uint16_t[m];
        for (uint j = 0; j < m; j++)
        {
            uint16_t x;
            cin >> x;
            data[i][j] = x;
        }
    }
    pair<uint, uint> p = *new pair<uint, uint>(n,m);
    return *(new pair<uint16_t**, pair<uint, uint>>(data, p));
}

int main()
{
    pair<uint16_t**, pair<uint, uint>> X = ConsoleTestInput();
    vector<pair<uint, uint>> result;
    uint solved = solve(X.second.first,X.second.second,X.first,result);
    cout << "Trosak: " << solved << endl;
    cout << "Putanja:" << endl;
    for (uint i = 0; i < result.size(); i++)
    {
        pair<uint, uint> res = result[i];
        cout << res.first << "," << res.second << endl;
    }
    return 0;
}
/* 5 5
2 1 0 0 0
0 1 0 1 0
0 1 0 1 0
0 1 0 1 0
0 0 0 1 0
*/

/* 3 4
100 200 1000 0 200 100 600 0 300 1600 100 0
*/