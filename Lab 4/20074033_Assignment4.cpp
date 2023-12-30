#include <iostream>
#include <string>
using namespace std;
#define DEBUG 0

enum NodeType { VALUE, OPERATION, NT_NONE };
enum Operation { PLUS, MULTIPLY, OP_NONE };
int uid() {
    static int i = 0;
    return ++i;
}

string getOperatorName(const Operation &op) {
    switch (op) {
    case PLUS:
        return "plus";
    case MULTIPLY:
        return "multiply";
    default:
        return "none";
    }
}

template <typename Ty> class Expression {
  private:
    const int id;
    NodeType nodeType;
    const Expression<Ty> *left;
    const Expression<Ty> *right;
    Operation op;
    Ty val;

    const void debug_print(const string &func, const int &pos,
                           const string &str, const int &idx) {
        if (DEBUG)
            cout << func << "\t" << pos << "." << idx << " : " << str << endl;
    }

    const Expression *func_E(const string &s, int &idx) {
        const Expression *T = func_T(s, idx);
        const Operation op = (idx < s.size() && s[idx] == '+') ? PLUS : OP_NONE;
        const Expression *Ed = func_Ed(s, idx);

        const Expression *E = new Expression(T, Ed, op);
        return E;
    }

    const Expression *func_T(const string &s, int &idx) {
        const Expression *F = func_F(s, idx);
        const Operation op =
            (idx < s.size() && s[idx] == '*') ? MULTIPLY : OP_NONE;
        const Expression *Td = func_Td(s, idx);

        const Expression *T = new Expression(F, Td, op);
        return T;
    }

    const Expression *func_Ed(const string &s, int &idx) {
        if (idx < s.size() && s[idx] == '+') {
            idx++;

            const Expression *T = func_T(s, idx);
            const Operation op =
                (idx < s.size() && s[idx] == '+') ? PLUS : OP_NONE;
            const Expression *Ed = func_Ed(s, idx);

            const Expression *Ed_ret = new Expression(T, Ed, op);
            return Ed_ret;
        }

        return NULL;
    }

    const Expression *func_Td(const string &s, int &idx) {
        if (idx < s.size() && s[idx] == '*') {
            idx++;

            const Expression *F = func_F(s, idx);
            const Operation op =
                (idx < s.size() && s[idx] == '*') ? MULTIPLY : OP_NONE;
            const Expression *Td = func_Ed(s, idx);

            const Expression *Td_ret = new Expression(F, Td, op);
            return Td_ret;
        }

        return NULL;
    }

    const Expression *func_F(const string &s, int &idx) {
        debug_print("F", 0, s, idx);
        if (s[idx] == '(') {
            int count = 1;
            idx++;
            string temp;

            while (count != 0) {
                temp += s[idx];
                idx++;

                if (s[idx] == ')')
                    count--;

                if (s[idx] == '(')
                    count++;
            }
            idx++;

            int temp_idx = 0;
            const Expression *E = func_E(temp, temp_idx);
            return E;
        }

        const Expression *val = new Expression(s[idx] - '0');
        idx++;
        return val;
    }

  public:
    Expression(const Expression *l, const Expression *r, const Operation oprtn)
        : id(uid()), nodeType(OPERATION), left(l), right(r), op(oprtn), val(0) {
        if (r == NULL)
            nodeType = NT_NONE;
    };

    Expression(const int i)
        : id(uid()), nodeType(VALUE), left(NULL), right(NULL), op(OP_NONE),
          val(i){};

    Expression(string s) : id(uid()) {
        int idx = 0;
        const Expression<Ty> *NE = func_E(s, idx);
        this->nodeType = NE->nodeType;
        this->left = NE->left;
        this->right = NE->right;
        this->op = NE->op;
        this->val = NE->val;
    }

    const void print_tree(const int &parent) const {

        cout << parent << "\t" << this->id << "\t";

        if (this->nodeType == NT_NONE) {
            cout << "pass" << endl;
            if (this->left)
                this->left->print_tree(this->id);
            return;
        }

        if (this->nodeType == OPERATION) {
            cout << "opt\t" << getOperatorName(this->op) << endl;
            if (this->left)
                this->left->print_tree(this->id);
            if (this->right)
                this->right->print_tree(this->id);
            return;
        }

        cout << "value\t" << this->val << endl;
    }
};

int main() {
    const string s = "(1+2)*";

    Expression<int> *NE = new Expression<int>(s);
    cout << "Parse Tree : " << endl;
    cout << "Parent\tChild\tType\tValue\n";
    cout << "--------------------------------\n";
    NE->print_tree(uid());
    return 0;
}