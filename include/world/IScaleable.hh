#ifndef I_SCALEABLE_HH
#define I_SCALEABLE_HH

class IScaleable {
public:
  virtual void setScale(float _scale) =0;
  virtual float getScale() =0;
protected:
  float scale;
};

#endif // I_SCALEABLE_HH
