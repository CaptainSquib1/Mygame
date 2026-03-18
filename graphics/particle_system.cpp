#include "particle_system.h"

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <algorithm>

ParticleSystem::ParticleSystem(std::size_t init_size)
    :num_active{0}, particles{init_size} {
    if (init_size == 0) {
        throw std::runtime_error("Particle system should be created with a non-zero number of inactive particles!");
    }

    vertex_buffer.reserve(init_size * 6);
}

void ParticleSystem::create_particle(const Physics& physics, float lifetime, const Color& color) {
    if (num_active >= particles.size()) { // full
        return; 
    }

    // activate a particle
    Particle& p = particles.at(num_active);
    p.physics = physics;
    p.lifetime = lifetime;
    p.lifetime_initial = lifetime;
    p.color = color;
    p.active = true;

    // now there is one more active particle
    ++num_active;
}


void ParticleSystem::update(double dt) {
    for (std::size_t i = 0; i < num_active;) {
        particles.at(i).update(dt);

        if (!particles.at(i).active) {
            // move the last active particle into this inactive particle's place
            particles.at(i) = particles.at(num_active - 1);

            // reduce number of active particles
            --num_active;

            // don't increment i, the particle that was at num_active
            // - 1 needs to have update() called
        }
        else {
            ++i; // if particle at i was active, move to next particle
        }
    }
}

void ParticleSystem::Particle::update(double dt) {
    float timestep = static_cast<float>(dt);

    // determine how much time is left in particle's lifetime
    float active_time = (lifetime < timestep) ? lifetime : timestep;
    
    if (active_time > 0.0f) {
        // do physics
        physics.velocity += 0.5f * physics.acceleration * active_time;
        physics.position += physics.velocity * active_time;
        physics.velocity += 0.5f * physics.acceleration * active_time;

        lifetime -= active_time;
    }

    // deactive if done
    if (lifetime <= 0.0f) {
        lifetime = 0.0f;
        active = false;
    }
}

const std::vector<SDL_Vertex>& ParticleSystem::render() {
    vertex_buffer.assign(num_active * 6, SDL_Vertex{});

    for (std::size_t i = 0; i < num_active; ++i) {
        const Particle& p = particles.at(i);
        std::size_t vertex_index = i * 6;

        // calculate alpha based on lifetime
        float ratio = p.lifetime / p.lifetime_initial;

        // set color
        SDL_FColor color = {
            p.color.red / 255.0f,
            p.color.green / 255.0f,
            p.color.blue / 255.0f,
            p.color.alpha / 255.0f * ratio
        };

        constexpr float size = 1;
        float x = p.physics.position.x;
        float y = p.physics.position.y;

        SDL_FPoint p1 = { x - size, y - size };
        SDL_FPoint p2 = { x + size, y - size };
        SDL_FPoint p3 = { x - size, y + size };
        SDL_FPoint p4 = { x + size, y + size };

        // Triangle 1
        vertex_buffer[vertex_index + 0] = { p1, color, {0,0} };
        vertex_buffer[vertex_index + 1] = { p2, color, {0,0} };
        vertex_buffer[vertex_index + 2] = { p3, color, {0,0} };
        // Triangle 2
        vertex_buffer[vertex_index + 3] = { p2, color, {0,0} };
        vertex_buffer[vertex_index + 4] = { p4, color, {0,0} };
        vertex_buffer[vertex_index + 5] = { p3, color, {0,0} };
    }

    return vertex_buffer;
}
