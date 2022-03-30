#include "collider.hpp"

#include "physics.hpp"
//
#include "alch/engine/context.hpp"
#include "alch/engine/entity.hpp"
//
#include <box2d/box2d.h>

#include <array>

Collider::~Collider() {
	auto& w = entity().context().systemRef<Physics>()._world;
	for (auto f : _fixtures) {
		entity().ref<Body>()._body->DestroyFixture(f);
	}
}

Collider::Collider(Entity& entity): Component(entity) {
}

Collider::Collider(Entity& entity, Box_t, const Vector2f& size, float density, const Vector2f& origin):
    Component(entity) {
	add(Box, size, density, origin);
}

void Collider::add(Box_t, const Vector2f& size, float density, const Vector2f& origin) {
	b2PolygonShape s;
	s.SetAsBox(size.x * 0.5f, size.y * 0.5f, {size.x * 0.5f, size.y * 0.5f}, {});
	_fixtures.push_back(entity().ref<Body>()._body->CreateFixture(&s, density));
}

void Collider::serialize(OArchive& ar) const {
	ar << static_cast<int>(_fixtures.size());
	for (auto fp : _fixtures) {
		auto& f = *fp;
		// f.Dump(1);
		ar(f.GetFriction(),                 //
		    f.GetRestitution(),             //
		    f.GetRestitutionThreshold(),    //
		    f.GetDensity(),                 //
		    f.IsSensor(),                   //
		    f.GetFilterData().categoryBits, //
		    f.GetFilterData().maskBits,     //
		    f.GetFilterData().groupIndex    //
		);

		ar(f.GetShape()->m_type);

		if (f.GetShape()->m_type == b2Shape::e_circle) {
			const auto& s = *static_cast<b2CircleShape*>(f.GetShape());
			ar(             //
			    s.m_radius, //
			    s.m_p       //
			);
		} else if (f.GetShape()->m_type == b2Shape::e_polygon) {
			const auto& s = *static_cast<b2PolygonShape*>(f.GetShape());
			ar(s.m_count);
			for (int i = 0; i != s.m_count; ++i) {
				ar(s.m_vertices[i]);
			}
		} else {
			assert(false && "Other types not implemented yet");
		}
	}
}

void Collider::deserialize(IArchive& ar) {
	int count;
	ar >> count;
	for (int i = 0; i != count; ++i) {
		b2FixtureDef fd;
		ar(fd.friction,              //
		    fd.restitution,          //
		    fd.restitutionThreshold, //
		    fd.density,              //
		    fd.isSensor,             //
		    fd.filter.categoryBits,  //
		    fd.filter.maskBits,      //
		    fd.filter.groupIndex     //
		);

		b2Shape::Type st;
		ar(st);

		if (st == b2Shape::e_circle) {
			b2CircleShape s;
			ar(             //
			    s.m_radius, //
			    s.m_p       //
			);
			fd.shape = &s;
			_fixtures.push_back(entity().ref<Body>()._body->CreateFixture(&fd));
		} else if (st == b2Shape::e_polygon) {
			decltype(b2PolygonShape{}.m_count) count;
			ar(count);
			std::array<b2Vec2, b2_maxPolygonVertices> verts;
			for (int i = 0; i != count; ++i) {
				ar(verts[i]);
			}
			b2PolygonShape s;
			s.Set(verts.data(), count);
			fd.shape = &s;
			_fixtures.push_back(entity().ref<Body>()._body->CreateFixture(&fd));
		} else {
			assert(false && "Other types not implemented yet");
		}
	}
}
