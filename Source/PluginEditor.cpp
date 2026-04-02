#include "PluginProcessor.h"
#include "PluginEditor.h"

BreakoutAudioProcessorEditor::BreakoutAudioProcessorEditor(BreakoutAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(800, 650);

    resetButton.setButtonText("Reset");
    resetButton.onClick = [this]
    { audioProcessor.resetGame(); };
    addAndMakeVisible(resetButton);

    // 60FPSでゲームを更新
    startTimerHz(60);
}

BreakoutAudioProcessorEditor::~BreakoutAudioProcessorEditor()
{
    stopTimer();
}

void BreakoutAudioProcessorEditor::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::black);

    const Ball ball = audioProcessor.getBallSnapshot();
    const auto bricks = audioProcessor.getBricksSnapshot();
    const float paddleX = audioProcessor.paddleX.load(std::memory_order_acquire);
    const int score = audioProcessor.score.load(std::memory_order_relaxed);

    // ブロックを描画
    for (const auto &brick : *bricks)
    {
        if (brick.active)
        {
            g.setColour(brick.colour);
            g.fillRect(brick.x, brick.y, brick.width, brick.height);

            g.setColour(juce::Colours::white.withAlpha(0.3f));
            g.drawRect(brick.x, brick.y, brick.width, brick.height, 1.0f);
        }
    }

    // パドルを描画
    g.setColour(juce::Colours::white);
    float paddleLeft = paddleX - audioProcessor.paddleWidth / 2;
    g.fillRoundedRectangle(paddleLeft, 550.0f,
                           audioProcessor.paddleWidth,
                           audioProcessor.paddleHeight, 5.0f);

    // ボールを描画
    g.setColour(juce::Colours::yellow);
    g.fillEllipse(ball.x - ball.radius,
                  ball.y - ball.radius,
                  ball.radius * 2,
                  ball.radius * 2);

    // スコアを描画
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("Score: " + juce::String(score),
               10, 10, 200, 30, juce::Justification::left);

    // 操作説明
    g.setFont(14.0f);
    g.drawText("Use the pitch wheel to control the paddle",
               getWidth() - 250, 10, 240, 30, juce::Justification::right);
}

void BreakoutAudioProcessorEditor::resized()
{
    resetButton.setBounds(getWidth() / 2 - 50, 600, 100, 30);
}

void BreakoutAudioProcessorEditor::timerCallback()
{
    audioProcessor.updateGame();
    repaint();
}