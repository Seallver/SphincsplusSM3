from flask import Flask, render_template, request
from ctypes import CDLL, Structure, c_int, c_char_p, POINTER, c_void_p, cast, create_string_buffer
import os
import argparse


#gunicorn -w 4 -b 0.0.0.0:5000 app:app
#sudo systemctl restart systemd-networkd
#python3 app.py --port 5000 --debug

app = Flask(__name__)

# 加载C库
lib = CDLL(os.path.abspath('./libtssapi.so'))  # 假设您的库名为libtssapp.so

# 设置函数原型
# 定义函数参数类型
lib.keygen_playerAPI.argtypes = [
    c_int,                # n
    c_int,                # t
    c_int,                # party_id
    POINTER(c_int),       # tid[] 数组指针
    POINTER(c_char_p),    # ip_[] 数组指针
    POINTER(c_int),       # port_[] 数组指针
]
lib.keygen_playerAPI.restype = c_int

lib.keygen_ttpAPI.argtypes = [
    c_int,                # n
    c_int,                # t
    POINTER(c_int),       # tid[] 数组指针
    POINTER(c_char_p),    # ip_[] 数组指针
    POINTER(c_int),       # port_[] 数组指针
]
lib.keygen_ttpAPI.restype = c_int

lib.sign_playerAPI.argtypes = [
    c_int,                # n
    c_int,                # t
    c_int,                # party_id
    POINTER(c_int),       # tid[] 数组指针
    POINTER(c_char_p),    # ip_[] 数组指针
    POINTER(c_int),       # port_[] 数组指针
]
lib.sign_playerAPI.restype = c_int

lib.sign_ttpAPI.argtypes = [
    c_int,                # n
    c_int,                # t
    POINTER(c_int),       # tid[] 数组指针
    POINTER(c_char_p),    # ip_[] 数组指针
    POINTER(c_int),       # port_[] 数组指针
]
lib.sign_ttpAPI.restype = c_int


@app.route('/keygen_player', methods=['GET', 'POST'])
def keygen_player_api():
    if request.method == 'POST':
        try:
            # 获取表单数据
            n = int(request.form['n'])
            t = int(request.form['t'])
            party_id = int(request.form['party_id'])
            # 处理数组型参数
            threshold_ids = list(map(int, request.form['threshold_ids'].split(',')))
            ips = request.form['ip'].split(',')          # 输入类似 "192.168.0.1,192.168.0.2,..."
            ports = list(map(int, request.form['port'].split(',')))  # 输入类似 "12345,12346,..."

            # 校验参数数量是否符合 C 函数要求
            if len(threshold_ids) != t:
                raise ValueError(f"threshold_ids 需要 {t} 个参数，收到 {len(threshold_ids)}")
            if len(ips) != n + 1:  # 注意 C 函数循环是 i <= n，需要 n+1 个 IP
                raise ValueError(f"需要 {n+1} 个 IP，收到 {len(ips)}")
            if len(ports) != n + 1:
                raise ValueError(f"需要 {n+1} 个端口，收到 {len(ports)}")

            # 将 Python 数组转换为 ctypes 兼容类型
            # (1) threshold_ids 转换为 c_int 数组
            tid_array = (c_int * t)(*threshold_ids)

            # (2). IP数组 - 确保每个IP以null结尾
            ip_array = (c_char_p * (n + 1))()
            for i, ip in enumerate(ips):
                # 复制字符串并确保null结尾
                ip_array[i] = create_string_buffer(ip.encode('utf-8')).raw

            # (3) 端口转换为 c_int 数组
            port_array = (c_int * (n + 1))(*ports)


            # 调用 C 函数
            result = lib.keygen_playerAPI(
                c_int(n),
                c_int(t),
                c_int(party_id),
                tid_array,      # 直接传递数组（自动转换为指针）
                ip_array,       # 同上
                port_array      # 同上
            )
            
            return render_template('result.html', 
                               result=f"Player API executed with result: {result}"
                               ,filename=f"Web_party_{party_id}_keygen_res.json"
                               )
        
        except Exception as e:
            return render_template('error.html', error=str(e))
    
    return render_template('keygen_player_form.html')

@app.route('/keygen_ttp', methods=['GET', 'POST'])
def keygen_ttp_api():
    if request.method == 'POST':
        try:
            # 获取表单数据
            n = int(request.form['n'])
            t = int(request.form['t'])
            # 处理数组型参数
            threshold_ids = list(map(int, request.form['threshold_ids'].split(',')))
            ips = request.form['ip'].split(',')          # 输入类似 "192.168.0.1,192.168.0.2,..."
            ports = list(map(int, request.form['port'].split(',')))  # 输入类似 "12345,12346,..."

            # 校验参数数量是否符合 C 函数要求
            if len(threshold_ids) != t:
                raise ValueError(f"threshold_ids 需要 {t} 个参数，收到 {len(threshold_ids)}")
            if len(ips) != n + 1:  # 注意 C 函数循环是 i <= n，需要 n+1 个 IP
                raise ValueError(f"需要 {n+1} 个 IP，收到 {len(ips)}")
            if len(ports) != n + 1:
                raise ValueError(f"需要 {n+1} 个端口，收到 {len(ports)}")

            # 将 Python 数组转换为 ctypes 兼容类型
            # (1) threshold_ids 转换为 c_int 数组
            tid_array = (c_int * t)(*threshold_ids)

            # (2). IP数组 - 确保每个IP以null结尾
            ip_array = (c_char_p * (n + 1))()
            for i, ip in enumerate(ips):
                # 复制字符串并确保null结尾
                ip_array[i] = create_string_buffer(ip.encode('utf-8')).raw

            # (3) 端口转换为 c_int 数组
            port_array = (c_int * (n + 1))(*ports)


            # 调用 C 函数
            result = lib.keygen_ttpAPI(
                c_int(n),
                c_int(t),
                tid_array,      # 直接传递数组（自动转换为指针）
                ip_array,       # 同上
                port_array      # 同上
            )
            
            return render_template('result.html', 
                               result=f"TTP API executed with result: {result}",
                               filename="--")
        
        except Exception as e:
            return render_template('error.html', error=str(e))
    
    return render_template('keygen_ttp_form.html')


@app.route('/sign_player', methods=['GET', 'POST'])
def sign_player_api():
    if request.method == 'POST':
        try:
            # 获取表单数据
            n = int(request.form['n'])
            t = int(request.form['t'])
            party_id = int(request.form['party_id'])
            # 处理数组型参数
            threshold_ids = list(map(int, request.form['threshold_ids'].split(',')))
            ips = request.form['ip'].split(',')          # 输入类似 "192.168.0.1,192.168.0.2,..."
            ports = list(map(int, request.form['port'].split(',')))  # 输入类似 "12345,12346,..."

            # 校验参数数量是否符合 C 函数要求
            if len(threshold_ids) != t:
                raise ValueError(f"threshold_ids 需要 {t} 个参数，收到 {len(threshold_ids)}")
            if len(ips) != n + 1:  # 注意 C 函数循环是 i <= n，需要 n+1 个 IP
                raise ValueError(f"需要 {n+1} 个 IP，收到 {len(ips)}")
            if len(ports) != n + 1:
                raise ValueError(f"需要 {n+1} 个端口，收到 {len(ports)}")

            # 将 Python 数组转换为 ctypes 兼容类型
            # (1) threshold_ids 转换为 c_int 数组
            tid_array = (c_int * t)(*threshold_ids)

            # (2). IP数组 - 确保每个IP以null结尾
            ip_array = (c_char_p * (n + 1))()
            for i, ip in enumerate(ips):
                # 复制字符串并确保null结尾
                ip_array[i] = create_string_buffer(ip.encode('utf-8')).raw

            # (3) 端口转换为 c_int 数组
            port_array = (c_int * (n + 1))(*ports)


            # 调用 C 函数
            result = lib.sign_playerAPI(
                c_int(n),
                c_int(t),
                c_int(party_id),
                tid_array,      # 直接传递数组（自动转换为指针）
                ip_array,       # 同上
                port_array      # 同上
            )
            
            return render_template('result.html', 
                               result=f"Player API executed with result: {result}"
                               ,filename=f"Web_party_{party_id}_sign_res.json"
                               )
        
        except Exception as e:
            return render_template('error.html', error=str(e))
    
    return render_template('sign_player_form.html')

@app.route('/sign_ttp', methods=['GET', 'POST'])
def sign_ttp_api():
    if request.method == 'POST':
        try:
            # 获取表单数据
            n = int(request.form['n'])
            t = int(request.form['t'])
            # 处理数组型参数
            threshold_ids = list(map(int, request.form['threshold_ids'].split(',')))
            ips = request.form['ip'].split(',')          # 输入类似 "192.168.0.1,192.168.0.2,..."
            ports = list(map(int, request.form['port'].split(',')))  # 输入类似 "12345,12346,..."

            # 校验参数数量是否符合 C 函数要求
            if len(threshold_ids) != t:
                raise ValueError(f"threshold_ids 需要 {t} 个参数，收到 {len(threshold_ids)}")
            if len(ips) != n + 1:  # 注意 C 函数循环是 i <= n，需要 n+1 个 IP
                raise ValueError(f"需要 {n+1} 个 IP，收到 {len(ips)}")
            if len(ports) != n + 1:
                raise ValueError(f"需要 {n+1} 个端口，收到 {len(ports)}")

            # 将 Python 数组转换为 ctypes 兼容类型
            # (1) threshold_ids 转换为 c_int 数组
            tid_array = (c_int * t)(*threshold_ids)

            # (2). IP数组 - 确保每个IP以null结尾
            ip_array = (c_char_p * (n + 1))()
            for i, ip in enumerate(ips):
                # 复制字符串并确保null结尾
                ip_array[i] = create_string_buffer(ip.encode('utf-8')).raw

            # (3) 端口转换为 c_int 数组
            port_array = (c_int * (n + 1))(*ports)


            # 调用 C 函数
            result = lib.sign_ttpAPI(
                c_int(n),
                c_int(t),
                tid_array,      # 直接传递数组（自动转换为指针）
                ip_array,       # 同上
                port_array      # 同上
            )
            
            return render_template('result.html', 
                               result=f"TTP API executed with result: {result}",
                               filename=f"--")
        
        except Exception as e:
            return render_template('error.html', error=str(e))
    
    return render_template('sign_ttp_form.html')


@app.route('/')
def index():
    return render_template('index.html')



if __name__ == '__main__':
    # 创建命令行参数解析器
    parser = argparse.ArgumentParser(description='运行 Flask 服务器')
    parser.add_argument('--port', type=int, default=5000, help='服务器端口号（默认: 5000）')
    parser.add_argument('--debug', action='store_true', help='是否启用调试模式')
    args = parser.parse_args()

    # 运行 Flask 服务器
    app.run(debug=args.debug, port=args.port)