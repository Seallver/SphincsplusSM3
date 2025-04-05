
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
        if (newStatus) statusElement.textContent = newStatus;
        
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
function updateStatus(message, isError = false) {
    statusElement.style.opacity = '0';
    setTimeout(() => {
        statusElement.textContent = message;
        statusElement.style.backgroundColor = isError ? '#fdecea' : '#f8f9fa';
        statusElement.style.color = isError ? '#d32f2f' : '#666';
        statusElement.style.opacity = '1';
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
    updateStatus("正在验证...");
    // 这里可以添加实际的Verify逻辑
    setTimeout(() => {
        updateStatus("验证成功！");
    }, 1500);
});

// TTP按钮点击事件
document.getElementById('ttp').addEventListener('click', function() {
    currentRole = 'ttp';
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

backToRoleFromTtpBtn.addEventListener('click', function() {
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
    
    updateStatus(`${mode}完成！`);
    
} catch (error) {
    updateStatus(error.message, true);
    showResultModal(
        '操作失败',
        error.message,
        '',
        true
    );
}
}

// 显示结果模态框
function showResultModal(title, message, filename, isError = false) {
    const modal = document.getElementById('result-modal');
    const titleElement = document.getElementById('modal-title');
    const messageElement = document.getElementById('modal-message');
    const filenameElement = document.getElementById('modal-filename');
    const modalActions = document.querySelector('.modal-actions'); // 获取按钮容器
    
    // 清空现有按钮
    modalActions.innerHTML = '';
    
    titleElement.textContent = title;
    
    if (isError) {
        messageElement.className = 'error-message';
        filenameElement.textContent = '';
    } else {
        messageElement.className = 'success-message';
        
        if (filename && filename !== '--') {
            filenameElement.textContent = `文件名: ${filename}`;
            
            // 动态创建下载按钮
            const downloadBtn = document.createElement('a');
            downloadBtn.className = 'download-btn';
            downloadBtn.href = `data/${filename}`;
            downloadBtn.textContent = `下载 ${filename}`;
            downloadBtn.setAttribute('download', '');
            modalActions.appendChild(downloadBtn);
        } else {
            filenameElement.textContent = '操作完成';
        }
    }
    
    // 始终添加关闭按钮
    const closeBtn = document.createElement('button');
    closeBtn.className = 'close-btn';
    closeBtn.id = 'modal-close';
    closeBtn.textContent = '关闭';
    modalActions.appendChild(closeBtn);
    
    // 重新绑定关闭事件
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
    
    // 只有在sign模式下才添加threshold_ids
    if (currentMode === 'sign') {
        formData.threshold_ids = document.getElementById('ttp-threshold-ids').value;
    }
    
    // 验证必填字段
    const requiredFields = ['n', 't', 'ips', 'ports'];
    if (currentMode === 'sign') requiredFields.push('threshold_ids');
    
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

