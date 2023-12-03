#include <stdio.h>

const char *s = "1000-7*100/(30+5*10-5*(100/50))+1";
size_t      p = 0;

int get_G();
int get_E();
int get_T();
int get_P();
int get_N();
void syn_assert(int cond);

int main()
{
    printf("Res: <%d>", get_G());

    return 0;
}

void syn_assert(int cond)
{
    if (!cond)
        printf("SYNTAX ERROR:\n%s\n^\n", s + p);
}

int get_N()
{
    int val = 0;
    size_t old_p = p;
    while ( '0' <= s[p] && s[p] <= '9' )
    {
        val = val*10 + (s[p] - '0');
        p++;
    }
    syn_assert(p > old_p);
    return val;
}

int get_E()
{
    int val = get_T();
    while ( s[p] == '+' || s[p] == '-')
    {
        char op = s[p];
        p++;
        int val2 = get_T();
        switch (op)
        {
        case '+':
            val += val2;
            break;
        case '-':
            val -= val2;
            break;
        default:
            syn_assert(0);
            break;
        }
    }
    return val;
}

int get_T()
{
    int val = get_P();
    while ( s[p] == '*' || s[p] == '/')
    {
        char op = s[p];
        p++;
        int val2 = get_P();
        switch (op)
        {
        case '*':
            val *= val2;
            break;
        case '/':
            val /= val2;
            break;
        default:
            syn_assert(0);
            break;
        }
    }
    return val;
}

int get_P()
{
    if (s[p] == '(')
    {
        int val = 0;
        p++;
        val = get_E();
        syn_assert(s[p] == ')');
        p++;
        return val;
    }
    else
    {
        return get_N();
    }
}

int get_G()
{
    int val = get_E();
    syn_assert(s[p] == '\0');
    return val;
}
