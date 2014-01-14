package main

import (
	"fmt"
	"net"
	"os"
)

func main() {
	var (
		host          = "127.0.0.1"
		port          = "9998"
		remote        = host + ":" + port
		msg    string = "Hello world.\n"
	)

	con, error := net.Dial("tcp", remote)
	defer con.Close()

}
