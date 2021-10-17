#pragma once
#include <scene.hpp>

class SpriteEditor : public Scene
{
public:
    SpriteEditor(Context& context)
        : Scene(context)
    {}

    void onFrame() override;

    bool active() const override;
    void active(bool active) override;

private:
    bool _active = false;
};