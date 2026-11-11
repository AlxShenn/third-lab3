package structures

import (
	"fmt"
	"hash/fnv"
)

type hashNode[T comparable] struct {
	data T
	next *hashNode[T]
}

type HashSet[T comparable] struct {
	table    []*hashNode[T]
	capacity int
	size     int
}

func NewHashSet[T comparable](cap int) *HashSet[T] {
	return &HashSet[T]{
		table:    make([]*hashNode[T], cap),
		capacity: cap,
		size:     0,
	}
}

func (h *HashSet[T]) hash(key T) int {
	// Упрощенная хеш-функция через fmt.Sprintf для универсальности
	hasher := fnv.New32a()
	hasher.Write([]byte(fmt.Sprintf("%v", key)))
	return int(hasher.Sum32()) % h.capacity
}

func (h *HashSet[T]) Add(value T) {
	idx := h.hash(value)
	cur := h.table[idx]
	for cur != nil {
		if cur.data == value {
			return // Уже есть
		}
		cur = cur.next
	}
	// Добавляем в начало цепочки
	h.table[idx] = &hashNode[T]{data: value, next: h.table[idx]}
	h.size++
}

func (h *HashSet[T]) Remove(value T) bool {
	idx := h.hash(value)
	cur := h.table[idx]
	var prev *hashNode[T]
	for cur != nil {
		if cur.data == value {
			if prev != nil {
				prev.next = cur.next
			} else {
				h.table[idx] = cur.next
			}
			h.size--
			return true
		}
		prev = cur
		cur = cur.next
	}
	return false
}

func (h *HashSet[T]) Contains(value T) bool {
	idx := h.hash(value)
	cur := h.table[idx]
	for cur != nil {
		if cur.data == value {
			return true
		}
		cur = cur.next
	}
	return false
}

func (h *HashSet[T]) GetSize() int {
	return h.size
}

func (h *HashSet[T]) GetAll() []T {
	result := make([]T, 0, h.size)
	for i := 0; i < h.capacity; i++ {
		cur := h.table[i]
		for cur != nil {
			result = append(result, cur.data)
			cur = cur.next
		}
	}
	return result
}

func (h *HashSet[T]) Clear() {
	for i := 0; i < h.capacity; i++ {
		h.table[i] = nil
	}
	h.size = 0
}
