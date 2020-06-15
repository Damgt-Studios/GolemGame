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
