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

    bytes public tssPublicKey = "This is tssPublicKey";

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

        // 设置 TSS 公钥
        marketplace.setTSSPublicKey(tssPublicKey);
    }

    function testBuyNFT() public {
        bytes memory sig = "This is tssPublicKey";

        // 卖家设置商品价格并批准市场合约转移其 NFT
        vm.startPrank(user);
        marketplace.setPrice(0, PRICE);
        nft.setApprovalForAll(address(marketplace), true);
        vm.stopPrank();


        uint256 initialSellerBalance = coin.balanceOf(user);
        uint256 initialBuyerBalance = coin.balanceOf(buyer);

        // 买家批准市场合约转移代币，准备购买NFT0
        vm.startPrank(buyer);
        coin.approve(address(marketplace), PRICE); 
        marketplace.buyNFT(0, sig);
        vm.stopPrank();
        
        // 检查买家是否已获得 NFT0
        assertEq(nft.ownerOf(0), buyer, "Buyer should own the NFT");

        // 检查卖家是否收到代币
        uint256 finalSellerBalance = coin.balanceOf(user);
        assertEq(finalSellerBalance, initialSellerBalance + PRICE, "Seller should receive the payment");

        // 检查买家的余额是否减少了支付的金额
        uint256 finalBuyerBalance = coin.balanceOf(buyer);
        assertEq(finalBuyerBalance, initialBuyerBalance - PRICE, "Buyer should pay the correct price");
    }
}
