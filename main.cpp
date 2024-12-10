#include <iostream>
#include <stack>
#include <chrono>
#include <string>
#include <unordered_map>

std::string errorMessage;

bool areParenthesesBalanced(const std::string& formula) {
    std::stack<int> s;
    for (size_t i = 0; i < formula.length(); ++i) {
        char ch = formula[i];
        if (ch == '(') {
            s.push(i);
        } else if (ch == ')') {
            if (s.empty()) {
                errorMessage = "Unbalanced parenthesis at position " + std::to_string(i);
                return false;
            }
            s.pop();
        }
    }
    if (!s.empty()) {
        errorMessage = "Unbalanced parenthesis at position " + std::to_string(s.top());
        return false;
    }
    return true;
}

bool hasValidCharacters(const std::string& formula) {
    for (size_t i = 0; i < formula.length(); ++i) {
        char ch = formula[i];
        if (!isalnum(ch) && ch != '(' && ch != ')' &&
            ch != '~' && ch != '|' && ch != '&' && ch != '-' &&
            !isspace(ch)) {
            errorMessage = "Invalid character '" + std::string(1, ch) + "' at position " + std::to_string(i);
            return false;
        }
    }
    return true;
}

bool isValidFormula(const std::string& formula) {
    if (formula.empty()) {
        errorMessage = "Formula is empty.";
        return false;
    }

    char prev = '\0';
    bool expectingOperand = true;

    for (size_t i = 0; i < formula.length(); ++i) {
        char ch = formula[i];

        if (isalnum(ch)) {
            if (!expectingOperand) {
                errorMessage = "Unexpected operand at position " + std::to_string(i);
                return false;
            }
            expectingOperand = false;
        } else if (ch == '~') {
            if (!expectingOperand) {
                errorMessage = "Unexpected operator '~' at position " + std::to_string(i);
                return false;
            }
        } else if (ch == '|' || ch == '&') {
            if (expectingOperand || prev == '(' || prev == '~') {
                errorMessage = "Unexpected operator '" + std::string(1, ch) + "' at position " + std::to_string(i);
                return false;
            }
            expectingOperand = true;
        } else if (ch == '-') {
            if (i + 1 < formula.length() && formula[i + 1] == '>') {
                if (expectingOperand || prev == '(' || prev == '~') {
                    errorMessage = "Unexpected operator '->' at position " + std::to_string(i);
                    return false;
                }
                expectingOperand = true;
                ++i; // Skip the '>' character
            } else {
                errorMessage = "Unexpected character '-' at position " + std::to_string(i);
                return false;
            }
        } else if (ch == '(') {
            if (!expectingOperand && prev != '(' && prev != '~') {
                errorMessage = "Unexpected '(' at position " + std::to_string(i);
                return false;
            }
            expectingOperand = true;
        } else if (ch == ')') {
            if (expectingOperand || prev == '(' || prev == '|' || prev == '&' || prev == '-') {
                errorMessage = "Unexpected ')' at position " + std::to_string(i);
                return false;
            }
        } else if (isspace(ch)) {
            continue;
        } else {
            errorMessage = "Invalid character '" + std::string(1, ch) + "' at position " + std::to_string(i);
            return false;
        }

        prev = ch;
    }

    if (expectingOperand) {
        errorMessage = "Formula ends with an incomplete expression.";
        return false;
    }
    return true;
}

int main() {
    std::string formula;

    // Keep the program running until the user decides to exit
    while (true) {
        std::cout << "\nEnter a propositional logic formula (or type 'exit' to quit): ";
        std::getline(std::cin, formula);

        // Check if the user wants to exit
        if (formula == "exit" || formula == "q") {
            std::cout << "Exiting the program.\n";
            break;
        }

        // Collect variable assignments
        std::unordered_map<char, bool> variableAssignments;
        char variable;
        bool value;
        std::cout << "Enter variable assignments (e.g., A=1 for true, A=0 for false). Enter 'q' to skip.\n";
        while (true) {
            std::cout << "Enter variable (or 'q' to skip): ";
            std::cin >> variable;
            if (variable == 'q') break;

            std::cout << "Enter value (1 for true, 0 for false): ";
            std::cin >> value;
            variableAssignments[variable] = value;
        }
        std::cin.ignore(); // To consume the newline character left in the input buffer

        auto overallStart = std::chrono::high_resolution_clock::now();

        auto start = std::chrono::high_resolution_clock::now();
        bool validChars = hasValidCharacters(formula);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> charTime = end - start;

        start = std::chrono::high_resolution_clock::now();
        bool balanced = validChars && areParenthesesBalanced(formula);
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> balanceTime = end - start;

        start = std::chrono::high_resolution_clock::now();
        bool validSyntax = balanced && isValidFormula(formula);
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> syntaxTime = end - start;

        auto overallEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> totalTime = overallEnd - overallStart;

        if (!validChars || !balanced || !validSyntax) {
            std::cout << "Error: " << errorMessage << "\n";
        } else {
            std::cout << "The formula is well-formed.\n";
        }

        std::cout << "\nTiming Analysis:\n";
        std::cout << "  - Character Validation: " << charTime.count() << " seconds\n";
        std::cout << "  - Parentheses Check: " << balanceTime.count() << " seconds\n";
        std::cout << "  - Syntax Validation: " << syntaxTime.count() << " seconds\n";
        std::cout << "  - Total Time: " << totalTime.count() << " seconds\n";
    }

    return 0;
}