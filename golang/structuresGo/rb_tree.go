package structures

import "fmt"

type RBNode struct {
	Data   int
	IsRed  bool
	Left   *RBNode
	Right  *RBNode
	Parent *RBNode
}

func NewRBNode(val int, red bool, parent *RBNode) *RBNode {
	return &RBNode{Data: val, IsRed: red, Parent: parent}
}

type RedBlackTree struct {
	root *RBNode
}

func NewRedBlackTree() *RedBlackTree {
	return &RedBlackTree{root: nil}
}

func (t *RedBlackTree) rotateLeft(x *RBNode) {
	y := x.Right
	x.Right = y.Left
	if y.Left != nil {
		y.Left.Parent = x
	}
	y.Parent = x.Parent
	if x.Parent == nil {
		t.root = y
	} else if x == x.Parent.Left {
		x.Parent.Left = y
	} else {
		x.Parent.Right = y
	}
	y.Left = x
	x.Parent = y
}

func (t *RedBlackTree) rotateRight(x *RBNode) {
	y := x.Left
	x.Left = y.Right
	if y.Right != nil {
		y.Right.Parent = x
	}
	y.Parent = x.Parent
	if x.Parent == nil {
		t.root = y
	} else if x == x.Parent.Right {
		x.Parent.Right = y
	} else {
		x.Parent.Left = y
	}
	y.Right = x
	x.Parent = y
}

func (t *RedBlackTree) Insert(value int) {
	z := NewRBNode(value, true, nil)
	var y *RBNode
	x := t.root

	for x != nil {
		y = x
		if z.Data < x.Data {
			x = x.Left
		} else if z.Data > x.Data {
			x = x.Right
		} else {
			return // Дубликат
		}
	}

	z.Parent = y
	if y == nil {
		t.root = z
	} else if z.Data < y.Data {
		y.Left = z
	} else {
		y.Right = z
	}

	t.fixInsert(z)
}

func (t *RedBlackTree) fixInsert(z *RBNode) {
	for z.Parent != nil && z.Parent.IsRed {
		if z.Parent == z.Parent.Parent.Left {
			y := z.Parent.Parent.Right
			if y != nil && y.IsRed {
				z.Parent.IsRed = false
				y.IsRed = false
				z.Parent.Parent.IsRed = true
				z = z.Parent.Parent
			} else {
				if z == z.Parent.Right {
					z = z.Parent
					t.rotateLeft(z)
				}
				z.Parent.IsRed = false
				z.Parent.Parent.IsRed = true
				t.rotateRight(z.Parent.Parent)
			}
		} else {
			y := z.Parent.Parent.Left
			if y != nil && y.IsRed {
				z.Parent.IsRed = false
				y.IsRed = false
				z.Parent.Parent.IsRed = true
				z = z.Parent.Parent
			} else {
				if z == z.Parent.Left {
					z = z.Parent
					t.rotateRight(z)
				}
				z.Parent.IsRed = false
				z.Parent.Parent.IsRed = true
				t.rotateLeft(z.Parent.Parent)
			}
		}
	}
	t.root.IsRed = false
}

func (t *RedBlackTree) Search(target int) bool {
	cur := t.root
	for cur != nil {
		if target == cur.Data {
			return true
		} else if target < cur.Data {
			cur = cur.Left
		} else {
			cur = cur.Right
		}
	}
	return false
}

func (t *RedBlackTree) printInOrder(node *RBNode) {
	if node == nil {
		return
	}
	t.printInOrder(node.Left)
	fmt.Printf("%d ", node.Data)
	t.printInOrder(node.Right)
}

func (t *RedBlackTree) Print() {
	t.printInOrder(t.root)
	fmt.Println()
}

func (t *RedBlackTree) Clear() {
	t.root = nil
}
