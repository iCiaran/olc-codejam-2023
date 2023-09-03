import random


class Point:
    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y

    def __str__(self):
        return f"({self.x},{self.y})"

    def __add__(self, other):
        x = self.x + other.x
        y = self.y + other.y

        return Point(x, y)


class MazeUtil:
    MAZE_SIZE = 25

    def __init__(self, seed=None, cells=None):
        if seed != None:
            self.seed = seed
            random.seed(seed)
            self.cell_groups = {}

            self.generate()

        if cells != None:
            self.cells = cells

    def get_cell(self, x, y):
        return self.cells[y * self.MAZE_SIZE + x]

    def set_cell(self, x, y, open):
        self.cells[y * self.MAZE_SIZE + x] = open

    def in_bounds(self, x, y):
        return x > 0 and x < self.MAZE_SIZE - 1 and y > 0 and y < self.MAZE_SIZE - 1

    def generate(self):
        self.cells = [0] * self.MAZE_SIZE * self.MAZE_SIZE

        group = 0
        for x in range(1, self.MAZE_SIZE, 2):
            for y in range(1, self.MAZE_SIZE, 2):
                self.set_cell(x, y, 1)
                self.cell_groups[(x, y)] = group
                group += 1

        for _ in range((self.MAZE_SIZE - 3) * (self.MAZE_SIZE + 1) // 4):
            self.iterate_kruskal()

    def random_initial(self):
        x = random.randrange(1, self.MAZE_SIZE, 2)
        y = random.randrange(1, self.MAZE_SIZE, 2)
        return Point(x, y)

    def random_direction(self):
        r = random.randint(0, 3)
        match r:
            case 0:
                return Point(1, 0)
            case 1:
                return Point(0, 1)
            case 2:
                return Point(-1, 0)
            case 3:
                return Point(0, -1)
        return Point(0, 0)

    def iterate_kruskal(self):
        while True:
            initial = self.random_initial()
            direction = self.random_direction()

            wall = initial + direction
            if self.get_cell(wall.x, wall.y):
                continue

            next_cell = wall + direction
            if not self.in_bounds(next_cell.x, next_cell.y):
                continue

            initial_group = self.cell_groups[(initial.x, initial.y)]
            next_group = self.cell_groups[(next_cell.x, next_cell.y)]

            if initial_group == next_group:
                continue

            self.set_cell(wall.x, wall.y, 1)

            for key, value in self.cell_groups.items():
                if value == initial_group:
                    self.cell_groups[key] = next_group

            break

    def print(self):
        for y in range(self.MAZE_SIZE):
            print("".join(" " if self.get_cell(x, y) else "#" for x in range(self.MAZE_SIZE)))

    def shortest_path(self, start=(1, 1), end=(MAZE_SIZE - 2, MAZE_SIZE - 2)):
        visited = set([start])
        queue = [(start, 0)]
        while len(queue) > 0:
            p, d = queue.pop(0)

            if p == end:
                return d

            for offset in [(1, 0), (0, 1), (-1, 0), (0, -1)]:
                next_p = (p[0] + offset[0], p[1] + offset[1])
                if self.get_cell(next_p[0], next_p[1]) and next_p not in visited:
                    queue.append((next_p, d + 1))
                    visited.add(next_p)
        return -1

    def validate(self, moves):
        position = Point(1, 1)
        for move in moves:
            match move:
                case 0:
                    position += Point(1, 0)
                case 1:
                    position += Point(0, 1)
                case 2:
                    position += Point(-1, 0)
                case 3:
                    position += Point(0, -1)
                case _:
                    return False

            if not self.get_cell(position.x, position.y):
                return False

        return position.x == self.MAZE_SIZE - 2 and position.y == self.MAZE_SIZE - 2
