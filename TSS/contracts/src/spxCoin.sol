// SPDX-License-Identifier: MIT
// 许可证声明：指定合约的版权许可为 MIT，允许代码的自由使用和修改。

pragma solidity ^0.8.20;
// 指定 Solidity 编译器的版本。这里选择的是 0.8.20，确保合约在这一版本下编译通过。

import {ERC20} from "../lib/openzeppelin/contracts/token/ERC20/ERC20.sol";
// 从 OpenZeppelin 导入 ERC20 合约，提供标准的 ERC20 代币功能。


contract spxCoin is ERC20 {
    // 声明 spxCoin 合约，继承 ERC20。

    event Mint(uint256 indexed amount);
    // 声明 Mint 事件：在代币铸造时记录铸造的数量。

    string public _name = "spxCoin";
    // 代币名称，供外部调用查询。

    string public _symbol = "SPX";
    // 代币符号，供外部调用查询。

    constructor(uint256 initialSupply) ERC20(_name, _symbol) {
        // 构造函数：初始化合约，设置代币名称和符号，并铸造初始供应量

        _mint(msg.sender, initialSupply);
        // 调用 OpenZeppelin 提供的 _mint 函数，将初始供应量铸造到合约创建者地址

        emit Mint(initialSupply);
        // 触发 Mint 事件，记录铸造的数量
    }

}
