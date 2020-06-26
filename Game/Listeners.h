#pragma once
#include "ADAudio.h"
#include <ADEventSystem.h>
#include <ADParticles.h>



class BigCloudEmitterEvent : public ADEvents::Listener
{
private:
    BigCloudEmitter& emitter;
    XMFLOAT4 position;
public:
    float lifespan;
    BigCloudEmitterEvent(BigCloudEmitter& _emitter) : emitter(_emitter) {};
    void HandleEvent(ADEvents::ADEvent* _event) override;
};

class RecoveryEmitterEvent : public ADEvents::Listener
{
private:
    HealthEmitter& emitter;
    XMFLOAT4 position;
public:
    float lifespan;
    RecoveryEmitterEvent(HealthEmitter& _emitter) : emitter(_emitter) {};
    void HandleEvent(ADEvents::ADEvent* _event) override;
};

class SmallCloudEmitterEvent : public ADEvents::Listener
{
private:
    SmallCloudEmitter& emitter;
    XMFLOAT4 position;
public:
    float lifespan;
    SmallCloudEmitterEvent(SmallCloudEmitter& _emitter) : emitter(_emitter) {};
    void HandleEvent(ADEvents::ADEvent* _event) override;
};

class WaterWaveEmitterEvent : public ADEvents::Listener
{
private:
    WaveEmitter& emitter;
    XMFLOAT4 position;
public:
    float lifespan;
    WaterWaveEmitterEvent(WaveEmitter& _emitter) : emitter(_emitter) {};
    void HandleEvent(ADEvents::ADEvent* _event) override;
};

class IronSkinEmitterEvent : public ADEvents::Listener
{
private:
    IronSkinEmitter& emitter;
    XMFLOAT4 position;
public:
    float lifespan;
    IronSkinEmitterEvent(IronSkinEmitter& _emitter) : emitter(_emitter) {};
    void HandleEvent(ADEvents::ADEvent* _event) override;
};

class FireballEmitterEvent : public ADEvents::Listener
{
private:
    FireballEmitter& emitter;
    XMFLOAT4 position;
public:
    float lifespan;
    FireballEmitterEvent(FireballEmitter& _emitter) : emitter(_emitter) {};
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