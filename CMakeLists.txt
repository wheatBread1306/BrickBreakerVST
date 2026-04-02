cmake_minimum_required(VERSION 3.22)

project(BrickBreaker VERSION 1.0.2)

# Fetch JUCE from the official repository
include(FetchContent)

FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        8.0.6
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(JUCE)

juce_add_plugin(BrickBreaker
    VERSION                     1.0.2
    COMPANY_NAME                WheatBreadPluginDev
    COMPANY_WEBSITE             "https://github.com/wheatBread1306"
    COMPANY_COPYRIGHT           "Copyright (c) 2026 wheatBread1306"
    PLUGIN_MANUFACTURER_CODE    Wbpd
    PLUGIN_CODE                 BkBr
    FORMATS                     AU VST3 Standalone
    IS_SYNTH                    TRUE
    NEEDS_MIDI_INPUT            TRUE
    NEEDS_MIDI_OUTPUT           FALSE
    IS_MIDI_EFFECT              FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    VST3_CATEGORIES             Instrument
    PRODUCT_NAME                "BrickBreaker"
)

target_sources(BrickBreaker
    PRIVATE
        Source/PluginEditor.cpp
        Source/PluginProcessor.cpp
)

target_compile_definitions(BrickBreaker
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_STRICT_REFCOUNTEDPOINTER=1
)

target_link_libraries(BrickBreaker
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
