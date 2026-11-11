package structures

import "fmt"

type DlistNode struct {
	text string
	next *DlistNode
	prev *DlistNode
}

type Dlist struct {
	head *DlistNode
	tail *DlistNode
}

func NewDlist() *Dlist {
	return &Dlist{head: nil, tail: nil}
}

func (d *Dlist) PushHead(value string) {
	newNode := &DlistNode{text: value, next: d.head, prev: nil}
	if d.head == nil {
		d.head = newNode
		d.tail = newNode
	} else {
		d.head.prev = newNode
		d.head = newNode
	}
}

func (d *Dlist) PushTail(value string) {
	newNode := &DlistNode{text: value, next: nil, prev: d.tail}
	if d.tail == nil {
		d.head = newNode
		d.tail = newNode
	} else {
		d.tail.next = newNode
		d.tail = newNode
	}
}

func (d *Dlist) PushAfter(target, value string) {
	current := d.head
	for current != nil && current.text != target {
		current = current.next
	}
	if current != nil {
		newNode := &DlistNode{text: value, next: current.next, prev: current}
		if current.next != nil {
			current.next.prev = newNode
		} else {
			d.tail = newNode
		}
		current.next = newNode
	}
}

func (d *Dlist) PushBefore(target, value string) {
	current := d.head
	for current != nil && current.text != target {
		current = current.next
	}
	if current != nil {
		newNode := &DlistNode{text: value, next: current, prev: current.prev}
		if current.prev != nil {
			current.prev.next = newNode
		} else {
			d.head = newNode
		}
		current.prev = newNode
	}
}

func (d *Dlist) DeleteHead() {
	if d.head == nil {
		return
	}
	d.head = d.head.next
	if d.head != nil {
		d.head.prev = nil
	} else {
		d.tail = nil
	}
}

func (d *Dlist) DeleteTail() {
	if d.tail == nil {
		return
	}
	d.tail = d.tail.prev
	if d.tail != nil {
		d.tail.next = nil
	} else {
		d.head = nil
	}
}

func (d *Dlist) DeleteAfter(target string) {
	current := d.head
	for current != nil && current.text != target {
		current = current.next
	}
	if current != nil && current.next != nil {
		temp := current.next
		current.next = temp.next
		if temp.next != nil {
			temp.next.prev = current
		} else {
			d.tail = current
		}
	}
}

func (d *Dlist) DeleteBefore(target string) {
	current := d.head
	for current != nil && current.text != target {
		current = current.next
	}
	if current != nil && current.prev != nil {
		temp := current.prev
		current.prev = temp.prev
		if temp.prev != nil {
			temp.prev.next = current
		} else {
			d.head = current
		}
	}
}

func (d *Dlist) DeleteValue(value string) {
	current := d.head
	for current != nil && current.text != value {
		current = current.next
	}
	if current != nil {
		if current.prev != nil {
			current.prev.next = current.next
		} else {
			d.head = current.next
		}
		if current.next != nil {
			current.next.prev = current.prev
		} else {
			d.tail = current.prev
		}
	}
}

func (d *Dlist) Find(value string) bool {
	current := d.head
	for current != nil {
		if current.text == value {
			return true
		}
		current = current.next
	}
	return false
}

func (d *Dlist) PrintForward() {
	current := d.head
	for current != nil {
		fmt.Printf("%s ", current.text)
		current = current.next
	}
	fmt.Println()
}

func (d *Dlist) PrintBackward() {
	current := d.tail
	for current != nil {
		fmt.Printf("%s ", current.text)
		current = current.prev
	}
	fmt.Println()
}

func (d *Dlist) Clear() {
	d.head = nil
	d.tail = nil
}
