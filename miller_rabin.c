/*
 * Miller-Rabin Algorithm Implementation.
 *
 * Copyright (C) 2014, Jonathan Gillett (100437638)
 * All rights reserved.
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <limits.h>
#include <tgmath.h>
#include <time.h>

int main()
{
    long double n = 0, a = 0, s = 1, k = 0, q = 0;
    srand(time(0));

    // Get the number to test (n) and witness (a)
    printf("Enter the number to test: ");
    scanf("%LF", &n);
    printf("Enter the witness OR 0 to specify random witnesses: ");
    scanf("%LF", &a);

    if (a == 0)
    {
        printf("Enter the number of random witnesses: ");
        scanf("%LF", &s);
    }
    else
    {
        // Validate the inputs
        if (a <= 1 || a >= (n - 1))
        {
            fprintf(stderr, "Invalid witness provided!\n");
            return 1;
        }
    }

    // Validate that the number is not even
    long double n_rem = remainder(n, 2);
    if (n_rem >= 0.0 - DBL_EPSILON && n_rem <= 0.0 + DBL_EPSILON)
    {
        fprintf(stderr, "Invalid number provided, it's even!\n");
        return 1;
    }

    for (long double i = 0; i < s; ++i)
    {
        // Pick a random a value if a not provided
        if (a == 0)
        {
            a = (long double) (rand() % (int) (n - 2)) + 1;
        }

        // Determine k > 0, q odd by factoring powers of 2 from n - 1
        for (k = 1; k < DBL_MAX; ++k)
        {
            long double rem = remainder(n - 1, pow(2, k));

            // If it's divisible get q
            if (rem >= 0.0 - DBL_EPSILON && rem <= 0.0 + DBL_EPSILON)
            {
                q = (n - 1) / pow(2, k);
                long double q_rem = remainder(q, 2);

                // If it's odd then use the value of q 
                if (! (q_rem >= 0.0 - DBL_EPSILON && q_rem <= 0.0 + DBL_EPSILON))
                {
                    // found q and k
                    break;
                }
            }
        }

        // Test if a^q mod n = 1
        long double rem = remainderl(powl(a, q), n);
        if (rem >= 1.0 - DBL_EPSILON && rem <= 1.0 + DBL_EPSILON)
        {
            printf("Inconclusive for n: %LF, witness: %LF, q: %LF\n", n, a, q);
            return 0;
        }

        // Perform thorough primality test
        for (long double j = 0; j < k - 1; ++j)
        {
            rem = remainderl(powl(a, powl(2, j) * q), n);

            if (rem >= (n - 1) - DBL_EPSILON && rem <= (n - 1) + DBL_EPSILON)
            {
                printf("Inconclusive for n: %LF, witness: %LF, j: %LF, q: %LF\n", n, a, j, q);
                return 0;
            }
        }

        // Reset a for next round
        a = 0;
    }

    printf("Composite!\n");
    return 0;
}