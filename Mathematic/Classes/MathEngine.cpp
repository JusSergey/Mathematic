#include "MathEngine.h"

USING_NS_CC;
 
using namespace std;
using namespace StringUtils;


static const char openScobe  = '(';
static const char closeScobe = ')';
static const char space = ' ';

float MathEngine::calculate(const std::string &expr)
{
    int scobe(0);

    string str;

    for (int i = 0; i < expr.length(); i++) {

        if (expr[i] == openScobe) {

            scobe++;

            for (int j = i+1; j < expr.length(); j++) {

                if (expr[j] == openScobe ) scobe++;
                else
                if (expr[j] == closeScobe) scobe--;

                if (scobe == 0) {
                    str += toString(calculate(getSubStr(i+1, j-1, expr))) + ' ';
                    i = j+1;
                    break /* cycle j */ ;
                }

            } // end j cycle
            scobe = 0;

        } else str += expr[i];

    } // end i cycle

    return simpleCaclulate(str);
}

float MathEngine::simpleCaclulate(string expr)
{
    if(expr[expr.length()-1] != ' ')
        expr += ' ';

    vector<string> args;

    string tmp;

    for (int i = 0; i < expr.length(); i++) {

        if (expr[i] == ' ') {
            if (!tmp.empty()) {
                args.push_back(tmp);
                tmp.clear();
            }
        }

        else tmp += expr[i];
    }

    float result = atof(args[0].c_str());

    for (int i = 1; i < args.size(); i += 2)
        result = operation(result, (args[i])[0], Value(args[i+1]).asFloat());

    return result;
}

float MathEngine::operation(float a, char op, float b)
{
    switch (op) {
    case '+': return (a+b);
    case '-': return (a-b);
    case '*': return (a*b);
    case '/': return (a/b);
    case 'a': return (a*ABS(b));
    case 'A': return (a*ABS(b));
    case 's': return (a*sin(b));
    case 'S': return (a*sin(b));
    case 'c': return (a*cos(b));
    case 'C': return (a*cos(b));
    case 't': return (a*tan(b));
    case 'T': return (a*tan(b));
    case 'q': return (a*sqrt(b));
    case 'Q': return (a*sqrt(b));
    case '^': return (pow(a, b));
    default : return (0.0);
    }
}

string MathEngine::getSubStr(int start, int end, const string &str)
{
    string result;

    for(int i = start; i <= end; i++)
        result += (str[i]);

    return result;
}

string MathEngine::ToValidExpr(const string &expr)
{
    string result;

    string::size_type posFindSymbol(0);

    return result;


}

bool MathEngine::isValidSymbol(char ch)
{
    if (std::isdigit(ch) || ch == '.' || ch == ',')
        return true;

    return false;
}
