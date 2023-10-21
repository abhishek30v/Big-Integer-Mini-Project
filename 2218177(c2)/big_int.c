#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    int digit;
    struct Node *link;
} Node;

typedef struct BigInt
{
    Node *head;
    int size;
} BigInt;

Node *nodeCreate(int digit)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->digit = digit;
    newNode->link = NULL;
    return newNode;
}

void appendDigit(BigInt *num, int digit)
{
    Node *newNode = nodeCreate(digit);
    newNode->link = num->head;
    num->head = newNode;
    num->size++;
}

int compare(BigInt *a, BigInt *b)
{
    if (a->size > b->size)
    {
        return 1;
    }
    else if (a->size < b->size)
    {
        return -1;
    }

    Node *current1 = a->head;
    Node *current2 = b->head;

    while (current1 != NULL && current2 != NULL)
    {
        if (current1->digit > current2->digit)
        {
            return 1;
        }
        else if (current1->digit < current2->digit)
        {
            return -1;
        }

        current1 = current1->link;
        current2 = current2->link;
    }

    return 0;
}

BigInt *BigIntCreate(const char *numStr)
{
    BigInt *num = (BigInt *)malloc(sizeof(BigInt));
    num->head = NULL;
    num->size = 0;
    int i;

    int len = strlen(numStr);
    for (i = 0; i < len; i++)
    {
        int digit = numStr[i] - '0';
        appendDigit(num, digit);
    }

    return num;
}
BigInt *BigIntCreateFromInt(int num)
{
    char numStr[1000];
    snprintf(numStr, sizeof(numStr), "%d", num);
    return BigIntCreate(numStr);
}
void copy(BigInt *dest, BigInt *src)
{
    dest->size = src->size;
    dest->head = NULL;

    Node *current = src->head;
    while (current != NULL)
    {
        appendDigit(dest, current->digit);
        current = current->link;
    }
}

void destroyBigInt(BigInt *num)
{
    while (num->head != NULL)
    {
        Node *temp = num->head;
        num->head = num->head->link;
        free(temp);
    }
    free(num);
}

void printBigInt(BigInt *num)
{
    Node *current = num->head;
    while (current != NULL)
    {
        printf("%d", current->digit);
        current = current->link;
    }
    printf("\n");
}

BigInt *add(BigInt *a, BigInt *b)
{
    BigInt *result = (BigInt *)malloc(sizeof(BigInt));
    result->head = NULL;
    result->size = 0;

    int carry = 0;
    Node *current1 = a->head;
    Node *current2 = b->head;

    while (current1 != NULL || current2 != NULL || carry != 0)
    {
        int digit1 = (current1 != NULL) ? current1->digit : 0;
        int digit2 = (current2 != NULL) ? current2->digit : 0;

        int sum = digit1 + digit2 + carry;
        carry = sum / 10;
        sum %= 10;

        appendDigit(result, sum);

        if (current1 != NULL)
        {
            current1 = current1->link;
        }
        if (current2 != NULL)
        {
            current2 = current2->link;
        }
    }

    return result;
}

BigInt *subtract(BigInt *a, BigInt *b)
{
    if (compare(a, b) < 0)
    {
        fprintf(stderr, "negative number would be the result.\n");
        exit(EXIT_FAILURE);
    }

    BigInt *result = (BigInt *)malloc(sizeof(BigInt));
    result->head = NULL;
    result->size = 0;

    Node *current1 = a->head;
    Node *current2 = b->head;
    int borrow = 0;

    while (current1 != NULL)
    {
        int digit1 = current1->digit;
        int digit2 = (current2 != NULL) ? current2->digit : 0;

        int diff = digit1 - digit2 - borrow;

        if (diff < 0)
        {
            diff += 10;
            borrow = 1;
        }
        else
        {
            borrow = 0;
        }

        appendDigit(result, diff);

        if (current1 != NULL)
        {
            current1 = current1->link;
        }
        if (current2 != NULL)
        {
            current2 = current2->link;
        }
    }

    while (result->size > 1 && result->head->digit == 0)
    {

        Node *temp = result->head;
        result->head = result->head->link;
        free(temp);
        result->size--;
    }

    return result;
}
BigInt *multiply(BigInt *a, BigInt *b)
{

    BigInt *result = BigIntCreate("0");

    Node *current2 = b->head;
    int shift = 0;

    while (current2 != NULL)
    {

        BigInt *tempResult = BigIntCreate("0");

        Node *current1 = a->head;
        int carry = 0;

        while (current1 != NULL)
        {
            int product = current1->digit * current2->digit + carry;
            carry = product / 10;
            product %= 10;

            appendDigit(tempResult, product);

            current1 = current1->link;
        }

        if (carry > 0)
        {
            appendDigit(tempResult, carry);
        }

        int i;

        for (i = 0; i < shift; i++)
        {
            appendDigit(tempResult, 0);
        }

        result = add(result, tempResult);

        BigIntDestroy(tempResult);

        current2 = current2->link;
        shift++;
    }

    return result;
}

BigInt *divide(BigInt *a, BigInt *b)
{
    if (compare(b, BigIntCreate("0")) == 0)
    {
        fprintf(stderr, "Division by zero is undefined.\n");
        exit(EXIT_FAILURE);
    }

    if (compare(a, b) < 0)
    {
        return BigIntCreate("0");
    }

    BigInt *quotient = BigIntCreate("0");
    BigInt *remainder = BigIntCreate("0");

    BigInt *tempa = BigIntCreate("0");
    copy(tempa, a);

    BigInt *tempResult, *tempSum;
    int count;

    while (compare(tempa, b) >= 0)
    {
        count = 0;

        while (compare(tempa, b) >= 0)
        {
            tempResult = BigIntCreate("0");

            Node *current1 = tempa->head;
            Node *current2 = b->head;
            int borrow = 0;

            while (current1 != NULL || current2 != NULL)
            {
                int digit1 = (current1 != NULL) ? current1->digit : 0;
                int digit2 = (current2 != NULL) ? current2->digit : 0;

                int diff = digit1 - digit2 - borrow;

                if (diff < 0)
                {
                    diff += 10;
                    borrow = 1;
                }
                else
                {
                    borrow = 0;
                }

                appendDigit(tempResult, diff);

                if (current1 != NULL)
                {
                    current1 = current1->link;
                }
                if (current2 != NULL)
                {
                    current2 = current2->link;
                }
            }

            if (borrow != 0)
            {
                appendDigit(tempResult, borrow);
            }

            count++;

            BigIntDestroy(tempa);
            tempa = tempResult;
        }

        tempResult = BigIntCreateFromInt(count);

        tempSum = add(quotient, tempResult);

        BigIntDestroy(quotient);
        BigIntDestroy(tempResult);

        quotient = tempSum;
    }

    copy(remainder, tempa);

    BigIntDestroy(tempa);

    return quotient;
}
