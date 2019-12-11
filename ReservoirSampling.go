package main

import (
	"fmt"
	"math"
	"math/rand"
	"time"
)

func ReservoirSampling(k, n int) []int {
	r := make([]int, k)
	for i := 0; i < k; i++ {
		r[i] = i
	}
	for i := k; i < n; i++ {
		j := rand.Intn(i)
		if j < k {
			r[j] = i
		}
	}
	return r
}

func main() {
	rand.Seed(time.Now().Unix())
	k, n, N := 10, 100, 10000
	s := make([]int, n)
	for i := 0; i < N; i++ {
		r := ReservoirSampling(k, n)
		for _, v := range r {
			s[v]++
		}
	}
	fmt.Println(s)
	avg := k * N / n
	fmt.Println(avg)
	res := 0
	for _, v := range s {
		res += (avg - v) * (avg - v)
	}
	fmt.Println(math.Pow(float64(res)/float64(n), 0.5))
}
