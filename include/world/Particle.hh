#ifndef PARTICLE_HH
#define PARTICLE_HH


struct Particle {
    glm::vec3 Position, Velocity;
    glm::vec4 Color;
    float Life;
  
    Particle() 
      : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};    


#endif // PARTICLE_HH