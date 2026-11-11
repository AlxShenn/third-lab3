package structures

import "fmt"

type Array struct {
	data     []string
	size     int
	capacity int
}

func NewArray(cap int) *Array {
	return &Array{
		data:     make([]string, cap),
		size:     0,
		capacity: cap,
	}
}

func (a *Array) resize() {
	newCapacity := 4
	if a.capacity > 0 {
		newCapacity = a.capacity * 2
	}
	newData := make([]string, newCapacity)
	copy(newData, a.data[:a.size])
	a.data = newData
	a.capacity = newCapacity
}

func (a *Array) AddEnd(text string) {
	if a.size == a.capacity {
		a.resize()
	}
	a.data[a.size] = text
	a.size++
}

func (a *Array) AddIndex(text string, index int) {
	if index < 0 || index > a.size {
		return
	}
	if a.size == a.capacity {
		a.resize()
	}
	for i := a.size; i > index; i-- {
		a.data[i] = a.data[i-1]
	}
	a.data[index] = text
	a.size++
}

func (a *Array) Get(index int) string {
	if index < 0 || index >= a.size {
		return ""
	}
	return a.data[index]
}

func (a *Array) RemoveIndex(index int) {
	if index < 0 || index >= a.size {
		return
	}
	for i := index; i < a.size-1; i++ {
		a.data[i] = a.data[i+1]
	}
	a.size--
}

func (a *Array) Set(index int, text string) {
	if index < 0 || index >= a.size {
		return
	}
	a.data[index] = text
}

func (a *Array) Length() int {
	return a.size
}

func (a *Array) Print() {
	for i := 0; i < a.size; i++ {
		fmt.Printf("%s ", a.data[i])
	}
	fmt.Println()
}
