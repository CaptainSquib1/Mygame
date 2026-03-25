#include "action.h"
#include "game_object.h"
#include "particle_system.h"
#include "world.h"

void Jump::perform(World&, GameObject& obj) {
    obj.physics.velocity.y = obj.physics.jump_velocity;
}

void MoveRight::perform(World&, GameObject& obj) {
    obj.physics.acceleration.x = obj.physics.walk_acceleration;
}
void MoveLeft::perform(World&, GameObject& obj) {
    obj.physics.acceleration.x = -obj.physics.walk_acceleration;
}
void DashRight::perform(World&, GameObject& obj) {
    obj.physics.velocity.x += obj.physics.dashing_velocity;
}
void DashLeft::perform(World&, GameObject& obj) {
    obj.physics.velocity.x += -obj.physics.dashing_velocity;
}

void Crouch::perform(World&, GameObject& obj) {
    // TODO:
}
void SwingRight::perform(World&, GameObject& obj) {
    obj.physics.velocity.x += obj.physics.dashing_velocity;
    obj.physics.velocity.y += obj.physics.dashing_velocity/4;
}
void SwingLeft::perform(World&, GameObject& obj) {
    obj.physics.velocity.x += -obj.physics.dashing_velocity;
    obj.physics.velocity.y += obj.physics.dashing_velocity/4;
}

void ParticleActivate::perform(World &, GameObject &obj) {

}
