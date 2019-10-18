package main

import (
	"fmt"
	"math/rand"
	"os"
)

func main() {
	intro()
	secret := genSecret()
	times := 10
	guess := ""
	for i := 1; i <= times; i++ {
		fmt.Printf("The %dth guess: ", i)
		fmt.Scanln(&guess)

		if guess == secret {
			break
		}

		fmt.Printf("Hints: %s\n", getHint(secret, guess))
	}

	if guess == secret {
		fmt.Printf("That's it! Congras!")
	} else {
		fmt.Printf("The answer is %s.\nGet better next time!", secret)
	}

	buf := make([]byte, 1)
	os.Stdin.Read(buf)
}

func intro() {
	fmt.Println("欢迎参加猜数字小游戏！")
	fmt.Println("游戏规则如下：")
	fmt.Println("1.系统会随机生成四个数字（0-9），每个数字均不相同")
	fmt.Println("2.你的任务是通过系统提示猜出这四个数字")
	fmt.Println("3.系统会先要求你输入你的猜测，然后通过A和B的数量来提示你猜测的准确性")
	fmt.Println("4.A表示数字正确且位置正确，B表示数字正确但位置不正确")
	fmt.Println("  比如系统生成了1234，你的猜测是5432，那么系统会提示1A2B")
	fmt.Println("  其中1A表示3的数字和位置均正确，2B表示2和4虽然数字正确但是位置不正确")
	fmt.Println("5.你只有十次机会")
	fmt.Println("")
	fmt.Println("让我们开始吧！")
	fmt.Println("")
}

func genSecret() string {
	digits := make([]bool, 10)
	for i := 0; i < 10; i++ {
		digits[i] = false
	}
	secret := ""
	for len(secret) < 4 {
		v := rand.Intn(10)
		if !digits[v] {
			digits[v] = true
			secret = fmt.Sprintf("%s%d", secret, v)
		}
	}
	return secret
}

func getHint(secret, guess string) string {
	A := 0
	B := 0
	bufA := make([]int, 0)
	bufB := make([]int, 0)
	iA := 0
	iB := 0
	for iA < len(secret) && iB < len(guess) {
		vA := int(secret[iA])
		vB := int(guess[iB])
		iA += 1
		iB += 1

		if vA == vB {
			A += 1
			continue
		}

		bufA = append(bufA, vA)
		bufB = append(bufB, vB)
		if idx := indexOf(bufA, vB); idx != -1 {
			B += 1
			bufA[idx], bufA[len(bufA)-1] = bufA[len(bufA)-1], bufA[idx]
			bufA = bufA[:len(bufA)-1]
			bufB = bufB[:len(bufB)-1]
		}
		if idx := indexOf(bufB, vA); idx != -1 {
			B += 1
			bufB[idx], bufB[len(bufB)-1] = bufB[len(bufB)-1], bufB[idx]
			bufB = bufB[:len(bufB)-1]
			bufA = bufA[:len(bufA)-1]
		}
	}
	return fmt.Sprintf("%dA%dB", A, B)
}

func indexOf(arr []int, target int) int {
	for i := range arr {
		if arr[i] == target {
			return i
		}
	}
	return -1
}
