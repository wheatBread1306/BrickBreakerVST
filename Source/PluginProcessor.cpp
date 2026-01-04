#include "PluginProcessor.h"
#include "PluginEditor.h"

BreakoutAudioProcessor::BreakoutAudioProcessor()
    : AudioProcessor (BusesProperties()
                     .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
    resetGame();
}

BreakoutAudioProcessor::~BreakoutAudioProcessor() {}

void BreakoutAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) {}
void BreakoutAudioProcessor::releaseResources() {}

bool BreakoutAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

void BreakoutAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, 
                                           juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();

    // ピッチホイールメッセージを処理
    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        
        if (message.isPitchWheel())
        {
            // ピッチホイール値を-1.0〜1.0に正規化
            int pitchValue = message.getPitchWheelValue();
            pitchWheelValue = (pitchValue - 8192) / 8192.0f;
            
            juce::ScopedLock lock(gameLock);
            // パドルのX座標を更新 (画面幅800と仮定)
            paddleX = 400.0f + pitchWheelValue * 350.0f;
            paddleX = juce::jlimit(paddleWidth / 2, 800.0f - paddleWidth / 2, paddleX);
        }
    }
}

void BreakoutAudioProcessor::resetGame()
{
    juce::ScopedLock lock(gameLock);
    
    ball.x = 400.0f;
    ball.y = 500.0f;
    ball.vx = 3.0f;
    ball.vy = -4.0f;
    
    paddleX = 400.0f;
    score = 0;
    gameStarted = true;
    
    initializeBricks();
}

void BreakoutAudioProcessor::initializeBricks()
{
    bricks.clear();
    
    const float brickWidth = 75.0f;
    const float brickHeight = 25.0f;
    const float padding = 5.0f;
    const int rows = 6;
    const int cols = 10;
    
    juce::Colour colours[] = {
        juce::Colours::red, juce::Colours::orange, juce::Colours::yellow,
        juce::Colours::green, juce::Colours::blue, juce::Colours::purple
    };
    
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            Brick brick;
            brick.x = col * (brickWidth + padding) + 10.0f;
            brick.y = row * (brickHeight + padding) + 50.0f;
            brick.width = brickWidth;
            brick.height = brickHeight;
            brick.active = true;
            brick.colour = colours[row];
            
            bricks.push_back(brick);
        }
    }
}

void BreakoutAudioProcessor::updateGame()
{
    juce::ScopedLock lock(gameLock);
    
    if (!gameStarted) return;
    
    // ボールの移動
    ball.x += ball.vx;
    ball.y += ball.vy;
    
    // 壁との衝突
    if (ball.x - ball.radius <= 0 || ball.x + ball.radius >= 800)
        ball.vx = -ball.vx;
    
    if (ball.y - ball.radius <= 0)
        ball.vy = -ball.vy;
    
    // 画面外に落ちた場合
    if (ball.y > 600)
    {
        resetGame();
        return;
    }
    
    checkCollisions();
}

void BreakoutAudioProcessor::checkCollisions()
{
    // パドルとの衝突
    float paddleLeft = paddleX - paddleWidth / 2;
    float paddleRight = paddleX + paddleWidth / 2;
    float paddleTop = 550.0f;
    
    if (ball.y + ball.radius >= paddleTop && 
        ball.y - ball.radius <= paddleTop + paddleHeight &&
        ball.x >= paddleLeft && ball.x <= paddleRight)
    {
        ball.vy = -std::abs(ball.vy);
        
        // パドルの中心からの距離に応じて角度を変更
        float relativePos = (ball.x - paddleX) / (paddleWidth / 2);
        ball.vx += relativePos * 2.0f;
        ball.vx = juce::jlimit(-6.0f, 6.0f, ball.vx);
    }
    
    // ブロックとの衝突
    for (auto& brick : bricks)
    {
        if (!brick.active) continue;
        
        if (ball.x + ball.radius >= brick.x && 
            ball.x - ball.radius <= brick.x + brick.width &&
            ball.y + ball.radius >= brick.y && 
            ball.y - ball.radius <= brick.y + brick.height)
        {
            brick.active = false;
            score += 10;
            
            // 衝突面を判定して反射
            float overlapLeft = (ball.x + ball.radius) - brick.x;
            float overlapRight = (brick.x + brick.width) - (ball.x - ball.radius);
            float overlapTop = (ball.y + ball.radius) - brick.y;
            float overlapBottom = (brick.y + brick.height) - (ball.y - ball.radius);
            
            float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});
            
            if (minOverlap == overlapLeft || minOverlap == overlapRight)
                ball.vx = -ball.vx;
            else
                ball.vy = -ball.vy;
            
            break;
        }
    }
}

juce::AudioProcessorEditor* BreakoutAudioProcessor::createEditor()
{
    return new BreakoutAudioProcessorEditor (*this);
}

void BreakoutAudioProcessor::getStateInformation (juce::MemoryBlock& destData) {}
void BreakoutAudioProcessor::setStateInformation (const void* data, int sizeInBytes) {}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BreakoutAudioProcessor();
}