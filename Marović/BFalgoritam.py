# ZADATAK:
# Usporedite brzinu izvođenja Bellman-Fordovog algoritma u njegovoj klasičnoj i bržoj inačici.
# Podsjetnik: U bržoj inačici ne obilaze se svaki puta svi bridovi, nego se obilaze vrhovi i
# to samo oni kojima se u prethodnom obilasku promijenila udaljenost.
# Također konstruirajte i primjerni graf koji će imati minimalno 20 vrhova, te minimalno 5
# negativnih težina. Pazite da nemate negativni ciklus! (Za bolju usporedbu brzine izvođenja
# nemojte unutar koda provjeravati imate li negativan ciklus).

import time


class Edge:
    def __init__(self, fr, to, weight):
        self.fr = fr
        self.to = to
        self.weight = weight


def BFclassic(V, E, source):
    numOfV = len(V)
    distanceDiction = {}
    predecessorDiction = {}
    inf = 100000
    for v in V:
        distanceDiction[v] = inf
        predecessorDiction[v] = None

    distanceDiction[source] = 0
    for i in range(numOfV - 1):
        for e in E:
            if distanceDiction[e.fr] + e.weight < distanceDiction[e.to]:
                distanceDiction[e.to] = distanceDiction[e.fr] + e.weight
                predecessorDiction[e.to] = e.fr


def BFfast(V, E, source):
    distanceDiction = {}
    predecessorDiction = {}
    inf = 100000
    for v in V:
        distanceDiction[v] = inf
        predecessorDiction[v] = None

    distanceDiction[source] = 0
    workingList = [source]
    while workingList:
        u = workingList[0]
        workingList.remove(u)
        for e in E:
            if e.fr == u:
                if distanceDiction[u] + e.weight < distanceDiction[e.to]:
                    distanceDiction[e.to] = distanceDiction[u] + e.weight
                    predecessorDiction[e.to] = u
                    if e.to not in workingList:
                        workingList.append(e.to)


def main():
    vertices = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "R", "S", "T", "U"}
    edges = []

    edges.append(Edge("A", "B", 5))
    edges.append(Edge("A", "C", 4))
    edges.append(Edge("A", "D", 7))
    edges.append(Edge("B", "F", 1))
    edges.append(Edge("B", "G", -2))
    edges.append(Edge("C", "F", 9))
    edges.append(Edge("C", "E", 1))
    edges.append(Edge("D", "E", -2))
    edges.append(Edge("E", "I", 3))
    edges.append(Edge("F", "H", 1))
    edges.append(Edge("G", "L", 7))
    edges.append(Edge("H", "K", -1))
    edges.append(Edge("I", "J", -5))
    edges.append(Edge("I", "U", 20))
    edges.append(Edge("J", "H", 3))
    edges.append(Edge("J", "K", 4))
    edges.append(Edge("K", "L", 4))
    edges.append(Edge("L", "M", 2))
    edges.append(Edge("L", "N", -1))
    edges.append(Edge("M", "P", 4))
    edges.append(Edge("N", "O", -2))
    edges.append(Edge("N", "U", 14))
    edges.append(Edge("O", "R", 3))
    edges.append(Edge("P", "S", -5))
    edges.append(Edge("R", "T", 3))
    edges.append(Edge("S", "R", 2))
    edges.append(Edge("S", "T", 6))
    edges.append(Edge("T", "U", 1))

    timeStart = time.perf_counter_ns()
    BFclassic(vertices, edges, "A")
    timeStop = time.perf_counter_ns()
    timeBFclassic = timeStop - timeStart

    timeStart = time.perf_counter_ns()
    BFfast(vertices, edges, "A")
    timeStop = time.perf_counter_ns()
    timeBFfast = timeStop - timeStart

    diff = timeBFclassic - timeBFfast

    print("Vrijeme izvođenja klasičnog BF algoritma (nanosekunde):", timeBFclassic)
    print("Vrijeme izvođenja brzog BF algoritma (nanosekunde):", timeBFfast)
    print("Brža verzija je brža za", diff, "nanosekundi!")


if __name__ == "__main__":
    main()
