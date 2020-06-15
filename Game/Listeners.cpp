#include "pchgame.h"
#include "Listeners.h"


void AudioSourceEvent::HandleEvent(ADEvents::ADEvent* _event)
{
    audioSource.Play();
}

void ParticleEmitterEvent::HandleEvent(ADEvents::ADEvent* _event)
{
    emitter.Activate(lifespan, *(XMFLOAT4*)_event->Parameter());
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
