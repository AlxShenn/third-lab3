package structures

import (
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// ================= ARRAY =================
func TestArray(t *testing.T) {
	arr := NewArray(2)
	assert.Equal(t, 0, arr.Length())
	assert.Equal(t, "", arr.Get(0))

	arr.AddEnd("A")
	arr.AddEnd("B")
	assert.Equal(t, 2, arr.Length())

	arr.AddIndex("C", 1)
	assert.Equal(t, "C", arr.Get(1))
	assert.Equal(t, "B", arr.Get(2))

	// Некорректный индекс (покрытие ветки return)
	arr.AddIndex("D", 10)

	arr.RemoveIndex(1)
	assert.Equal(t, "B", arr.Get(1))

	// Некорректный индекс
	arr.RemoveIndex(10)

	arr.Set(0, "X")
	assert.Equal(t, "X", arr.Get(0))
	arr.Set(10, "Y") // Некорректный индекс
}

func TestArrayResize(t *testing.T) {
	arr := NewArray(1)
	arr.AddEnd("1")
	arr.AddEnd("2") // Вызовет resize
	assert.Equal(t, 2, arr.Length())
}

// ================= SLIST =================
func TestSlist(t *testing.T) {
	list := NewSlist()
	assert.False(t, list.Find("A"))

	list.PushHead("B")
	list.PushHead("A")
	list.PushTail("C")
	list.PushAfter("B", "D")
	list.PushBefore("B", "E")

	assert.True(t, list.Find("A"))
	assert.True(t, list.Find("D"))
	assert.True(t, list.Find("E"))

	list.DeleteHead()
	list.DeleteTail()
	list.DeleteAfter("E")
	list.DeleteBefore("E")
	list.DeleteValue("E")
	list.DeleteValue("NonExistent") // Покрытие ветки if current.next == nil
	list.Clear()
}

// ================= DLIST =================
func TestDlist(t *testing.T) {
	list := NewDlist()
	list.PushTail("B")
	list.PushHead("A")
	list.PushTail("C")
	list.PushAfter("B", "D")
	list.PushBefore("B", "E")

	assert.True(t, list.Find("D"))
	assert.True(t, list.Find("E"))

	list.DeleteHead()
	list.DeleteTail()
	list.DeleteAfter("E")
	list.DeleteBefore("E")
	list.DeleteValue("E")
	list.DeleteValue("NonExistent")
	list.Clear()
}

// ================= STACK =================
func TestStack(t *testing.T) {
	stack := NewStack()
	assert.True(t, stack.IsEmpty())

	stack.Push("1")
	stack.Push("2")
	assert.False(t, stack.IsEmpty())

	var val string
	ok := stack.Pop(&val)
	require.True(t, ok)
	assert.Equal(t, "2", val)

	ok = stack.Pop(&val)
	require.True(t, ok)
	assert.Equal(t, "1", val)

	ok = stack.Pop(&val) // Попытка удалить из пустого
	assert.False(t, ok)
	stack.Clear()
}

// ================= QUEUE =================
func TestQueue(t *testing.T) {
	queue := NewQueue()
	assert.True(t, queue.IsEmpty())

	queue.Push("1")
	queue.Push("2")
	assert.False(t, queue.IsEmpty())

	var val string
	ok := queue.Pop(&val)
	require.True(t, ok)
	assert.Equal(t, "1", val)

	queue.Pop(&val)      // Удаляем второй
	ok = queue.Pop(&val) // Из пустой
	assert.False(t, ok)
	queue.Clear()
}

// ================= HASHSET =================
func TestHashSet(t *testing.T) {
	set := NewHashSet[string](10)
	assert.False(t, set.Contains("apple"))

	set.Add("apple")
	set.Add("banana")
	set.Add("apple") // Дубликат
	assert.Equal(t, 2, set.GetSize())

	all := set.GetAll()
	assert.Len(t, all, 2)

	set.Remove("apple")
	assert.False(t, set.Contains("apple"))
	assert.True(t, set.Remove("banana"))
	assert.False(t, set.Remove("NonExistent"))
	set.Clear()
}

func TestHashSetCollisions(t *testing.T) {
	// Маленькая таблица для вызова коллизий
	set := NewHashSet[string](2)
	set.Add("A")
	set.Add("B")
	set.Add("C")
	assert.Equal(t, 3, set.GetSize())
}

// ================= CUCKOOHASH =================
func TestCuckooHash(t *testing.T) {
	ch := NewCuckooHash[string](4)
	assert.False(t, ch.Contains("key1"))

	ch.Add("key1")
	ch.Add("key2")
	ch.Add("key3")
	ch.Add("key4")
	ch.Add("key5") // Вызовет rehash
	ch.Add("key1") // Дубликат

	assert.True(t, ch.Contains("key1"))
	assert.True(t, ch.Contains("key5"))
	assert.False(t, ch.Contains("key6"))
}

// ================= RED BLACK TREE =================
func TestRedBlackTree(t *testing.T) {
	tree := NewRedBlackTree()
	assert.False(t, tree.Search(10))

	tree.Insert(10)
	tree.Insert(20)
	tree.Insert(30) // Балансировка (левый поворот)
	tree.Insert(15) // Балансировка (зигзаг)
	tree.Insert(5)  // Балансировка (правый поворот)
	tree.Insert(10) // Дубликат (покрытие ветки return)

	assert.True(t, tree.Search(10))
	assert.True(t, tree.Search(30))
	assert.False(t, tree.Search(100))

	tree.Print() // Покрытие вывода
	tree.Clear()
	assert.False(t, tree.Search(10))
}

func TestArrayEdgeCases(t *testing.T) {
	arr := NewArray(0)
	arr.AddEnd("A")
	assert.Equal(t, "A", arr.Get(0))
	arr.Print()
}

func TestSlistEdges(t *testing.T) {
	list := NewSlist()
	list.DeleteHead()
	list.DeleteTail()
	list.PushHead("A")
	list.DeleteTail()
	list.Print()
}

func TestDlistEdges(t *testing.T) {
	list := NewDlist()
	list.DeleteHead()
	list.DeleteTail()
	list.PushHead("A")
	list.DeleteHead()
	list.PushHead("B")
	list.PushHead("A")
	list.PrintBackward()
}

// ================= ФИНАЛЬНЫЕ ТЕСТЫ ДЛЯ 85%+ =================

func TestStackEdge(t *testing.T) {
	// Проверка пустого стека (покрывает ветки IsEmpty и Pop)
	s := NewStack()
	assert.True(t, s.IsEmpty())
	var val string
	assert.False(t, s.Pop(&val))

	// Проверка Print на пустом и непустом стеке
	s.Print()
	s.Push("A")
	s.Print()
	s.Clear()
}

func TestQueueEdge(t *testing.T) {
	q := NewQueue()
	assert.True(t, q.IsEmpty())
	var val string
	assert.False(t, q.Pop(&val)) // Pop из пустой очереди

	q.Print() // Print пустой очереди
	q.Push("A")
	q.Print() // Print с элементом
	q.Clear()
}

func TestSlistEdge(t *testing.T) {
	s := NewSlist()
	// Покрытие удалений из пустого списка
	s.DeleteHead()
	s.DeleteTail()
	s.DeleteAfter("A")
	s.DeleteBefore("A")

	s.PushHead("A")
	s.DeleteTail() // Удаление единственного элемента
}

func TestDlistEdge(t *testing.T) {
	d := NewDlist()
	// Покрытие удалений из пустого
	d.DeleteHead()
	d.DeleteTail()
	d.DeleteAfter("A")
	d.DeleteBefore("A")

	d.PushHead("A")
	d.DeleteHead() // Удаление единственного
}

func TestCuckooHashEdge(t *testing.T) {
	ch := NewCuckooHash[string](2)
	// Вызываем Contains до добавления
	assert.False(t, ch.Contains("A"))

	// Добавляем так, чтобы вызвать rehash (переполнение)
	ch.Add("A")
	ch.Add("B")
	ch.Add("C")

	assert.True(t, ch.Contains("C"))
}

func TestRBTreeEdge(t *testing.T) {
	tree := NewRedBlackTree()
	// Поиск в пустом дереве
	assert.False(t, tree.Search(10))

	// Вставка дубликата (покрывает ветку else return)
	tree.Insert(10)
	tree.Insert(10)

	// Печать пустого и непустого дерева
	tree.Print()
	tree.Clear()
	tree.Print()
}

// ================= ДОБИВАЕМ 85% =================

// 1. Жесткая провокация коллизий в CuckooHash (вызовет все ветки переселения)
func TestCuckooHashExtreme(t *testing.T) {
	// Берем очень маленькую емкость (2), чтобы хеши совпадали чаще
	ch := NewCuckooHash[int](2)

	// Добавляем много чисел. Это заставит элементы "выселять" друг друга
	// из table1 в table2 и обратно, покрывая все if/else.
	for i := 0; i < 20; i++ {
		ch.Add(i)
	}

	// Проверяем, что они там
	assert.True(t, ch.Contains(5))
	assert.True(t, ch.Contains(15))
}

// 2. Провокация сложных поворотов в Красно-черном дереве (Зигзаги)
func TestRBTreeZigZag(t *testing.T) {
	tree := NewRedBlackTree()

	// Эта последовательность специально вызывает:
	// - перекраску (когда дядя красный)
	// - левый/правый поворот (когда дядя черный)
	// - двойной поворот (зигзаг)
	tree.Insert(10)
	tree.Insert(20)
	tree.Insert(15) // Зигзаг (двойной поворот)

	tree.Insert(5)
	tree.Insert(1) // Линия (одиночный поворот)

	tree.Insert(30)
	tree.Insert(25) // Еще один зигзаг

	assert.True(t, tree.Search(15))
	assert.True(t, tree.Search(25))
	tree.Print() // Вызываем печать (покрывает рекурсивный обход)
}

// 3. Добиваем Dlist (удаление из середины и хвоста)
func TestDlistFinal(t *testing.T) {
	d := NewDlist()
	d.PushTail("A")
	d.PushTail("B")
	d.PushTail("C")

	d.DeleteTail()     // Удаляем C
	d.DeleteAfter("A") // Удаляем B
	d.DeleteValue("A") // Удаляем A

	assert.False(t, d.Find("A"))
	d.PrintForward()
	d.PrintBackward()
}
