from ctypes import CDLL, Structure, c_int, c_char_p, POINTER, c_void_p, cast, create_string_buffer,c_ubyte 
from flask import Flask, render_template, request, jsonify,send_from_directory
import os
import argparse
import base64

#开启前端服务
#gunicorn -w 20 -b 0.0.0.0:5000 app:app
# or
#python3 app.py --port 5000 --debug
#重启网络服务
#sudo systemctl restart systemd-networkd


app = Flask(__name__)

# 加载C库
lib = CDLL(os.path.abspath('./libtssapi.so'))  # 假设您的库名为libtssapp.so

# 设置函数原型
# 定义函数参数类型
lib.keygen_playerAPI.argtypes = [
    c_int,                # n
    c_int,                # t
    c_int,                # party_id
    POINTER(c_char_p),    # ip_[] 数组指针
    POINTER(c_int),       # port_[] 数组指针
]
lib.keygen_playerAPI.restype = c_int

lib.keygen_ttpAPI.argtypes = [
    c_int,                # n
    c_int,                # t
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
    c_char_p,             # mess 指针
    c_int,                # mess_len
]
lib.sign_playerAPI.restype = c_int

lib.sign_ttpAPI.argtypes = [
    c_int,                # n
    c_int,                # t
    POINTER(c_char_p),    # ip_[] 数组指针
    POINTER(c_int),       # port_[] 数组指针
    c_char_p,             # mess 指针
    c_int,                # mess_len
]
lib.sign_ttpAPI.restype = c_int

lib.verify.argtypes = [
    c_int,                # mlen
    c_char_p,             # Sig
    c_char_p,             # pk
]
lib.verify.restype = c_int


@app.route('/')
def index():
    return render_template('index.html')

@app.route('/api/<mode>/<role>', methods=['POST'])
def handle_api(mode, role):
    try:
        data = request.get_json()
        
        # 公共参数
        n = int(data['n'])
        t = int(data['t'])
        ips = data['ips'].split(' ')
        ports = list(map(int, data['ports'].split(' ')))
        
        # 验证参数
        if len(ips) != n + 1:
            raise ValueError(f"需要 {n+1} 个 IP，收到 {len(ips)}")
        if len(ports) != n + 1:
            raise ValueError(f"需要 {n+1} 个端口，收到 {len(ports)}")
        
        # 准备参数
        ip_array = (c_char_p * (n + 1))()
        for i, ip in enumerate(ips):
            ip_array[i] = create_string_buffer(ip.encode('utf-8')).raw
        port_array = (c_int * (n + 1))(*ports)
        
        # 根据模式和角色调用不同的API
        if mode == 'keygen':
            if role == 'ttp':
                result = lib.keygen_ttpAPI(c_int(n), c_int(t), ip_array, port_array)
                return jsonify({
                    'status': 'success',
                    'result': result,
                    'filename': '--'
                })
            elif role == 'player':
                party_id = int(data['party_id'])
                result = lib.keygen_playerAPI(c_int(n), c_int(t), c_int(party_id), ip_array, port_array)
                return jsonify({
                    'status': 'success',
                    'result': result,
                    'filename': f'Web_party_{party_id}_keygen_res.json'
                })
                
        elif mode == 'sign':
            mess = create_string_buffer(data['mess'].encode('utf-8')).raw
            mess_len = len(mess)
            if role == 'ttp':
                result = lib.sign_ttpAPI(c_int(n), c_int(t), ip_array, port_array, mess, c_int(mess_len))
                return jsonify({
                    'status': 'success',
                    'result': result,
                    'filename': '--'
                })
            elif role == 'player':
                threshold_ids = list(map(int, data['threshold_ids'].split(' ')))
                if len(threshold_ids) != t:
                    raise ValueError(f"threshold_ids 需要 {t} 个参数，收到 {len(threshold_ids)}")
                tid_array = (c_int * t)(*threshold_ids)

                party_id = int(data['party_id'])
                result = lib.sign_playerAPI(c_int(n), c_int(t), c_int(party_id), tid_array, ip_array, port_array, mess, c_int(mess_len))
                return jsonify({
                    'status': 'success',
                    'result': result,
                    'filename': f'Web_party_{party_id}_sig.json'
                })
                
        raise ValueError("无效的模式或角色")
        
    except Exception as e:
        return jsonify({
            'status': 'error',
            'message': str(e)
        }), 400

@app.route('/data/<filename>')
def download_file(filename):
    return send_from_directory('data', filename)  # 从data目录提供文件

@app.route('/api/verify', methods=['POST'])
def handle_verify():
    try:
        # 1. 获取并解析JSON数据
        data = request.get_json()
        if not data:
            raise ValueError("未接收到有效JSON数据")

        # 2. 提取必需参数
        mlen = data.get('mlen')
        sm = data.get('Sig')  # 签名消息
        pk = data.get('pk')  # 公钥

        # 3. 参数验证
        if None in (sm, pk, mlen):
            missing = [k for k in ['sm', 'pk', 'mlen'] if data.get(k) is None]
            raise ValueError(f"Missing required fields: {', '.join(missing)}")
        
        
        try:
            sm = base64.b64decode(sm)  # 解码签名
            pk = base64.b64decode(pk)    # 解码公钥
        except Exception as e:
            raise ValueError(f"Base64 解码失败: {str(e)}")

        # 5. 调用C函数
        result = lib.verify(
            c_int(mlen),
            sm,
            pk,
        )

        # 6. 返回结果
        return jsonify({
            'message': 'Verification successful' if result == 0 else 'Verification failed',
            'valid': result == 0
        })
        
    except Exception as e:
        return jsonify({
            'status': 'error',
            'message': str(e),
            'result': -1  # 错误代码
        }), 400

if __name__ == '__main__':
    # 创建命令行参数解析器
    parser = argparse.ArgumentParser(description='运行 Flask 服务器')
    parser.add_argument('--port', type=int, default=5000, help='服务器端口号（默认: 5000）')
    parser.add_argument('--debug', action='store_true', help='是否启用调试模式')
    args = parser.parse_args()

    # 运行 Flask 服务器
    app.run(debug=args.debug, port=args.port)