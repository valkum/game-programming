#ifndef EASING_HH
#define EASING_HH




float cubicInOut(float t) {
  return t < 0.5
    ? 4.0 * t * t * t
    : 0.5 * pow(2.0 * t - 2.0, 3.0) + 1.0;
}
float cubicOut(float t) {
  float f = t - 1.0;
  return f * f * f + 1.0;
}

float cubicIn(float t) {
  return t * t * t;
}

float quarticInOut(float t) {
  return t < 0.5
    ? +8.0 * pow(t, 4.0)
    : -8.0 * pow(t - 1.0, 4.0) + 1.0;
}

#endif // EASING_HH
