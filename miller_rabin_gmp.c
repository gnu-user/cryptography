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
#include <gmp.h>
#include <time.h>

#define LIMIT 4294967295

int main()
{
    mpz_t n, n_1, a, j, k, k_1, q, x, exp_a, rem;
    gmp_randstate_t rand;
    unsigned long in_n = 0, in_a = 0, in_s = 1;
    mpz_init(n), mpz_init(n_1), mpz_init(a), mpz_init(j), mpz_init(k), 
    mpz_init(k_1), mpz_init(q), mpz_init(x), mpz_init(exp_a), mpz_init(rem);
    mpz_set_ui(j, 0), mpz_set_ui(k, 1), mpz_set_ui(k_1, 0), mpz_set_ui(q, 0), 
    mpz_set_ui(x, 0), mpz_set_ui(exp_a, 0), mpz_set_ui(rem, 0);
    gmp_randinit_default(rand);

    // Get the number to test (n) and witness (a)
    printf("Enter the number to test: ");
    scanf("%lu", &in_n);
    printf("Enter the witness OR 0 to specify random witnesses: ");
    scanf("%lu", &in_a);

    if (in_a == 0)
    {
        printf("Enter the number of random witnesses: ");
        scanf("%lu", &in_s);
    }
    else
    {
        // Validate the inputs
        if (in_a <= 1 || in_a >= (in_n - 1))
        {
            fprintf(stderr, "Invalid witness provided!\n");
            return 1;
        }
    }

    // Convert values to GMP
    mpz_set_ui(n, in_n), mpz_set_ui(a, in_a);
    mpz_sub_ui(n_1, n, 1);

    // Validate that the number is not even
    if (mpz_even_p(n))
    {
        fprintf(stderr, "Invalid number provided, it's even!\n");
        return 1;
    }

    for (unsigned long i = 0; i < in_s; ++i)
    {
        // Pick a random a value if a not provided
        if (mpz_cmp_ui(a, 0) == 0)
        {
            mpz_t temp;
            mpz_init_set_ui(temp, 0);
            mpz_sub_ui(temp, n, 2);
            mpz_urandomm(a, rand, temp);
            mpz_add_ui(a, a, 1);
            mpz_clear(temp);
        }

        // Determine k > 0, q odd by factoring powers of 2 from n - 1
        for (mpz_set_ui(k, 1); mpz_cmp_ui(k, LIMIT) < 0; mpz_add_ui(k, k, 1))
        {
            mpz_t exp_k, base_2;
            mpz_init_set_ui(exp_k, 0), mpz_init_set_ui(base_2, 2);

            mpz_powm(exp_k, base_2, k, n);

            // If it's divisible get q
            if (mpz_divisible_p(n_1, exp_k))
            {
                mpz_divexact(q, n_1, exp_k);

                // If it's odd then use the value of q 
                if (mpz_odd_p(q))
                {
                    // found q and k
                    mpz_clear(exp_k);
                    mpz_clear(base_2);
                    break;
                }
            }
        }

        // Test if a^q mod n = 1
        mpz_powm(exp_a, a, q, n);
        if (mpz_cmp_ui(exp_a, 1) == 0)
        {
            gmp_printf("Inconclusive for n: %Zd, witness: %Zd, q: %Zd\n", n, a, q);
            return 0;
        }

        
        // Perform thorough primality test
        mpz_sub_ui(k_1, k, 1);
        mpz_set(x, exp_a);
        for (mpz_set_ui(j, 1); mpz_cmp(j, k_1) < 0; mpz_add_ui(j, j, 1))
        {
            mpz_powm_ui(x, x, 2, n);
            
            if (mpz_cmp(x, n_1) == 0)
            {
                gmp_printf("Inconclusive for n: %Zd, witness: %Zd, j: %Zd, q: %Zd\n", n, a, j, q);
                return 0;
            }
        }

        // Reset a for next round
        mpz_set_ui(a, 0);
    }

    printf("Composite!\n");
    return 0;
}