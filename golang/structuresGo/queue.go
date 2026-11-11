package structures

import "fmt"

type QueueNode struct {
	text string
	next *QueueNode
}

type Queue struct {
	head *QueueNode
	tail *QueueNode
}

func NewQueue() *Queue {
	return &Queue{head: nil, tail: nil}
}

func (q *Queue) Push(value string) {
	newNode := &QueueNode{text: value, next: nil}
	if q.tail == nil {
		q.head = newNode
		q.tail = newNode
		return
	}
	q.tail.next = newNode
	q.tail = newNode
}

func (q *Queue) Pop(outValue *string) bool {
	if q.IsEmpty() {
		return false
	}
	*outValue = q.head.text
	q.head = q.head.next
	if q.head == nil {
		q.tail = nil
	}
	return true
}

func (q *Queue) IsEmpty() bool {
	return q.head == nil
}

func (q *Queue) Print() {
	if q.IsEmpty() {
		return
	}
	current := q.head
	for current != nil {
		fmt.Printf("%s", current.text)
		if current.next != nil {
			fmt.Print(" -> ")
		}
		current = current.next
	}
	fmt.Println()
}

func (q *Queue) Clear() {
	q.head = nil
	q.tail = nil
}
