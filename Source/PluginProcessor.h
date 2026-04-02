#pragma once

#include <JuceHeader.h>
#include <atomic>
#include <memory>

struct Ball
{
    float x, y;
    float vx, vy;
    float radius = 8.0f;
};

struct Brick
{
    float x, y, width, height;
    bool active = true;
    juce::Colour colour;
};

class BreakoutAudioProcessor : public juce::AudioProcessor
{
public:
    BreakoutAudioProcessor();
    ~BreakoutAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

    juce::AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Brick Breaker"; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int index) override {}
    const juce::String getProgramName(int index) override { return {}; }
    void changeProgramName(int index, const juce::String &newName) override {}

    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

    // ゲーム関連
    void updateGame();
    void resetGame();
    [[nodiscard]] Ball getBallSnapshot() const;
    [[nodiscard]] std::shared_ptr<const std::vector<Brick>> getBricksSnapshot() const;

    std::atomic<float> paddleX { 400.0f };
    float paddleWidth = 100.0f;
    float paddleHeight = 15.0f;

    std::atomic<int> score { 0 };
    std::atomic<bool> gameStarted { false };

private:
    void initializeBricks();
    void checkCollisions(Ball& ballState, std::vector<Brick>& bricksState);

    Ball ballBuffers[2];
    std::atomic<int> activeBallIndex { 0 };
    std::shared_ptr<std::vector<Brick>> bricksSnapshot { std::make_shared<std::vector<Brick>>() };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BreakoutAudioProcessor)
};