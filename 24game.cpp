// 给定1-9范围内的4个数字，判断是否可以用+-*/()来得到24
// 思路是对4个数字进行全排列，然后在各个位置插入运算符形成后序表达式，算出所有可能的结果，判断是否有24
// 如果不让用括号，那就换成中序表达式，算的时候先算一遍*/，再算一遍+-，很好改
// 官方题解的思路是每次从列表里取两个数进行运算并用结果取代原来的两个数，这样每次会减少一个数，一步步将列表长度缩小至1，判断是否为24
// 这份代码可以用来玩游戏作弊了哈哈哈

class Solution
{
public:
    bool is24(const string& express)
    {
        if (!is24(cal(express))) return false;
        cout << express << endl;
        return true;
    }

    bool is24(float num)
    {
        static const float e = 1e-5;
        return num - 24 > -e && num - 24 < e;
    }

    float cal(const string& express)
    {
        //cout << express << endl;
        stack<float> si;
        for (size_t i = 0; i < express.size(); ++i)
        {
            if (express[i] == ' ') continue;

            if ('1' <= express[i] && '9' >= express[i])
            {
                si.push(express[i] - '0');
            }
            else
            {
                float b = si.top();
                si.pop();
                float a = si.top();
                si.pop();
                switch (express[i])
                {
                    case '+':
                        si.push(a + b);
                        break;
                    case '-':
                        si.push(a - b);
                        break;
                    case '*':
                        si.push(a * b);
                        break;
                    case '/':
                        si.push(a / b);
                        break;
                }
            }
        }
        return si.top();
    }

    template <typename T>
    void printV(const vector<T>& nums)
    {
        for (size_t i = 0; i < nums.size(); ++i) cout << nums[i] << " ";
        cout << endl;
    }

    bool judgePoint24(vector<int>& nums)
    {
        static const vector<char> ops = {' ', '+', '-', '*', '/'};

        sort(nums.begin(), nums.end());
        bool next = true;
        while (next)
        {
            for (size_t i = 0; i < ops.size(); ++i)
                for (size_t j = 0; j < ops.size(); ++j)
                    for (size_t k = 1; k < ops.size(); ++k)
                    {
                        char exp[10];
                        if (i == 0 && j == 0)
                        {
                            for (size_t l = 1; l < ops.size(); ++l)
                                for (size_t m = 1; m < ops.size(); ++m)
                                {
                                    sprintf(exp,
                                            "%d%d%d%d%c%c%c",
                                            nums[0],
                                            nums[1],
                                            nums[2],
                                            nums[3],
                                            ops[k],
                                            ops[l],
                                            ops[m]);
                                    if (is24(exp)) return true;
                                }
                        }
                        else if (i == 0 || j == 0)
                        {
                            for (size_t l = 1; l < ops.size(); ++l)
                            {
                                sprintf(exp,
                                        "%d%d%c%d%c%d%c%c",
                                        nums[0],
                                        nums[1],
                                        ops[i],
                                        nums[2],
                                        ops[j],
                                        nums[3],
                                        ops[k],
                                        ops[l]);
                                if (is24(exp)) return true;
                            }
                        }
                        else
                        {
                            sprintf(exp, "%d%d%c%d%c%d%c", nums[0], nums[1], ops[i], nums[2], ops[j], nums[3], ops[k]);
                            if (is24(exp)) return true;
                        }
                    }
            next = next_permutation(nums.begin(), nums.end());
        }
        return false;
    }
};
