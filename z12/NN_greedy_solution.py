import itertools
from time import time
from collections import namedtuple
import math
Point = namedtuple("Point", ['x', 'y'])

class TspSolver(object):
    def __init__(self, points):
        self.points = points
        self.node_count = len(points)
        self.cycle = list(range(len(points))) + [0]
        self.obj = self.distance_of_cycle()

    def __str__(self, uniqueNumber):
        if not self.valid():
            raise ValueError("Solution is not valid")
        Sequence = []
        for x in self.cycle:
            Sequence.append(uniqueNumber[x])
        output = ' '.join(map(str, Sequence))
        return output

    @staticmethod
    def point_distance(a, b):
        return math.sqrt((a.x - b.x) ** 2 + (a.y - b.y) ** 2)

    def valid(self):
        return len(set(self.cycle[:-1])) == len(self.points) == len(self.cycle[:-1])

    def edge_length(self, a, b):
        x = self.points[a]
        y= self.points[b]
        return self.point_distance(x, y)

    def distance_of_cycle(self):
        return sum(self.edge_length(x, y) for x, y in zip(self.cycle[:-1], self.cycle[1:]))

    def greedyAlgo(self):
##### Жадный алгоритм NN
        cycle = [0]
        candidates = set(self.cycle[1:-1])
        while candidates:
            curr_point = cycle[-1]
            near_neighbor = None
            near_dist = math.inf
            for neighbor in candidates:
                neighbor_dist = int(self.edge_length(curr_point, neighbor) * 1e3)
                if neighbor_dist < near_dist:
                    near_neighbor = neighbor
                    near_dist = neighbor_dist
            cycle.append(near_neighbor)
            candidates.remove(near_neighbor)
        cycle.append(0)
        self.cycle = cycle
        self.obj = self.distance_of_cycle()
        return cycle, self.obj


class Solver(TspSolver):
    def swap(self, start, end):
# Локальный поиск, 2-окрестность
        is_improved = False
        new_cycle = self.cycle[:start] + self.cycle[start:end + 1][::-1] + self.cycle[end + 1:]
        new_obj = self.obj - \
                  (self.edge_length(self.cycle[start - 1], self.cycle[start]) +
                   self.edge_length(self.cycle[end], self.cycle[(end + 1)])) + \
                  (self.edge_length(new_cycle[start - 1], new_cycle[start]) +
                   self.edge_length(new_cycle[end], new_cycle[(end + 1)]))
        if self.obj - new_obj > 0:
            self.cycle = new_cycle
            self.obj = new_obj
            is_improved = True
        return is_improved

    def solve(self, max_time=600):

        # greedy approximation + initialization
        greedy_cycle, greedy_obj = self.greedyAlgo()
        is_improved = True
        t = time()
        while is_improved:
            if max_time and time() - t >= max_time:
                break
            is_improved = False
            workspace = itertools.combinations(range(1, self.node_count), 2)
            for start, end in workspace:
                if self.swap(start, end):
                    is_improved = True
                    break
        if self.obj > greedy_obj:
            self.cycle = greedy_cycle
            self.obj = greedy_obj


n = int(input())
Points = []
ID = []
i = 0
for i in range(n):
    stream = input().split()
    ID.append(int(stream[0]))
    Points.append(Point(float(stream[1]), float(stream[2])))
Solution = Solver(Points)
Solution.solve()
answer = Solution.__str__(ID)
print(answer)
