package structures

import "testing"

func BenchmarkArrayPushBack(b *testing.B) {
	arr := NewArray(10)
	for i := 0; i < b.N; i++ {
		arr.AddEnd("test")
	}
}

func BenchmarkSlistPushHead(b *testing.B) {
	list := NewSlist()
	for i := 0; i < b.N; i++ {
		list.PushHead("test")
	}
}

func BenchmarkDlistPushTail(b *testing.B) {
	list := NewDlist()
	for i := 0; i < b.N; i++ {
		list.PushTail("test")
	}
}

func BenchmarkHashSetInsert(b *testing.B) {
	set := NewHashSet[int](1000)
	for i := 0; i < b.N; i++ {
		set.Add(i)
	}
}

func BenchmarkRBTreeInsert(b *testing.B) {
	tree := NewRedBlackTree()
	for i := 0; i < b.N; i++ {
		tree.Insert(i)
	}
}
