
// 获取所有视图元素
const mainButtons = document.getElementById('main-buttons');
const roleButtons = document.getElementById('role-buttons');
const ttpForm = document.getElementById('ttp-form');
const playerForm = document.getElementById('player-form');
const statusElement = document.getElementById('status');
const instructionElement = document.getElementById('instruction');

// 获取所有按钮
const backToMainBtn = document.getElementById('back-to-main');
const backToRoleFromTtpBtn = document.getElementById('back-to-role-from-ttp');
const backToRoleFromPlayerBtn = document.getElementById('back-to-role-from-player');
const ttpSubmitBtn = document.getElementById('ttp-submit');
const playerSubmitBtn = document.getElementById('player-submit');

let currentMode = ''; // 'keygen' or 'sign'
let currentRole = ''; // 'ttp' or 'player'

// 切换视图的函数
function switchViews(hideElement, showElement, newInstruction, newStatus) {
    // 先淡出当前视图
    hideElement.classList.add('fade-out');
    instructionElement.style.opacity = '0';
    statusElement.style.opacity = '0';
    
    setTimeout(() => {
        // 隐藏当前视图
        hideElement.classList.add('hidden');
        hideElement.classList.remove('fade-in');
        
        // 更新文本内容
        if (newInstruction) instructionElement.textContent = newInstruction;

        if (newStatus) updateStatus(newStatus);
        
        // 显示新视图
        showElement.classList.remove('hidden');
        showElement.classList.remove('fade-out');
        showElement.classList.add('fade-in');
        
        // 淡入新视图和文本
        setTimeout(() => {
            instructionElement.style.opacity = '1';
            statusElement.style.opacity = '1';
        }, 50);
    }, 300); // 等待淡出动画完成
}

// 更新状态消息的函数
function updateStatus(message, isError = false, isSuccess = false) {
    statusElement.style.opacity = '0';
    setTimeout(() => {
        if (isSuccess) {
            statusElement.textContent = message;
            statusElement.style.backgroundColor = '#e9f7ef';
            statusElement.style.color = '#28a745';
            statusElement.style.opacity = '1';
        }
        else {
            statusElement.textContent = message;
            statusElement.style.backgroundColor = isError ? '#fdecea' : '#f8f9fa';
            statusElement.style.color = isError ? '#d32f2f' : '#666';
            statusElement.style.opacity = '1';
        }
    }, 50);
}

// KeyGen按钮点击事件
document.getElementById('keygen').addEventListener('click', function() {
    currentMode = 'keygen';
    hideThresholdIds();
    switchViews(
        mainButtons, 
        roleButtons, 
        "请选择密钥生成角色", 
        "准备生成密钥..."
    );
});

// Sign按钮点击事件
document.getElementById('sign').addEventListener('click', function() {
    currentMode = 'sign';
    showThresholdIds();
    switchViews(
        mainButtons, 
        roleButtons, 
        "请选择签名角色", 
        "准备签名..."
    );
});

// 辅助函数：隐藏Threshold IDs
function hideThresholdIds() {
    document.querySelectorAll('.threshold-ids-row').forEach(row => {
        row.classList.add('hidden');
    });
}

// 辅助函数：显示Threshold IDs
function showThresholdIds() {
    document.querySelectorAll('.threshold-ids-row').forEach(row => {
        row.classList.remove('hidden');
    });
}

// Verify按钮点击事件
document.getElementById('verify').addEventListener('click', function() {
    switchViews(
        mainButtons,
        document.getElementById('verify-form'),
        "请上传验证文件",
        "准备验证..."
    );
});

// TTP按钮点击事件
document.getElementById('ttp').addEventListener('click', function() {
    currentRole = 'ttp';
    hideThresholdIds();
    switchViews(
        roleButtons, 
        ttpForm, 
        "请输入TTP配置参数", 
        "准备接收TTP配置..."
    );
});

// Player按钮点击事件
document.getElementById('player').addEventListener('click', function() {
    currentRole = 'player';
    switchViews(
        roleButtons, 
        playerForm, 
        "请输入Player配置参数", 
        "准备接收Player配置..."
    );
});

// 返回按钮事件
backToMainBtn.addEventListener('click', function () {
    showThresholdIds(); // 重置为显示状态
    switchViews(
        roleButtons, 
        mainButtons, 
        "请选择您需要的操作", 
        "就绪"
    );
});

document.getElementById('back-to-main-from-verify').addEventListener('click', function() {
    switchViews(
        document.getElementById('verify-form'),
        mainButtons,
        "请选择您需要的操作",
        "就绪"
    );
});

backToRoleFromTtpBtn.addEventListener('click', function () {
    switchViews(
        ttpForm, 
        roleButtons, 
        currentMode === 'keygen' ? "请选择密钥生成角色" : "请选择签名角色", 
        "准备" + (currentMode === 'keygen' ? "生成密钥..." : "签名...")
    );
});

backToRoleFromPlayerBtn.addEventListener('click', function() {
    switchViews(
        playerForm, 
        roleButtons, 
        currentMode === 'keygen' ? "请选择密钥生成角色" : "请选择签名角色", 
        "准备" + (currentMode === 'keygen' ? "生成密钥..." : "签名...")
    );
});


// 表单提交函数
async function submitForm(mode, role, formData) {    
    showLoading(); // 显示加载动画
    try {
        updateStatus(`正在${mode}...`);
        
        const response = await fetch(`/api/${mode}/${role}`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(formData)
        });
        
        const data = await response.json();
        
        if (data.status === 'error') {
            throw new Error(data.message);
        }
        
        // 显示结果模态框
        showResultModal(
            `${mode === 'keygen' ? '密钥生成' : '签名'}成功`,
            `结果代码: ${data.result}`,
            data.filename
        );
        
        updateStatus(`${mode}完成！`, false, true);
        
    } catch (error) {
        updateStatus(error.message, true);
        showResultModal(
            '操作失败',
            error.message,
            '',
            true
        );
    }finally {
        hideLoading(); // 无论成功失败都隐藏加载动画
    }
    
}

// 验证数据提交函数
async function submitVerifyData(jsonData) {
    showLoading(); // 显示加载动画
    updateStatus("正在验证数据...");
    
    try {
        const response = await fetch('/api/verify', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(jsonData)
        });

        const result = await response.json();
        
        if (result.status === 'error') {
            throw new Error(result.message);
        }

        showResultModal(
            '验证结果',
            `验证完成：${result.message}`,
            '',
            false
        );
        updateStatus("验证成功！", false, true);
    } catch (error) {
        updateStatus(`验证失败: ${error.message}`, true);
        showResultModal(
            '验证错误',
            error.message,
            '',
            true
        );
    }finally {
        hideLoading(); // 无论成功失败都隐藏加载动画
    }
}

// 添加验证提交事件
document.getElementById('verify-submit').addEventListener('click', function() {
    const fileInput = document.getElementById('verify-json');
    if (!fileInput.files.length) {
        updateStatus("请选择JSON文件", true);
        return;
    }

    const file = fileInput.files[0];
    const reader = new FileReader();

    reader.onload = function(e) {
        try {
            const jsonData = JSON.parse(e.target.result);
            submitVerifyData(jsonData);
        } catch (error) {
            updateStatus("JSON文件格式错误", true);
        }
    };

    reader.readAsText(file);
});


// 显示结果模态框
function showResultModal(title, message, filename, isError = false) {
    const modal = document.getElementById('result-modal');
    const titleElement = document.getElementById('modal-title');
    const messageElement = document.getElementById('modal-message');
    const filenameElement = document.getElementById('modal-filename');
    const modalActions = document.querySelector('.modal-actions');
    
    // 清空现有按钮
    modalActions.innerHTML = '';
    
    titleElement.textContent = title;
    messageElement.textContent = message;
    
    // 根据操作类型决定标题颜色
    if (title.includes('成功')) {
        titleElement.className = 'success-message';  // 绿色标题
    } else if (isError) {
        titleElement.className = 'error-message';  // 红色标题
    } else {
        titleElement.style.color = '#2c3e50';  // 默认颜色
    }

    
    
    if (isError) {
        messageElement.style.color = '#d32f2f';  // 红色错误提示
        filenameElement.textContent = '';
    } else {
        messageElement.style.color = '#2ecc71';  // 绿色成功提示
        
        if (filename && filename !== '--') {
            // 创建下载按钮
            const downloadBtn = document.createElement('a');
            downloadBtn.className = 'download-btn';
            downloadBtn.href = `data/${filename}`;
            downloadBtn.textContent = `${filename}`;
            downloadBtn.setAttribute('download', '');
            modalActions.appendChild(downloadBtn);
        }
    }
    
    // 创建关闭按钮
    const closeBtn = document.createElement('button');
    closeBtn.className = 'close-btn';
    closeBtn.textContent = '关闭';
    modalActions.appendChild(closeBtn);
    
    // 绑定关闭事件
    closeBtn.addEventListener('click', function() {
        modal.classList.remove('active');
    });
    
    modal.classList.add('active');
}

// TTP表单提交
document.getElementById('ttp-submit').addEventListener('click', function () {
    const formData = {
        n: document.getElementById('ttp-n').value,
        t: document.getElementById('ttp-t').value,
        ips: document.getElementById('ttp-ips').value,
        ports: document.getElementById('ttp-ports').value
    };
    
    // 验证必填字段
    const requiredFields = ['n', 't', 'ips', 'ports'];
    
    for (const field of requiredFields) {
        if (!formData[field]) {
            updateStatus(`请填写${field}字段`, true);
            return;
        }
    }
    
    submitForm(currentMode, 'ttp', formData);
});

// Player表单提交
document.getElementById('player-submit').addEventListener('click', function() {
    const formData = {
        n: document.getElementById('player-n').value,
        t: document.getElementById('player-t').value,
        party_id : document.getElementById('player-party-id').value,
        ips: document.getElementById('player-ips').value,
        ports: document.getElementById('player-ports').value
    };
    
    // 只有在sign模式下才添加threshold_ids
    if (currentMode === 'sign') {
        formData.threshold_ids = document.getElementById('player-threshold-ids').value;
    }
    
    // 验证必填字段
    const requiredFields = ['n', 't', 'ips', 'ports', 'party_id'];
    if (currentMode === 'sign') requiredFields.push('threshold_ids');
    
    for (const field of requiredFields) {
        if (!formData[field]) {
            updateStatus(`请填写${field}字段`, true);
            return;
        }
    }
    
    submitForm(currentMode, 'player', formData);
});

// 关闭模态框
document.getElementById('modal-close').addEventListener('click', function() {
    document.getElementById('result-modal').classList.remove('active');
});


// 显示加载动画
function showLoading() {
    document.getElementById('loading-modal').classList.add('active');
}

// 隐藏加载动画
function hideLoading() {
    document.getElementById('loading-modal').classList.remove('active');
}