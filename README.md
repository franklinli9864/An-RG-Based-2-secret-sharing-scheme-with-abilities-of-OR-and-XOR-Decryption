---
# An RG-Based 2-Secret Sharing Scheme with Abilities of OR and XOR Decryption

本專案實作了一種基於**隨機網格 (Random Grids, RG)** 的雙秘密分享 (Visual Secret Sharing) 系統 。透過改進學長姊的研究，以**水平橫移 (Horizontal Shift)** 取代旋轉操作，解決了秘密影像必須為正方形的限制 。

## 1. 研究動機與目標

* **視覺密碼原理**：將秘密圖像拆解為 $n$ 個看起來像雜訊的份額 (Shares)，當達到門檻數量堆疊時，秘密影像可直觀重建 。


* **雙秘密還原**：本研究探討在同一組 Shares 中隱藏兩張秘密影像 ($SA, SB$) 。


* **多樣化解密**：除了傳統的 **OR 運算 (堆疊)**，本專案亦支援 **XOR 運算**，以避免疊加導致的影像變暗問題 。



## 2. 演算法流程 (Algorithm)

本專案提供 **KK1, KK2, KK3** 三種加密條件 。其核心邏輯如下：

1. 從第一張秘密 $SA$ 中選取像素 。


2. 透過指定演算法產生網格 $G1, G2$ 。


3. 利用 $SB$ 與 $G1$ 產生 $G2$，並進行 $1/p$ 的水平橫移 (本實驗預設 $p=4$) 。


4. 利用 $SA$ 與 $G2$ 產生 $G1$，同樣位移 $1/p$，重複至填滿影像 。



## 3. 實驗結果展示 (Experimental Results)

### 3.1 原始影像與加密份額

| 第一秘密 (SA) | 第二秘密 (SB) | Share 1 (G1) | Share 2 (G2) |
| --- | --- | --- | --- |
|  |  |  |  |

### 3.2 還原影像對比

| 模式 | 還原 SA (NCNU) | 還原 SB (CSIE - 橫移 $1/p$) |
| --- | --- | --- |
| **OR 還原** |  |  |
| **XOR 還原** |  |  |

## 4. 數據分析 (Data Analysis)

程式會自動計算對比度，白色定義為 1 (光透度最高)，黑色為 0 。詳細準確度數據記錄於產出的 `output.txt` ：

* **Accuracy-all**: 整體像素一致率。
* **Black-hit**: 黑點還原精確度。
* **White-hit**: 白點還原精確度。

## 5. 使用說明

1. **環境**：Visual Studio 2022 + OpenCV 4.x。
2. **圖片位置**：
* 第一秘密：`./pic/`
* 第二秘密：`./pic2/`


3. **執行**：啟動程式後可選擇執行 KK1、KK2 或 KK3 演算法。

## 6. 參考文獻

* CHANG, Joy Jo-Yi, et al. "Two-image encryption by random grids." 2010 IEEE 。

---
