# Matrix Mini R4 ps2 搖桿測試

## 專案說明

此專案是使用 Arduino UNO R4 WiFi 開發板搭配 MatrixMiniR4 函式庫開發的智能機器人控制系統。透過 PS2 手把控制機器人的運動和各項功能。

## 硬體配置

- **開發板**: Arduino UNO R4 WiFi
- **函式庫**: MatrixMiniR4 v1.2.2
- **電源**: 2 顆鋰電池
- **感測器與模組**:
  - MXLaser 雷射測距模組 (I2C 1)
  - MXColor 顏色感測模組 (I2C 2)
  - LED 控制 (7 個)
  - 馬達驅動 (M1 左馬達、M2 右馬達)
- **控制輸入**: PS2 手把

## 功能說明

### 主要控制功能

#### 1. **馬達控制**
- **左搖桿 Y 軸 (PWR)**: 控制機器人前進/後退
  - 死區範圍: 120-135 (避免誤動)
  - 輸出範圍: -100 到 100
- **右搖桿 X 軸 (STR)**: 控制機器人轉向
  - 無死區設定
  - 輸出範圍: -100 到 100

#### 2. **按鈕功能**
可透過以下按鈕監測機器人的控制狀態：
- **SQUARE 按鈕**: 讀取方形按鈕狀態
- **CIRCLE 按鈕**: 讀取圓形按鈕狀態
- **BLUE 按鈕**: 讀取藍色按鈕狀態
- **GREEN 按鈕**: 讀取綠色按鈕狀態
- **R1 按鈕**: 讀取 R1 按鈕狀態
- **R2 按鈕**: 讀取 R2 按鈕狀態

#### 3. **運動算法**
```
M1 馬達速度 = 動力 + 轉向
M2 馬達速度 = -動力 + 轉向
```
此算法實現差速轉向，讓機器人可以平順轉向。

### 預留功能

以下功能已編寫但註解掉，可根據需要啟用：
- **顏色感測**: 讀取 RGB 顏色數值並識別顏色
- **雷射測距**: 通過雷射模組測量距離

## PS2 手把函式詳細說明

### 初始化函式

#### `MiniR4.PS2.read_gamepad(bool, int)`
讀取 PS2 手把狀態。

**參數說明**：
- 第一個參數 (`bool`): 震動開啟/關閉（true/false）
- 第二個參數 (`int`): 震動強度（0-255，當第一個參數為 true 時生效）

**使用範例**：
```cpp
MiniR4.PS2.read_gamepad(false, 0);  // 關閉震動
MiniR4.PS2.read_gamepad(true, 128); // 開啟震動，強度 128
```

### 搖桿讀取函式

#### `MiniR4.PS2.Analog(int button)`
讀取搖桿的類比值 (0-255)。

**搖桿常數**：

*左搖桿*：
| 常數 | 說明 | 數值範圍 |
|------|------|----------|
| `PSS_LY` | 左搖桿 Y 軸 (上/下) | 0-255 (0=上, 255=下) |
| `PSS_LX` | 左搖桿 X 軸 (左/右) | 0-255 (0=左, 255=右) |
| `PSB_L3` | 左搖桿按下 | 0 或 1 |

*右搖桿*：
| 常數 | 說明 | 數值範圍 |
|------|------|----------|
| `PSS_RY` | 右搖桿 Y 軸 (上/下) | 0-255 (0=上, 255=下) |
| `PSS_RX` | 右搖桿 X 軸 (左/右) | 0-255 (0=左, 255=右) |
| `PSB_R3` | 右搖桿按下 | 0 或 1 |

*類比肩鈕*：
| 常數 | 說明 | 數值範圍 |
|------|------|----------|
| `PSS_L2` | L2 類比按鈕 (左上肩外) | 0-255 |
| `PSS_R2` | R2 類比按鈕 (右上肩外) | 0-255 |

**搖桿值對應**：
- **0**: 最小方向（上/左）
- **127/128**: 中心（無動作）
- **255**: 最大方向（下/右）

**使用範例**：
```cpp
int pwr_raw = MiniR4.PS2.Analog(PSS_LY);  // 讀取左搖桿 Y 軸原始值
Serial.println(pwr_raw); // 輸出 0-255

// 配合死區處理
int pwr = (pwr_raw > 120 && pwr_raw < 135) ? 0 : map(pwr_raw, 0, 255, 100, -100);
```

### 按鈕讀取函式

#### `MiniR4.PS2.Button(int button)`
讀取按鈕狀態 (1 = 按下, 0 = 未按下)。

**按鈕常數**：

*方向鍵 (D-Pad)*：
| 常數 | 說明 | 位置 |
|------|------|------|
| `PSB_PAD_UP` | 方向上 | 左上 |
| `PSB_PAD_DOWN` | 方向下 | 左下 |
| `PSB_PAD_LEFT` | 方向左 | 左左 |
| `PSB_PAD_RIGHT` | 方向右 | 左右 |

*色彩按鈕 (右側)*：
| 常數 | 說明 | 位置 |
|------|------|------|
| `PSB_TRIANGLE` | △ 按鈕 (綠) | 右上 |
| `PSB_CIRCLE` | ○ 按鈕 (紅) | 右右 |
| `PSB_CROSS` | × 按鈕 (藍) | 右下 |
| `PSB_SQUARE` | □ 按鈕 (粉) | 右左 |

*肩鈕 (上方)*：
| 常數 | 說明 | 位置 |
|------|------|------|
| `PSB_L1` | L1 按鈕 | 左上肩內側 |
| `PSB_L2` | L2 按鈕 | 左上肩外側 |
| `PSB_R1` | R1 按鈕 | 右上肩內側 |
| `PSB_R2` | R2 按鈕 | 右上肩外側 |

*中央按鈕*：
| 常數 | 說明 | 位置 |
|------|------|------|
| `PSB_SELECT` | SELECT 按鈕 | 中央左 |
| `PSB_START` | START 按鈕 | 中央右 |

*搖桿按下*：
| 常數 | 說明 | 位置 |
|------|------|------|
| `PSB_L3` | L3 按鈕 (左搖桿按下) | 左搖桿 |
| `PSB_R3` | R3 按鈕 (右搖桿按下) | 右搖桿 |

*其他按鈕*：
| 常數 | 說明 | 位置 |
|------|------|------|
| `PSB_GREEN` | GREEN 按鈕 | 額外按鈕 |
| `PSB_BLUE` | BLUE 按鈕 | 額外按鈕 |

**使用範例**：
```cpp
int button_pressed = MiniR4.PS2.Button(PSB_SQUARE);
if (button_pressed) {
  Serial.println("SQUARE 按鈕被按下");
}

// 檢查多個按鈕
if (MiniR4.PS2.Button(PSB_L1) && MiniR4.PS2.Button(PSB_R1)) {
  Serial.println("L1 和 R1 同時被按下");
}
```

### 實用程式碼範例

#### 搖桿值對應馬達速度
```cpp
int left_stick_y = MiniR4.PS2.Analog(PSS_LY);
int right_stick_x = MiniR4.PS2.Analog(PSS_RX);

// 申請死區 (120-135 視為中心)
int pwr = (left_stick_y > 120 && left_stick_y < 135) ? 0 : map(left_stick_y, 0, 255, 100, -100);
int str = map(right_stick_x, 0, 255, -100, 100);

// 驅動馬達
MiniR4.M1.setSpeed(pwr + str);
MiniR4.M2.setSpeed(-pwr + str);
```

#### 組合按鈕控制
```cpp
// 按下 L1 加速
if (MiniR4.PS2.Button(PSB_L1)) {
  max_speed = 150;
} else {
  max_speed = 100;
}

// 按下 START 開始
if (MiniR4.PS2.Button(PSB_START)) {
  robot_mode = MODE_AUTO;
}

// 按下 SELECT 停止
if (MiniR4.PS2.Button(PSB_SELECT)) {
  robot_mode = MODE_STOP;
}
```

#### 類比肩鈕控制
```cpp
// 讀取 L2 和 R2 類比值
int l2_value = MiniR4.PS2.Analog(PSS_L2);
int r2_value = MiniR4.PS2.Analog(PSS_R2);

// 控制 LED 亮度
int brightness = map(l2_value, 0, 255, 0, 255);
MiniR4.LED.setLight(brightness);
```

## 編譯與上傳

### 環境要求
- PlatformIO IDE 或 Visual Studio Code + PlatformIO 擴充功能
- Arduino IDE 2.0 或更新版本

### 編譯和上傳步驟

1. 在 PlatformIO 中開啟此專案
2. 執行 `platformio run --target upload` 編譯並上傳到開發板
3. 開啟序列埠監視視窗（波特率 9600）以查看調試信息

## 輸出訊息

程式透過序列埠輸出即時控制資料（波特率 9600）：
```
PWR: [動力值]  STR: [轉向值]  R_square:[值] R_circle:[值] R_blue:[值] R_green:[值] R1_pressed:[值] R2_pressed:[值]
```

## 注意事項

1. PS2 手把需確保正確配對和連接
2. 電池設定為 2 顆鋰電池
3. 左搖桿設有死區 (120-135) 以避免微小誤差
4. 右搖桿無死區設定，可根據需要調整
5. 馬達方向可在程式碼中透過符號調整

## 未來改進

- [ ] 整合雷射避障功能
- [ ] 加入顏色追蹤功能
- [ ] 實現自主導航模式
- [ ] 添加遠程控制通訊功能

## 開發者資訊

- **專案名稱**: Matrix Mini R4 PS2 gamepad測試說明
- **建立日期**: 2026 年 2 月
- **開發平台**: PlatformIO
- **Author**:Tseng

## 相關資源

- [MatrixMiniR4 函式庫文檔](https://github.com/matrix-robotics/MatrixMiniR4)
- [Arduino UNO R4 文檔](https://docs.arduino.cc/hardware/uno-r4-wifi)
- [PlatformIO 官方網站](https://platformio.org/)
