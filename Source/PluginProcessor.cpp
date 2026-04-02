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
            int pitchValue = message.getPitchWheelValue();
            const float pitchWheelValue = (pitchValue - 8192) / 8192.0f;

            // パドルのX座標を更新 (画面幅800と仮定)
            float newPaddleX = 400.0f + pitchWheelValue * 350.0f;
            newPaddleX = juce::jlimit(paddleWidth / 2, 800.0f - paddleWidth / 2, newPaddleX);
            paddleX.store(newPaddleX, std::memory_order_release);
        }
    }
}

void BreakoutAudioProcessor::resetGame()
{
    const int currentIndex = activeBallIndex.load(std::memory_order_acquire);
    const int nextIndex = 1 - currentIndex;

    Ball resetBall;
    resetBall.x = 400.0f;
    resetBall.y = 500.0f;
    resetBall.vx = 3.0f;
    resetBall.vy = -4.0f;
    ballBuffers[nextIndex] = resetBall;
    activeBallIndex.store(nextIndex, std::memory_order_release);

    paddleX.store(400.0f, std::memory_order_release);
    score.store(0, std::memory_order_relaxed);
    gameStarted.store(true, std::memory_order_release);
    
    initializeBricks();
}

void BreakoutAudioProcessor::initializeBricks()
{
    auto newBricks = std::make_shared<std::vector<Brick>>();
    newBricks->reserve(60);
    
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
            
            newBricks->push_back(brick);
        }
    }

    std::atomic_store_explicit(&bricksSnapshot, newBricks, std::memory_order_release);
}

void BreakoutAudioProcessor::updateGame()
{
    if (!gameStarted.load(std::memory_order_acquire))
        return;

    const int currentIndex = activeBallIndex.load(std::memory_order_acquire);
    const int nextIndex = 1 - currentIndex;
    Ball nextBall = ballBuffers[currentIndex];
    
    // ボールの移動
    nextBall.x += nextBall.vx;
    nextBall.y += nextBall.vy;
    
    // 壁との衝突
    if (nextBall.x - nextBall.radius <= 0 || nextBall.x + nextBall.radius >= 800)
        nextBall.vx = -nextBall.vx;
    
    if (nextBall.y - nextBall.radius <= 0)
        nextBall.vy = -nextBall.vy;
    
    // 画面外に落ちた場合
    if (nextBall.y > 600)
    {
        resetGame();
        return;
    }

    auto currentBricks = std::atomic_load_explicit(&bricksSnapshot, std::memory_order_acquire);
    auto nextBricks = std::make_shared<std::vector<Brick>>(*currentBricks);

    checkCollisions(nextBall, *nextBricks);

    std::atomic_store_explicit(&bricksSnapshot, nextBricks, std::memory_order_release);
    ballBuffers[nextIndex] = nextBall;
    activeBallIndex.store(nextIndex, std::memory_order_release);
}

void BreakoutAudioProcessor::checkCollisions(Ball& ballState, std::vector<Brick>& bricksState)
{
    const float currentPaddleX = paddleX.load(std::memory_order_acquire);

    // パドルとの衝突
    float paddleLeft = currentPaddleX - paddleWidth / 2;
    float paddleRight = currentPaddleX + paddleWidth / 2;
    float paddleTop = 550.0f;
    
    if (ballState.y + ballState.radius >= paddleTop && 
        ballState.y - ballState.radius <= paddleTop + paddleHeight &&
        ballState.x >= paddleLeft && ballState.x <= paddleRight)
    {
        ballState.vy = -std::abs(ballState.vy);
        
        // パドルの中心からの距離に応じて角度を変更
        float relativePos = (ballState.x - currentPaddleX) / (paddleWidth / 2);
        ballState.vx += relativePos * 2.0f;
        ballState.vx = juce::jlimit(-6.0f, 6.0f, ballState.vx);
    }
    
    // ブロックとの衝突
    for (auto& brick : bricksState)
    {
        if (!brick.active) continue;
        
        if (ballState.x + ballState.radius >= brick.x && 
            ballState.x - ballState.radius <= brick.x + brick.width &&
            ballState.y + ballState.radius >= brick.y && 
            ballState.y - ballState.radius <= brick.y + brick.height)
        {
            brick.active = false;
            score.fetch_add(10, std::memory_order_relaxed);
            
            // 衝突面を判定して反射
            float overlapLeft = (ballState.x + ballState.radius) - brick.x;
            float overlapRight = (brick.x + brick.width) - (ballState.x - ballState.radius);
            float overlapTop = (ballState.y + ballState.radius) - brick.y;
            float overlapBottom = (brick.y + brick.height) - (ballState.y - ballState.radius);
            
            float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});
            
            if (minOverlap == overlapLeft || minOverlap == overlapRight)
                ballState.vx = -ballState.vx;
            else
                ballState.vy = -ballState.vy;
            
            break;
        }
    }
}

Ball BreakoutAudioProcessor::getBallSnapshot() const
{
    const int snapshotIndex = activeBallIndex.load(std::memory_order_acquire);
    return ballBuffers[snapshotIndex];
}

std::shared_ptr<const std::vector<Brick>> BreakoutAudioProcessor::getBricksSnapshot() const
{
    auto snapshot = std::atomic_load_explicit(&bricksSnapshot, std::memory_order_acquire);
    return std::shared_ptr<const std::vector<Brick>>(snapshot);
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