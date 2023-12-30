#include <asm-generic/errno-base.h>
#include <cstdlib>
#include <iostream>
#include <map>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;
#define DEBUG 0

enum LexemeType {
    ID,
    OP_MULTIPLY,
    OP_ADD,
    LEFT_PAREN,
    RIGHT_PAREN,
    _E,
    _ED,
    _T,
    _TD,
    _F,
    EPSILON,
    END
};

string getTokenName(LexemeType t) {
    switch (t) {
    case ID:
        return "ID";
    case OP_MULTIPLY:
        return "*";
    case OP_ADD:
        return "+";
    case LEFT_PAREN:
        return "(";
    case RIGHT_PAREN:
        return ")";
    case _E:
        return "_E";
    case _ED:
        return "_ED";
    case _T:
        return "_T";
    case _TD:
        return "_TD";
    case _F:
        return "_F";
    case EPSILON:
        return "Ɛ";
    case END:
        return "$";
    default:
        return "ERROR";
    }
}

class Expression {
  private:
    int idx = 0;
    const string expression;
    stack<LexemeType> st;
    const map<const pair<LexemeType, LexemeType>, const vector<LexemeType>>
        parse_table = {{{_E, ID}, {_T, _ED}},
                       {{_T, ID}, {_F, _TD}},
                       {{_F, ID}, {ID}},
                       {{_ED, OP_ADD}, {OP_ADD, _T, _ED}},
                       {{_TD, OP_ADD}, {EPSILON}},
                       {{_TD, OP_MULTIPLY}, {OP_MULTIPLY, _F, _TD}},
                       {{_E, LEFT_PAREN}, {_T, _ED}},
                       {{_T, LEFT_PAREN}, {_F, _TD}},
                       {{_F, LEFT_PAREN}, {LEFT_PAREN, _E, RIGHT_PAREN}},
                       {{_ED, RIGHT_PAREN}, {EPSILON}},
                       {{_TD, RIGHT_PAREN}, {EPSILON}},
                       {{_ED, END}, {EPSILON}},
                       {{_TD, END}, {EPSILON}}};

    LexemeType getLexemeType(const char &c) {
        switch (expression[idx]) {
        case '(':
            return LEFT_PAREN;
        case ')':
            return RIGHT_PAREN;
        case '+':
            return OP_ADD;
        case '*':
            return OP_MULTIPLY;
        case ' ':
            return getNextToken();
        default:
            return ID;
        }
    }

    LexemeType getCurrentToken() {
        return idx < expression.size() ? getLexemeType(expression[idx]) : END;
    }

    LexemeType getNextToken() {
        idx++;
        return getCurrentToken();
    }

    LexemeType errorRoutine(LexemeType t) {
        cout << expression << endl;
        for(int i = 0; i<idx; i++) cout << ' ';
        cout << '^' << '\n';
        cout << "Failed at '" << getTokenName(t) << "' !\n";
        exit(EXIT_FAILURE);
    }

  public:
    Expression(const string &s, bool error_recovery) : expression{s} {

        st.push(_E);

        while (!st.empty()) {
            const LexemeType a = getCurrentToken();
            const LexemeType X = st.top();
            st.pop();

            if (X == a) {
                getNextToken();
                continue;
            }

            if (X < _E)
                errorRoutine(a);

            try {
                const vector<LexemeType> v = parse_table.at({X, a});
                const vector<LexemeType> vr(v.rbegin(), v.rend());

                for (const LexemeType &t : vr)
                    if (t != EPSILON)
                        st.push(t);

            } catch (const std::out_of_range &e) {
                if (error_recovery) {
                    cout << "Skipping " << getTokenName(a) << endl;
                    getNextToken();
                    st.push(X);
                } else
                    errorRoutine(a);
            }
        }

        if (idx < expression.size())
            errorRoutine(getCurrentToken());
    }
};

int main() {
    const string s = "1+(2**2)";
    Expression *e = new Expression(s, true);
    cout << "Success\n";
    return 0;
}