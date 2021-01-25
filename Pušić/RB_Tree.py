from __future__ import annotations

from enum import Enum
from sys import argv
from typing import List
from graphviz import Digraph


class Node:
    class Colors(Enum):
        BLACK = 0
        RED = 1

    def __init__(self, value, color=None, parent=None, left=None, right=None):
        self.value = value
        self.color = color
        self.parent = parent
        self.left = left
        self.right = right

    def __repr__(self):
        return str(self.__dict__)

    def __str__(self):
        return '{0} ({1})'.format(self.value, self.color)

    def grandparent(self):
        return self.parent.parent if self.parent is not None else None

    def sibling(self):
        if self.parent is not None:
            return self.parent.left if self.value >= self.parent.value else self.parent.right


class RedBlackTree:
    root = None
    n_graph = 0
    i_dummy = 0

    class Rotations(Enum):
        LEFT = 0
        RIGHT = 1

    def __init__(self):
        pass

    def __repr__(self):
        return str(self.__dict__)

    def __str__(self):
        return '\n'.join(self.preorder())

    def draw(self):
        queue = [self.root]

        g = Digraph('RBTree')

        while len(queue) > 0:
            temp = queue[0]
            queue = queue[1:]

            value = str(temp.value)
            color = 'black' if temp.color == Node.Colors.BLACK else 'red'
            g.node(value + 'img', value, color=color)

            if temp.parent is not None:
                parent_value = str(temp.parent.value)
                if temp.parent.left is None:
                    id_dummy = 'a' + str(self.i_dummy)
                    g.node(id_dummy, '', shape='square', style='filled', color='black', width='0.25')
                    g.edge(parent_value + 'img', id_dummy)
                    self.i_dummy += 1
                g.edge(parent_value + 'img', value + 'img')
                if temp.parent.right is None:
                    id_dummy = 'a' + str(self.i_dummy)
                    g.node(id_dummy, '', shape='square', style='filled', color='black', width='0.25')
                    g.edge(parent_value + 'img', id_dummy)
                    self.i_dummy += 1

            if temp.left is not None:
                queue.append(temp.left)
            if temp.right is not None:
                queue.append(temp.right)

            if temp.left is None and temp.right is None:
                id_dummy = 'a' + str(self.i_dummy)
                g.node(id_dummy, '', shape='square', style='filled', color='black', width='0.25')
                g.edge(value + 'img', id_dummy)
                self.i_dummy += 1

                id_dummy = 'a' + str(self.i_dummy)
                g.node(id_dummy, '', shape='square', style='filled', color='black', width='0.25')
                g.edge(value + 'img', id_dummy)
                self.i_dummy += 1

        g.format = 'png'
        g.render('graphics/{0}'.format(self.n_graph), view=True)
        self.n_graph += 1

    def preorder(self, node: Node = None, level: int = 0) -> List[str]:
        node = self.root if node is None else node

        if node is None:
            return []
        else:
            nodes_in_tree = ['|{0}{1}\tL = {2}\tD = {3}'.format('-' * level, node, node.left, node.right)]

            if node.left is not None:
                next_level = level + 1
                nodes_in_tree += self.preorder(node.left, next_level)

            if node.right is not None:
                next_level = level + 1
                nodes_in_tree += self.preorder(node.right, next_level)

            return nodes_in_tree

    def right_rotation(self, q: Node):
        p = q.left

        q.left = p.right
        if q.left is not None:
            q.left.parent = q

        p.right = q

        p.parent = q.parent
        if p.parent is None:
            self.root = p
            p.color = Node.Colors.BLACK
        else:
            if q.parent.left == q:
                q.parent.left = p
            elif q.parent.right == q:
                q.parent.right = p

        q.parent = p

    def left_rotation(self, p: Node):
        q = p.right

        p.right = q.left
        if p.right is not None:
            p.right.parent = p

        q.left = p

        q.parent = p.parent
        if p.parent is None:
            self.root = q
            q.color = Node.Colors.BLACK
        else:
            if p.parent.left == p:
                p.parent.left = q
            elif p.parent.right == p:
                p.parent.right = q

        p.parent = q

    def insert(self, value: int):
        if self.root is None:
            new = Node(value=value, color=Node.Colors.RED, parent=None)
            self.root = new
        else:
            current = self.root

            while True:
                if value < current.value:
                    if current.left is None:
                        new = Node(value=value, color=Node.Colors.RED, parent=current)
                        current.left = new
                        break
                    else:
                        current = current.left
                elif value >= current.value:
                    if current.right is None:
                        new = Node(value=value, color=Node.Colors.RED, parent=current)
                        current.right = new
                        break
                    else:
                        current = current.right

        self.draw()
        self.repair(new)
        self.draw()

        return

    def repair(self, new: Node):
        parent = new.parent
        grandparent = new.grandparent()
        uncle = parent.sibling() if parent is not None else None

        # First case
        if parent is None:
            new.color = Node.Colors.BLACK
            return

        # Second case
        elif parent.color == Node.Colors.BLACK:
            return

        # Third case
        elif uncle is not None and uncle.color == Node.Colors.RED:
            parent.color = Node.Colors.BLACK
            uncle.color = Node.Colors.BLACK
            grandparent.color = Node.Colors.RED

            self.repair(grandparent)

        # Fourth case
        # elif uncle is not None and uncle.color == Node.Colors.BLACK:
        else:
            if new == parent.left and parent == grandparent.left:
                self.right_rotation(grandparent)
                parent.color = Node.Colors.BLACK
                grandparent.color = Node.Colors.RED
            elif new == parent.left and parent == grandparent.right:
                self.right_rotation(parent)
                self.left_rotation(grandparent)
                new.color = Node.Colors.BLACK
                grandparent.color = Node.Colors.RED
            elif new == parent.right and parent == grandparent.left:
                self.left_rotation(parent)
                self.right_rotation(grandparent)
                new.color = Node.Colors.BLACK
                grandparent.color = Node.Colors.RED
            elif new == parent.right and parent == grandparent.right:
                self.left_rotation(grandparent)
                parent.color = Node.Colors.BLACK
                grandparent.color = Node.Colors.RED

    def delete(self, value: int):
        if self.root is None:
            return False

        current = self.root
        while True:
            if value < current.value:
                if current.left is not None:
                    current = current.left
                else:
                    return False
            elif value == current.value:
                break
            elif value > current.value:
                if current.right is not None:
                    current = current.right
                else:
                    return False

        # BST deletion

        if current.left is None and current.right is None:
            deleted_color = current.color
            replacement_color = None

            x = None
            x_parent = current.parent

            if self.root == current:
                self.root = x
            elif current == current.parent.left:
                current.parent.left = x
            elif current == current.parent.right:
                current.parent.right = x

            self.draw()

        elif current.left is not None and current.right is None:
            deleted_color = current.color
            replacement_color = current.left.color

            current.value = current.left.value
            current.color = current.left.color

            current.left = None

            x = current

            self.draw()

        elif current.left is None and current.right is not None:
            deleted_color = current.color
            replacement_color = current.right.color

            current.value = current.right.value
            current.color = current.right.color

            current.right = None

            x = current

            self.draw()

        else:
            predecessor = current.left
            while predecessor.right is not None:
                predecessor = predecessor.right
            x = predecessor.left
            if x is None:
                x_parent = predecessor.parent
            else:
                x.parent = predecessor.parent

            deleted_color = current.color
            replacement_color = predecessor.color

            current.value = predecessor.value
            current.color = predecessor.color

            if predecessor == current.left:
                current.left = predecessor.left
            else:
                predecessor.parent.right = None

            self.draw()

        # Adjustments #1
        if deleted_color == Node.Colors.RED and replacement_color in (Node.Colors.RED, None):

            self.draw()
            return True

        elif deleted_color == Node.Colors.RED and replacement_color == Node.Colors.BLACK:
            current.color = Node.Colors.RED

            self.draw()

        elif deleted_color == Node.Colors.BLACK and replacement_color == Node.Colors.RED:
            current.color = Node.Colors.BLACK

            self.draw()
            return True

        else:
            pass

        # Adjustments #2
        while True:
            if x is None:
                sibling = x_parent.left if x_parent.right is None else x_parent.right
            else:
                sibling = x.parent.left if x == x.parent.right else x.parent.right

            # First case
            if x is not None and x.color == Node.Colors.RED:
                x.color = Node.Colors.BLACK

                self.draw()
                return True

            # Second case
            left_child = x_parent.left is None if x is None else x.parent.left == x
            if (x is None or x.color == Node.Colors.BLACK) and sibling.color == Node.Colors.RED:
                sibling.color = Node.Colors.BLACK
                sibling.parent.color = Node.Colors.RED

                if left_child:
                    self.left_rotation(sibling.parent)
                else:
                    self.right_rotation(sibling.parent)

                if left_child:
                    sibling = x_parent.right if x is None else x.parent.right
                else:
                    sibling = x_parent.left if x is None else x.parent.left

                self.draw()

            # Third case
            if (x is None or x.color == Node.Colors.BLACK) and sibling.color == Node.Colors.BLACK\
                    and (sibling.left is None or sibling.left.color == Node.Colors.BLACK)\
                    and (sibling.right is None or sibling.right.color == Node.Colors.BLACK):
                sibling.color = Node.Colors.RED
                if x is None:
                    x = x_parent
                else:
                    x = x.parent

                if x.color == Node.Colors.RED:
                    x.color = Node.Colors.BLACK

                    self.draw()
                    return True
                else:
                    self.draw()
                    continue

            # Fourth case
            left_child = x_parent.left is None if x is None else x.parent.left == x
            other_side_cousin = sibling.left if left_child else sibling.right
            same_side_cousin = sibling.right if left_child else sibling.left
            other_side_cousin_red = False if other_side_cousin is None or other_side_cousin.color == Node.Colors.BLACK\
                else True
            same_side_cousin_black = True if same_side_cousin is None or same_side_cousin.color == Node.Colors.BLACK\
                else False
            if (x is None or x.color == Node.Colors.BLACK) and sibling.color == Node.Colors.BLACK\
                    and other_side_cousin_red and same_side_cousin_black:
                other_side_cousin.color = Node.Colors.BLACK
                sibling.color = Node.Colors.RED

                if left_child:
                    self.right_rotation(sibling)
                else:
                    self.left_rotation(sibling)

                if left_child:
                    sibling = x_parent.right if x is None else x.parent.right
                else:
                    sibling = x_parent.left if x is None else x.parent.left

                self.draw()

            # Fifth case
            left_child = x_parent.left is None if x is None else x.parent.left == x
            mirror_cousin = sibling.right if left_child else sibling.left
            mirror_cousin_red = False if mirror_cousin is None or mirror_cousin.color == Node.Colors.BLACK else True
            if (x is None or x.color == Node.Colors.BLACK) and sibling.color == Node.Colors.BLACK and mirror_cousin_red:
                sibling.color = sibling.parent.color
                sibling.parent.color = Node.Colors.BLACK
                mirror_cousin.color = Node.Colors.BLACK

                if left_child:
                    self.left_rotation(sibling.parent)
                else:
                    self.right_rotation(sibling.parent)

                self.draw()
                return True
