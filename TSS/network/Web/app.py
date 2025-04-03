from flask import Flask, render_template, request
from ctypes import CDLL, c_char_p, c_int
import os

app = Flask(__name__)


@app.route('/', methods=['GET', 'POST'])
def index():
    output = ""
    
    return render_template('index.html', output=output)

if __name__ == '__main__':
    app.run(debug=True)