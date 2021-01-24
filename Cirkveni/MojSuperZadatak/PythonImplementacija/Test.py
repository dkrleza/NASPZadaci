from BinaryTree import BinaryTree


def main():
    T: BinaryTree = BinaryTree()
    for e in [5, 2, 8, 1, 3, 6, 4, 7]:
        T.insert(e)
    T.print()
    T.mirror()
    print("-")
    T.print()
    return


if __name__ == "__main__":
    main()
