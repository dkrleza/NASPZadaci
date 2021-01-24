#include <iostream>
class Element
{
public:
    long value;
    Element* left = nullptr;
    Element* right = nullptr;
    Element(long _value)
    {
        value = _value;
    }
    Element* Direct(long newValue, bool* isRight)
    {
        long x = newValue - value;
        *isRight = x > 0;
        if (x == 0) return this;
        return *isRight ? right : left;
    }
};

class Tree
{
public:
    Element* root = nullptr;
    Element* Insert(long newValue)
    {
        Element* newEl = new Element(newValue);
        if (root==nullptr)
        {
            root = newEl;
        }
        Element* last = nullptr;
        Element* current = root;
        bool isRight = false;
        while (current != nullptr)
        {
            last = current;
            current = last->Direct(newValue, &isRight);
            if (current != last) return nullptr;
        }
        if (isRight) last->right = newEl;
        else last->left = newEl;
        return newEl;
    }
    void Mirror()
    {
        Element* current = root;
        Element* last = nullptr;
        Element* temp = nullptr;
        while (true)
        {
            if (current == last || current == current->left || current == current->right)
            {
                temp = current->right;
                current->right = last;
                last = current->left;
                current->left = temp;
                if (last == current)
                {
                    temp = current->left;
                    current->left = current->right;
                    current->right = temp;
                    break;
                }
                else
                {
                    int k = 0;
                    if (last==nullptr) k++;
                    if (current->left==nullptr) k++;
                    if (current->right==nullptr) k++;
                    switch (k)
                    {
                    case 0:
                        break;
                    case 1:
                        if (current->left == current->right)
                        {
                            current->right = last;
                            last = current->left;
                            current->left = nullptr;
                        }
                        else if (current->left == last)
                        {
                            last = current->right;
                            current->right = nullptr;
                        }
                        else
                        {
                            temp = current->right;
                            current->right = last;
                            last = current->left;
                            current->left = temp;
                            if ((current->left->value < current->value) && (current->value < current->right->value))
                            {
                                temp = current->left;
                                current->left = current->right;
                                current->right = temp;
                            }
                        }
                        break;
                    case 2:
                        if (current->left==nullptr) current->left = current->right;
                        else if (current->right==nullptr) current->right = last;
                        else last = current;
                        temp = current->right;
                        current->right = last;
                        last = current->left;
                        current->left = temp;
                        break;
                    case 3:
                        return;
                    default:
                        break;
                    }
                    if (last != nullptr)
                    {
                        temp = current;
                        current = last;
                        last = temp;
                    }
                }
            }
        }
    }
};

int main()
{
    std::cout << "Hello World!\n";
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
