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

To build this plugin from source:

1. Clone this repository.
2. Open the `CMakeLists.txt` in your IDE (VS Code, CLion) or use the **Projucer** to create a project from the source files.
3. Ensure you have the [JUCE framework](https://juce.com/) installed and configured.
4. Build the target (VST3 or AU).
5. Load the plugin into your DAW and ensure MIDI input is routed to the plugin track.

## Acknowledgments

This software was built using the **JUCE 8.0.12** framework. 
- [JUCE Website](https://juce.com/)
- [JUCE License](https://juce.com/get-juce)

## License

This project is open-source and available under the [MIT License](LICENSE).
