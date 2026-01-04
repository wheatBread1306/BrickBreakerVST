# Brick Breaker VST3 Plugin

**🎮 MIDIピッチホイールで操作するブロック崩しプラグイン**

A breakout/brick breaker game that runs inside your DAW, controlled by MIDI pitch wheel!

---

## 📝 日本語

### 概要
DAW上で動作するブロック崩しゲームプラグインです。MIDIキーボードのピッチホイールでパドルを操作し、ブロックを壊していきます。音楽制作の合間の息抜きに最適なジョークプラグインです。

### 特徴
- 🎹 **MIDIピッチホイール操作**: MIDIキーボードのピッチホイールでパドルを左右に移動
- 🎨 **カラフルなブロック**: 6色のブロックを破壊
- 🎯 **スコアシステム**: ブロック1個につき10点
- 🔄 **リセット機能**: ゲームオーバー後も即座にリセット可能
- 60FPSのスムーズなゲームプレイ

### 動作環境
- **Windows**: Windows 10/11 (64-bit)
- **macOS**: macOS 10.13以降
- **DAW**: VST3対応のDAW（Cubase, FL Studio, Ableton Live, Reaper等）
- **必須**: MIDIキーボードまたはピッチホイール機能を持つMIDIコントローラー

### インストール方法

#### Windows
1. [Releases](../../releases/latest)から最新版をダウンロード
2. ZIPファイルを解凍
3. `BrickBreaker.vst3`フォルダを以下にコピー:
   ```
   C:\Program Files\Common Files\VST3\
   ```
4. DAWを再起動してプラグインをスキャン

#### macOS
1. [Releases](../../releases/latest)から最新版をダウンロード
2. ZIPファイルを解凍
3. `BrickBreaker.vst3`フォルダを以下にコピー:
   ```
   /Library/Audio/Plug-Ins/VST3/
   ```
   または
   ```
   ~/Library/Audio/Plug-Ins/VST3/
   ```
4. DAWを再起動してプラグインをスキャン

### 使い方
1. DAWでプラグインを読み込む（エフェクトまたはインストゥルメントトラックに追加）
2. MIDIキーボードをそのトラックに接続
3. **ピッチホイールを左右に動かしてパドルを操作**
4. ボールを跳ね返してブロックを壊す
5. 画面下に落ちるとゲームオーバー→自動的にリセット
6. 手動でリセットする場合は「Reset」ボタンをクリック

### 操作方法
- **ピッチホイール左**: パドルを左に移動
- **ピッチホイール右**: パドルを右に移動
- **Resetボタン**: ゲームをリセット

### 注意事項
⚠️ **このプラグインはエンターテイメント目的のジョークプラグインです**
- 音声処理は行いません（オーディオはスルーされます）
- 音楽制作ツールとしての実用性はありません
- 作業の合間の息抜き用として楽しんでください

---

## 📝 English

### Overview
A brick breaker game plugin that runs inside your DAW. Control the paddle with your MIDI keyboard's pitch wheel and break all the bricks! Perfect for taking a break during music production sessions.

### Features
- 🎹 **MIDI Pitch Wheel Control**: Move the paddle left and right using pitch wheel
- 🎨 **Colorful Bricks**: Destroy 6 rows of colorful bricks
- 🎯 **Score System**: Earn 10 points per brick
- 🔄 **Reset Function**: Instantly restart after game over
- Smooth 60 FPS gameplay

### System Requirements
- **Windows**: Windows 10/11 (64-bit)
- **macOS**: macOS 10.13 or later
- **DAW**: VST3-compatible DAW (Cubase, FL Studio, Ableton Live, Reaper, etc.)
- **Required**: MIDI keyboard or MIDI controller with pitch wheel functionality

### Installation

#### Windows
1. Download the latest release from [Releases](../../releases/latest)
2. Extract the ZIP file
3. Copy the `BrickBreaker.vst3` folder to:
   ```
   C:\Program Files\Common Files\VST3\
   ```
4. Restart your DAW and rescan plugins

#### macOS
1. Download the latest release from [Releases](../../releases/latest)
2. Extract the ZIP file
3. Copy the `BrickBreaker.vst3` folder to:
   ```
   /Library/Audio/Plug-Ins/VST3/
   ```
   or
   ```
   ~/Library/Audio/Plug-Ins/VST3/
   ```
4. Restart your DAW and rescan plugins

### How to Use
1. Load the plugin in your DAW (add to an effect or instrument track)
2. Connect your MIDI keyboard to that track
3. **Move the pitch wheel left/right to control the paddle**
4. Bounce the ball to break all the bricks
5. Game over when the ball falls off the bottom → automatically resets
6. Click the "Reset" button to manually restart

### Controls
- **Pitch Wheel Left**: Move paddle left
- **Pitch Wheel Right**: Move paddle right
- **Reset Button**: Restart the game

### Disclaimer
⚠️ **This is a joke plugin for entertainment purposes only**
- Does not process audio (audio passes through unchanged)
- Has no practical use for music production
- Intended for fun during work breaks

---

## 🛠️ Technical Details

### Built With
- [JUCE Framework](https://juce.com/) v7.x
- C++17
- VST3 SDK

### Architecture
- Real-time game loop at 60 FPS
- MIDI pitch wheel input processing in audio thread
- Thread-safe game state management using `CriticalSection`
- Collision detection with paddle and bricks

---

## 📄 License

© 2026 wheatBread1306 - For Entertainment Purposes Only

This plugin is provided "as is", without warranty of any kind. Built with JUCE Framework under the Personal License.

---

## 🤝 Contributing

This is a joke project, but if you have fun ideas or find bugs, feel free to open an issue!

---

## 🎵 Tips for Best Experience

- Use a MIDI keyboard with a smooth pitch wheel
- Try different DAWs - it works in any VST3-compatible host
- Challenge your friends to beat your high score
- Perfect for live coding/production streams as a fun break

---

## ⚡ Known Issues

- Audio processing is intentionally bypassed (this is a game, not an effect!)
- Requires MIDI input - won't work without a pitch wheel controller
- May cause productivity loss due to excessive gameplay 😄

---

**Have fun and happy breaking! 🎮🎹**
