package structures

import "fmt"

type StackNode struct {
	text string
	next *StackNode
}

type Stack struct {
	head *StackNode
}

func NewStack() *Stack {
	return &Stack{head: nil}
}

func (s *Stack) Push(value string) {
	s.head = &StackNode{text: value, next: s.head}
}

func (s *Stack) Pop(outValue *string) bool {
	if s.head == nil {
		return false
	}
	*outValue = s.head.text
	s.head = s.head.next
	return true
}

func (s *Stack) IsEmpty() bool {
	return s.head == nil
}

func (s *Stack) Print() {
	if s.head == nil {
		fmt.Println("Empty")
		return
	}
	current := s.head
	for current != nil {
		fmt.Printf("%s ", current.text)
		current = current.next
	}
	fmt.Println()
}

func (s *Stack) Clear() {
	s.head = nil
}
