#include "catch.hpp"

#include <mtypes/include/types.hpp>
#include <lib/scene/include/scenenode.hpp>
#include <lib/scene/include/scene.hpp>
#include <lib/scene/include/componentcontainer.hpp>
#include <lib/scene/include/icomponent.hpp>

using namespace lib;
using namespace lib::scene;

struct TestComponent : public IComponent
{
    int data_{0};

    mtps::sptr<TestComponent> addAnother()
    {
        return attachedNode()->addComponentOfType<TestComponent>();
    }

private:
    void update() override
    {
        ++data_;
    }
};

TEST_CASE("lib::scene::ComponentContainer", "[ComponentContainer][constructor]")
{
    // Create scenario for testing
    mtps::sptr<ComponentContainer> component_container_no_parent(mtps::msptr<ComponentContainer>());

    SECTION("Add component")
    {
        mtps::sptr<TestComponent> component = component_container_no_parent->addComponentOfType<TestComponent>();

        SECTION("Update")
        {
            component_container_no_parent->updateComponents();
            CHECK(component->data_ == 1);

            SECTION("Add twice")
            {
                mtps::sptr<TestComponent> component2 = component;
                component_container_no_parent->ensureComponentOfType(component2);
                component_container_no_parent->updateComponents();
                CHECK(component->data_ == 2);
                CHECK(component2->data_ == 2);
                CHECK(component == component2);
            }
        }

        SECTION("Withoout update")
        {
            SECTION("Add twice")
            {
                mtps::sptr<TestComponent> component2 = component;
                component_container_no_parent->ensureComponentOfType(component2);
                CHECK(component == component2);
            }
        }
    }

    SECTION("Clear")
    {
        mtps::sptr<TestComponent> component = component_container_no_parent->addComponentOfType<TestComponent>();
        auto data_copy(component->data_);
        component_container_no_parent->clearComponents();
        CHECK(data_copy == component->data_);
    }
}
