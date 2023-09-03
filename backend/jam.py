from flask import Flask, request, Response
from flask_cors import CORS
import json

from models import Maze, Score
from maze import MazeUtil

app = Flask(__name__)
cors = CORS(app, resources={r"/api/*": {"origins": ["http://localhost:6931", "https://olc.iciaran.com"]}})

from database import db_session, init_db

init_db()


@app.teardown_appcontext
def shutdown_session(exception=None):
    db_session.remove()


@app.route("/api/mazes", methods=["GET"])
def list_mazes():
    mazes = Maze.query.all()
    return [maze.id for maze in mazes]


@app.route("/api/mazes/<int:id>", methods=["GET"])
def get_maze(id):
    maze = Maze.query.filter(Maze.id == id).first()
    if not maze:
        mu = MazeUtil(seed=id)
        maze = Maze(id, json.dumps(mu.cells), mu.shortest_path())
        db_session.add(maze)
        db_session.commit()

    return {"cells": json.loads(maze.cells), "shortest": maze.shortest}


@app.route("/api/scores", methods=["GET"])
def list_scores():
    scores = Score.query.order_by(Score.id).limit(10)
    return [{"maze": score.maze_id, "name": score.name, "score": score.score} for score in scores]


@app.route("/api/scores", methods=["POST"])
def add_score():
    body = request.json
    if any(param not in body for param in ["id", "moves", "name"]):
        return Response(status=400)

    maze = Maze.query.filter(Maze.id == body["id"]).first()
    if not maze:
        return Response(status=404)

    mu = MazeUtil(cells=maze.cells)
    if not mu.validate(body["moves"]):
        return Response(status=400)

    score = Score(body["id"], len(body["moves"]), body["name"])
    db_session.add(score)
    db_session.commit()

    return Response(status=201)


@app.route("/api/scores/<int:id>", methods=["GET"])
def get_score(id):
    scores = Score.query.filter(Score.maze_id == id).order_by(Score.score.desc(), Score.id).limit(10)
    return [{"maze": score.maze_id, "name": score.name, "score": score.score} for score in scores]
