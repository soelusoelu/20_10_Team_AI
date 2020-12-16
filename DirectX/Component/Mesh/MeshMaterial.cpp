#include "MeshMaterial.h"
#include <cassert>

MeshMaterial::MeshMaterial(GameObject& gameObject)
    : Component(gameObject)
{
}

MeshMaterial::~MeshMaterial() = default;

void MeshMaterial::setMaterial(const MaterialPtr & material, unsigned index) {
    mMaterials[index] = material;
}

const Material& MeshMaterial::getMaterial(unsigned index) {
    assert(isSetMaterial(index));
    return *mMaterials[index];
}

bool MeshMaterial::isSetMaterial(unsigned index) const {
    return (mMaterials.find(index) != mMaterials.end());
}
