#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

int main()
{
    mpz_t a, b, sum, sub, mul, div;
    mpz_inits(a, b, sum, sub, mul, div, NULL);

    char string1[1000];
    char string2[1000];
    printf("Enter the first number: ");
    scanf("%s", string1);
    printf("Enter the second number: ");
    scanf("%s", string2);

    mpz_set_str(a, string1, 10);
    mpz_set_str(b, string2, 10);

    mpz_add(sum, a, b);
    mpz_sub(sub, a, b);
    mpz_mul(mul, a, b);

    if (mpz_sgn(b) != 0)
    {
        mpz_fdiv_q(div, a, b);
    }
    else
    {
        printf("Division by zero is undefined.\n");
        mpz_clears(a, b, sum, sub, mul, div, NULL);
        return 1;
    }
    gmp_printf("a + b = %Zd\n", sum);
    gmp_printf("a - b = %Zd\n", sub);
    gmp_printf("a * b = %Zd\n", mul);
    gmp_printf("a / b = %Zd\n", div);

    mpz_clears(a, b, sum, sub, mul, div, NULL);
    return 0;
}
