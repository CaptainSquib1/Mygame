#pragma once
#include <vector>
#include "physics.h"
#include "graphics.h"




class ParticleSystem {
public:
    ParticleSystem(std::size_t init_size);
    virtual ~ParticleSystem() = default;
    
    void create_particle(const Physics& physics, float lifetime, const Color& color);

    void update(double dt);

    const std::vector<SDL_Vertex>& render();
    
private:

    class Particle {
    public:
        Physics physics;
        Color color;
        float lifetime = 0.0f;
        float lifetime_initial = 0.0; // used for calculating alpha
        bool active = false;
        
        void update(double dt);
    };

    std::size_t num_active; // active within range [0, num_active)
    std::vector<Particle> particles;
    std::vector<SDL_Vertex> vertex_buffer;
};
