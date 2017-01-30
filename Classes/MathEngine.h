#ifndef __MATHENGINE_H__
#define __MATHENGINE_H__

#include "cocos2d.h"

namespace MathEngine {
using std::string;
string ToValidExpr(const string &expr);
float calculate (const string &expr);
float simpleCaclulate ( string expr);
float operation (float a, char op, float b);
string getSubStr (int start, int end, const string &str);
template <typename Type> inline constexpr Type ABS(const Type &arg) { return (arg < 0 ? -arg : arg); }
bool isValidSymbol(char ch);
}

#endif // __MATHENGINE_H__
//tan(x)
//(1 T x)
