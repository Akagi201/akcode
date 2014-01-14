package main

import "fmt"
// for的三种形式
func main(){
	// 经典的for语句 init; condition; post
	for i := 0; i < 10; i++{
		fmt.Println(i)
	}
	
	// 精简的for语句 condition
	i := 1
	for i < 10 {
		fmt.Println(i)
		i++
	}
	
	// 死循环的for语句 相当于for(;;)
	i := 1
	for {
		if i > 10 {
			break
		}
		i++
	}
}
