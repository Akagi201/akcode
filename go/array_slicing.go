package main

import "fmt"
// 数组的切片操作，跟Python很象
func main(){
	a := [5]int{1, 2, 3, 4, 5}
	
	b := a[2:4]  // a[2] 和 a[3]，但不包括a[4] 
	fmt.Println(b)
	
	b = a[:4]    // 从a[0]到a[4], 但不包括a[4]
	fmt.Println(b)
	
	b = a[2:]    // 从a[2]到a[4], 且包括a[2]
	fmt.Println(b)
}