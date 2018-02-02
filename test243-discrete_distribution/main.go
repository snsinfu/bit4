package main

import (
	"errors"
	"fmt"
	"math/rand"
	"time"
)

func main() {
	random := rand.New(rand.NewSource(time.Now().UnixNano()))
	distr := NewDistribution([]string{"A", "B", "C"}, []int{10, 3, 1})

	for i := 0; i < 30; i++ {
		choice, _ := distr.Draw(random)
		fmt.Print(choice)
	}
	fmt.Println()
}

type Distribution struct {
	choices []string
	weights []int
	sum     int
}

func NewDistribution(choices []string, weights []int) *Distribution {
	sum := 0
	for _, w := range weights {
		sum += w
	}
	return &Distribution{
		choices: choices,
		weights: weights,
		sum:     sum,
	}
}

func (distr *Distribution) Draw(random *rand.Rand) (string, error) {
	n := random.Intn(distr.sum)
	for i := 0; i < len(distr.choices); i++ {
		n -= distr.weights[i]
		if n < 0 {
			return distr.choices[i], nil
		}
	}
	return "", errors.New("no choice")
}
