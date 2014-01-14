package main

import "fmt"

// go 是静态类型的语言，变量的声明类似javascript

func main(){
	// 声明初始化一个变量
	var x int = 100
	var str string = "hello world"
	
	// 声明初始化多个变量
	var i, j, k int = 1, 2, 3
	
	// 不用指明类型，通过初始化值来推导
	var b = true // bool
	
	// 另一种定义变量方式,类似Pascal，但完全不同
	x := 100 // 等价于 var x int = 100
	
	// 常量使用const关键字
	const s string = "hello world"
	const pi float32 = 3.1415926
}