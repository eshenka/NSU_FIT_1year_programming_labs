#include <stdio.h>
#include <malloc.h>

#define kMaxOperatorsSize 6
#define kMaxOperandsSize 10

const char operators[kMaxOperatorsSize] = "()+-*/";
const char operands[kMaxOperandsSize] = "0123456789";

enum Error {
    SYNTAX_ERROR,
    DIVISION_BY_ZERO,
    NO_ERROR,
};

enum TokenType {
    OPERATOR,
    OPERAND,
    INVALID,
};

struct Token {
    enum TokenType token_type;
    char operator;
    int operand;
};

struct Token Tokenize(char c) {
    struct Token t;
    for (int i = 0; i < kMaxOperatorsSize; i++) {
        if (c == operators[i]) {
            t.token_type = OPERATOR;
            t.operator = c;
            t.operand = 0;
            return t;
        }
    }
    for (int i = 0; i < kMaxOperandsSize; i++) {
        if (c == operands[i]) {
            int d = c - '0';
            t.token_type = OPERAND;
            t.operator = '0';
            t.operand = d;
            return t;
        }
    }
    t.token_type = INVALID;
    t.operator = '0';
    t.operand = 0;
    return t;
}

struct ListNode {
    struct Token token;
    struct ListNode *next;
};

struct ListNode *CreateListNode(struct Token t) {
    struct ListNode *head = (struct ListNode *) malloc(sizeof(struct ListNode));
    head->token = t;
    head->next = NULL;
    return head;
}

struct ListNode *PushList(struct ListNode *current, struct Token t) {
    struct ListNode *node = (struct ListNode *) malloc(sizeof(struct ListNode));
    current->next = node;
    node->token = t;
    node->next = NULL;
    return node;
}

struct ListNode *PopList(struct ListNode *head) {
    struct ListNode *tmp = head->next;
    free(head);
    return tmp;
}

void ClearListNode(struct ListNode *head) {
    while (head != NULL) {
        head = PopList(head);
    }
    free(head);
}

struct Stack {
    struct ListNode *top;
};

struct Stack *CreateStack() {
    struct Stack *stack = (struct Stack *) malloc(sizeof(struct Stack));
    stack->top = NULL;
    return stack;
}

void PushStack(struct Stack *stack, struct Token t) {
    struct ListNode *node = (struct ListNode *) malloc(sizeof(struct ListNode));
    node->token = t;
    node->next = stack->top;
    stack->top = node;
}


int IsEmpty(struct Stack *stack) {
    return stack->top == NULL;
}

struct Token GetTop(struct Stack *stack) {
    return stack->top->token;
}

struct Token PopStack(struct Stack *stack) {
    struct ListNode *current_top = stack->top;
    struct ListNode *next_top = current_top->next;
    stack->top = next_top;
    struct Token to_return = current_top->token;
    free(current_top);
    return to_return;
}

void ClearStack(struct Stack *stack) {
    while (!IsEmpty(stack)) {
        PopStack(stack);
    }
    free(stack);
}

int Priority(struct Token t) {
    if (t.operator == '(') {
        return 0;
    }
    if (t.operator == '+' || t.operator == '-') {
        return 1;
    }
    if (t.operator == '*' || t.operator == '/') {
        return 2;
    }
    if (t.operator == ')') {
        return 3;
    }
    return -1;
}

enum Error CreationTokenListNode(struct ListNode **head_infix) {
    char c;
    if (!scanf("%c", &c)) {
        return SYNTAX_ERROR;
    }
    struct Token t = Tokenize(c);
    if (t.token_type == INVALID) {
        return SYNTAX_ERROR;
    }
    *head_infix = CreateListNode(t);
    int is_met_digit = 0;
    if (t.token_type == OPERAND) {
        is_met_digit = 1;
    }
    struct ListNode *current = *head_infix;
    if (!scanf("%c", &c)) {
        return SYNTAX_ERROR;
    }
    while (c != '\n') {
        t = Tokenize(c);
        if (t.token_type == INVALID) {
            return SYNTAX_ERROR;
        }
        if (t.token_type == OPERAND && is_met_digit == 1) {
            current->token.operand = current->token.operand * 10 + t.operand;
            if (!scanf("%c", &c)) {
                return SYNTAX_ERROR;
            }
            continue;
        }
        if (t.token_type == OPERAND) {
            is_met_digit = 1;
        } else {
            is_met_digit = 0;
            if (t.operator == ')' && current->token.operator == '(') {
                return SYNTAX_ERROR;
            }
        }
        current = PushList(current, t);
        if (!scanf("%c", &c)) {
            return SYNTAX_ERROR;
        }
    }
    if (current->token.operator == '(') {
        return SYNTAX_ERROR;
    }
    return NO_ERROR;
}

void ClearMemory(struct ListNode *head_infix, struct ListNode *head_postfix) {
    ClearListNode(head_infix);
    ClearListNode(head_postfix);
}

enum Error CreationPostfixListNode(struct ListNode **head_infix, struct ListNode **head_postfix) {
    struct Stack *stack = CreateStack();
    struct Token t;
    struct ListNode *current = *head_postfix;
    while (*head_infix != NULL) {
        t = (*head_infix)->token;
        *head_infix = PopList(*head_infix);
        if (t.token_type == OPERAND) {
            if (*head_postfix == NULL) {
                *head_postfix = CreateListNode(t);
                current = *head_postfix;
                continue;
            }
            current = PushList(current, t);
        } else if (t.operator == '(') {
            PushStack(stack, t);
        } else if (t.operator == ')') {
            if (*head_postfix == NULL) {
                ClearStack(stack);
                return SYNTAX_ERROR;
            }
            if (IsEmpty(stack)) {
                ClearStack(stack);
                return SYNTAX_ERROR;
            }
            while (GetTop(stack).operator != '(') {
                current = PushList(current, PopStack(stack));
            }
            if (GetTop(stack).operator != '(') {
                ClearStack(stack);
                return SYNTAX_ERROR;
            }
            PopStack(stack);
        } else {
            if (*head_postfix == NULL) {
                ClearStack(stack);
                return SYNTAX_ERROR;
            }
            while (!IsEmpty(stack) && Priority(GetTop(stack)) >= Priority(t)) {
                current = PushList(current, PopStack(stack));
            }
            PushStack(stack, t);
        }
    }
    while (!IsEmpty(stack)) {
        struct Token is_bracket = PopStack(stack);
        if (is_bracket.operator == '(') {
            ClearStack(stack);
            return SYNTAX_ERROR;
        }
        current = PushList(current, is_bracket);
    }
    ClearStack(stack);
    return NO_ERROR;
}

struct Token ExpressionValue(struct Token t, struct Token a, struct Token b) {
    struct Token to_return;
    char operation = t.operator;
    if (operation == '+') {
        to_return.token_type = OPERAND;
        to_return.operator = '0';
        to_return.operand = b.operand + a.operand;
        return to_return;
    }
    if (operation == '-') {
        to_return.token_type = OPERAND;
        to_return.operator = '0';
        to_return.operand = b.operand - a.operand;
        return to_return;
    }
    if (operation == '/') {
        if (a.operand == 0) {
            to_return.token_type = INVALID;
            to_return.operator = '0';
            to_return.operand = a.operand;
            return to_return;
        }
        to_return.token_type = OPERAND;
        to_return.operator = '0';
        to_return.operand = b.operand / a.operand;
        return to_return;
    }
    to_return.token_type = OPERAND;
    to_return.operator = '0';
    to_return.operand = b.operand * a.operand;
    return to_return;
}

enum Error Calculate(int *answer, struct ListNode **head_postfix) {
    struct Stack *stack = CreateStack();
    struct Token t;
    while (*head_postfix != NULL) {
        t = (*head_postfix)->token;
        *head_postfix = PopList(*head_postfix);
        if (t.token_type == OPERAND) {
            PushStack(stack, t);
        } else {
            if (IsEmpty(stack)) {
                ClearStack(stack);
                return SYNTAX_ERROR;
            }
            struct Token a = PopStack(stack);
            if (IsEmpty(stack)) {
                ClearStack(stack);
                return SYNTAX_ERROR;
            }
            struct Token b = PopStack(stack);
            struct Token result = ExpressionValue(t, a, b);
            if (result.token_type == INVALID) {
                ClearStack(stack);
                return DIVISION_BY_ZERO;
            }
            PushStack(stack, result);
        }
    }
    *answer = PopStack(stack).operand;
    if (!IsEmpty(stack)) {
        ClearStack(stack);
        return SYNTAX_ERROR;
    }
    ClearStack(stack);
    return NO_ERROR;
}

int main() {
    struct ListNode *head_infix = NULL;
    enum Error error;
    error = CreationTokenListNode(&head_infix);
    if (error == SYNTAX_ERROR) {
        ClearListNode(head_infix);
        printf("syntax error");
        return 0;
    }
    struct ListNode *head_postfix = NULL;
    error = CreationPostfixListNode(&head_infix, &head_postfix);
    if (error == SYNTAX_ERROR) {
        ClearMemory(head_infix, head_postfix);
        printf("syntax error");
        return 0;
    }
    int answer = 0;
    error = Calculate(&answer, &head_postfix);
    switch (error) {
        case SYNTAX_ERROR:
            ClearMemory(head_infix, head_postfix);
            printf("syntax error");
            break;
        case DIVISION_BY_ZERO:
            ClearMemory(head_infix, head_postfix);
            printf("division by zero");
            break;
        default:
            ClearMemory(head_infix, head_postfix);
            printf("%d", answer);
            break;
    }
    return 0;
}
