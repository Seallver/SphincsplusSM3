// SPDX-License-Identifier: MIT

pragma solidity ^0.8.20;

import {ERC721} from "../lib/openzeppelin/contracts/token/ERC721/ERC721.sol";
import {Ownable} from "../lib/openzeppelin/contracts/access/Ownable.sol";


// spxNFT 是一个简单的 ERC-721 NFT 合约
contract spxNFT is ERC721, Ownable {
    uint256 public nextTokenId;

    constructor() ERC721("spxNFT", "SPXNFT") Ownable(msg.sender) {}

    // mint 函数允许合约拥有者铸造 NFT
    function mint(address to) public onlyOwner {
        _safeMint(to, nextTokenId);
        nextTokenId++;
    }
}