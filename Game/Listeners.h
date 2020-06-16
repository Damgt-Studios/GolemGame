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
    ParticleEmitterEvent(FountainEmitter& _emitter) : emitter(_emitter) {};
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

class ApplyEffectEvent : public ADEvents::Listener
{
private:
    ADResource::ADGameplay::Effect& effect;
    ADResource::ADGameplay::GameObject* target;
public:
    float lifespan;
    ApplyEffectEvent(ADResource::ADGameplay::Effect& _effect) : effect(_effect) {};
    void SetTarget(ADResource::ADGameplay::GameObject* _target);
    void HandleEvent(ADEvents::ADEvent* _event) override;
};

//
//void ImplementListeners()
//{
//
//}