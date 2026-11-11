package structures

import "fmt"

type SlistNode struct {
	text string
	next *SlistNode
}

type Slist struct {
	head *SlistNode
}

func NewSlist() *Slist {
	return &Slist{head: nil}
}

func (s *Slist) PushHead(value string) {
	s.head = &SlistNode{text: value, next: s.head}
}

func (s *Slist) PushTail(value string) {
	newNode := &SlistNode{text: value, next: nil}
	if s.head == nil {
		s.head = newNode
		return
	}
	current := s.head
	for current.next != nil {
		current = current.next
	}
	current.next = newNode
}

func (s *Slist) PushAfter(target, value string) {
	current := s.head
	for current != nil && current.text != target {
		current = current.next
	}
	if current != nil {
		current.next = &SlistNode{text: value, next: current.next}
	}
}

func (s *Slist) PushBefore(target, value string) {
	if s.head == nil {
		return
	}
	if s.head.text == target {
		s.PushHead(value)
		return
	}
	current := s.head
	for current.next != nil && current.next.text != target {
		current = current.next
	}
	if current.next != nil {
		current.next = &SlistNode{text: value, next: current.next}
	}
}

func (s *Slist) DeleteHead() {
	if s.head == nil {
		return
	}
	s.head = s.head.next
}

func (s *Slist) DeleteTail() {
	if s.head == nil {
		return
	}
	if s.head.next == nil {
		s.head = nil
		return
	}
	current := s.head
	for current.next.next != nil {
		current = current.next
	}
	current.next = nil
}

func (s *Slist) DeleteAfter(target string) {
	current := s.head
	for current != nil && current.text != target {
		current = current.next
	}
	if current != nil && current.next != nil {
		current.next = current.next.next
	}
}

func (s *Slist) DeleteBefore(target string) {
	if s.head == nil || s.head.next == nil {
		return
	}
	if s.head.next.text == target {
		s.head = s.head.next
		return
	}
	current := s.head
	for current.next.next != nil && current.next.next.text != target {
		current = current.next
	}
	if current.next.next != nil {
		current.next = current.next.next
	}
}

func (s *Slist) DeleteValue(value string) {
	if s.head == nil {
		return
	}
	if s.head.text == value {
		s.head = s.head.next
		return
	}
	current := s.head
	for current.next != nil && current.next.text != value {
		current = current.next
	}
	if current.next != nil {
		current.next = current.next.next
	}
}

func (s *Slist) Find(value string) bool {
	current := s.head
	for current != nil {
		if current.text == value {
			return true
		}
		current = current.next
	}
	return false
}

func (s *Slist) Print() {
	current := s.head
	for current != nil {
		fmt.Printf("%s ", current.text)
		current = current.next
	}
	fmt.Println()
}

func (s *Slist) Clear() {
	s.head = nil
}
