//
// Created by yash on 7/12/24.
//

#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H
#include <complex>
#include <iostream>

#include "audiostream.h"
#include <cmath>
#include <map>

namespace synthesizer
{
    enum WaveType
    {
        SINE,
        SAWTOOTH,
        SQUARE,
        PULSE
    };

    struct Note
    {
        std::string NoteId;
        unsigned long startFrame;
        unsigned long stopFrame;
        bool isNoteOn;
    };

    inline float getOscillatorAmplitude(const WaveType type, unsigned long currFrame, float maxAmplitude, float frequency);
    inline float adsrEnvelope( float attackTime,  float decayTime,  unsigned long startFrame,  float stayTime,  float releaseTIme,  float attackAmplitude,  float stayAmplitude,  unsigned long currFrame,  float maxAmplitude, float frequency, WaveType type);
    class Synthesizer {
    private:
        unsigned long sampleRate;
    public:
        float maxAmplitude;
        explicit Synthesizer(audioStream::BufferState* bufferState);
        unsigned long frames;
        static int callback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);
    };


    class Instrument
    {
    private:
        unsigned long sampleRate;
        const std::string instrumentName;
    public:
        std::unordered_map<std::string, long int> noteToFrequencyMap;
        Instrument();
        inline std::string getNoteId(std::string);
        virtual bool playNote(std::string noteChar) = 0;
        virtual bool stopNote(std::string noteChar) = 0;
    };

    class currNotes
    {
    private:
        Synthesizer* synth;
        std::unordered_map<std::string, synthesizer::Note> Notes;
        std::mutex noteMutex;
    public:
        explicit currNotes(Synthesizer *synth);
        ~currNotes();
        inline bool insertAndPlay(std::string noteId);
        inline bool stopNote(std::string noteId);
    };

    class Piano:public Instrument
    {
        bool playNote(std::string noteChar) override;
        bool stopNote(std::string noteChar) override;
    };
};


#endif //SYNTHESIZER_H
