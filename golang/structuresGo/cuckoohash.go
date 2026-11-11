package structures

import (
	"fmt"
	"hash/fnv"
)

type CuckooEntry[T comparable] struct {
	key      T
	occupied bool
}

type CuckooHash[T comparable] struct {
	table1   []CuckooEntry[T]
	table2   []CuckooEntry[T]
	capacity int
	size     int
}

func NewCuckooHash[T comparable](cap int) *CuckooHash[T] {
	return &CuckooHash[T]{
		table1:   make([]CuckooEntry[T], cap),
		table2:   make([]CuckooEntry[T], cap),
		capacity: cap,
		size:     0,
	}
}

func (c *CuckooHash[T]) hashHelper(key T, salt string, mod int) int {
	hasher := fnv.New32a()
	hasher.Write([]byte(fmt.Sprintf("%v%s", key, salt)))
	return int(hasher.Sum32()) % mod
}

func (c *CuckooHash[T]) hash1(key T) int { return c.hashHelper(key, "", c.capacity) }
func (c *CuckooHash[T]) hash2(key T) int { return c.hashHelper(key, "salt", c.capacity) }

func (c *CuckooHash[T]) rehash() {
	old1 := c.table1
	old2 := c.table2
	oldCap := c.capacity

	c.capacity *= 2
	c.table1 = make([]CuckooEntry[T], c.capacity)
	c.table2 = make([]CuckooEntry[T], c.capacity)
	c.size = 0

	for i := 0; i < oldCap; i++ {
		if old1[i].occupied {
			c.Add(old1[i].key)
		}
		if old2[i].occupied {
			c.Add(old2[i].key)
		}
	}
}

func (c *CuckooHash[T]) Add(key T) {
	if c.Contains(key) {
		return
	}

	currentKey := key
	for i := 0; i < 100; i++ {
		pos1 := c.hash1(currentKey)
		if !c.table1[pos1].occupied {
			c.table1[pos1] = CuckooEntry[T]{key: currentKey, occupied: true}
			c.size++
			return
		}
		// Выселяем
		currentKey, c.table1[pos1].key = c.table1[pos1].key, currentKey

		pos2 := c.hash2(currentKey)
		if !c.table2[pos2].occupied {
			c.table2[pos2] = CuckooEntry[T]{key: currentKey, occupied: true}
			c.size++
			return
		}
		currentKey, c.table2[pos2].key = c.table2[pos2].key, currentKey
	}
	c.rehash()
	c.Add(currentKey)
}

func (c *CuckooHash[T]) Contains(key T) bool {
	pos1 := c.hash1(key)
	if c.table1[pos1].occupied && c.table1[pos1].key == key {
		return true
	}
	pos2 := c.hash2(key)
	if c.table2[pos2].occupied && c.table2[pos2].key == key {
		return true
	}
	return false
}
