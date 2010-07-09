// naïve prime sieve, divisors are set to 0

package main

import (
	"fmt"
	"os"
	"strconv"
)

func main() {
	if len(os.Args) < 2 {
		os.Stderr.WriteString("usage: primes n\n")
		os.Exit(1)
	}
	
	max, _ := strconv.Atoi(os.Args[1])
	max -= 2	// exclude 0 and 1

	a := make([]int, max+1)	// we want numbers up to n, not n-1

	for i := 0; i <= max; i++ {
			a[i] = i+2;
	}

	for i, p := range a {
		// may have already been set as divisor
		if p == 0 {
			continue
		}

		for j := i+1; j < len(a); j++{
			if a[j] % p == 0 {
				a[j] = 0
			}
		}
	}

	for _, v := range a {
		if v != 0 {
			fmt.Printf("%d\n", v)
		}
	}
}
