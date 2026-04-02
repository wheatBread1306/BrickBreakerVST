# MIDI Brick Breaker (JUCE Audio Plugin)

A classic Breakout-style game implemented as a VST3/AU audio plugin. Instead of a mouse or keyboard, you control the paddle using your MIDI controller's **Pitch Wheel**.

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Framework](https://img.shields.io/badge/framework-JUCE-green.svg)

## Features

- **MIDI Integration**: Real-time paddle control via MIDI Pitch Wheel messages.
- **Physics Engine**: Realistic ball-to-brick and ball-to-paddle collision detection with angle variations.
- **Score System**: Earn points by destroying bricks; the game tracks your progress.
- **Smooth Visuals**: 60 FPS rendering using JUCE's `Timer` and `Graphics` classes.
- **Thread-Safe Design**: Uses atomic variables and snapshots to ensure safe communication between the Audio Processor (logic) and the Plugin Editor (UI).

## Controls

- **Paddle Movement**: Use the **Pitch Bend Wheel** on your MIDI keyboard. 
  - Centered: Paddle stays in the middle.
  - Up: Moves paddle to the right.
  - Down: Moves paddle to the left.
- **Reset**: Click the "Reset" button at the bottom of the UI to restart the game.

## Requirements

- **DAW**: Any Digital Audio Workstation that supports VST3 or AU (Ableton Live, Logic Pro, FL Studio, etc.).
- **MIDI Input**: A MIDI controller with a pitch wheel, or a DAW that can route MIDI data to the plugin.

## Technical Implementation Notes

This project demonstrates several advanced JUCE concepts:
- **Lock-Free Synchronization**: Using `std::atomic` and `std::shared_ptr` (atomic load/store) to pass game state from the processing thread to the UI thread without blocking the audio path.
- **MIDI Processing**: Extracting and normalizing Pitch Wheel values within the `processBlock` loop.
- **Double Buffering**: Implementing a snapshot system for the game objects to ensure the UI always renders a consistent state of the game world.
  
## Build Instructions

To build this plugin from source using the Projucer:

1. Clone this repository to your local machine.
2. Ensure you have the [JUCE framework](https://juce.com/get-juce) installed.
3. Open the `BrickBreaker.jucer` file in the **Projucer** application.
4. If necessary, check the Global Paths in Projucer to ensure they point to your local JUCE modules.
5. Click the **"Save and Open in IDE"** button to generate the project for your environment (e.g., Visual Studio for Windows, Xcode for macOS).
6. Build the target (VST3, AU, or Standalone) from within your IDE.
7. Load the compiled plugin into your DAW and ensure MIDI input is routed to the plugin track to control the paddle.

## Download

You can download the pre-built Windows version (.vst3) from the [Releases](https://github.com/wheatBread1306/BrickBreakerVST/releases) page.
*Note: Since the binary is unsigned, Windows SmartScreen may show a warning. You will need to click "More info" and "Run anyway" to use it.*

## Acknowledgments

This software was built using the **JUCE 8.0.12** framework. 
- [JUCE Website](https://juce.com/)
- [JUCE License](https://juce.com/get-juce)

## License

This project is open-source and available under the [MIT License](LICENSE).
