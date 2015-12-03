Button
===========
  Button *button = new Button(parent, "Button");
  button->setCallback([&] {
    ...
  });
  button->setBackgroundColor(Color(0, 96, 128, 255));
  button->setPosition(ivec2(200, 170)); //Position
  button->setSize(ivec2(170, 28)); //Size