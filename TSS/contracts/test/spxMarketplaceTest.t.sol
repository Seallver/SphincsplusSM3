// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

import "forge-std/Test.sol";
import "../src/spxMarketplace.sol";
import "../src/spxNFT.sol";
import "../src/spxCoin.sol";

contract spxMarketplaceTest is Test {
    spxCoin public coin;
    spxNFT public nft;
    spxMarketplace public marketplace;
    
    address public owner;
    address public user;
    address public buyer;

    uint256 public constant PRICE = 100 * 10 ** 18; // NFT 价格

    function setUp() public {
        owner = address(this); // 合约拥有者是部署合约的地址
        user = address(0x123); // 用户地址（卖家）
        buyer = address(0x456); // 买家地址

        // 部署 spxCoin 合约并给卖家和买家一些代币
        coin = new spxCoin(1000 * 10 ** 18);
        coin.transfer(user, 500 * 10 ** 18);
        coin.transfer(buyer, 500 * 10 ** 18);

        // 部署 spxNFT 合约并铸造 NFT
        nft = new spxNFT();
        nft.mint(user); // 给用户铸造一个 NFT

        // 部署 spxMarketplace 合约
        marketplace = new spxMarketplace(coin, nft);
    }

    function testBuyNFT() public {
        // 让买家批准市场合约转账代币
        vm.prank(buyer);
        coin.approve(address(marketplace), PRICE);

        // 卖家设置价格
        vm.prank(user);
        marketplace.setPrice(0, PRICE);

        // 让卖家批准市场合约转移其 NFT
        vm.prank(user);
        nft.setApprovalForAll(address(marketplace), true); // 允许市场合约转移 NFT
        
        uint256 initialSellerBalance = coin.balanceOf(user);
        uint256 initialBuyerBalance = coin.balanceOf(buyer);

        // 买家购买 NFT
        vm.prank(buyer);
        marketplace.buyNFT(0);

        // 检查买家是否已获得 NFT
        assertEq(nft.ownerOf(0), buyer, "Buyer should own the NFT");

        // 检查卖家是否收到代币
        uint256 finalSellerBalance = coin.balanceOf(user);
        assertEq(finalSellerBalance, initialSellerBalance + PRICE, "Seller should receive the payment");

        // 检查买家的余额是否减少了支付的金额
        uint256 finalBuyerBalance = coin.balanceOf(buyer);
        assertEq(finalBuyerBalance, initialBuyerBalance - PRICE, "Buyer should pay the correct price");
    }
}
