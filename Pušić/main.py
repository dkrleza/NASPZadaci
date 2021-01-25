import os
from sys import argv

from RB_Tree import RedBlackTree

if __name__ == '__main__':
    # path = argv[1]

    red_black_tree = RedBlackTree()

    command = input('Za učitati stablo iz datoteke upišite load [lokacija]: ')
    path = command[5:]

    with open(path, mode='r') as number_sequence_source:
        for line in number_sequence_source:
            for number in line.split(' '):
                red_black_tree.insert(int(number))

    print(red_black_tree)

    end = False
    while not end:
        command = input('Upišite "+ N" za dodati ili "- N" za obrisati čvor s vrijedosti N: ')
        try:
            operation = command.rstrip(' ').split(' ')[0]
            argument = command.rstrip(' ').split(' ')[1]

            try:
                argument = int(argument)
            except ValueError:
                raise TypeError('Vrijednost čvora mora biti prirodan broj!')

            if argument <= 0:
                raise TypeError('Vrijednost čvora mora biti prirodan broj!')

            if operation not in {'+', '-'}:
                raise ValueError('Naredba nije definirana!')

        except IndexError:
            print('Izraz je nepotpun!')
            continue
        except TypeError or ValueError as error:
            print(error)
            continue

        if operation == '+':
            red_black_tree.insert(value=argument)
            print(red_black_tree)
        elif operation == '-':
            red_black_tree.delete(value=argument)
            print(red_black_tree)

        choice = input('Gotovi? Upišite Y za izlaz ili V ako biste spremili animaciju stabla: ')
        if choice == 'V':
            os.chdir(os.path.dirname(os.path.realpath(__file__)) + '\graphics')

            print(os.system("echo %CD%"))
            os.system('ffmpeg -r 1 -i "%d.png" -vcodec libx264 -vf scale=640:-2,format=yuv420p  -y "movie.mp4"')

        end = True if choice in ('Y', 'V') else False
