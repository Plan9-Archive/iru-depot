package main

// todo: how to sync all the goroutines to start together? check was SEAforth or
// GreenArrays machines do.

import (
	"fmt"
	"flag"
	"net"
	"os"
	"time"
	"rand"
)

func reach(host string, c chan string) {
	conn, err := net.Dial("tcp", "", host)
	if err != nil {
		return
	}
	fmt.Printf("connected to %s\n", host)

	const NBUF = 512
	var buf [NBUF]byte

	for i := 0; i < 10; i++ {
		_, err = conn.Write([]byte("reaching..."))
		if err != nil {
			return
		}

		_, err = conn.Read(buf[:])
		if err != nil {
			return
		}
		fmt.Printf("read: %s\n", buf)
	}
	_ = c <- host	// non-blocking send
}

func rsleep(id int, c chan int) {
	fmt.Printf("%d going to sleep\n", id)
	time.Sleep(rand.Int63()*1e9)
	_ = c <- id
}	

func main() {
	if flag.NArg() == 0 {
		fmt.Fprintf(os.Stderr, "usage: fast host:port...\n");
		os.Exit(1);
	}

	rand.Seed(time.Nanoseconds())

	ch := make(chan string, 1)
	timeout := make(chan bool, 1)

	for i := 0; i < flag.NArg(); i++ {
		go reach(flag.Arg(i), ch)
	}
	
	go func() {
		time.Sleep(2e9)
		timeout <- true
	}()

	select {
		case host := <-ch:
			fmt.Printf("%s responded\n", host)
		case <-timeout:
			fmt.Fprintf(os.Stderr, "timeout\n")
			os.Exit(1)
	}
}

