
#ifndef CLOUD_PARTICLE_HH
#define CLOUD_PARTICLE_HH


struct CloudParticle {
    glm::vec3 Position, Velocity;
    glm::vec4 Color;
    float Life;
  
    CloudParticle() 
      : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};    


#endif // CLOUD_PARTICLE_HH