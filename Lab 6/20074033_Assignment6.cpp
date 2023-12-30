#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>

using namespace std;

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

const map<int, pair<LexemeType, vector<LexemeType>>> Grammar = {
    {1, {_E, {_T, _ED}}},
    {2, {_ED, {OP_ADD, _T, _ED}}},
    {3, {_ED, {EPSILON}}},
    {4, {_T, {_F, _TD}}},
    {5, {_TD, {OP_MULTIPLY, _F, _TD}}},
    {6, {_TD, {EPSILON}}},
    {7, {_F, {LEFT_PAREN, _E, RIGHT_PAREN}}},
    {8, {_F, {ID}}},
};

const map<pair<int, LexemeType>, string> Action = {
    {{0, LEFT_PAREN}, "s4"},    {{0, ID}, "s5"},
    {{1, END}, "accept"},       {{2, OP_ADD}, "s7"},
    {{2, END}, "r3"},           {{3, OP_ADD}, "r6"},
    {{3, OP_MULTIPLY}, "s9"},   {{3, END}, "r6"},
    {{4, LEFT_PAREN}, "s13"},   {{4, ID}, "s14"},
    {{5, OP_ADD}, "r8"},        {{5, OP_MULTIPLY}, "r8"},
    {{5, END}, "r8"},           {{6, END}, "r1"},
    {{7, LEFT_PAREN}, "s4"},    {{7, ID}, "s5"},
    {{8, OP_ADD}, "r4"},        {{8, END}, "r4"},
    {{9, LEFT_PAREN}, "s4"},    {{9, ID}, "s5"},
    {{10, RIGHT_PAREN}, "s17"}, {{11, OP_ADD}, "s19"},
    {{11, RIGHT_PAREN}, "r3"},  {{12, OP_ADD}, "r6"},
    {{12, OP_MULTIPLY}, "s21"}, {{12, RIGHT_PAREN}, "r6"},
    {{13, LEFT_PAREN}, "s13"},  {{13, ID}, "s14"},
    {{14, OP_ADD}, "r8"},       {{14, OP_MULTIPLY}, "r8"},
    {{14, RIGHT_PAREN}, "r8"},  {{15, OP_ADD}, "s7"},
    {{15, END}, "r3"},          {{16, OP_ADD}, "r6"},
    {{16, OP_MULTIPLY}, "s9"},  {{16, END}, "r6"},
    {{17, OP_ADD}, "r7"},       {{17, OP_MULTIPLY}, "r7"},
    {{17, END}, "r7"},          {{18, RIGHT_PAREN}, "r1"},
    {{19, LEFT_PAREN}, "s13"},  {{19, ID}, "s14"},
    {{20, OP_ADD}, "r4"},       {{20, RIGHT_PAREN}, "r4"},
    {{21, LEFT_PAREN}, "s13"},  {{21, ID}, "s14"},
    {{22, RIGHT_PAREN}, "s27"}, {{23, END}, "r2"},
    {{24, OP_ADD}, "r5"},       {{24, END}, "r5"},
    {{25, OP_ADD}, "s19"},      {{25, RIGHT_PAREN}, "r3"},
    {{26, OP_ADD}, "r6"},       {{26, OP_MULTIPLY}, "s21"},
    {{26, RIGHT_PAREN}, "r6"},  {{27, OP_ADD}, "r7"},
    {{27, OP_MULTIPLY}, "r7"},  {{27, RIGHT_PAREN}, "r7"},
    {{28, RIGHT_PAREN}, "r2"},  {{29, OP_ADD}, "r5"},
    {{29, RIGHT_PAREN}, "r5"},
};

map<pair<int, LexemeType>, int> Goto = {
    {{0, _E}, 1},    {{0, _T}, 2},    {{0, _F}, 3},    {{2, _ED}, 6},
    {{3, _TD}, 8},   {{4, _E}, 10},   {{4, _T}, 11},   {{4, _F}, 12},
    {{7, _T}, 15},   {{7, _F}, 3},    {{9, _F}, 16},   {{11, _ED}, 18},
    {{12, _TD}, 20}, {{13, _E}, 22},  {{13, _T}, 11},  {{13, _F}, 12},
    {{15, _ED}, 23}, {{16, _TD}, 24}, {{19, _T}, 25},  {{19, _F}, 12},
    {{21, _F}, 26},  {{25, _ED}, 28}, {{26, _TD}, 29},
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
    stack<int> stateStack;
    stack<LexemeType> symbolStack;

    LexemeType getLexemeType(const char &c) {
        switch (c) {
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
        for (int i = 0; i < idx; i++)
            cout << ' ';
        cout << '^' << '\n';
        cout << "Failed at '" << getTokenName(t) << "' !\n";
        exit(EXIT_FAILURE);
    }

  public:
    Expression(const string &s, bool error_recovery) : expression{s} {
        symbolStack.push(END);
        stateStack.push(0);

        while (true) {
            const LexemeType current_lexeme = getCurrentToken();
            const int top_state = stateStack.top();

            if (!Action.count({top_state, current_lexeme}))
                errorRoutine(current_lexeme);
            string action = Action.at({top_state, current_lexeme});

            if (action.front() == 's') {
                action = action.substr(1);
                stateStack.push(stoi(action));
                symbolStack.push(current_lexeme);
                getNextToken();

            } else if (action.front() == 'r') {
                action = action.substr(1);

                const pair<LexemeType, vector<LexemeType>> grammar =
                    Grammar.at(stoi(action));
                int len = grammar.second.size();

                if (grammar.second.front() == EPSILON)
                    len = 0;

                while (len--) {
                    stateStack.pop();
                    symbolStack.pop();
                }

                int t = stateStack.top();
                stateStack.push(Goto.at({t, grammar.first}));
                symbolStack.push(grammar.first);


            } else if (action == "accept") {
                cout << "Accepted" << endl;
                return;
            } else {
                errorRoutine(current_lexeme);
            }
        }
        errorRoutine(getCurrentToken());
    }
};

int main() {
    const string s = "1*1*+(1+1)";
    Expression *e = new Expression(s, true);
    cout << "Success\n";
    return 0;
}