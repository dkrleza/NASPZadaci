class BinaryTreeNode:
    def __init__(self, value):
        self.value = value
        self.left: (BinaryTreeNode, None) = None
        self.right: (BinaryTreeNode, None) = None

    def insertRaw(self, newElement, rightChild: bool):
        newElement: BinaryTreeNode
        if rightChild:
            self.right = newElement
        else:
            self.left = newElement
        return

    def direct(self, value):
        if value < self.value:
            return self.left, False
        else:
            return self.right, True

    def print(self, indent=0):
        if self.left is not None:
            self.left.print(indent + 1)
        else:
            print("|" * indent + "|>None")
        print("|" * indent + ">" + str(self.value))
        if self.right is not None:
            self.right.print(indent + 1)
        else:
            print("|" * indent + " >None")
        return


class BinaryTree:
    def __init__(self):
        self.root = None
        return

    def print(self):
        if self.root is None:
            print(">None")
        else:
            self.root.print()
        return

    def GetValue(self, node, default):
        return

    def findInsertionPoint(self, value):
        last: (BinaryTreeNode, None) = None
        current = self.root
        lastTurn = None
        while current is not None:
            last = current
            current, lastTurn = last.direct(value)
        return last, lastTurn

    def insert(self, newValue):
        newNode = BinaryTreeNode(newValue)
        if self.root is None:
            self.root = newNode
        else:
            nodePlace: BinaryTreeNode
            nodeTurn: bool
            nodePlace, nodeTurn = self.findInsertionPoint(newValue)
            nodePlace.insertRaw(newNode, nodeTurn)
        return

    def mirror(self):
        current: BinaryTreeNode = self.root
        last = current
        while True:
            if current in (last, current.left, current.right):  # Current is root if this is true
                current.left, current.right, last = current.right, last, current.left
                if last == current:
                    current.left, current.right = current.right, current.left
                    break  # Mission accomplished
                else:
                    current, last = last, current
            else:
                k = int(last is None) + int(current.left is None) + int(current.right is None)
                if k == 2:  # Leaf
                    pass
                elif k == 1:  # Unprocessed single child element
                    if current.left is None:  # Mark right leaning version
                        current.left = current.right
                    elif current.right is None:  # Mark right leaning version
                        current.right = last
                    else:
                        last = current
                    current.left, current.right, last = current.right, last, current.left
                elif k == 0:  # Double child element or processed single child element
                    if current.left == current.right:  # Catch right leaning version
                        current.right = last
                        last = current.left
                        current.left = None
                    elif current.left == last:  # Catch left leaning version
                        last = current.right
                        current.right = None
                    else:
                        current.left, current.right, last = current.right, last, current.left
                        if current.left.value < current.value < current.right.value:
                            current.left, current.right = current.right, current.left
                if last is not None:
                    current, last = last, current
        return
