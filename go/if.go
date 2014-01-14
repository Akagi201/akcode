package main

import "fmt"
// if语句没有圆括号，而必需要有花括号
func main(){
	// if 语句
	if x % 2 == 0{
		// ...
	}
	// if - else
	if x % 2 == 0{
		// 偶数...
	}else{
		// 奇数...
	}
	//多分枝
	if num < 0{
		// 负数
	}else if num == 0{
		// 零
	}else{
		// 正数
	}
}