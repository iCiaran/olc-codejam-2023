from sqlalchemy import Column, Integer, String
from database import Base


class Maze(Base):
    __tablename__ = "mazes"
    id = Column(Integer, primary_key=True, autoincrement=False)
    cells = Column(String, nullable=False)
    shortest = Column(Integer, nullable=False)

    def __init__(self, id, cells, shortest):
        self.id = id
        self.cells = cells
        self.shortest = shortest

    def __repr__(self):
        return f"<Maze {self.id} {self.shortest=}>"


class Score(Base):
    __tablename__ = "scores"
    id = Column(Integer, primary_key=True)
    maze_id = Column(Integer, nullable=False)
    score = Column(Integer, nullable=False)
    name = Column(String, nullable=False)

    def __init__(self, maze_id, score, name):
        self.maze_id = maze_id
        self.score = score
        self.name = name
