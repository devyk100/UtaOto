//
// Created by yash on 7/12/24.
//

#include "../include/synthesizer.h"
inline float synthesizer::getOscillatorAmplitude(const WaveType type, unsigned long currFrame, float maxAmplitude, float frequency)
{
    switch(type)
    {
    case SINE:
        {
            return static_cast<float>(std::sin(2*M_PI*static_cast<float>(frequency)*static_cast<double>(currFrame)/SAMPLE_RATE) * static_cast<double>(maxAmplitude)) ;
        }
    case SQUARE:
        {
            return static_cast<float>(std::sin(2*M_PI*static_cast<float>(frequency)*static_cast<double>(currFrame)/SAMPLE_RATE) * static_cast<double>(maxAmplitude)) > 0 ? maxAmplitude: (-1*maxAmplitude);
        }
    case SAWTOOTH:
        {
            const float period = SAMPLE_RATE / frequency;
            float t = static_cast<float>(currFrame);
            return 2.0f * (maxAmplitude) * static_cast<float>(t) / period - std::floor((t / period + 0.5f));
        }
    default:
            return 0.0f;
    }
}


synthesizer::Synthesizer::Synthesizer(audioStream::BufferState* bufferState):maxAmplitude(0.5), sampleRate(SAMPLE_RATE)
{
    frames = 0;
    maxAmplitude = 0.5f;
    bufferState->setCustomCallback(callback, this);

}

inline float synthesizer::adsrEnvelope(const float attackTime, const float decayTime, const unsigned long startFrame, const float stayTime, const float releaseTime, const float attackAmplitude, const float stayAmplitude, const unsigned long currFrame, const float maxAmplitude,const float frequency, WaveType type)
{
    auto currTime = static_cast<float>(static_cast<double>(*currFrame)/SAMPLE_RATE);
    auto startTime = static_cast<float>(static_cast<double>(*startFrame)/SAMPLE_RATE);
    auto diffTime = currTime-startTime;
    float fValue;
    if((currTime-startTime) < *attackTime)
    {
        float maxAmplitude = (diffTime/attackTime)*attackAmplitude;
        fValue = getOscillatorAmplitude(type,currFrame, maxAmplitude, frequency);
    }
    else if((currTime-startTime) >= *attackTime && (diffTime < (*attackTime+*decayTime)))
    {
        float maxAmplitude =  attackAmplitude - ((diffTime-attackTime)/(decayTime))*(attackAmplitude-stayAmplitude);
        fValue = getOscillatorAmplitude(type, currFrame, maxAmplitude, frequency);
    }
    else if((currTime-startTime) >= (*attackTime+*decayTime+*stayTime))
    {
        float maxAmplitude =  *stayAmplitude - ((diffTime-*attackTime-*decayTime-*stayTime)/(*releaseTime))*(*stayAmplitude);
        return sineOsciAmplitude(currFrame, &maxAmplitude, frequency);
    }
    else
    {
        return sineOsciAmplitude(currFrame, stayAmplitude, frequency);
    }
}


int synthesizer::Synthesizer::callback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
{
    auto data = static_cast<Synthesizer*>(userData);
    auto out = static_cast<float*>(outputBuffer);
    static_cast<void>(inputBuffer);
    // float f = getOscillatorAmplitude();
    float f = 520.0000;
    float m = 0.5f;
    // std::cout << data->maxAmplitude<<std::endl;
    for(unsigned int i = 0; i < framesPerBuffer; i++)
    {
        // m = adsrEnvelope(&attackTime, &decayTime, &startFrame, &stayTime, &releaseTime, &attackAmplitude, &stayAmplitude, &(data->frames), &maxAmp,&f);
        *(out++) = getOscillatorAmplitude( SAWTOOTH,(data->frames), (data->maxAmplitude), f);
        *(out++) = getOscillatorAmplitude(SQUARE, (data->frames), (data->maxAmplitude), f);
        // *(out++) = m;
        // *(out++) = m;
        data->frames++;
    }
    return paContinue;
}


inline std::string synthesizer::Instrument::getNoteId(std::string noteChar)
{
    return instrumentName + " " + noteChar;
}


synthesizer::currNotes::currNotes(Synthesizer *synth)
{
    this->synth = synth;
    Notes.reserve(60);
}

bool synthesizer::currNotes::insertAndPlay(std::string noteId)
{
    Note note;
    note.startFrame = synth->frames;
    note.isNoteOn = true;
    note.stopFrame = 0;
    note.NoteId = noteId;
    noteMutex.lock();
    Notes.emplace(noteId, note);
    noteMutex.unlock();
    return true;
}

bool synthesizer::currNotes::stopNote(std::string noteId)
{
    noteMutex.lock();
    auto it = Notes.find(noteId);
    if(it != Notes.end())
    {
        it->second.isNoteOn = false;
        it->second.stopFrame = synth->frames;
    }
    noteMutex.unlock();
    return true;
}


bool synthesizer::Piano::playNote(std::string noteChar)
{

}
