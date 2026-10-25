#include "tests/test_runner.h"

#include "terra/scene/entity.h"

using namespace terra;

// Entity tests that avoid Transform::localMatrix() (trig-heavy, crashes on
// this legacy g++ 6.3 toolchain). Transform math is covered separately once a
// newer toolchain is available.
TERRA_TEST_CASE(entity_add_child) {
    Entity parent(1, "p");
    Entity child(2, "c");
    parent.addChild(&child);
    TERRA_CHECK(child.parent() == &parent);
    TERRA_CHECK(parent.children().size() == 1);
    TERRA_CHECK(parent.children()[0] == &child);
}

TERRA_TEST_CASE(entity_set_parent_detaches_from_previous) {
    Entity a(1, "a");
    Entity b(2, "b");
    Entity c(3, "c");

    b.setParent(&a);
    TERRA_CHECK(b.parent() == &a);
    TERRA_CHECK(a.children().size() == 1);

    // Reparenting b to c detaches b from a.
    b.setParent(&c);
    TERRA_CHECK(b.parent() == &c);
    TERRA_CHECK(a.children().size() == 0);
    TERRA_CHECK(c.children().size() == 1);
}

TERRA_TEST_CASE(entity_active_flag) {
    Entity e(1, "e");
    TERRA_CHECK(e.active() == true);
    e.setActive(false);
    TERRA_CHECK(e.active() == false);
    e.setActive(true);
    TERRA_CHECK(e.active() == true);
}
