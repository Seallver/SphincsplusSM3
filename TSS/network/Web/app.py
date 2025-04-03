from flask import Flask, render_template, request
from ctypes import CDLL, Structure, c_int, c_char_p, POINTER, c_void_p, cast, create_string_buffer
import os

app = Flask(__name__)

# 定义C库中的结构体（根据您的实际结构体定义调整）
class KeygenNet_ctx(Structure):
    _fields_ = [
        ("party_id", c_int),
        ("local_ip", c_char_p),
        ("port", c_int),
        # 添加其他必要的字段
        # 注意：必须与C结构体完全匹配，包括所有字段的顺序和类型
    ]

# 加载C库
lib = CDLL(os.path.abspath('./libtssapi.so'))  # 假设您的库名为libtssapp.so

# 设置函数原型
lib.playerAPI.argtypes = [c_int, c_int, c_int]
lib.playerAPI.restype = c_int
lib.ttpAPI.argtypes = [c_int, c_int]
lib.ttpAPI.restype = c_int

@app.route('/player', methods=['GET', 'POST'])
def player_api():
    if request.method == 'POST':
        try:
            # 获取表单数据
            n = int(request.form['n'])
            t = int(request.form['t'])
            party_id = int(request.form['party_id'])
            threshold_ids = request.form['threshold_ids'].split(',')
            ip_ports = request.form['ip_ports'].split(',')
            
            # 准备参数列表
            args = [n, t, party_id]
            
            # 添加threshold_ids
            args.extend([int(tid) for tid in threshold_ids])
            
            # 添加IP和端口
            args.extend(ip_ports)
            
            # 调用C函数
            result = lib.playerAPI(*args)
            
            return render_template('result.html', 
                               result=f"Player API executed with result: {result}",
                               filename=f"party_{party_id}_keygen_res.json")
        
        except Exception as e:
            return render_template('error.html', error=str(e))
    
    return render_template('player_form.html')

@app.route('/ttp', methods=['GET', 'POST'])
def ttp_api():
    if request.method == 'POST':
        try:
            # 获取表单数据
            n = int(request.form['n'])
            t = int(request.form['t'])
            threshold_ids = request.form['threshold_ids'].split(',')
            ip_ports = request.form['ip_ports'].split(',')
            
            # 准备参数列表
            args = [n, t]
            
            # 添加threshold_ids
            args.extend([int(tid) for tid in threshold_ids])
            
            # 添加IP和端口
            args.extend(ip_ports)
            
            # 调用C函数
            result = lib.ttpAPI(*args)
            
            return render_template('result.html', 
                               result=f"TTP API executed with result: {result}",
                               filename="ttp_keygen_res.json")
        
        except Exception as e:
            return render_template('error.html', error=str(e))
    
    return render_template('ttp_form.html')

@app.route('/')
def index():
    return """
    <h1>TSS API Demo</h1>
    <ul>
        <li><a href="/player">Player API</a></li>
        <li><a href="/ttp">TTP API</a></li>
    </ul>
    """

if __name__ == '__main__':
    app.run(debug=True, port=5000)