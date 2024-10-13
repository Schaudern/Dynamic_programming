#include <fstream>
#include <iostream>
#include <algorithm>
#include <typeinfo>

using namespace std;


struct well
{
    int profit;
    int cost;
};

struct coordinates
{
    int row;
    int column;
};

class Exception : public exception
{
protected:
    string str;
public:
    Exception(string s)
    {
        str = s;
    }
    virtual void print()
    {
        cout << "Exception: " << str;
    }
};

class BigNumbersException : public Exception
{
protected:
    int budg;
    int count;
public:
    BigNumbersException(string s, int b, int c) : Exception(s)
    {
        budg = b;
        count = c;
    }
    virtual void print()
    {
        cout << "BigNumbersException: " << str << "Budget = " << budg << ", " << "count = " << count;
    }
};

class BigNumbersBeiFixLimit : public BigNumbersException
{
public:
    BigNumbersBeiFixLimit(string s, int c, int b = 3000000) : BigNumbersException(s, b, c) {}
    void print()
    {
        cout << "BigNumbersException: " << str << "count = " << count;
    }
};

class WrongIndexesException : public Exception
{
protected:
    int indexc;
    int indexr;
public:
    WrongIndexesException(string s, int indx1, int indx2) : Exception(s)
    {
        indexr = indx1;
        indexc = indx2;
    }
    virtual void print()
    {
        cout << "WrongIndexesException: " << str << "Row index = " << indexr << ", " << "Column index = " << indexc;
    }
};

class NegativeElementException : public Exception
{
protected:
    int row;
    int column;
public:
    NegativeElementException(string s, int c, int p) : Exception(s)
    {
        row = c;
        column = p;
    }
    virtual void print()
    {
        cout << "NegativeElementException: " << str << "Row " << row << ", " << "Column " << column;
    }
};

class NegativeParameterException : public Exception
{
protected:
    int num;
    string inf;
public:
    NegativeParameterException(string s, string Inf, int n) : Exception(s)
    {
        inf = Inf;
        num = n;
    }
    virtual void print()
    {
        cout << "NegativeParameterException: " << str << inf << " = " << num;
    }
};

class FixBudget
{
protected:
    well* ptr;
    int height;
    int width;
    int budget;
public:
    FixBudget(ifstream& f, int Budget = 3000000)
    {
        f >> height;
        f >> width;
        if (height <= 0)
            throw NegativeParameterException("Height must be positive. ", "Height", height);
        if (width <= 0)
            throw NegativeParameterException("Width must be positive. ", "Width", width);
        if (Budget < 0)
            throw NegativeParameterException("Budget must be non-negative. ", "Budget", Budget);
        budget = Budget;
        if (height * width * budget > 1200000000 and budget != 0)
            throw BigNumbersException("The product of height, width and budget must not exceed 1200000000. ", budget, height * width);
        if (height * width > 1200000000)
            throw BigNumbersException("The product of height, width and budget must not exceed 1200000000. ", budget, height * width);
        ptr = new well[height * width];
        int k = 0;
        while (!f.eof())
        {
            for (int i = 0; i < width; i++)
            {
                f >> ptr[i + k].profit;
                if (ptr[i + k].profit < 0)
                    throw NegativeElementException("Profits and costs must be non-negative. ", (i + k) / width, (i + k) % width);
            }
            for (int i = 0; i < width; i++)
            {
                f >> ptr[i + k].cost;
                if (ptr[i + k].cost < 0)
                    throw NegativeElementException("Profits and costs must be non-negative. ", (i + k) / width, (i + k) % width);
            }
            k = k + width;
        }
    }

    FixBudget(int H = 5, int W = 5)
    {
        if (H <= 0)
            throw NegativeParameterException("Height must be positive. ", "Height", H);
        if (W <= 0)
            throw NegativeParameterException("Width must be positive. ", "Width", W);
        height = H;
        width = W;
        ptr = new well[height * width];
        for (int i = 0; i < height * width; i++)
        {
            ptr[i].profit = rand() % 900 + 200;
            ptr[i].cost = rand() % 100000 + 10000;
        }
        budget = rand() % 100000 + 10000;
        if (height * width * budget > 1200000000)
            throw BigNumbersException("The product of height, width and budget must not exceed 1200000000. ", budget, height * width);
    }

    FixBudget(const FixBudget& objekt)
    {
        budget = objekt.budget;
        height = objekt.height;
        width = objekt.width;
        ptr = new well[height * width];
        for (int i = 0; i < height * width; i++)
        {
            ptr[i] = objekt.ptr[i];
        }
    }

    ~FixBudget()
    {
        if (ptr != NULL)
        {
            delete[] ptr;
        }
    }

    well getElement(int indexR, int indexC)
    {
        if (indexR >= height or indexC >= width or indexR < 0 or indexC < 0)
            throw WrongIndexesException("You are out of bounds array. ", indexR, indexC);
        return ptr[indexR * width + indexC];
    }

    int getHeight()
    {
        return height;
    }

    int getWidth()
    {
        return width;
    }

    int getBudget()
    {
        return budget;
    }

    void setBudget(int B)
    {
        if (B < 0)
            throw NegativeParameterException("Budget must be non-negative. ", "Budget", B);
        if (height * width * B > 1200000000)
            throw BigNumbersException("The product of height, width and budget must not exceed 1200000000. ", B, height * width);
        budget = B;
    }

    void setElement(well element, int indexR, int indexC)
    {
        if (indexR >= height or indexC >= width or indexR < 0 or indexC < 0)
            throw WrongIndexesException("You are out of bounds array. ", indexR, indexC);
        if (element.cost < 0 or element.profit < 0)
            throw NegativeElementException("Profits and costs must be non-negative. ", indexR, indexC);
        ptr[indexR * width + indexC] = element;
    }

    void Remove(int indexR, int indexC)
    {
        if (indexR >= height or indexC >= width or indexR < 0 or indexC < 0)
            throw WrongIndexesException("You are out of bounds array. ", indexR, indexC);
        ptr[indexR * width + indexC].cost = 0;
        ptr[indexR * width + indexC].profit = 0;
    }

    void push(well element)
    {

        for (int i = 0; i < height * width; i++)
        {
            if (ptr[i].cost == 0 and ptr[i].profit == 0)
            {
                if (element.cost < 0 or element.profit < 0)
                    throw NegativeElementException("Profits and costs must be non-negative. ", (i + 1) / width, (i + 1) % width);
                ptr[i].cost = element.cost;
                ptr[i].profit = element.profit;
                return;
            }
        }
        throw Exception("There is no free space for a new well in field.");
    }

    coordinates* MaxProfit(int& count)
    {
        int** subtask = new int* [height * width];
        for (int i = 0; i < height * width; i++)
        {
            subtask[i] = new int[budget];
        }

        if (ptr[0].cost <= 1)
        {
            subtask[0][0] = ptr[0].profit;
        }
        else
        {
            subtask[0][0] = 0;
        }

        for (int i = 1; i < height * width; i++)
        {
            if (ptr[i].cost <= 1 and ptr[i].profit > subtask[i - 1][0])
            {
                subtask[i][0] = ptr[i].profit;
            }
            else
            {
                subtask[i][0] = subtask[i - 1][0];
            }
        }

        for (int i = 1; i < budget; i++)
        {
            if (ptr[0].cost > i + 1)
            {
                subtask[0][i] = 0;
            }
            else
            {
                subtask[0][i] = ptr[0].profit;
            }
        }

        for (int i = 1; i < budget; i++)
        {
            for (int j = 1; j < height * width; j++)
            {
                if (i + 1 - ptr[j].cost > 0)
                {
                    if (subtask[j - 1][i] < subtask[j - 1][i - ptr[j].cost] + ptr[j].profit)
                    {
                        subtask[j][i] = subtask[j - 1][i - ptr[j].cost] + ptr[j].profit;
                    }
                    else
                    {
                        subtask[j][i] = subtask[j - 1][i];
                    }
                }
                else if (i + 1 - ptr[j].cost == 0)
                {
                    if (ptr[j].profit > subtask[j - 1][i])
                    {
                        subtask[j][i] = ptr[j].profit;
                    }
                    else
                    {
                        subtask[j][i] = subtask[j - 1][i];
                    }
                }

                else
                {
                    subtask[j][i] = subtask[j - 1][i];
                }
            }
        }

        coordinates* rez = new coordinates[width * height];
        int i = height * width - 1;
        int j = budget - 1;
        count = 0;
        while (i != -1 and subtask[i][j] != 0 and j >= 0)
        {
            int i2 = i;
            int j2 = j;
            while (i != -1 and subtask[i][j] == subtask[i2][j2])
            {
                i--;
            }
            rez[count].row = (i + 1) / width;
            rez[count].column = (i + 1) % width;
            j = j - ptr[i + 1].cost;
            count++;
        }
        for (int i = 0; i < height * width; i++)
        {
            delete[] subtask[i];
        }
        delete[] subtask;
        return rez;
    }
    friend ostream& operator<<(ostream& s, FixBudget w);
};

ostream& operator<<(ostream& s, FixBudget w)
{
    s << '\n';
    s << '\n';
    s << "budget: " << w.budget << "$";
    s << '\n';
    int k = 0;
    for (int j = 0; j < w.height; j++)
    {
        for (int i = 0; i < w.width; i++)
        {
            s << w.ptr[i + k].profit;
            s << '\t';
        }
        s << '\n';
        for (int i = 0; i < w.width; i++)
        {
            s << w.ptr[i + k].cost;
            s << '\t';
        }
        s << '\n';
        s << '\n';
        k += w.width;
    }
    return s;
}

bool cmp(well s, well n)
{
    return s.profit > n.profit;
}

class FixLimit : public FixBudget
{
protected:
    int limitation;
public:
    FixLimit(ifstream& f, int Limit) : FixBudget(f)
    {
        limitation = Limit;
    }

    FixLimit(int H = 5, int W = 5)
    {
        if (H <= 0)
            throw NegativeParameterException("Height must be positive. ", "Height", H);
        if (W <= 0)
            throw NegativeParameterException("Width must be positive. ", "Width", W);
        if (H * W > 25000000)
            throw BigNumbersBeiFixLimit("Too many elements. ", H * W);
        height = H;
        width = W;
        ptr = new well[height * width];
        for (int i = 0; i < height * width; i++)
        {
            ptr[i].profit = rand() % 900 + 200;
            ptr[i].cost = rand() % 100000 + 10000;
        }
        limitation = rand() % height * width + 1;
        budget = 3000000;
    }

    FixLimit(const FixLimit& objekt) : FixBudget(objekt)
    {
        limitation = objekt.limitation;
    }

    ~FixLimit() {}

    void setLimitation(int limit)
    {
        if (limit < 0)
            throw NegativeParameterException("Limitation must be non-negative. ", "Limitation", limit);
        limitation = limit;
    }

    coordinates* MaxProfit(int& count)
    {
        FixLimit obj = *this;
        for (int i = 0; i < height * width; i++)
        {
            obj.ptr[i].cost = i;
        }
        sort(obj.ptr, obj.ptr + height * width, cmp);
        coordinates* rez = new coordinates[width * height];
        if (limitation < height * width)
        {
            count = limitation;
            for (int i = 0; i < limitation; i++)
            {
                rez[i].row = obj.ptr[i].cost / width;
                rez[i].column = obj.ptr[i].cost % width;
            }
        }
        else
        {
            count = height * width;
            for (int i = 0; i < height * width; i++)
            {
                rez[i].row = obj.ptr[i].cost / width;
                rez[i].column = obj.ptr[i].cost % width;
            }
        }
        return rez;
    }
    friend ostream& operator<<(ostream& s, FixLimit w);
};

ostream& operator<<(ostream& s, FixLimit w)
{
    s << '\n';
    s << '\n';
    s << "limitation: " << w.limitation;
    s << '\n';
    int k = 0;
    for (int j = 0; j < w.height; j++)
    {
        for (int i = 0; i < w.width; i++)
        {
            s << w.ptr[i + k].profit;
            s << '\t';
        }
        s << '\n';
        for (int i = 0; i < w.width; i++)
        {
            s << w.ptr[i + k].cost;
            s << '\t';
        }
        s << '\n';
        s << '\n';
        k += w.width;
    }
    return s;
}

void PrintAntwort(coordinates* arr, int len)
{
    cout << '\n';
    cout << "count = " << len;
    cout << '\n';
    for (int i = 0; i < len; i++)
    {
        cout << "(";
        cout << arr[i].row;
        cout << "; ";
        cout << arr[i].column;
        cout << ")";
        cout << '\n';
    }
}

int main()
{
    ifstream fin("example_of_an_input_file.txt");
    if (fin)
    {
        FixBudget test(fin, 67891);
        cout << test;
        fin.close();
        int count = 0;
        coordinates* res = test.MaxProfit(count);
        PrintAntwort(res, count);
    }

    char c; cin >> c;

    return 0;
}


