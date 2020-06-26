#include "pchgame.h"
#include "Listeners.h"


void AudioSourceEvent::HandleEvent(ADEvents::ADEvent* _event)
{
    audioSource.Play();
}

void BigCloudEmitterEvent::HandleEvent(ADEvents::ADEvent* _event)
{
    XMFLOAT3 float4Event = (*static_cast<XMFLOAT3*>(_event->Parameter()));
    emitter.Activate(lifespan, { float4Event.x, float4Event.y, float4Event.z, 0 });
}

void RecoveryEmitterEvent::HandleEvent(ADEvents::ADEvent* _event)
{
    XMFLOAT3 float4Event = (*static_cast<XMFLOAT3*>(_event->Parameter()));
    emitter.Activate(lifespan, { float4Event.x, float4Event.y, float4Event.z - 10, 0 }, 15.0f);
}

void SmallCloudEmitterEvent::HandleEvent(ADEvents::ADEvent* _event)
{
    XMFLOAT3 float4Event = (*static_cast<XMFLOAT3*>(_event->Parameter()));
    emitter.Activate(lifespan, { float4Event.x, float4Event.y, float4Event.z, 0 });
}

void WaterWaveEmitterEvent::HandleEvent(ADEvents::ADEvent* _event)
{
    XMFLOAT3 float4Event = (*static_cast<XMFLOAT3*>(_event->Parameter()));
    emitter.Activate(lifespan, { float4Event.x, float4Event.y, float4Event.z, 0 });
}

void IronSkinEmitterEvent::HandleEvent(ADEvents::ADEvent* _event)
{
    XMFLOAT3 float4Event = (*static_cast<XMFLOAT3*>(_event->Parameter()));
    emitter.Activate(lifespan, { float4Event.x, float4Event.y, float4Event.z, 0 });
}

void FireballEmitterEvent::HandleEvent(ADEvents::ADEvent* _event)
{
    XMFLOAT3 float4Event = (*static_cast<XMFLOAT3*>(_event->Parameter()));
    emitter.Activate(lifespan, { float4Event.x, float4Event.y, float4Event.z, 0 });
}

void ApplyEffectEvent::SetTarget(ADResource::ADGameplay::GameObject* _target)
{
    target = _target;
}

void ApplyEffectEvent::HandleEvent(ADEvents::ADEvent* _event)
{
    target->effects.push_back(effect.clone());
    target->effects[target->effects.size() - 1].get()->OnApply(target->GetStatSheet());
}
