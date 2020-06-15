#pragma once
#include "ADAudio.h"
#include <ADEventSystem.h>
#include <ADParticles.h>



class ParticleEmitterEvent : public ADEvents::Listener
{
private:
    FountainEmitter& emitter;
    XMFLOAT4 position;
public:
    float lifespan;
    ParticleEmitterEvent(FountainEmitter& _audioSource) : emitter(_audioSource) {};
    void HandleEvent(ADEvents::ADEvent* _event) override;
};

class AudioSourceEvent : public ADEvents::Listener
{
private:
    AD_AUDIO::AudioSource& audioSource;
public:
    AudioSourceEvent(AD_AUDIO::AudioSource& _audioSource) : audioSource(_audioSource) {};
    void HandleEvent(ADEvents::ADEvent* _event) override;
};