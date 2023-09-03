FROM python:3.10-alpine

RUN pip install --upgrade pip

WORKDIR /app

COPY requirements.txt requirements.txt
RUN pip3 install -r requirements.txt --no-cache-dir
COPY . .

CMD ["python3", "-m", "gunicorn.app.wsgiapp", "jam:app", "--bind=0.0.0.0:5000", "--timeout", "10", "-k", "gevent", "--workers=4"]