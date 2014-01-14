package main // 声明本文件的package名

import "fmt" // import语言的fmt库 -- 用于输出

func main(){
	fmt.Println("hello world")
}

// 解释执行(实际是编译成a.out再执行)
// $go run hello.go 
// hello world 

// 编译执行
// $go build hello.go 
// $./hello