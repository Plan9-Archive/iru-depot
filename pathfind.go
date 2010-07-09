package main

import (
	"bytes"
	"fmt"
	"rand"
	"time"
)

const (
	Free = '.'
	Locked = '|'
	Source = 'S'
	Destination = 'D'

	Vert = iota
	Hort
)
	
type Map [][]byte

var (
	Start [2]int
	End [2]int
)

// create a Map from a string array
func createmap(lines []string) Map {
	m := make(Map, len(lines))

	for i, _ := range lines {
		m[i] = bytes.NewBufferString(lines[i]).Bytes()
	}

	rand.Seed(time.Seconds())
	
	// source point in the first column
	src := rand.Intn(m.nrows())
	m[src][0] = 'S'

	// destination point in last column
	dst := rand.Intn(m.nrows())
	m[dst][m.ncols()-1] = 'D'

	return m
}

func (m *Map) print() {
	if m == nil {
		return
	}
	
	for _, line := range *m {
		for _, c := range line {
			fmt.Printf("%c ", c)
		}
		println("")
	}
}

func (m *Map) nrows() int {
	return len(*m)
}

func (m *Map) ncols() int {
	s := *m
	return len(s[0])
}

func walk(m Map) {
	width := m.ncols()
	height := m.nrows()

	dir := rand.Intn(2)

}

func main() {
	test := createmap(&[...]string{
								"...|....",
								"....|...",
								"....|...",
								"....|...",
								"........"})
	
	test.print()
	walk(test)
}
