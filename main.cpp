#include <iostream>
#include <stack>
#include <chrono>

bool areParenthesesBalanced(const std::string& formula) {
    std::stack<char> s;
    for (char ch : formula) {
        if (ch == '(') {
            s.push(ch);
        } else if (ch == ')') {
            if (s.empty()) return false; // Closing parenthesis without opening
            s.pop();
        }
    }
    return s.empty(); // Check if all opened parentheses are closed
}

bool hasValidCharacters(const std::string& formula) {
    for (char ch : formula) {
        if (!isalnum(ch) && ch != '(' && ch != ')' &&
            ch != '~' && ch != '|' && ch != '&' && ch != '-' &&
            !isspace(ch)) {
            return false;
        }
    }
    return true;
}

bool isValidFormula(const std::string& formula) {
    if (formula.empty()) return false;

    char prev = '\0'; // Tracks the previous character
    for (size_t i = 0; i < formula.length(); ++i) {
        char ch = formula[i];

        if (isalnum(ch)) {
            // Ensure variables are single letters
            if (isalpha(ch) && prev != '\0' && isalnum(prev)) return false;
        } else if (ch == '~' || ch == '|' || ch == '&' || ch == '-') {
            // Check for "->" for implies
            if (ch == '-' && i + 1 < formula.length() && formula[i + 1] == '>') {
                ++i; // Skip '>' character in "->"
            } else {
                // Operators must not appear consecutively
                if (prev == '~' || prev == '|' || prev == '&' || prev == '-') return false;
            }
        } else if (ch == '(' || ch == ')') {
            // Parentheses validation already handled separately
            continue;
        }
        prev = ch;
    }
    return true;
}



int main() {
    std::string formula;
    std::cout << "Enter a propositional logic formula: ";
    std::getline(std::cin, formula);

    auto start = std::chrono::high_resolution_clock::now();

    if (!hasValidCharacters(formula)) {
        std::cout << "Error: Formula contains invalid characters.\n";
    } else if (!areParenthesesBalanced(formula)) {
        std::cout << "Error: Parentheses are not balanced.\n";
    } else if (!isValidFormula(formula)) {
        std::cout << "Error: Formula is not well-formed.\n";
    } else {
        std::cout << "The formula is well-formed.\n";
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Verification time: " << elapsed.count() << " seconds.\n";

    return 0;
}

